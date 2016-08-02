/*
 * matrix_math.c
 *
 *  Created on: Apr 23, 2016
 *      Author: Pneumonics
 */
#include <stdint.h>
#include <math.h>
#include <float.h>

void cross_product(int16_t * in_mat1, int16_t *in_mat2, int16_t *out_mat){
	int n,k;
	int b[3][2]={{1,2},{0,2},{0,1}};
	float detval1=0,detval2=0,tmat[3],utmat[3];
	for (k=0;k<3;k++){
		detval1=0;
		detval2=0;
			detval1+=in_mat1[b[k][0]];
			detval1*=in_mat2[b[k][1]];
			detval2+=in_mat1[b[k][1]];
			detval2*=in_mat2[b[k][0]];
			detval1-=detval2;
			tmat[k]=detval1;
			if (detval1<0)
				utmat[k]=-1*detval1;
			else
				utmat[k]=detval1;

	}
	detval1=utmat[0];
	for (k=0;k<2;k++){
		if (utmat[k]>utmat[k+1])
			detval1=utmat[k+1];
	}
	for (k=0;k<3;k++){
		detval2=tmat[k]/detval1;
		out_mat[k]=detval2;
	}


}

void angle_difference(int16_t *in_vec1,int16_t *in_vec2, float *angle){
	float tval1,tval2,tval3,temp,signvec[]={1, -1, -1};
	int k;
	tval1=0;
	tval2=0;
	for (k=0;k<3;k++){
		tval1=in_vec1[k];
		tval1*=in_vec2[k];
//		tval1*=signvec[k];
		tval2+=tval1;
	}
	tval1=0;
	tval3=0;
	for (k=0;k<3;k++){
//		temp=in_vec1[k];
		tval1+=(in_vec1[k]*in_vec1[k]);
		tval3+=(in_vec2[k]*in_vec2[k]);

	}
	tval1=sqrt(tval3)*sqrt(tval1);
	tval1=tval2/tval1;
//	if (tval1>1)
//		tval1=0;
	angle[0]=acos(tval1);
}
