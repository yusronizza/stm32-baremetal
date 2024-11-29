/*
 * uart.h
 *
 *  Created on: Nov 29, 2024
 *      Author: YusronIzzaF
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

void usart2_txrx_init(void);
char usart2_read(void);

#endif /* UART_H_ */
