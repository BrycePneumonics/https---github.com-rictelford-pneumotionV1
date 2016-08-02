/*********************************************************************************

Copyright(c) 2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/

/*
 *
 * This example demonstrates how to use the ADAU1761 codec driver to receive audio
 * samples from the Line Input and transmit those audio samples to the headphone
 * output (talkthrough).
 *
 * On the ADSP-BF706 EZ-KIT Mini™:
 * Connect an audio source to the LINE IN jack (J1)
 * Connect headphones to the HP jack (J2).
 */

#include <drivers/codec/adau1761/adi_adau1761.h>
#include <services/pwr/adi_pwr.h>
#include <stdio.h>
#include <math.h>
/* SigmaStudio exported file */
#include "SigmaStudio\export\export_IC_1.h"

#include "VU_main.h"
#include "VU_Filter_Alg.h"
#include "I2C_Com.h"
#include "matrix_math.h"

/* ADI initialization header */
#include "system/adi_initialize.h"

/* User Macro - select sample rate */
#define SAMPLE_RATE  (ADI_ADAU1761_SAMPLE_RATE_8KHZ)

/* User Macro - select the number of audio samples (stereo) for each buffer */
#define NUM_AUDIO_SAMPLES      800u  /* 32 left + 32 right */

#define NUM_ACCEL_SAMPLES      10u
#define NUM_ACCEL_SAMPLES_LT   4u

#define MEAN_VEC_LEN 12u

#define ENV_VEC_LEN  20u

/* 32-bits per sample (24-bit audio) */
#define BUFFER_SIZE      (NUM_AUDIO_SAMPLES*sizeof(uint32_t))

/* used for exit timeout */
#define MAXCOUNT (500000u)

/*
 * SPORT device memory
 */
#pragma align(4)
static uint8_t sportRxMem[ADI_SPORT_DMA_MEMORY_SIZE];

#pragma align(4)
static uint8_t sportTxMem[ADI_SPORT_DMA_MEMORY_SIZE];

/*
 * Audio buffers
 */
#pragma align(4)
static uint32_t RxBuffer1[NUM_AUDIO_SAMPLES];

#pragma align(4)
static uint32_t RxBuffer2[NUM_AUDIO_SAMPLES];

#pragma align(4)
static uint32_t TxBuffer1[NUM_AUDIO_SAMPLES];

#pragma align(4)
static uint32_t TxBuffer2[NUM_AUDIO_SAMPLES];

//#pragma align(4)
//static uint32_t AbsBuffer[NUM_AUDIO_SAMPLES];




/* audio buffer pointers */
static uint32_t *pRxAudioBuffer;
static uint32_t *pTxAudioBuffer;
//static uint32_t AudioEnvVal;
//static uint32_t *pAbsAudioBuffer;



/* SPORT info struct */
static ADI_ADAU1761_SPORT_INFO sportRxInfo;
static ADI_ADAU1761_SPORT_INFO sportTxInfo;

typedef enum
{
	NONE,
	START_TALKTHROUGH,
	TALKTHROUGH,
	STOP_TALKTHROUGH
} MODE;

static MODE eMode = NONE;

/* Memory required for codec driver - must add PROGRAM_SIZE_IC_1 size for data transfer to codec */
static uint8_t codecMem[ADI_ADAU1761_MEMORY_SIZE + PROGRAM_SIZE_IC_1];

/* adau1761 device handle */
static ADI_ADAU1761_HANDLE  hADAU1761;

static bool bError;
static volatile uint32_t count;

/* error check */
static void CheckResult(ADI_ADAU1761_RESULT result)
{
	if (result != ADI_ADAU1761_SUCCESS) {
		printf("Codec failure\n");
		bError = true;
	}
}

/* codec record mixer */
static void MixerEnable(bool bEnable)
{
	ADI_ADAU1761_RESULT result;
	uint8_t value;

	if (bEnable)
	{
		/* enable the record mixer (left) */
		result = adi_adau1761_SetRegister (hADAU1761, REC_MIX_LEFT_REG, 0x0B); /* LINP mute, LINN 0 dB */
		CheckResult(result);

		/* enable the record mixer (right) */
		result = adi_adau1761_SetRegister (hADAU1761, REC_MIX_RIGHT_REG, 0x0B);  /* RINP mute, RINN 0 dB */
		CheckResult(result);
	}
	else
	{
		/* disable the record mixer (left) */
		result = adi_adau1761_GetRegister (hADAU1761, REC_MIX_LEFT_REG, &value);
		result = adi_adau1761_SetRegister (hADAU1761, REC_MIX_LEFT_REG, value & 0xFE);
		CheckResult(result);

		/* disable the record mixer (right) */
		result = adi_adau1761_GetRegister (hADAU1761, REC_MIX_RIGHT_REG, &value);
		result = adi_adau1761_SetRegister (hADAU1761, REC_MIX_RIGHT_REG, value & 0xFE);
		CheckResult(result);
	}
}

