#include <msp430.h>

#define RAM_S0_BASE 0x002400
#define RAM_S1_BASE 0x002C00
#define RAM_S2_BASE 0x003400
#define RAM_S3_BASE 0x003C00

#define RAM_SEC_SZ 0x800


#define LED1_MASK BIT0  // Mask for LED_RED (bit 0)
#define LED2_MASK BIT7  // Mask for LED_GREEN (bit 7)

int ramSectorTestB(volatile unsigned char *ramPtr, unsigned char ramVal);
int ramSectorTestW(volatile unsigned char *ramPtr, unsigned int ramVal);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= LED1_MASK;
    P4DIR |= LED2_MASK;

    P1OUT &= ~LED1_MASK;
    P4OUT &= ~LED2_MASK;

    volatile unsigned char * const ramPtr0 = (unsigned char *)RAM_S0_BASE;
    //volatile unsigned char * const ramPtr1 = (unsigned char *)RAM_S1_BASE;
    //volatile unsigned char * const ramPtr2 = (unsigned char *)RAM_S2_BASE;
    //volatile unsigned char * const ramPtr3 = (unsigned char *)RAM_S3_BASE;

    int r = ramSectorTestB(ramPtr0, 0x66);
    //ramSectorTestW(ramPtr0, 0xA8B7);

    if(r == 0)
    {
        unsigned int i;

        for(i = 0; i < 4; i++)
        {
            P4OUT ^= LED2_MASK;
            __delay_cycles(500000);
        }

        return 1;
    }

        P4OUT &= ~LED2_MASK;

        P1OUT |= LED1_MASK;
}

int ramSectorTestB(volatile unsigned char *ramPtr, unsigned char ramVal)
{
    unsigned char readVal;
    unsigned int i;

        for(i = 0; i < RAM_SEC_SZ; i++)
        {
        *(ramPtr + i) = ramVal;
        }

        for(i = 0; i < RAM_SEC_SZ; i++)
        {
            readVal = *(ramPtr + i);
            if (readVal != ramVal){
                return 1;
            }
        }

    return 0;
}

int ramSectorTestW(volatile unsigned char *ramPtr, unsigned int ramVal)
{
    unsigned int readVal;
    unsigned int i;

       for(i = 0; i < RAM_SEC_SZ; i++)
       {
           *(ramPtr + (i+1)) = ramVal;
       }

       for(i = 0; i < RAM_SEC_SZ; i++)
           {
               readVal = *(ramPtr + (i+1));
               if (readVal != ramVal){
                   return 1;
               }
           }

       return 0;
}



