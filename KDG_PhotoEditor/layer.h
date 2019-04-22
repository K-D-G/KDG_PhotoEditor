#pragma once
#include "font.h"
#include <vector>
#include <string>
#include <cmath>
#include <stb_image.h>
#include <wx/math.h>

namespace KDG_PhotoEditor{
	class Layer{
	private:
		std::vector<std::vector<unsigned int>> original_data;
		std::vector<std::vector<unsigned int>> data; //unsigned int because 4 bytes and we are using RGBA

		int width, height, channels;
	public:
		Layer(std::string file_path);
		Layer();
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
	};
}