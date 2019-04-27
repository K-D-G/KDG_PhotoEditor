#include "layer.h"

using namespace KDG_PhotoEditor;
using namespace TTFFontParser;
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

Layer::Layer(unsigned char* d, int w, int h){
	width=w;
	height=h;
	data.reserve(width);
	for(int i=0; i<width; i++){
		vector<unsigned int> temp;
		temp.reserve(height);
		for(int j=0; j<height; j++){
			temp.emplace_back((d[i*j]<<24)|(d[i*j+1]<<16)|(d[i*j+2])<<8|(d[i*j+3]));
		}
		data.emplace_back(temp);
	}
	original_data=data;
}

Layer::Layer(int w, int h){
	width=w;
	height=h;
	data.reserve(width);
	for(int i=0; i<width; i++){
		vector<unsigned int> temp;
		for(int j=0; j<height; j++){
			temp.emplace_back(0);
		}
		data.emplace_back(temp);
	}
	original_data=data;
}

Layer::~Layer(){

}

void Layer::rotate(float degrees, int centrex, int centrey){
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
				//Translate back when assigning the values
				data[x_new+centrex][y_new+centrey]=data[x][y];
			}catch(...){
			
			}
		}
	}
}
void Layer::scale(float sx, float sy, string type){
	int new_width=width*sx;
	int new_height=height*sy;
	vector<vector<unsigned int>> new_data;
	new_data.reserve(new_width);
	for(int i=0; i<new_width; i++){
		new_data[i].reserve(new_height);
	}

	if(type=="linear_interpolation"){
		//Linear interpolation?
		for(int x=0; x<width; x++){
			for(int y=0; y<height; y++){
				new_data[x*new_width][y*new_height]=data[x][y];
				for(int i=1; i<sy; i++){
					new_data[x*new_width][y*new_height+i]=(((sy-i)/sy)*data[x][y])+((i/sy)*data[x][y]);
				}
			}
		}

		for(int y=0; y<new_height; y++){
			for(int x=0; x<width; x++){
				new_data[x*new_width][y*new_height]=data[x][y/sy];
				for(int i=1; i<sx; i++){
					new_data[x*new_width+i][y*new_height]=(((sx-i)/sx)*data[x][y/sy])+((i/sx)*data[x][y/sy]);
				}
			}
		}

	}else if(type=="nearest_neighbour"){
		//Loop through all the pixels in the original image
		for(int x=0; x<width; x++){
			for(int y=0; y<height; y++){
				for(int i=0; i<sy; i++){
					new_data[x*new_width][y*new_height+i]=data[x][y];
				}
			}
		}

		for(int y=0; y<new_height; y++){
			for(int x=0; x<width; x++){
				for(int i=0; i<sx; i++){
					new_data[x*new_width+i][y*new_height]=data[x][y/sy];
				}
			}
		}
	}
	width=new_width;
	height=new_height;
	data=new_data;
}

void Layer::translate(int xmov, int ymov, bool make_transparent){
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
void Layer::reflect(char var_name, int val, bool left_or_top){
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

//Gaussian blur. Implementation using the theory in the computerphile lecture
void Layer::blur(int topleftx, int toplefty, int w, int h){
	int kernel[3][3]={{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
	int total_kernel_value=0;

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			total_kernel_value+=kernel[i][j];
		}
	}

	vector<vector<unsigned int>> new_data;
	new_data.reserve(width);
	for(int i=0; i<width; i++){
		new_data[i].reserve(height);
	}

	for(int x=topleftx; x<topleftx+w; x++){
		for(int y=toplefty; y<toplefty+h; y++){
			int value=0;
			int kernel_value=0;
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					try{
						//Not sure about the subtraction. Tried to translate to the top left corner and then add i and j
						value+=(data[x-1+i][y-1+j]*kernel[i][j]);
						kernel_value+=kernel[i][j];
					}catch(...){
						
					}	
				}
			}
			new_data[x][y]=(int)value/kernel_value;
		}
	}

	for(int x=0; x<width; x++){
		for(int y=0; y<height; y++){
			if(!new_data[x][y]){
				new_data[x][y]=data[x][y];
			}
		}
	}

	data=new_data;
}

void Layer::set_pixel(int x, int y, char r, char g, char b, char a){
	data[x][y]=(r<<24)|(g<<16)|(b<<8)|a;
}

void KDG_PhotoEditor::font_parsed(void* args, void* font_data, int error){
	KDG_PhotoEditor::current_parsed_font={args, font_data, error};
}

//Add stuff for filling the text, colours, line thickness and stuff like that
//X and Y is the top left coord of the text
void Layer::add_text(string text, Font font, int x, int y){
	string full_path=FONT_BASE_PATH;
	full_path.append(font.name);
	full_path.append(".ttf");

	FontData font_data;
	uint8_t condition_variable=0;
	int8_t error=parse_file(full_path.c_str(), &font_data, &KDG_PhotoEditor::font_parsed, &condition_variable);

	while(!condition_variable&&!KDG_PhotoEditor::current_parsed_font.args){
		this_thread::sleep_for(chrono::milliseconds(1));
	}

	if(error||KDG_PhotoEditor::current_parsed_font.error){
		return;
	}

	FontData* font_data=(FontData*)KDG_PhotoEditor::current_parsed_font.font_data;

	KDG_PhotoEditor::current_parsed_font={};

	//Assuming with the ttf FontData that you put your character into the glyph map and then put that into the
	//glyphs structure and then you have the glyph
	vector<Glyph> glyphs;
	for(int i=0; i<text.length(); i++){
		glyphs.push_back(font_data.glyphs[font_data.glyph_map[(uint32_t)(text.c_str()[i])]]);
	}

	for(int i=0; i<glyphs.size(); i++){
		for(int j=0; j<glyphs[i].path_list.size(); j++){
			for(int k=0; k<glyphs[i].path_list[j].curves.size(); k++){
				vector<KDG_PhotoEditor::float_v2> points;
				if(glyphs[i].path_list[j].curves[k].is_curve){
					points=bezier_bresenham_algorithm(glyphs[i].path_list[j].curves[k].p0.x+x, glyphs[i].path_list[j].curves[k].p0.y+y, glyphs[i].path_list[j].curves[k].p1.x+x, glyphs[i].path_list[j].curves[k].p1.y+y, glyphs[i].path_list[j].curves[k].p2.x+x, glyphs[i].path_list[j].curves[k].p2.y+y);
				}else{
					points=bresenham_algorithm(glyphs[i].path_list[j].curves[k].p0.x+x, glyphs[i].path_list[j].curves[k].p0.y+y, glyphs[i].path_list[j].curves[k].p2.x+x, glyphs[i].path_list[j].curves[k].p2.y+y);
				}
				for(int p=0; p<points.size(); p++){
					set_pixel((int)points[p].x, (int)points[p].y, 0, 0, 0, 0);
				}
			}
		}
	}

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
	int new_width=width-(lmi+rmi);
	int new_height=height-(tmi+bmi);
	vector<vector<unsigned int>> new_data;
	new_data.reserve(new_width);
	for(int i=0; i<new_width; i++){
		new_data[i].reserve(new_height);
	}

	for(int x=lmi; x<=rmi; x++){
		for(int y=tmi; y<=bmi; y++){
			new_data[x-lmi][y-tmi]=data[x][y];
		}
	}

	width=new_width;
	height=new_height;
	data=new_data;
}