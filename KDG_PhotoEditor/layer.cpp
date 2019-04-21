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

void Layer::rotate_layer(float degrees, int centrex, int centrey){
	//Loop through all of the pixels
	for(int x=0; x<width; x++){
		for(int y=0; y<height; y++){
			//"Translate" to the centre of origin
			//Then use the equations. 
			//x_new=y*sin(theta)+x*cos(theta)
			//y_new=y*cos(theta)-x*sin(theta)
			//Where theta is a clockwise rotation
			int x_new=(y-centrey)*sin(degrees)+(x-centrex)*cos(degrees);
			int y_new=(y-centrey)*cos(degrees)-(x-centrex)*sin(degrees);
			try{
				data[x_new][y_new]=data[x][y];
			}catch(...){
			
			}
		}
	}
}
void Layer::scale_layer(float scale_factor, int centrex, int centrey){

}

void Layer::translate_layer(int xmov, int ymov, bool make_transparent){
	std::vector<std::vector<unsigned int>> temp=data;
	for(int x=0; x<width; x++){
		for(int y=0; y<height; y++){
			try{
				temp[x][y]=data[x-xmov][y-ymov];
			}catch(...){
				if(make_transparent){
					temp[x][y]=0;
				}else{
					temp[x][y]=2147483647;
				}
			}
		}
	}
}
//Line meaning something like x=4 or y=2
void Layer::reflect_layer(char var_name, int val, bool left_or_top){
	//Find the equation in ax+by+c=0 form
	int first_point[2]={val, 4};
	int second_point[2]={val, 2};
	int a=first_point[1]-second_point[1];
	int b=second_point[0]-first_point[0];
	int c=(first_point[0]*second_point[1])-(second_point[0]*first_point[1]);
	if(var_name==(char)"x"){	
		//Loop through all of the pixels on the correct side of the line
		if(left_or_top){
			for(int x=0; x<val; x++){
				for(int y=0; y<height; y++){
					//Find the distance between the point and the line then add the distance to the opposite side of the line and copy the colour
					int distance=abs(a*x+b*y+c)/sqrt(a*a+b*b);
					data[x+distance*2][y]=data[x][y];
				}
			}
		}else{
			for(int x=val; x<width; x++){
				for(int y=0; y<height; y++){
					int distance=abs(a*x+b*y+c)/sqrt(a*a+b*b);
					data[x-distance*2][y]=data[x][y];
				}
			}
		}
	}else if(var_name==(char)"y"){
		if(left_or_top){
			for(int y=0; y<val; y++){
				for(int x=0; x<width; x++){
					int distance=abs(a*x+b*y+c)/sqrt(a*a+b*b);
					data[x][y+distance*2]=data[x][y];
				}
			}
		}else{
			for(int y=val; y<height; y++){
				for(int x=0; x<width; x++){
					int distance=abs(a*x+b*y+c)/sqrt(a*a+b*b);
					data[x][y-distance*2]=data[x][y];
				}
			}
		}
	}
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
	for(int x=0; x<width; x++){
		for(int y=0; y<height; y++){
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