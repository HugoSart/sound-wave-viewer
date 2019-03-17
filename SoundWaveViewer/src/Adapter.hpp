#pragma once

#include <string>
#include "Specifications.hpp"

namespace efram { namespace adapter { 

namespace glew {

	std::string GetErrorString(GLenum err) {
		auto *bytes = glewGetErrorString(err);
		return std::string(reinterpret_cast<const char*>(bytes));
	}

}

namespace gl {
	
	void SetColor(space::Color color) {
		glColor4ub(color.r, color.g, color.b, color.a);
	}

}

}}
