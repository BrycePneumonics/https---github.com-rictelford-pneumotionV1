/*
 * I2C_Com.c
 *
 *  Created on: Mar 12, 2016
 *      Author: Pneumonics
 */

#include <drivers/twi/adi_twi.h>
#include "I2C_COm.h"

#define ADXLADDR 0x53

void init_ADXL345(void){
	char txbuffer[10];
	txbuffer[0]=0x2D;		//Power control address
	txbuffer[1]=0x08;		//Turn on the power control
	send_I2C(ADXLADDR,txbuffer,2);	//Transmit the power on sequence
	txbuffer[0]=0x2C;
	txbuffer[1]=0x07;
	send_I2C(ADXLADDR,txbuffer,2);

	txbuffer[0]=0x31;		//Resolution address Data Format
	txbuffer[1]=0x0C;		//Full range resolution set
	send_I2C(ADXLADDR,txbuffer,2);		//Transmit the resolution data
	txbuffer[0]=0x2D;		//Power control address
	txbuffer[1]=0x08;		//Turn on the power control
	send_I2C(ADXLADDR,txbuffer,2);		//Transmit the power on sequence
//	txbuffer[0]=0x2C;
//	txbuffer[1]=0x08;
//	send_I2C(ADXLADDR,txbuffer,2);

	txbuffer[0]=0x31;		//Resolution address Data Format
	txbuffer[1]=0x0C;		//Full range resolution set
	send_I2C(ADXLADDR,txbuffer,2);		//Transmit the resolution data

}



ADI_TWI_RESULT send_I2C(char hwaddress, char *buffer, uint32_t buflen){
	ADI_TWI_HANDLE hDevice;
	uint16_t  * Value;
	uint32_t counter;
	/* TWI driver memory */
	uint8_t driverMemory[ADI_TWI_MEMORY_SIZE];
	/* driver API result code */
	ADI_TWI_RESULT result;

	/* buffer which holds data to transfer over TWI */

	/* open the TWI driver in master mode */
	result = adi_twi_Open(0, ADI_TWI_MASTER, driverMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	result = adi_twi_SetBitRate(hDevice,100);
	result = adi_twi_SetDutyCycle(hDevice,50);
	//    result = adi_twi_SetPrescale(hDevice,100);
	result = adi_twi_SetHardwareAddress(hDevice,hwaddress);
	result = adi_twi_SubmitTxBuffer(hDevice, buffer, buflen, false);
//	result = adi_twi_Write(hDevice, buffer, sizeof(buffer), false);
	/* enable the TWI transfer */
	result = adi_twi_Enable(hDevice);
	/* do other processing here while TWI is transmiting */
		for (counter=0;counter<(buflen*20000);counter++);
//	Value[0]=0x3f;
//	while(&Value!= ADI_TWI_ISTAT_MERR){
//		adi_twi_GetInterruptStatus(hDevice,Value);
//	}
	/* close the TWI driver */
	result = adi_twi_Close(hDevice);
	return result;
}

ADI_TWI_RESULT receive_I2C(char hwaddress, void *buffer, uint32_t buflen){
	ADI_TWI_HANDLE hDevice;
	uint32_t counter;
	/* TWI driver memory */
	uint8_t driverMemory[ADI_TWI_MEMORY_SIZE];
	/* driver API result code */
	ADI_TWI_RESULT result;

	/* buffer which holds data to transfer over TWI */

	/* open the TWI driver in master mode */
	result = adi_twi_Open(0, ADI_TWI_MASTER, driverMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	result = adi_twi_SetBitRate(hDevice,100);
	result = adi_twi_SetDutyCycle(hDevice,50);
	//    result = adi_twi_SetPrescale(hDevice,100);
	result = adi_twi_SetHardwareAddress(hDevice,hwaddress);
	result = adi_twi_SubmitRxBuffer(hDevice, buffer, buflen, false);
//	result = adi_twi_Read(hDevice, buffer, sizeof(buffer), false);
	/* enable the TWI transfer */
	result = adi_twi_Enable(hDevice);
	/* do other processing here while TWI is transmiting */
	//MAY NEEd a waiting function here.
	for (counter=0;counter<(buflen*20000);counter++);
	/* close the TWI driver */
	result = adi_twi_Close(hDevice);
	return result;
}


void get_data_ADXL345(int16_t *outbuffer){
	char i2cbuf[6];
	int16_t tval=0;
	i2cbuf[0]=0x32;
	send_I2C(ADXLADDR,i2cbuf,1);


	receive_I2C(ADXLADDR,i2cbuf,6);
//	tval=(i2cbuf[0]+(i2cbuf[1]<<8));
	outbuffer[0]=(i2cbuf[0]+(i2cbuf[1]<<8));
//	tval=(i2cbuf[2]+(i2cbuf[3]<<8));
	outbuffer[1]=(i2cbuf[2]+(i2cbuf[3]<<8));
//	tval=(i2cbuf[4]+(i2cbuf[5]<<8));
	outbuffer[2]=(i2cbuf[4]+(i2cbuf[5]<<8));


}

void send_mcu_data(uint32_t audioval, uint32_t meanval, int16_t *accel_buf){
	char outbuf[24];
//	audioval>>=16;
	outbuf[0]='X';
	outbuf[5]=(accel_buf[0]>>8)&0xFF;
	outbuf[6]=(accel_buf[0])&0xFF;
	outbuf[7]=(accel_buf[1]>>8)&0xFF;
	outbuf[8]=(accel_buf[1])&0xFF;
	outbuf[9]=(accel_buf[2]>>8)&0xFF;
	outbuf[10]=(accel_buf[2])&0xFF;
	outbuf[1]=((meanval>>8)&0xFF);
	outbuf[2]=((meanval>>0)&0xFF);
	outbuf[3]=((audioval>>8)&0xFF);
	outbuf[4]=((audioval>>0)&0xFF);
	if (audioval>10000)
		audioval=0;
	send_I2C(0x48, outbuf, 11);
}
