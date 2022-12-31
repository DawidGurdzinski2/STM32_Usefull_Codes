#ifndef INC_ARTIFICIAL_HORIZON_H_
#define INC_ARTIFICIAL_HORIZON_H_
#include "stdint.h"
#define Acc_X_max 16000
#define Acc_Y_max 16000
#define Acc_Z_max 16000
#define Image_radius 50
#define Horizon_1_low 2.618 //5/6*M_PI
#define Horizon_2_low 3.6652 //7/6*M_PI
#define Horizon_1_high 0.523 //M_PI/6
#define Horizon_2_high 5.7596 //11/6*M_PI
#define Air_high 6.2831 //2*M_PI




void setPitchAngle(float xg, float zg);

void setBankLine(float zg, float yg);

void setPitchPoint(float alfax);


#endif /* INC_ARTIFICIAL_HORIZON_H_ */
