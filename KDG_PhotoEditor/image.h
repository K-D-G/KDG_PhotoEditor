#pragma once
#include "layer.h"
#include "font.h"
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

namespace KDG_PhotoEditor{
	class Image{
	private:
		int width, height;
		int channels;

		std::vector<Layer> layers;

		std::stack<std::string> undo_stack; //The string in here tells us what to do (the opposite of what was done previously) so we just analyse and run the function. Using scanf
		std::stack<std::string> redo_stack; //Also add an option to the functions that asks if they should be recorded

		std::string image_path;
	public:
		Image(std::string path);
		~Image();

		void save(std::string path=NULL); //Uses the image_path as standard
		void export_png(std::string export_path);
		void export_jpg(std::string export_path);

		void new_layer();
		void delete_layer(int layer_number);
		void swap_layers(int current_layer_number, int next_layer_number);
		
		void rotate(int layer_number, float degrees, int centrex, int centrey);
		void scale(int layer_number, float sx, float sy, std::string type);
		void translate(int layer_number, int xmov, int ymov, bool make_transparent);
		void reflect(int layer_number, char var_name, int val, bool left_or_top); //Line meaning something like x=4 or y=2, the bool means are those sides the ones being reflected
		void blur(int layer_number, int topleftx, int toplefty, int w, int h);
		void set_pixel(int layer_number, int x, int y, char r, char g, char b, char a);
		void add_text(int layer_number, std::string text, Font font, int x, int y); //X and Y is the top left coord of the text
		void colour_filter(int layer_number, char r, char g, char b, char a); //Remember each pixel will be multiplied with the values
		void crop(int layer_number, int lmi, int rmi, int tmi, int bmi);
	
		void undo();
		void redo();
	};
}