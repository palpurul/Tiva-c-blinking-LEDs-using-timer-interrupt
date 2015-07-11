

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"


int Clock;
int countTimer0 = 0x01;



void Timer0Conf(void);
/*
		Timer0 interrupt handler
*/

void Timer0IntHandler(void){
	
	TimerIntClear(TIMER0_BASE,INT_TIMER0A);																							// Clear the interrupt flag
	GPIOPinWrite(GPIO_PORTF_BASE,(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3),countTimer0);		// Write to pin
	
	if(countTimer0<0x08)
		countTimer0 = countTimer0<<1;
	else 
		countTimer0 = 0x01;
}
int main(void)
{	
	//Setting the clock frequency
	
	SysCtlClockSet(SYSCTL_SYSDIV_3 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);  // Refer to tm4c123gh6pm data sheet page 223
																																													 // and TivaWare™ Peripheral Driver Library refrence page: 489 for more options
																																													 // In this case the clock freq is 66.67 MHz
	
	
	//Get the clock
	Clock = SysCtlClockGet();           //In case you want to confirm the clock frequency
	
	
	//Setup peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);																							// enable timer0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);																							// enable PORTF
	
	//set outputs
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_3)); 			// set pins of portf as output
	
	//Enable global interrupt
	IntMasterEnable();
	
	//Timer0 configuration
	Timer0Conf();
	
	while(1){
		SysCtlSleep(); 				
	}
	
	
}

void Timer0Conf(void){

	TimerConfigure(TIMER0_BASE,(TIMER_CFG_PERIODIC_UP));																		//  Periodic up counter
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2);																	// load the value of the counter.Loading a timer with SysCtlClockGet() gives 1 second of delay 																																													// and in the same way SysCtlClockGet()/2 give half a second of delay.
	
	
	// setup the interrupt for timers
	
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);															//Timeout interrupt
	TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0IntHandler);												
	TimerEnable(TIMER0_BASE, TIMER_A);
}