/* codec driver configuration */
static void CodecSetup(void)
{
	ADI_ADAU1761_RESULT result;

	/* Open the codec driver */
	result = adi_adau1761_Open(ADAU1761_DEV_NUM,
			codecMem,
			sizeof(codecMem),
			ADI_ADAU1761_COMM_DEV_TWI,
			&hADAU1761);
	CheckResult(result);

	/* Configure TWI - BF706 EZ-Kit MINI version 1.0 uses TWI */
	result = adi_adau1761_ConfigTWI(hADAU1761, TWI_DEV_NUM, ADI_ADAU1761_TWI_ADDR0);
	CheckResult(result);

	/* load Sigma Studio program exported from *_IC_1.h */
	result = adi_adau1761_SigmaStudioLoad(hADAU1761, default_download_IC_1);
	CheckResult(result);

	/* config SPORT for Rx data transfer */
	sportRxInfo.nDeviceNum = SPORT_RX_DEVICE;
	sportRxInfo.eChannel = ADI_HALF_SPORT_B;
	sportRxInfo.eMode = ADI_ADAU1761_SPORT_I2S;
	sportRxInfo.hDevice = NULL;
	sportRxInfo.pMemory = sportRxMem;
	sportRxInfo.bEnableDMA = true;
	sportRxInfo.eDataLen = ADI_ADAU1761_SPORT_DATA_24;
	sportRxInfo.bEnableStreaming = true;

	result = adi_adau1761_ConfigSPORT (hADAU1761,
			ADI_ADAU1761_SPORT_INPUT, &sportRxInfo);
	CheckResult(result);

	/* config SPORT for Tx data transfer */
	sportTxInfo.nDeviceNum = SPORT_TX_DEVICE;
	sportTxInfo.eChannel = ADI_HALF_SPORT_A;
	sportTxInfo.eMode = ADI_ADAU1761_SPORT_I2S;
	sportTxInfo.hDevice = NULL;
	sportTxInfo.pMemory = sportTxMem;
	sportTxInfo.bEnableDMA = true;
	sportTxInfo.eDataLen = ADI_ADAU1761_SPORT_DATA_24;
	sportTxInfo.bEnableStreaming = true;

	result = adi_adau1761_ConfigSPORT (hADAU1761,
			ADI_ADAU1761_SPORT_OUTPUT, &sportTxInfo);
	CheckResult(result);

	result = adi_adau1761_SelectInputSource(hADAU1761, ADI_ADAU1761_INPUT_ADC);
	CheckResult(result);

	/* disable mixer */
	MixerEnable(false);

	result = adi_adau1761_SetVolume (hADAU1761,
			ADI_ADAU1761_VOL_HEADPHONE,
			ADI_ADAU1761_VOL_CHAN_BOTH,
			true,
			57); /* 0 dB */
	CheckResult(result);

	result = adi_adau1761_SetSampleRate (hADAU1761, SAMPLE_RATE);
	CheckResult(result);
}


