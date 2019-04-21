#include "layer.h"

using namespace KDG_PhotoEditor;
using namespace std;

Layer::Layer(string file_path){
	unsigned char* temp_data=stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	data.reserve(width);
	for(int i=0; i<width; i++){
		vector<unsigned int> temp;
		temp.reserve(height);
		for(int j=0; j<height; j++){
			temp.emplace_back((temp_data[i*j]<<24)|(temp_data[i*j+1]<<16)|(temp_data[i*j+2])<<8|(temp_data[i*j+3]));
		}
		data.emplace_back(temp);
	}
	original_data=data;
}
Layer::Layer(){

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

void Layer::set_pixel(int x, int y, char r, char g, char b, char a){
	data[x][y]=(r<<24)|(g<<16)|(b<<8)|a;
}

//X and Y is the top left coord of the text
void Layer::add_text(string text, Font font, int x, int y){

}

void Layer::colour_filter(char r, char g, char b, char a){
	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			char rcomponent=r*((char)data[x][y]>>24);
			char gcomponent=g*(((char)data[x][y]<<8)>>24);
			char bcomponent=b*(((char)data[x][y]<<16)>>24);
			char acomponent=a*(((char)data[x][y]<<24)>>24);

			data[x][y]=(rcomponent<<24)|(gcomponent<<16)|(bcomponent<<8)|acomponent;
		}
	}
}

void Layer::crop(int lmi, int rmi, int tmi, int bmi){

}