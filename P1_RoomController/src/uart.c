#include "uart.h"

int usart_number(usart_t *USARTx)
{
    if(USARTx == USART1) return 1;
    else if(USARTx == USART2) return 2;
    else if(USARTx == USART3) return 3;
    else if(USARTx == UART_4) return 4;
    else if(USARTx == UART_5) return 5;
    else return 0xF;
}

void usart_init(usart_config_t *usart_config_struct)
{
    usart_t *USARTx = usart_config_struct->USART_PORT;
    gpio_t *GPIOx = usart_config_struct->GPIO_PORT;
    PINx tx = usart_config_struct->TX_PIN;
    PINx rx = usart_config_struct->RX_PIN;
    stopBit_t stop = usart_config_struct->STOP_BITS;
    lenghtBit_t wordLenght = usart_config_struct->WORD_LENGHT;
    parity_t parity = NO_PARITY;
    if(usart_config_struct->PARITY != parity)
        parity = usart_config_struct->PARITY;
    int baudRate = usart_config_struct->BAUDRATE;

    usart_activate(usart_number(USARTx));

    configure_gpio_usart(GPIOx, tx);
    configure_gpio_usart(GPIOx, rx);

    USARTx->CR1 &= ~ENABLE_USART;                       //disable uart for configuration
    USARTx->BRR = 4000000 / baudRate;                   //set the baud rate
    usart_set_word_lenght(USARTx, wordLenght);
    usart_set_stop_bits(USARTx, stop);
    usart_set_parity(USARTx, parity);
    USARTx->CR1 |= ENABLE_TX | ENABLE_RX;               //enable tx and rx
    USARTx->CR1 |= ENABLE_USART;                        //enable uart
}

void usart_set_stop_bits(usart_t *USARTx, stopBit_t stop)
{
    switch(stop) {
        case HALF_STOP_BIT:
            USARTx->CR2 &= ~(0x3U << 12);
            USARTx->CR2 |= (0x1U << 12);
            break;
        case ONE_STOP_BIT:
            USARTx->CR2 &= ~(0x3U << 12);
            break;
        case ONE_HALF_STOP_BIT:
            USARTx->CR2 |= (0x3U << 12);
            break;
        case TWO_STOP_BITS:
            USARTx->CR2 &= ~(0x3U << 12);
            USARTx->CR2 |= (0x2U << 12);
            break;
    }
}

void usart_set_word_lenght(usart_t *USARTx, lenghtBit_t lenght)
{
    switch(lenght) {
        case EIGHT_BITS_LENGHT:
            USARTx->CR1 &= ~WORD_8BITS_0_9BITS_1;
            break;
        case NINE_BITS_LENGHT:
            USARTx->CR1 |= WORD_8BITS_0_9BITS_1;
            break;
        case SEVEN_BITS_LENGHT:
            USARTx->CR1 |= WORD_7BITS;
            break;
    }
}

void usart_set_parity(usart_t *USARTx, parity_t parity)
{
    if(parity == NO_PARITY)
        return;
    USARTx->CR1 |= (0x1U << 10);
    USARTx->CR1 &= ~(0x1U << 9);
    if(parity == ODD_PARITY)
        USARTx->CR1 |= (0x1U << 9);
}

void enable_RXNE(usart_t *USARTx)
{
    USARTx->CR1 |= (0x1U << 5);                         //enable RXNEIE register for lauching the RXNE interrupt
    usart_interrupt_enable(usart_number(USARTx));
}

void usart_send_string(usart_t *USARTx, const char *str)
{
    while(*str) {
        while(!(USARTx->ISR & TRANSMIT_ENABLE));
        USARTx->TDR = (*str++); //& 0xFF;
    }
}

char* usart_receive_string(usart_t *USARTx)
{
    static char buffer[50];
    int i = 0;

    for(;;) {
        buffer[i] = USARTx->RDR;
        if(buffer[i] == '\n' || buffer[i] == '\r' || i >= 48) {
            buffer[i + 1] = '\0';
            return buffer;
        }
        ++i;
    }
}
