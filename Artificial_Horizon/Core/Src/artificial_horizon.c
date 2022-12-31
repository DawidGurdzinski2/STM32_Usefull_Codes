#include "artificial_horizon.h"
#include "st7735.h"
#include <math.h>
extern float pitch_point; // od 1 do 160 na srodku ekranu punkt horyzontu w osi X
extern float width_point;
extern float height_point;
extern float triangle_point_w;
extern float triangle_point_h;

extern float alfay;
extern float alfax;

void setPitchAngle(float xg, float zg){
	if(zg > 0 && xg > 0){ // 1 cwiartka plaszczyzny gziemskiego i y samolot (obrot od 0 do 90 w prawo) alfa od 0 do pi/2
			alfax = acos(xg);

		}
		else if(zg > 0 && xg < 0){ // cwiartka 2 (obrot od 0 do -90 w prawo) alfa od pi/2 do pi
			alfax = acos(xg);

		}
		else if(zg < 0 && xg < 0){ // cwiartka 3 (obrot od -90 do -180 w prawo) alfa od pi do 3/4 pi
			alfax = 2*M_PI - acos(xg); // te same co w 2 cwiartce ale alfa wieksza o 90 stopni

		}
		else if(zg < 0 && xg > 0){ // cwiartka 4 (obrot od -180 do -270 w prawo) alfa od 3/4 pi do 2 pi
			alfax = 2*M_PI - acos(xg); //te same wartosci co w 1 cwiartce ale alfa wieksza o 270 stopni

		}
}
void setBankLine(float zg, float yg){


	if(zg > 0 && yg > 0){ // 1 cwiartka plaszczyzny gziemskiego i y samolot (obrot od 0 do 90 w prawo) alfa od 0 do pi/2
		alfay = acos(yg);
		height_point = sin(alfay-M_PI_2)*Image_radius;
		width_point = cos(alfay-M_PI_2)*Image_radius;
		triangle_point_h = sin(alfay)*Image_radius/5;
		triangle_point_w = cos(alfay)*Image_radius/5;
	}
	else if(zg > 0 && yg < 0){ // cwiartka 2 (obrot od 0 do -90 w prawo) alfa od pi/2 do pi
		alfay = acos(yg);
		height_point = sin(alfay-M_PI_2)*Image_radius;
		width_point = cos(alfay-M_PI_2)*Image_radius;
		triangle_point_h = sin(alfay)*Image_radius/5;
		triangle_point_w = cos(alfay)*Image_radius/5;
	}
	else if(zg < 0 && yg < 0){ // cwiartka 3 (obrot od -90 do -180 w prawo) alfa od pi do 3/4 pi
		alfay = 2*M_PI - acos(yg); // te same co w 2 cwiartce ale alfa wieksza o 90 stopni
		height_point = sin(alfay-M_PI_2)*Image_radius;
		width_point = cos(alfay-M_PI_2)*Image_radius;
		triangle_point_h = sin(alfay)*Image_radius/5;
		triangle_point_w = cos(alfay)*Image_radius/5;
	}
	else if(zg < 0 && yg > 0){ // cwiartka 4 (obrot od -180 do -270 w prawo) alfa od 3/4 pi do 2 pi
		alfay = 2*M_PI - acos(yg); //te same wartosci co w 1 cwiartce ale alfa wieksza o 270 stopni
		height_point = sin(alfay-M_PI_2)*Image_radius;
		width_point = cos(alfay-M_PI_2)*Image_radius;
		triangle_point_h = sin(alfay)*Image_radius/5;
		triangle_point_w = cos(alfay)*Image_radius/5;
	}
}

void setPitchPoint(float alfax){

	if(alfax >Horizon_1_high && alfax < Horizon_1_low){ //zakres normalny nachylenia od -60 do 60 stopni
	        pitch_point = ((240/M_PI)*alfax -40);

	    }
	    else if(alfax >Horizon_2_low && alfax < Horizon_2_high){
	        pitch_point = ((-240/M_PI)*alfax +440);

	    }
	    else if(alfax >Horizon_1_low && alfax < Horizon_2_low){
	        pitch_point = 160;

	    }
	    else if((alfax >0 && alfax < Horizon_1_high) || (alfax >Horizon_2_high && alfax < Air_high)){
	        pitch_point = 0;

	    }
}
