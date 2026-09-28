/****************************************************************
* Author: T. LUM and R. MARTIN
* CPEG222 DemoProgram5, 9/24/26
* NucleoF466ZE CMSIS STM32F4xx example Demo5
* Write simple text to the serial monitor using UART3
****************************************************************/
#include "stm32f4xx.h"// Include the STM32F4xx CMSIS header file

#define UART3_TX_PIN     8
#define UART3_RX_PIN     9
#define UART3_TX_PORT    GPIOD
#define UART3_RX_PORT    GPIOD
void uart3_send_char(char c) {
  while (!(USART3->SR & USART_SR_TXE))// Wait until TXE (Transmit Data Register Empty) flag is set
    ;
  USART3->DR = c; // Write the character to the data register
}
void uart3_send_string(const char *s) {
  while (*s) // Loop until the null terminator is reached
    uart3_send_char(*s++); // Send each character in the string
}
void uart3_init(void) {
    // Step 1: Enable GPIOD and USART3 peripheral clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    // Step 2: Set PD8 and PD9 to Alternate Function Mode (Bits 16 and 17 for PD8, Bits 18 and 19 for PD9 set to 10)
    UART3_TX_PORT->MODER &= ~(0x3 << (UART3_TX_PIN * 2));
    UART3_TX_PORT->MODER |= (0x2 << (UART3_TX_PIN * 2));
    UART3_RX_PORT->MODER &= ~(0x3 << (UART3_RX_PIN * 2));
    UART3_RX_PORT->MODER |= (0x2 << (UART3_RX_PIN * 2));
    // Step 3: Set Alternate Function to AF7 (USART3) for PD8 and PD9
    UART3_TX_PORT->AFR[1] &= ~(0xF << ((UART3_TX_PIN - 8) * 4));
    UART3_TX_PORT->AFR[1] |= (0x7 << ((UART3_TX_PIN - 8) * 4));
    UART3_RX_PORT->AFR[1] &= ~(0xF << ((UART3_RX_PIN - 8) * 4));
    UART3_RX_PORT->AFR[1] |= (0x7 << ((UART3_RX_PIN - 8) * 4));
    // Step 4: Configure USART3 (e.g., 115200 baud, 8N1)
    USART3->BRR = 0x8B; // Assuming 16 MHz PCLK1, 115200 baud
    USART3->CR1 |= USART_CR1_TE; // Enable transmitter
    USART3->CR1 |= USART_CR1_UE; // Enable USART3
}
int main(void) {
    uart3_init(); // Initialize UART3
    uart3_send_string("UD CPEG222 Demo5\r\n");
    while (1) {
        uart3_send_string("Microprocessors are awesome!\r\n");
        for (volatile int i = 0; i < 1000000; i++); // Simple delay
    }
    return 0;
}