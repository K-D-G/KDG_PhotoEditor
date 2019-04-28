#pragma once
#include <string>

namespace KDG_PhotoEditor{
	struct Font{
		std::string name;
		int size;
		int colour_fill; //RGBA
		int colour_border; //RGBA
		int border_thickness; //In pixels
	};

	struct FontParsedData{
		void* args;
		void* font_data;
		int error;
	};
}