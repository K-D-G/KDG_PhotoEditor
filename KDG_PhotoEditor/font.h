#pragma once
#include <string>

namespace KDG_PhotoEditor{
	//Font struct that is used just to pass the args in easier and keep font settings together
	struct Font{
		std::string name; //Name of the font (this is OS specific as it is directly opened with the .ttf extension)
		int size; //The size of the font so 14 etc
		int colour_fill; //In RGBA format and is the colour that will fill the text
		int colour_border; //In RGBA format and is the border colour
		int border_thickness; //How many pixels wide the border is
	};

	//Struct used when dealing with the ttf parse callback
	struct FontParsedData{
		void* args;
		void* font_data;
		int error;
	};
}