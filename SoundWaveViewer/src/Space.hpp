#pragma once

#include <malloc.h>
#include "Specifications.hpp"

namespace efram { namespace space {
	
class Color {

	public: cubyte r = 0;
	public: cubyte g = 0;
	public: cubyte b = 0;
	public: cubyte a = 0;

	public: Color(cubyte r, cubyte g, cubyte b, cubyte a)	: r(r), g(g), b(b), a(a) {}
	public: Color(cubyte r, cubyte g, cubyte b)				: r(r), g(g), b(b), a(0) {}
	public: Color()											: r(0), g(0), b(0), a(0) {}

	Color operator + (const Color color) {
		Color newColor = Color();
		newColor.r = r + color.r;
		newColor.g = g + color.g;
		newColor.b = b + color.b;
		newColor.a = a + color.a;
		return newColor;
	}

};

}}