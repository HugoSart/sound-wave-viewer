#include <iostream>
#include <fstream>
#include <thread>
#include <glm/vec2.hpp>
#include "Engine.hpp"
#include "Sample.hpp"

using namespace glm;

using namespace efram::logging;
using namespace efram::space;
using namespace efram::engine;
using namespace efram::adapter;

struct HexCharStruct
{
	unsigned char c;
	HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
	return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
	return HexCharStruct(_c);
}

class WaveLifeCycle : public LifeCycle {

	private: Logger log = Logger(typeid(*this).name());

	// Properties
	private: const std::string path;	

	// File settings
	private: std::ifstream ifs;
	private: char *bytes;
	private: int sampleSize = 1280;

	// Render settings
	private: vec2 source = vec2(-0.25,-0.25), target = vec2(0, 0);
	private: Color color = Color(255, 255, 255, 255);	

	public: WaveLifeCycle(std::string path) : path(path) {}
	
	public: void Start() override {
		ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
		std::ifstream::pos_type pos = ifs.tellg();
		ifs.seekg(44, std::ios::beg);		
	}

	public: void Update(cint deltaTime) override {		
		static bool eof = false;
		static cint elapsedTime = 0, bytesRead = 0;
		if (elapsedTime >= 1000) elapsedTime = 0;
		elapsedTime += deltaTime;			

		if (elapsedTime == deltaTime && !ifs.eof()) {
			if (bytes != nullptr) delete bytes;
			char *pChars = new char[sampleSize];
			ifs.read(pChars, sampleSize);
			bytesRead += sampleSize;
			bytes = pChars;
		} else if (!ifs.eof() && !eof) {
			log.Info("End of file: " + std::to_string(bytesRead));
			eof = true;
		}
		
	}	

	public: void Render() override {		
		static cint count = 0;
		for (int i = 0; i < sampleSize; i += 1) {			
				cfloat x = i / 640.0f - 1.0f, y = bytes[i] / 127.0f / 2;
				glLineWidth(0.1);
				glPointSize(0.1);
				glBegin(GL_LINE_LOOP);
					glVertex2f(x, y);
					glVertex2f(x, -y);
				glEnd();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	public: void Destroy() override {
		ifs.close();		
	}

};

int main(int argc, char *argv[]) {    
	auto engine = new Engine();
	auto window = new Window("Sound Wave Viewer", 800, 600);
	
	engine->window = window;
	engine->lifeCycle = new WaveLifeCycle("slender_1.wav");
	engine->Start();

	return 0;
}
