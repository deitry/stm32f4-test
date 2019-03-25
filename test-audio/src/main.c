
#include "stm32f4_discovery.h"
#include "init.h"


uint8_t state = 0x00;
uint8_t mode = 0x00;

const uint8_t max_mode = 12;


int main(void)
{
    // Разрешаем прерывания
    __enable_irq();
    // инициализация
    initGPIO();
    initTimerDefault();
    initI2C();
    initI2S();
    initCS32L22();

    // Включаем SPI3
    I2S_Cmd(SPI3, ENABLE);

    // настраиваем прерывание по переполнению таймера
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);

    // настраиваем кнопку на генерацию прерывания EXTI0
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

    while(1)
    {
        // Если флаг выставлен, то можно передавать данные
        if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE))
        {
            if (mode == 0x00 || state == 0x00)
            {
                // Если переменная state = 0, то посылаем нули
                SPI_I2S_SendData(SPI3, 0x00);
            }
            else
            {
                // А если переменная state != 0, то посылаем
                // максимальное значение, в итоге получаем
                // прямоугольные импульсы
                SPI_I2S_SendData(SPI3, 0xFF);
            }
        }
    }
}

// обработчик нажатия кнопки
void EXTI0_IRQHandler(void)
{
    //Clear interrupt
    EXTI->PR = EXTI_Line0;

    if (++mode >= max_mode) mode = 0;

    switch (mode)
    {
        // последовательный перебор различных частот таймера
        case 0: initTimer(7200); break;
        case 1: initTimer(7200); break;
        case 2: initTimer(3600); break;
        case 3: initTimer(5400); break;
        case 4: initTimer(9000); break;
        case 5: initTimer(10000); break;
        case 6: initTimer(14400); break;
        case 7: initTimer(7200); break;
        case 8: initTimer(7300); break;
        case 9: initTimer(7500); break;
        case 10: initTimer(8000); break;
        case 11: initTimer(7200); break;
        default: initTimer(2000); break;
    }
}

void TIM2_IRQHandler()
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    state ^= 0x01;
}
