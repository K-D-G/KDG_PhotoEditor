#pragma once
#include "font.h"
#include <vector>
#include <string>
#include <stb_image.h>

namespace KDG_PhotoEditor{
	class Layer{
	private:
		std::vector<std::vector<unsigned int>> data; //unsigned int because 4 bytes and we are using RGBA

		int width, height, channels;
	public:
		Layer(std::string file_path);
		Layer();
		~Layer();

		inline std::vector<std::vector<unsigned int>> get_data(){return data;}

		void rotate_layer(float degrees);
		void scale_layer(float scale_factor);
		void translate_layer(int xmov, int ymov);
		void reflect_layer(std::string line); //Line meaning something in the form y=mx+c where they are floats

		void blur_circle(int x, int y, int r);
		void blur_rectangle(int x, int y, int w, int h);

		void set_pixel(int x, int y, int r, int g, int b, int a);

		void add_text(std::string text, Font font, int x, int y); //X and Y is the top left coord of the text

		void colour_filter(int r, int g, int b);

		void crop(int lmi, int rmi, int tmi, int bmi);
	};
}