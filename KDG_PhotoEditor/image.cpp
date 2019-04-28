#include "image.h"

using namespace KDG_PhotoEditor;
using namespace std;

Image::Image(string path){
	//Flip the backslashes to forward slashes for later use
	while(path.find("\\")){
		path.replace(path.find("\\"), 1, "/");
	}

	//See if the file is of our type if so read it in
	if(path.substr(path.rfind(".")+1, 3)=="kgp"){
		struct stat results;
		int size;
		if(stat(path.c_str(), &results)==0){
			size=results.st_size;
		}else{
			return;
		}

		ifstream file(path, ios::in|ios::binary);
		char* buffer=new char[size];
		file.read(buffer, size);
		if(!file){
			return;
		}
		string str_buffer=buffer;

		meta_data.name_length=(buffer[0]<<24)|(buffer[1]<<16)|(buffer[2]<<8)|buffer[3];
		meta_data.name=(char*)((str_buffer.substr(4, meta_data.name_length).c_str()));
		meta_data.width=(buffer[4+meta_data.name_length]<<24)|(buffer[4+meta_data.name_length+1]<<16)|(buffer[4+meta_data.name_length+2]<<8)|(buffer[4+meta_data.name_length+3]);
		meta_data.height=(buffer[4+meta_data.name_length+4]<<24)|(buffer[4+meta_data.name_length+5]<<16)|(buffer[4+meta_data.name_length+6]<<8)|(buffer[4+meta_data.name_length+7]);		
		meta_data.last_edited=(buffer[4+meta_data.name_length+8]<<24)|(buffer[4+meta_data.name_length+9]<<16)|(buffer[4+meta_data.name_length+10]<<8)|(buffer[4+meta_data.name_length+11]);
		meta_data.created_at=(buffer[4+meta_data.name_length+12]<<24)|(buffer[4+meta_data.name_length+13]<<16)|(buffer[4+meta_data.name_length+14]<<8)|(buffer[4+meta_data.name_length+15]);
		meta_data.number_of_layers=(buffer[4+meta_data.name_length+16]<<24)|(buffer[4+meta_data.name_length+17]<<16)|(buffer[4+meta_data.name_length+6]<<18)|(buffer[4+meta_data.name_length+19]);
		int offset=4+meta_data.name_length+20;

		layers.reserve(meta_data.number_of_layers);
		for(int i=0; i<meta_data.number_of_layers; i++){
			layers.emplace_back((unsigned char*)(str_buffer.substr(offset+(i*meta_data.width*meta_data.height), meta_data.width*meta_data.height).c_str()), meta_data.width, meta_data.height);
		}
	}else{
		//Construct a layer with the path this will use the
		//constructor which uses stb_image to load in image data
		layers.emplace_back(path);
	}
}

Image::~Image(){
	//Deleting the cache folder so it doesn't take up space on the users machine
	//Since I am using the system function preprocessor statements
	//As the system command for deleting folders is OS specific
	string delete_folder_path=image_path;
	delete_folder_path.substr(0, delete_folder_path.rfind("/"));
#ifdef _WIN32
	system(strcat((char*)"rmdir", delete_folder_path.c_str()));
#endif

#if defined(__APPLE__)||defined(__linux__)
	system(strcat((char*)"rm -rf", delete_folder_path.c_str()));
#endif
}

//Uses the image_path as standard
void Image::save(string path=""){
	//Create the meta data
	MetaData write_meta_data;
	write_meta_data.name_length=path.length();
	write_meta_data.name=(char*)path.c_str();
	write_meta_data.last_edited=(unsigned long long int)time(0);
	write_meta_data.created_at=(meta_data.created_at)?(meta_data.created_at):((unsigned long long int)time(0));
	write_meta_data.number_of_layers=layers.size();

	//Determin which path we are using
	string file_path=(path!="")?(path):(image_path);

	//Write the data to the file and close it
	ofstream file(file_path, ofstream::out|ofstream::binary);
	file.write((const char*)&write_meta_data, sizeof(write_meta_data));
	for(int i=0; i<layers.size(); i++){
		auto write_data=layers[i].get_data();
		file.write((const char*)&write_data, sizeof(write_data));
	}
	file.close();

	//Deleting the cache folder
	string delete_folder_path=image_path;
	delete_folder_path.substr(0, delete_folder_path.rfind("/"));
#ifdef _WIN32
	system(strcat((char*)"rmdir", delete_folder_path.c_str()));
#endif

#if defined(__APPLE__)||defined(__linux__)
	system(strcat((char*)"rm -rf", delete_folder_path.c_str()));
#endif
}

