#pragma once
#include "font.h"
#include "util.h"
#include "preprocessor_definitions.h"
#include <vector>
#include <stack>
#include <string>
#include <cmath>
#include <thread>
#include <wx/math.h>
#include <stb_image.h>
#include <fstream>
#include <ttf_parser.h>
#include <filesystem>

namespace KDG_PhotoEditor{
	FontParsedData current_parsed_font;
	void font_parsed(void* args, void* font_data, int error);

	class Layer{
	private:
		std::vector<std::vector<unsigned int>> original_data;
		std::vector<std::vector<unsigned int>> data; //unsigned int because 4 bytes and we are using RGBA

		int width, height, channels;

		FontParsedData current_font;

		std::stack<std::string> undo_stack;
		std::stack<std::string> redo_stack;
	public:
		Layer(std::string file_path);
		Layer(unsigned char* d, int w, int h);
		Layer(int w, int h);
		~Layer();

		inline std::vector<std::vector<unsigned int>> get_data(){return data;}

		void rotate(float degrees, int centrex, int centrey);
		void scale(float sx, float sy, std::string type);
		void translate(int xmov, int ymov, bool make_transparent);
		void reflect(char var_name, int val, bool left_or_top); //Line meaning something like x=4 or y=2, the bool means are those sides the ones being reflected
		void blur(int topleftx, int toplefty, int w, int h);
		void set_pixel(int x, int y, char r, char g, char b, char a);
		void add_text(std::string text, Font font, int x, int y); //X and Y is the top left coord of the text
		void colour_filter(char r, char g, char b, char a); //Remember each pixel will be multiplied with the values
		void crop(int lmi, int rmi, int tmi, int bmi);

		/*
		How the undo/redo system works

		Hidden folder called .cache stores all the undo redo stuff
		Then inside the folder there are 2 folders called undo and redo
		Then inside them there are version folders for how far back the undo or redo
		Inside them folders there are jpeg files for each of the layers
		.cache folder is stored in the local directory for the image it is deleted when the program shuts down 

		.cache
		-undo
		-version_0
		-layer_0.jpg
		-layer_1.jpg
		-redo
		-version_9
		-layer_0.jpg
		-layer_1.jpg
		*/
		void undo();
		void redo();
		void push_undo(std::string path);
		void push_redo(std::string path);
	};
}