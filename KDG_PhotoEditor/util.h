#pragma once
#include <vector>
#include <assert.h>

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

	//Help from:http://members.chello.at/~easyfilter/bresenham.html
	std::vector<float_v2> bezier_bresenham_algorithm(int x0, int y0, int x1, int y1, int x2, int y2){
		std::vector<float_v2> result;
		int sx=x2-x1;
		int sy=y2-y1;
		long xx=x0-x1;
		long yy=y0-y1;
		long xy;
		double dx, dy, err, ed, cur=xx*sy-yy*sx;

		assert(xx*sx>=0&&yy*sy>=0);

		if(sx*(long)sx+sy*(long)sy>xx*xx+yy*yy){
			x2=x0;
			x0=sx+x1;
			y2=y0;
			y0=sy+y1;
			cur=-cur;
		}

		if(cur!=0){
			xx+=sx;
			xx*=sx=x0<x2?1:-1;
			yy+=sy;
			yy*=sy=y0<y2?1:-1;
			if(cur*sx*sy<0){
				xx=-xx;
				yy=-yy;
				xy=-xy;
				cur=-cur;
			}
			dx=4.0*sy*(x1-x0)*cur+xx-xy;
			dy=4.0*sx*(y0-y1)*cur+yy-xy;
			do{
				cur=fmin(dx+xy, -xy-dy);
				ed=fmax(dx+xy, -xy-dy);
				ed=255/(ed+2*ed*cur*cur/(4.*ed*ed+cur*cur));
				//setPixelAA(x0, y0, ed*fabs(err-dx-dy-xy));
				if(x0==x2&&y0==y2){return result;}
				x1=x0; 
				cur=dx-err;
				y1=2*err+dy<0;
				if(2*err+dx>0){
					if(err-dy<ed){
						//setPixelAA(x0, y0+sy, ed*fabs(err-dy));
						result.emplace_back(x0, y0+sy);
					}
					x0+=sx;
					dx-=xy;
					err+=dx+=xx;
				}
				if(y1){
					if(cur<ed){
						//setPixelAA(x1+sx, y0, ed*fabs(cur));
						result.emplace_back(x1+sx, y0);
						y0+=sy;
						dy-=xy;
						err+=dx+=xx;
					}
				}

			}while(dy<dx);
		}
		//plotLineAA(x0, y0, x2, y2);
		std::vector<float_v2> rest_of_line=bresenham_algorithm(x0, y0, x2, y2);
		for(int i=0; i<rest_of_line.size(); i++){
			result.push_back(rest_of_line[i]);
		}
		return result;
	}
}