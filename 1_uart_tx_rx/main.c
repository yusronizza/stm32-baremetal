#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"

//On Board LED
#define GPIOAEN    (1U<<0)
#define PA5        (1U<<5)
#define LED_PIN    PA5

char received_data;

int main(void)
{
	//Give clock access to GPIO A
	RCC->AHB1ENR |= GPIOAEN;

	//Set PA5 to general output mode
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	//Initiate UART
	usart2_txrx_init();

	printf("Program started...\n\r");
	printf("Press button 1\n\r");

	while(1)
	{
		//Read UART
		received_data = usart2_read();
		printf("%c \n", received_data);

		if(received_data)
		{
			//Turn on PA5 LED
			GPIOA->ODR ^= LED_PIN;
		}
	}
}
