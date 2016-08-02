/*
 * VU_Filter_Alg.c
 *
 *  Created on: Mar 10, 2016
 *      Author: Pneumonics
 */

#include <string.h>
#include <filter.h>
#include <fract2float_conv.h>
#include <stdio.h>
#include <stdint.h>
#define SAMPLES_PER_CHAN   400u
#define NUM_STAGES          2

fract32 delayLeft[(4 * NUM_STAGES) + 2];
fract32 delayRight[(4 * NUM_STAGES) + 2];

fract32 df1_coeffs[(4 * NUM_STAGES) + 2];

fract32 inLeft[SAMPLES_PER_CHAN];
fract32 inRight[SAMPLES_PER_CHAN];

fract32 outLeft[SAMPLES_PER_CHAN];
fract32 outRight[SAMPLES_PER_CHAN];
const long double a_coeffs[(2 * NUM_STAGES)]     = {-3.90629342L, 5.74132017L, -3.76262386L, 0.92766971L};
const long double b_coeffs[(2 * NUM_STAGES) + 1] = {0.01004288L, -0.03833192L, 0.05662947L, -0.03833192L, 0.01004288L};

iirdf1_state_fr32 stateLeft;
iirdf1_state_fr32 stateRight;


void FilterInit(void)
{
	int i;

	/* Configure filter state */
	iirdf1_init (stateLeft, df1_coeffs, delayLeft, NUM_STAGES);
	iirdf1_init (stateRight, df1_coeffs, delayRight, NUM_STAGES);

	/* Zero delay line to start or reset the filter */
	for (i = 0; i < ((4 * NUM_STAGES) + 2); i++)
	{
		delayLeft[i] = 0;
		delayRight[i] = 0;
	}

	coeff_iirdf1_fr32 (a_coeffs, b_coeffs, df1_coeffs, NUM_STAGES);

	//	if (mode == LOW_PASS)
	//	{
	//		coeff_iirdf1_fr32 (a_coeffs_low_1khz, b_coeffs_low_1khz, df1_coeffs, NUM_STAGES);
	//	}
	//	else if (mode == HIGH_PASS)
	//	{
	//		coeff_iirdf1_fr32 (a_coeffs_high_1khz, b_coeffs_high_1khz, df1_coeffs, NUM_STAGES);
	//	}
}


void myiir(float inval, float *a_vals, float *b_vals, float *b_vec, float *a_vec, uint16_t order){
	uint16_t k;
	for(k=0;k<order;k++){
		b_vals[k]=b_vals[k+1];
	}
	b_vals[order]=inval;
	for(k=0;k<order;k++){
		a_vals[k]=a_vals[k+1];
	}
	a_vals[order]=0;
	for(k=0;k<=order;k++){
		a_vals[order]+=b_vec[k]*b_vals[order-k];
	}
	for(k=0;k<order;k++){
		a_vals[order]-=a_vec[k+1]*a_vals[order-k+1];
	}
}

void makeenvelope(const uint32_t dataIn[], uint32_t dataOut[],int32_t *AudioEnvVal, float *meanval){
	int n;
	int i,l;
	fract32 fmeanval=0;
	float tval;
	int myval;
	/* separate channels (2D DMA would be better) */
	i = 0;
	for (n=0; n<SAMPLES_PER_CHAN; n++){
		inLeft[n] = dataIn[i++];
		inRight[n] = dataIn[i++];
		outLeft[n]=inLeft[n];
		outRight[n]=inRight[n];

	}

	/* left channel filter */
//	iirdf1_fr32 (inLeft, outLeft, SAMPLES_PER_CHAN, &stateLeft);

	/* right channel filter */
//		iirdf1_fr32 (inRight, outRight, SAMPLES_PER_CHAN, &stateRight);
	AudioEnvVal[0]=0;
	tval=0;
	for (n=0;n<SAMPLES_PER_CHAN;n++){
		tval+=outLeft[n];
	}
	tval/=SAMPLES_PER_CHAN;
	fmeanval=tval;
	tval=0;
	for (n=0;n<SAMPLES_PER_CHAN;n++){
		if (outLeft[n]<fmeanval){
//			tval+=-1*fr32_to_float(outLeft[n]);
			tval+=-outLeft[n];
		}
		else{
//			tval+=fr32_to_float(outLeft[n]);
			tval+=outLeft[n];
		}

//		send_mcu_data(tval, meanval[0],meanval);
	}
//	tval-=meanval[0];
//	if (tval<0)
//		tval=0;
	tval/=100;
	tval/=SAMPLES_PER_CHAN;
//	tval+=1000;
	if (tval<0)
	tval=0;
	AudioEnvVal[0]=tval;

//	/* combine channels (2D DMA would be better) */
	i = 0;
	meanval[0]=0;
	for (n=0; n<SAMPLES_PER_CHAN; n++){
		dataOut[i++] = outLeft[n];
		dataOut[i++] = outLeft[n];
		meanval[0]+=(outLeft[n]);

	}
	meanval[0]/=SAMPLES_PER_CHAN;


}
