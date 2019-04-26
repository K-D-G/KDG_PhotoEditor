#pragma once
#include <string>

namespace KDG_PhotoEditor{
	struct Font{
		std::string name;
		int size;
	};

	struct FontParsedData{
		void* args;
		void* font_data;
		int error;
	};
}