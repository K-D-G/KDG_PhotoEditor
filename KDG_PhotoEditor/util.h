#pragma once
#include <vector>

namespace KDG_PhotoEditor{
	struct float_v2{
		float x, y;

		float_v2(float x_, float y_):x(x_), y(y_){
		
		}
	};

	//Modified version of code from:https://www.thecrazyprogrammer.com/2017/01/bresenhams-line-drawing-algorithm-c-c.html
	std::vector<float_v2> bresenham_algorithm(int x0, int y0, int x1, int y1){
		std::vector<float_v2> result;
		int dx=x1-x0;
		int dy=y1-y0;

		int x=x0;
		int y=y0;

		int p=2*dy-dx;
		while(x<x1){
			if(p>=0){
				result.emplace_back(x, y);
				y++;
				p+=2*dy-2*dx;
			}else{
				result.emplace_back(x, y);
				p+=2*dy;
			}
			x++;
		}
		return result;
	}

}