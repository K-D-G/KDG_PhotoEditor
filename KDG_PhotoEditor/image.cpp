#include "image.h"

using namespace KDG_PhotoEditor;
using namespace std;

Image::Image(string path){
	layers.emplace_back(path);
}

Image::~Image(){

}

//Uses the image_path as standard
void Image::save(string path=NULL){

}

void Image::export_png(string export_path){
	vector<vector<unsigned int>> new_data;
	for(int i=0; i<layers.size(); i++){
		vector<vector<unsigned int>> layer_data=layers[i].get_data();
		for(int x=0; x<layer_data.size(); x++){
			for(int y=0; y<layer_data[x].size(); y++){
				if((layer_data[x][y]<<24)>>24<255){
					new_data[x][y]=layer_data[x][y];
				}
			}
		}
	}
	char* final_data;
	for(int x=0; x<new_data.size(); x++){
		strcat(final_data, (char*)new_data[x].data());
	}

	stbi_write_png(export_path.c_str(), new_data.size(), new_data[0].size(), 0, (const void*)final_data, 0);
}

void Image::export_jpg(string export_path){

}

void Image::new_layer(){
	layers.emplace_back();
}

void Image::delete_layer(int layer_number){
	layers.erase(layers.begin()+layer_number);
}

void Image::swap_layers(int current_layer_number, int next_layer_number){
	swap(layers[current_layer_number], layers[next_layer_number]);
}

void Image::rotate(int layer_number, float degrees, int centrex, int centrey){
	layers[layer_number].rotate(degrees, centrex, centrey);
}

void Image::scale(int layer_number, float sx, float sy, string type){
	layers[layer_number].scale(sx, sy, type);
}

void Image::translate(int layer_number, int xmov, int ymov, bool make_transparent){
	layers[layer_number].translate(xmov, ymov, make_transparent);
}

//Line meaning something like x=4 or y=2, the bool means are those sides the ones being reflected
void Image::reflect(int layer_number, char var_name, int val, bool left_or_top){
	layers[layer_number].reflect(var_name, val, left_or_top);
}

void Image::blur(int layer_number, int topleftx, int toplefty, int w, int h){
	layers[layer_number].blur(topleftx, toplefty, w, h);
}

void Image::set_pixel(int layer_number, int x, int y, char r, char g, char b, char a){
	layers[layer_number].set_pixel(x, y, r, g, b, a);
}

//X and Y is the top left coord of the text
void Image::add_text(int layer_number, string text, Font font, int x, int y){
	layers[layer_number].add_text(text, font, x, y);
}

//Remember each pixel will be multiplied with the values
void Image::colour_filter(int layer_number, char r, char g, char b, char a){
	layers[layer_number].colour_filter(r, g, b, a);
}

void Image::crop(int layer_number, int lmi, int rmi, int tmi, int bmi){
	layers[layer_number].crop(lmi, rmi, tmi, bmi);
}