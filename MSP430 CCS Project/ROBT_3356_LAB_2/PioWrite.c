#include <msp430.h> 
#define P3_BASE_ADDR 0x0222

void pioWrite (volatile unsigned char * portPtr, unsigned char outputVal, unsigned char action);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    volatile unsigned char * const portPtr = (unsigned char *) P3_BASE_ADDR;
    P3DIR |= 0xFF;


    while (1)
        {
            // set the MSBit and LBit and leave other bits alone
            pioWrite (portPtr, 0x81, 0x1);
            __delay_cycles(500000); // Delay for visibility

            // clear all bit
            pioWrite(portPtr, 0x00, 0);
            __delay_cycles(1000000);

            // set the MSnibble and leave other bits alone
            pioWrite (portPtr, 0xF0, 0x1);
            __delay_cycles(500000); // Delay for visibility

            // clear the MSnibble and leave other bits alone
            pioWrite (portPtr, 0xF0, 0x2);
            __delay_cycles(500000); // Delay for visibility

            // clear all bit
            pioWrite(portPtr, 0x00, 0);
            __delay_cycles(1000000);



        }

}

void pioWrite (volatile unsigned char * portPtr, unsigned char outputVal, unsigned char action)
{
    switch(action)
    {
        case 0:
            *portPtr = outputVal;
            break;
        case 1:
            *portPtr |= outputVal;
            break;
        case 2:
            *portPtr &= ~outputVal;
            break;
        case 3:
            *portPtr ^= outputVal;
            break;
        default:
            break;
    }
}
