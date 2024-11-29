/*
 * uart.c
 *
 *  Created on: Nov 28, 2024
 *      Author: YusronIzzaF
 */

#include "uart.h"

//GPIO & UART Enable
#define GPIOAEN			(1U<<0)
#define USART2ENR		(1U<<17)

//Control Register (CR) Transmitter Enable (TE), Receiver Enable (RE) & UART Enable (UE)
//Status Register (SR) Transmitter Empty (TXE), Receiver Not Empty (RXNE)
#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)
#define SR_RXNE			(1U<<5)

//Base SYSTEM default clock
#define SYSTEM_FREQ 	16000000
#define APB1_CLK		SYSTEM_FREQ
#define BAUD_RATE		115200

//Function Prototyping
static void uart_set_baud_rate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baud_rate);
static uint16_t calculate_divsr(uint32_t periph_clk, uint32_t baud_rate);
void usart2_write(int ch);
int __io_putchar(int ch);

//printf function overriding
int __io_putchar(int ch)
{
	usart2_write(ch);
	return ch;
}

void usart2_txrx_init(void)
{
	/*****Enable GPIO Pin for UART*****/
	//Enable clock to AHB1
	RCC->AHB1ENR |= GPIOAEN;

	//Set GPIOA Mode Register for PA2 Alternate Function mode
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	//Set GPIOA Alternate Function Register
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	//Set GPIOA Mode Register for PA3 Alternate Function mode
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	//Set GPIOA Alternate Function Register
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	//Enable clock to UART module
	RCC->APB1ENR |= USART2ENR;

	//Set UART Configuration
	uart_set_baud_rate(USART2, APB1_CLK, BAUD_RATE);

	//Set Control Register to enable Transmitter
	USART2->CR1 = CR1_TE;

	//Set Control Register to enable Receiver
	USART2->CR1 |= CR1_RE;

	//Enable the UART Module Controller
	USART2->CR1 |= CR1_UE;
}

char usart2_read(void)
{
	//Read the Status Register and wait until the register is not empty
	while(!(USART2->SR & SR_RXNE)){}

	//Return the value
	return USART2->DR;

}

void usart2_write(int ch)
{
	//Read the Status Register and wait until the register is empty
	while(!(USART2->SR & SR_TXE)){}

	//If empty write the data to register
	USART2->DR = (ch & 0xFF);
}

static void uart_set_baud_rate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baud_rate)
{
	USARTx->BRR = calculate_divsr(periph_clk, baud_rate);
}

static uint16_t calculate_divsr(uint32_t periph_clk, uint32_t baud_rate)
{
	return ((periph_clk + (baud_rate/2U))/baud_rate);
}
