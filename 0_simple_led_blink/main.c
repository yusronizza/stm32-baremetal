/*
 ********************************************
 * Bare Metal Driver Development            *
 * Addresses based on reference manual book *
 * By Yusron Izza Faradisa                  *
 * ******************************************
*/

//Get Base Peripheral Address
#define PERIPHERAL_BASE_ADDR 		(0x40000000UL)
#define AHB1_PERIPH_OFFSET 			(0x00020000UL)
#define AHB1_BASE_ADDR				(PERIPHERAL_BASE_ADDR + AHB1_PERIPH_OFFSET)

//Get GPIO Address
#define GPIOA_OFFSET				(0x0000UL)
#define GPIOA_BASE_ADDR				(AHB1_BASE_ADDR + GPIOA_OFFSET)

//Get RCC Address
#define AHB1_RCC_OFFSET				(0x3800UL)
#define AHB1_RCC_BASE_ADDR			(AHB1_BASE_ADDR + AHB1_RCC_OFFSET)

//Get AHB1 Enable Register
#define AHB1_RCC_AHB1ENR_OFFSET		(0x30UL)
#define AHB1_RCC_AHB1ENR_ADDR		(AHB1_RCC_BASE_ADDR + AHB1_RCC_AHB1ENR_OFFSET)
#define AHB1_RCC_AHB1ENR_R			(*(volatile unsigned int*)AHB1_RCC_AHB1ENR_ADDR)

//Get GPIOA Mode Register
#define GPIOA_MODER_OFFSET			(0x0UL)
#define GPIOA_MODER_ADDR			(GPIOA_BASE_ADDR + GPIOA_MODER_OFFSET)
#define GPIOA_MODER_ADDR_R			(*(volatile unsigned int*)GPIOA_MODER_ADDR)

//Get GPIOA Output Data Register
#define GPIOA_ODR_OFFSET			(0x14UL)
#define GPIOA_ODR_ADDR				(GPIOA_BASE_ADDR + GPIOA_ODR_OFFSET)
#define GPIOA_ODR_ADDR_R			(*(volatile unsigned int*)GPIOA_ODR_ADDR)

#define GPIOAEN						(1U<<0)
#define PA5							(1U<<5)
#define LED_PIN						PA5

int main(void)
{
	//Enable GPIOA from RCC on AHB1
	AHB1_RCC_AHB1ENR_R |= GPIOAEN;

	//Set GPIOA Mode Register bit 10th equal 1 and bit 11th equal 0
	//Keep the rest of the bit value
	GPIOA_MODER_ADDR_R |= (1U<<10);
	GPIOA_MODER_ADDR_R &=~(1U<<11);

	while(1)
	{
		//Set GPIOA Output Data Register bit 5th to 1
		GPIOA_ODR_ADDR_R ^= LED_PIN;

		//Delay
		for(int i = 0; i <= 100000;i++){}
	}

}
