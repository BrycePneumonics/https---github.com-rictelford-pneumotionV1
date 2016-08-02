/*
 * I2C_Com.h
 *
 *  Created on: Mar 12, 2016
 *      Author: Pneumonics
 */

#ifndef I2C_COM_H_
#define I2C_COM_H_
void init_ADXL345(void);
ADI_TWI_RESULT send_I2C(char , char *, uint32_t );
ADI_TWI_RESULT receive_I2C(char , void *, uint32_t);
void get_data_ADXL345(int16_t *);
void send_mcu_data(uint32_t ,uint32_t, int16_t *);


#endif /* I2C_COM_H_ */
