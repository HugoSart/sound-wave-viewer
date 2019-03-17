#pragma once

#include <string>
#include "Specifications.hpp"

namespace efram { namespace engine {

class Window {
	
	public: std::string title, icon;	
	public: cint width, height;

	public: Window(std::string title, cint width, cint height) : title(title), width(width), height(height) {

	}

	public: ~Window() {

	}

};

}}