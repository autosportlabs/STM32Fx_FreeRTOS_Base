#ifndef _DEBUG_USART_H_
#define _DEBUG_USART_H_

#include <stdbool.h>
#include <stdint.h>

#define UART_BUFFER_SIZE 256

#define USART_1 0
#define USART_2 1

/* TODO: Move these into a common driver includes folder */
int usart_init(int usart_no, uint32_t baud);
int usart_enable_autocrlf(int usart_no, bool enable);
int usart_write(int usart_no, char *s, int len);
int usart_read(int usart_no, char *d, int len);
int usart_putchar(int usart_no, char c);
int usart_getchar(int usart_no, char *c);
#endif /* _DEBUG_USART_H_ */
