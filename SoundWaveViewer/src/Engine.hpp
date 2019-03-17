#pragma once

#include <iostream>
#include <string>
#include "Specifications.hpp"
#include "Space.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "Adapter.hpp"

using namespace efram::logging;

namespace efram { namespace engine {

class Engine *engine;

class LifeCycle {
	public: virtual void Start() {};
	public: virtual void Update(int deltaTime) {};
	public: virtual void Render() {};
	public: virtual void Destroy() { delete this; };
};

class Engine {

	private: Logger log = Logger(typeid(*this).name());
	
	public: Context *context;
	public: Window *window;
	public: LifeCycle *lifeCycle;
	public: bool clearOnRender = true;

	public: Engine() {
		
	}

	public: ~Engine() {

	}	

	public: void Start() {
	
		log.Info("Starting engine ...");				

		// Init GLFW
		if (!glfwInit()) {
			log.Error("Failed to start GLFW.");
			return;
		}		

		context = glfwCreateWindow(window->width, window->height, window->title.c_str(), NULL, NULL);
		if (!context) {
			log.Error("Failed to create context.");
			glfwTerminate();
			return;
		}		
			
		glfwMakeContextCurrent(context);

		// Init GLEW
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			log.Error("Failed to start engine: " + adapter::glew::GetErrorString(err));
			return;
		}
	
		log.Info("Engine started!");

		// Init gloal references
		if (lifeCycle == nullptr) lifeCycle = new LifeCycle();
		engine = this;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0.0, 0.0, 0.25, 0.5);

		// Set callbacks
		glfwSetWindowSizeCallback(context, SizeCallback);
		glfwSetErrorCallback(ErrorCallback);
	
		// Start Engine
		engine = this;
		lifeCycle->Start();		
		while (!glfwWindowShouldClose(context)) {
			
			// Calculate delta time
			static cint oldTimeSinceStart = 0;
			cint timeSinceStart = glfwGetTime();
			cint deltaTime = timeSinceStart - oldTimeSinceStart;
			oldTimeSinceStart = timeSinceStart;

			// Call engine methods
			engine->Update(deltaTime);
			engine->Render();

			glfwPollEvents();

		}

		lifeCycle->Destroy();
		glfwTerminate();
	
	}

	private: void Update(cint deltaTime) {
		lifeCycle->Update(deltaTime);
	}

	private: void Render() {		
		if (clearOnRender) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		lifeCycle->Render();
		glfwSwapBuffers(context);
	}

	private: static void SizeCallback(Context *context, int width, int height) {
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 1 / width, 0, 1 / height);		
		glMatrixMode(GL_MODELVIEW);

		engine->window->width = width;
		engine->window->height = height;
	}

	private: static void ErrorCallback(int error, const char *description) {
		Logger(typeid(Engine).name()).Error("Code " + std::to_string(error) + ": " + std::string(description));
	}

};

}}