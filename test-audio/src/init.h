#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_spi.h"


void delay(uint32_t delayTime);

void initGPIO();

void initI2S();
void initI2C();

void writeI2CData(uint8_t bytesToSend[], uint8_t numOfBytesToSend);

void initCS32L22();

void initTimerDefault(void);
void initTimer(uint16_t prescaler);