//Following functions convert the data into a
//unsigned char* array and then use stb_image to 
//write the data to a png or jpeg file

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

	stbi_write_jpg(export_path.c_str(), new_data.size(), new_data[0].size(), 0, (const void*)final_data, 100);
}

//Creates a blank new layer
void Image::new_layer(){
	layers.emplace_back(width, height);
}

//Deletes a layer PERMANENTLY
void Image::delete_layer(int layer_number){
	layers.erase(layers.begin()+layer_number);
}

//Swap the layer order so they are rendered in a different order
void Image::swap_layers(int current_layer_number, int next_layer_number){
	swap(layers[current_layer_number], layers[next_layer_number]);
}

//Rotate a layer using degrees NOT radians
void Image::rotate(int layer_number, float degrees, int centrex, int centrey){
	layers[layer_number].rotate(degrees, centrex, centrey);
	push_undo();
}

//Scale a layer using nearest neighbour or linear interpolation
void Image::scale(int layer_number, float sx, float sy, string type){
	layers[layer_number].scale(sx, sy, type);
	push_undo();
}

//Translate a layer so it appears in a different location
void Image::translate(int layer_number, int xmov, int ymov, bool make_transparent){
	layers[layer_number].translate(xmov, ymov, make_transparent);
	push_undo();
}

//Line meaning something like x=4 or y=2, the bool means are those sides the ones being reflected
void Image::reflect(int layer_number, char var_name, int val, bool left_or_top){
	layers[layer_number].reflect(var_name, val, left_or_top);
	push_undo();
}

//Blur a layer using a gaussian blur
void Image::blur(int layer_number, int topleftx, int toplefty, int w, int h){
	layers[layer_number].blur(topleftx, toplefty, w, h);
	push_undo();
}

//Change the colour of a pixel
//Doesn't do a push_undo as it would mean that there would be alot
//of copies
void Image::set_pixel(int layer_number, int x, int y, char r, char g, char b, char a){
	layers[layer_number].set_pixel(x, y, r, g, b, a);
}

//x and y is the top left coord of the text
void Image::add_text(int layer_number, string text, Font font, int x, int y){
	layers[layer_number].add_text(text, font, x, y);
	push_undo();
}

//Image that creates a filter (basically each layer is multiplied by some values)
//Remember each pixel will be multiplied with the values
void Image::colour_filter(int layer_number, char r, char g, char b, char a){
	layers[layer_number].colour_filter(r, g, b, a);
	push_undo();
}

//Remove a part of an image
void Image::crop(int layer_number, int lmi, int rmi, int tmi, int bmi){
	layers[layer_number].crop(lmi, rmi, tmi, bmi);
	push_undo();
}


//Undo an redo system explained in the .h file
void Image::undo(){
	for(int i=0; i<layers.size(); i++){
		layers[i].undo();
	}
	undo_stack.pop();
	push_redo();
}

void Image::redo(){
	for(int i=0; i<layers.size(); i++){
		layers[i].redo();
	}
	redo_stack.pop();
	push_undo();
}

void Image::push_undo(){
	string path=image_path.substr(0, image_path.rfind("/"));
	path.append(".cache/undo/version_");
	string command=image_path.substr(0, image_path.rfind("/"));
	path.append(to_string(undo_stack.size()));
	path.append("/");
	command.append(path);
	system(strcat((char*)"mkdir", command.c_str()));
	path.append("layer_");
	
	undo_stack.push(undo_stack.size());
	string temp=path;
	for(int i=0; i<layers.size(); i++){
		temp.append(to_string(i));
		layers[i].push_undo(temp);
	}
}

void Image::push_redo(){
	string path=image_path.substr(0, image_path.rfind("/"));
	path.append(".cache/redo/version_");
	string command=image_path.substr(0, image_path.rfind("/"));
	path.append(to_string(redo_stack.size()));
	path.append("/");
	command.append(path);
	system(strcat((char*)"mkdir", command.c_str()));
	path.append("layer_");

	redo_stack.push(redo_stack.size());
	string temp=path;
	for(int i=0; i<layers.size(); i++){
		temp.append(to_string(i));
		layers[i].push_redo(temp);
	}
}