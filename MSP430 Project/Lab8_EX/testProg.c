#include <msp430.h> 

/**
 * main.c
 */
#define ROW P2IN         // Define ROW input port
#define COL P6OUT        // Define COL output port
#define ROW_PULLDOWN P2REN // Enable pull-down resistors for ROW input

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

        // Configure COL port as output
        P6DIR |= 0x0F;    // Set P6.0 - P6.3 as output (COL<4:1>)
        COL = 0x0F;       // Initialize COL to all high (0xF)

        // Configure ROW port as input with pull-down resistors
        P2DIR &= ~0xF0;   // Set P2.4 - P2.7 as input (ROW<4:1>)
        ROW_PULLDOWN |= 0xF0;  // Enable pull-down resistors for P2.4 - P2.7
        P2OUT &= ~0xF0;   // Ensure pull-down configuration

        while (1) {
            // Read ROW pins
            unsigned char rowValue = ROW & 0xF0;

            if (rowValue != 0x00) {
                // Pause program here (could be a breakpoint for debugging)
                __no_operation();

                // Print rowValue for verification (platform-specific print function may be required)
                // Example: printf("ROW Value: 0x%02X\n", rowValue);
            }
        }
        return 0;
}
