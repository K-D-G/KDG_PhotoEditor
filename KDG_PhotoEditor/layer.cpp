#include "layer.h"

using namespace KDG_PhotoEditor;
using namespace std;

Layer::Layer(string file_path){
	unsigned char* temp_data=stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	for(int i=0; i<height; i++){
		vector<unsigned int> temp;
		for(int j=0; j<width; j++){
			temp.push_back((temp_data[i*j]<<24)|(temp_data[i*j+1]<<16)|(temp_data[i*j+2])<<8|(temp_data[i*j+3]));
		}
		data.push_back(temp);
	}
}
Layer::Layer(){
	for(int i=0; i<height; i++){
		vector<unsigned int> temp;
		for(int j=0; j<width; j++){
			temp.push_back(0);
		}
		data.push_back(temp);
	}
}
Layer::~Layer(){

}

void Layer::rotate_layer(float degrees){

}
void Layer::scale_layer(float scale_factor){

}
void Layer::translate_layer(int xmov, int ymov){

}
//Line meaning something in the form y=mx+c where they are floats
void Layer::reflect_layer(string line){

} 

void Layer::blur_circle(int x, int y, int r){

}
void Layer::blur_rectangle(int x, int y, int w, int h){

}

void Layer::set_pixel(int x, int y, int r, int g, int b, int a){

}

//X and Y is the top left coord of the text
void Layer::add_text(string text, Font font, int x, int y){

}

void Layer::colour_filter(int r, int g, int b){

}

void Layer::crop(int lmi, int rmi, int tmi, int bmi){

}