/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h> 

int main(void)
{
    DDRB = 1 << 5;           /* make the LED pin an output */
    char i;
    for(;;){
        for(i = 0; i < 10; i++){
            _delay_ms(30);  /* max is 262.14 ms / F_CPU in MHz */
        }
        PORTB ^= 1 << 5;    /* toggle the LED */
    }
    return 0;   /* never reached */
}
