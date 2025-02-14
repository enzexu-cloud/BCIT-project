#include <msp430.h>

#define ON_CYCLES 2515
#define OFF_CYCLES 1677

void selSMCLK();

void main(void){
    WDTCTL = WDTPW | WDTHOLD;

    selSMCLK();

    while(1){
        P2OUT |= BIT2;
        __delay_cycles(ON_CYCLES);

        P2OUT &= ~BIT2;
        __delay_cycles(OFF_CYCLES);
    }
}

void selSMCLK(){
    P2DIR |= BIT2;

    P2SEL &= ~BIT2;

}




