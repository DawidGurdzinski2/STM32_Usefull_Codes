#include "distance_sensor.h"
#include "vl53l1x_api.h"
#define dev 0x52  // address of IR distance sensor



void IR_Init()
{
	uint8_t sensorState=0;
	 while(sensorState==0){
			VL53L1X_BootState(dev, &sensorState);
		HAL_Delay(2);
	  }


	  /* This function must to be called to initialize the sensor with the default setting  */
	  VL53L1X_SensorInit(dev);
	  /* Optional functions to be used to change the main ranging parameters according the application requirements to get the best ranging performances */
	  VL53L1X_SetDistanceMode(dev, 1); /* 1=short, 2=long */
	  VL53L1X_SetTimingBudgetInMs(dev, 50); /* in ms possible values [20, 50, 100, 200, 500] */
	  VL53L1X_SetInterMeasurementInMs(dev, 100); /* in ms, IM must be > = TB */
	//  status = VL53L1X_SetOffset(dev,20); /* offset compensation in mm */
	//  status = VL53L1X_SetROI(dev, 16, 16); /* minimum ROI 4,4 */
	//	status = VL53L1X_CalibrateOffset(dev, 140, &offset); /* may take few second to perform the offset cal*/
	//	status = VL53L1X_CalibrateXtalk(dev, 1000, &xtalk); /* may take few second to perform the xtalk cal */
}


uint16_t IR_Get_Distance()
{
	uint8_t sensorState=0;
	uint16_t Distance;
	uint8_t dataReady;


//	if(VL53L1X_BootState(dev, &sensorState)==0)
//	{
//		return 1;
//	}

	VL53L1X_StartRanging(dev);

	while (dataReady == 0){
			  VL53L1X_CheckForDataReady(dev, &dataReady);
			  HAL_Delay(2);
		  }
		  dataReady = 0;
		  VL53L1X_GetDistance(dev, &Distance);

		  VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt*/
		  VL53L1X_StopRanging(dev);
		  return Distance;
}
