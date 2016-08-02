/*
 * TalkThrough_BF706Mini.h
 */

#ifndef VU_main_H_
#define VU_main_H_

/* SPORT device instance used for communicating with the codec device */
#define SPORT_TX_DEVICE  0
#define SPORT_RX_DEVICE  0

/* TWI device instance used for communicating with the codec device */
#define TWI_DEV_NUM      0

/* codec device instance to be tested */
#define ADAU1761_DEV_NUM   0

/* the ADAU1761 Rec Mixer Left 0 register */
#define REC_MIX_LEFT_REG    (0x400A)
/* the ADAU1761 Rec Mixer Right 0 register */
#define REC_MIX_RIGHT_REG   (0x400C)

#define MHZTOHZ       (1000000)
#define CLKIN         (25 * MHZTOHZ)
#define MSEL          (32)  /* 800 MHz PLL */
#define CSEL          (2)   /* 400 MHz core */
#define SYSSEL        (4)   /* 200 MHz sysclk */


#endif /* TALKTHROUGH_H_ */
