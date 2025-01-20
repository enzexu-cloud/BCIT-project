#include <msp430.h> 

#define P1_BASE_ADDR 0x0202
#define P4_BASE_ADDR 0x0223

#define LED1_MASK BIT0  // Mask for LED1 (bit 0)
#define LED2_MASK BIT7  // Mask for LED2 (bit 7)

void ledToggle(volatile unsigned char *led1Ptr, volatile unsigned char *led2Ptr,
unsigned char led, unsigned int delay);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	volatile unsigned char * const port1Ptr = (unsigned char *) P1_BASE_ADDR;
    volatile unsigned char * const port4Ptr = (unsigned char *) P4_BASE_ADDR;

    P1DIR |= LED1_MASK;
    P4DIR |= LED2_MASK;

    *port1Ptr &= ~LED1_MASK;
    *port4Ptr &= ~LED2_MASK;

    while(1)
    {
    ledToggle (port1Ptr, port4Ptr, 1, 50000);
    }
}
void ledToggle (volatile unsigned char * led1Ptr, volatile unsigned char * led2Ptr,
unsigned char led, unsigned int delay)
{
    if(led == 0)
    {
        *led1Ptr ^= LED1_MASK;
    }
    else if(led == 1)
    {
        *led2Ptr ^= LED2_MASK;
    }
    else if(led == 2)
    {
        *led1Ptr ^= LED1_MASK;
        *led2Ptr ^= LED2_MASK;
    }
    volatile unsigned int i;
        for (i = 0; i < delay * 5; i++);
}
