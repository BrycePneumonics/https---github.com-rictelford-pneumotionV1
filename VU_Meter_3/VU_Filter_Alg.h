/*
 * VU_Filter_Alg.h
 *
 *  Created on: Mar 10, 2016
 *      Author: Pneumonics
 */

#ifndef VU_FILTER_ALG_H_
#define VU_FILTER_ALG_H_
void FilterInit(void);
void makeenvelope(const uint32_t *, uint32_t *,uint32_t *,float *);
void myiir(float inval, float *, float *, float *, float *, uint16_t);


#endif /* VU_FILTER_ALG_H_ */
