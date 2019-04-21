#pragma once
#include "layer.h"
#include "font.h"
#include <string>
#include <vector>
#include <stack>

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

		void new_layer(std::string name);
		void delete_layer(int layer_number);
		void move_layer(int current_layer_number, int next_layer_number);
		
		void rotate_layer(int layer_number, float degrees);
		void scale_layer(int layer_number, float scale_factor);
		void translate_layer(int layer_number, int xmov, int ymov);
		void reflect_layer(int layer_number, std::string line); //Line meaning something in the form y=mx+c where they are floats
		
		void blur_circle(int layer_number, int x, int y, int r);
		void blur_rectangle(int layer_number, int x, int y, int w, int h);

		void set_pixel(int layer_number, int x, int y, int r, int g, int b, int a);

		void add_text(int layer_number, std::string text, Font font, int x, int y); //X and Y is the top left coord of the text

		void colour_filter(int layer_number, int r, int g, int b);
		
		void crop(int layer_number, int lmi, int rmi, int tmi, int bmi);
	
		void undo();
		void redo();
	};
}