int main(void)
{
	bool bExit;
	bool bRxAvailable;
	bool bTxAvailable;
	float meanvector[MEAN_VEC_LEN];
	float meanvectoraccum=0;
//	float b1[]={ 0.067455,   0.134911,   0.067455};
//	float a1[]={1,-1.14298 ,  0.41280};
	float b1[]={.020083, .040167, .020083};
	float a1[]={1, -1.56102, 0.64135};
	float au_iir_in[]={0,0,0};
	float au_iir_out[]={0,0,0};
	uint32_t meanvecptr=0;
	uint16_t AudioEnvVal;
	float meanval;
	uint16_t envvector[ENV_VEC_LEN];
	float envvectoraccum=0;
	uint32_t envvecptr=0;
	uint32_t accelptr=0;
	float accel_accum[]={0,0,0,0};
	float temp;
	uint16_t t16;
	int16_t accelbuffer[3];
	int16_t accelbuffert[3];
	int16_t accelbufferave[NUM_ACCEL_SAMPLES][3];
	uint32_t accelptrlt=0;
	float accel_accum_lt[]={0,0,0,0};
	int16_t accelbufferlt[3];
	int16_t accelbufferavelt[NUM_ACCEL_SAMPLES][3];
	//	int16_t accelbufferaccum[NUM_ACCEL_SAMPLES][3];
	int k,n;
	int16_t inmat_1[]={2,1,-2};
	int16_t inmat_2[]={1,1,1};
	int16_t outmat[3];
	ADI_ADAU1761_RESULT result;
	ADI_PWR_RESULT pwrResult;

	bExit = false;
	/* setup processor mode and frequency */
	pwrResult = adi_pwr_Init(0, CLKIN);
	pwrResult = adi_pwr_SetPowerMode(0,	ADI_PWR_MODE_FULL_ON);
	pwrResult = adi_pwr_SetClkControlRegister(0, ADI_PWR_CLK_CTL_MSEL, MSEL);
	pwrResult = adi_pwr_SetClkDivideRegister(0, ADI_PWR_CLK_DIV_CSEL, CSEL);
	pwrResult = adi_pwr_SetClkDivideRegister(0, ADI_PWR_CLK_DIV_SYSSEL, SYSSEL);

	adi_initComponents(); /* auto-generated code */


	/* initialize filter and envelope maker */
	FilterInit();
	init_ADXL345();
	/* Establish the mean value vector */
	for (k=0;k<MEAN_VEC_LEN;k++){
		meanvector[k]=0;
	}
	for (k=0;k<NUM_ACCEL_SAMPLES;k++){
		for (n=0;n<3;n++){
			accelbufferave[k][n]=0;
			//			accelbufferaccum[k][n]=0;
		}
	}
	for (k=0;k<NUM_ACCEL_SAMPLES_LT;k++){
			for (n=0;n<3;n++){
				accelbufferavelt[k][n]=0;
				//			accelbufferaccum[k][n]=0;
			}
		}
	for (k=0;k<ENV_VEC_LEN;k++){
		envvector[k]=0;
	}

	/* configure the codec driver */
	CodecSetup();

	count = 0u;
	eMode = START_TALKTHROUGH;

	while(!bExit && !bError)
	{
		switch(eMode)
		{
		case START_TALKTHROUGH:

			/* stop current input */
			result = adi_adau1761_EnableInput (hADAU1761, false);
			CheckResult(result);

			/* stop current output */
			result = adi_adau1761_EnableOutput (hADAU1761, false);
			CheckResult(result);

			/* submit Rx buffer1 */
			result = adi_adau1761_SubmitRxBuffer(hADAU1761, RxBuffer1, BUFFER_SIZE);
			CheckResult(result);

			/* submit Rx buffer2 */
			result = adi_adau1761_SubmitRxBuffer(hADAU1761,	RxBuffer2, BUFFER_SIZE);
			CheckResult(result);

			/* submit Tx buffer1 */
			result = adi_adau1761_SubmitTxBuffer(hADAU1761, TxBuffer1, BUFFER_SIZE);
			CheckResult(result);

			/* submit Tx buffer2 */
			result = adi_adau1761_SubmitTxBuffer(hADAU1761,	TxBuffer2, BUFFER_SIZE);
			CheckResult(result);

			result = adi_adau1761_EnableInput (hADAU1761, true);
			CheckResult(result);

			result = adi_adau1761_EnableOutput (hADAU1761, true);
			CheckResult(result);

			/* enable record mixer */
			MixerEnable(true);

			eMode = TALKTHROUGH;
			break;
		case TALKTHROUGH:
			result = adi_adau1761_GetRxBuffer (hADAU1761, (void**)&pRxAudioBuffer);
			CheckResult(result);

			if (pRxAudioBuffer != NULL)
			{
				/* re-submit buffer */
				result = adi_adau1761_SubmitRxBuffer(hADAU1761, pRxAudioBuffer, BUFFER_SIZE);
				CheckResult(result);

				/* get an available Tx buffer */
				result = adi_adau1761_GetTxBuffer (hADAU1761, (void**)&pTxAudioBuffer);
				CheckResult(result);

				if (pTxAudioBuffer != NULL)
				{
					//  Average the mean value
					//					meanval=0;
					//					for (k=0;k<MEAN_VEC_LEN;k++){
					//						meanval+=(meanvector[k]/MEAN_VEC_LEN);
					//					}

					/*
					 * Filter the audio, return filtered audio, decimated envelope
					 */


					makeenvelope(pRxAudioBuffer,pTxAudioBuffer,&AudioEnvVal, &meanval);
					myiir(AudioEnvVal, au_iir_out, au_iir_in, b1, a1, 2);
					AudioEnvVal=au_iir_out[2];

					//
////					temp=0;
////					temp=AudioEnvVal;
////					temp/=ENV_VEC_LEN;
////					envvectoraccum+=(temp);
////					temp=0;
////					temp=envvector[envvecptr];
////					temp/=ENV_VEC_LEN;
////					envvectoraccum-=(temp);

//			f
////					if (envvectoraccum>10)
////						envvectoraccum=10;
////					meanvectoraccum+=(meanval/MEAN_VEC_LEN);
////					meanvectoraccum-=(meanvector[meanvecptr]/MEAN_VEC_LEN);
////					meanvector[meanvecptr]=meanvectoraccum;
////					meanval=meanvectoraccum;
////
////					meanvecptr++;
////					if (meanvecptr>MEAN_VEC_LEN)
////						meanvecptr=0;
//					//
//					//					meanvector[meanvecptr]=AudioEnvVal;
//					//					meanvecptr++;
//					//					if (meanvecptr>MEAN_VEC_LEN){
//					//						//						meanvector[meanvecptr2]=0;
//					//						//						for (k=0;k<MEAN_VEC_LEN;k++){
//					//						//
//					//						//						}
//					//						meanvecptr=0;
//					//					}
//					// store mean value from several 1000 sample gruops to establish mean value
//
//					/*  Retreive the accelerometer values*/
					get_data_ADXL345(accelbuffer);
//					//					accel_accum[0]=0;
//					//					accel_accum[1]=0;
//					//					accel_accum[2]=0;
//					//					for (k=0;k<NUM_ACCEL_SAMPLES;k++){
//					//					}
					for (k=0;k<3;k++){
						temp = accelbuffer[k];
						temp /= NUM_ACCEL_SAMPLES;
						accel_accum[k+1]+=(temp);
						temp=accelbufferave[accelptr][k];
						temp/=NUM_ACCEL_SAMPLES;
						accel_accum[k+1]-=(temp);
						accelbufferave[accelptr][k]=accelbuffer[k];
						accelbuffer[k]=(accel_accum[k+1]);
//						//						accelbufferaccum[accelptr][k]=accelbuffer[k];
					}
					accelptr++;
					if (accelptr>=NUM_ACCEL_SAMPLES){
//
						for (k=0;k<3;k++){
							temp=accelbuffer[k];
							temp/=NUM_ACCEL_SAMPLES_LT;
							accel_accum_lt[k+1]+=(temp);
							temp=accelbufferavelt[accelptrlt][k];
							temp/=NUM_ACCEL_SAMPLES_LT;
							accel_accum_lt[k+1]-=(temp);
							accelbufferavelt[accelptrlt][k]=accelbuffer[k];
							accelbufferlt[k]=accel_accum_lt[k+1];
						}
						accelptrlt++;
						if (accelptrlt>=NUM_ACCEL_SAMPLES_LT)
							accelptrlt=0;
						accelptr=0;
					}
//					/*  Filter accelerometer values (all three) */
//
//					/*  Rotate the matrix of the accelerometers and find the angle of most motion */
//
//					/*  Dump data to the uart/I2c */
//
////					angle_difference(accelbufferlt, accelbuffer, &temp);
//
////					temp*=573*4;
////					temp+=1810;
////					t16=temp;
					send_mcu_data(AudioEnvVal,AudioEnvVal, accelbuffer);

					/* talkthrough - copy the audio data from Rx to Tx */
					//										memcpy(&pTxAudioBuffer[0], &pRxAudioBuffer[0], BUFFER_SIZE);

					/*
					 * End audio algorithm
					 */

					/* re-submit buffer */
					result = adi_adau1761_SubmitTxBuffer(hADAU1761, pTxAudioBuffer, BUFFER_SIZE);
					CheckResult(result);
				}

			}
			break;
		case STOP_TALKTHROUGH:
			result = adi_adau1761_EnableInput (hADAU1761, false);
			CheckResult(result);

			result = adi_adau1761_EnableOutput (hADAU1761, false);
			CheckResult(result);

			eMode = NONE;
			bExit = true;
			break;

		default:
			break;
		}

		/* exit the loop after a while */
		if (count > MAXCOUNT)
		{
			eMode = STOP_TALKTHROUGH;
		}
		count++;

	}

	result = adi_adau1761_Close(hADAU1761);
	CheckResult(result);

	if (!bError) {
		printf("All done\n");
	} else {
		printf("Audio error\n");
	}

	return 0;
}
