
#include "usredniarka.h"
#include "tim.h"



float usrednianie(float probka){
	//float avg_probka_new = (NUMBER_OF_SAMPLES-1)/NUMBER_OF_SAMPLES * avg_probki_old + 1/NUMBER_OF_SAMPLES*probka;
	static uint16_t nrProbki;
	static float probki;

	if(nrProbki == NUMBER_OF_SAMPLES ){
		nrProbki = 0;
		float temp = probki/NUMBER_OF_SAMPLES;
		probki = 0;
		return temp;

	}
	else{
		probki = probki + probka;
		nrProbki++;
        return 25.0;
	}
}