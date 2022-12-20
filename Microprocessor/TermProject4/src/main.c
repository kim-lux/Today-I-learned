#include "device_registers.h"
#include "clocks_and_modes.h"
#include "LPUART.h"
#include "Setting.h"

#define PTD0 0 // Port PTD, bit 0 : Output to blue LED
#define PTD15 15 // Port PTD, bit 0 : Output to red LED
#define PTD16 16 // Port PTD, bit 0 : Output to green LED
#define PTC12 12 // Port PTC, bit 12 : Input from SW2
#define PTC13 13 // Port PTC, bit 12 : Input from SW1
#define PTA0   0
#define PTA1   1
#define PTA2   2
#define PTA3   3

#define NUM_OF_STATES 8 //There are 8 different states in this particular example.
#define DELAY_MAX 100 //The maximum # of counts used to create a time delay

int i, j;

char state_array[NUM_OF_STATES] = {0x06, 0x02, 0x0A, 0x08, 0x09, 0x01, 0x05, 0x04};

int steps_to_move;
int next_state;
char send;
int flag=0;
int idle_counter = 0; /*< main loop idle counter */
int lpit0_ch0_flag_counter = 0; /*< LPIT0 chan 0 timeout counter */

int step_counter = 0;

int sig_switch = 0;
int dc_counter = 10;
int servo_counter = 10;
int paz_counter = 10;

int cell_counter = 0;
char money[5] = "";
int num=0;
int i = 0;
int selectMenu = 0;

void convert(char a[])
{
	for (i = 0; a[i]; i++)
	{
		a[i] = a[i] - 48;
		num = num * 10;
		num = num + a[i];
	}
}

void MotorDelay()
{
	 for(i = 0; i < DELAY_MAX; i++)
	 {
		 for(j = 0; j < DELAY_MAX; j++) ;
			 //Wait here for a while.
	 }
}

void PortOut(char Phase)
{
	if (Phase & 0x01 )	{
		PTA->PSOR |= 1;
	}
	else	{
		PTA->PCOR |= 1;
	}

	if (Phase & 0x02 )	{
		PTA->PSOR |= 2;
	}
	else	{
		PTA->PCOR |= 2;
	}

	if (Phase & 0x04 )	{
		PTA->PSOR |= 4;
	}
	else	{
		PTA->PCOR |= 4;
	}

	if (Phase & 0x08 )	{
		PTA->PSOR |= 8;
	}
	else	{
		PTA->PCOR |= 8;
	}
}

void StepForward(void)
{
	while (steps_to_move > 0)
	{
		if (next_state > (NUM_OF_STATES - 1)) //If next_state is greater than the highest
												//available state, 7, then cycle back to 0
		{
			next_state = 0;
		}
		// PTU = state_array[next_state]; //Place new value in Port U. Rotation may be observed

		//StepPhase = PTA->PDIR;
		//StepPhase &= ~(0x0F);
		//PTA->PDOR =  StepPhase | (state_array[next_state] & 0x0F);
		//PTA->PDOR =  state_array[next_state];
		PortOut(state_array[next_state]);

		MotorDelay();

		next_state++; //Increment next_state. Cycling though the states causes rotation
						//in one direction. Decrementing states causes opposite rotation.
		steps_to_move--; //Subtract 1 from the total # of steps remaining to be moved.
	}
	steps_to_move = 360;
}

void StepBackward(void)
{
	while (steps_to_move > 0)
	{
		if (next_state < 0 ) //If next_state is greater than the highest
												//available state, 7, then cycle back to 0
		{
			next_state = (NUM_OF_STATES - 1);
		}
		//PTU = state_array[next_state]; //Place new value in Port U. Rotation may be observed
		// PTA->PDOR = (PTA->PDOR & ~(0x0F)) | (state_array[next_state] & 0x0F);
		//StepPhase = (PTA->PDIR & ~(0x0F));
		//StepPhase = PTA->PDIR;
		//StepPhase &= ~(0x0F);
		//PTA->PDOR =  StepPhase | (state_array[next_state] & 0x0F);
		//PTA->PDOR =  state_array[next_state];
		PortOut(state_array[next_state]);

		MotorDelay();

		next_state--;   //Decrement next_state. Cycling though the states causes rotation
						//in one direction. Decrementing states causes opposite rotation.
		steps_to_move--; //Subtract 1 from the total # of steps remaining to be moved.
	}
	steps_to_move = 360;
}




int main(void)
{
	/*!
	 * Initialization:
	 * =======================
	 */

  int counter = 0;
  int mimoney = 0;
  steps_to_move = 360; //Set the # of steps to move. An arbitrary positive # can be used.
  next_state = 0;
  WDOG_disable();        /* Disable WDOG */
  SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
  SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
  PORT_init();           /* Configure ports */

  NVIC_init_IRQs(); /* Enable desired interrupts and priorities*/
  LPIT0_init(); /* Initialize PIT0 for 1 second timeout */

  LPUART1_init();        /* Initialize LPUART @ 9600*/
  LPUART1_transmit_string("Welcome to Coffee Machine\n\r");     /* Transmit char string */
  LPUART1_transmit_string("Select Your Menu Input Number\n\r"); /* Transmit char string */
  LPUART1_transmit_string("1. Espresso \n\r"); /* Transmit char string */
  LPUART1_transmit_string("2. Americano\n\r"); /* Transmit char string */
  LPUART1_transmit_string("3. Cafe latte\n\r"); /* Transmit char string */
  LPUART1_transmit_string("4. Cappuccino\n\r"); /* Transmit char string */
  LPUART1_transmit_string("Select Your Menu\n\r"); /* Transmit char string */
  PTD-> PCOR |= 1<<PTD0; // Port D0: LED OFF
  PTD-> PCOR |= 1<<PTD15; // Port D15: LED OFF
  PTD-> PCOR |= 1<<PTD16; // Port D16: LED OFF
	/*!
	 * Infinite for:
	 * ========================
	 */
	  for(;;)
	  {
		  //LPUART1_transmit_char('>');  		/* Transmit prompt character*/
		  idle_counter++; /* Increment idle counter */

		  send = LPUART1_receive_char();        /* Receive Char */
		  LPUART1_transmit_char(send);               /* Transmit same char back to the sender */
		  if(sig_switch == 1)
		  {

			  money[cell_counter] = send;
			  cell_counter++;
			  if(cell_counter >= 4)
			  {
				  //LPUART1_transmit_char('\n');
				  //LPUART1_transmit_string(money); /* Transmit char string */
				  //LPUART1_transmit_char('\n');
				  convert(money);
				  cell_counter = 0;
				  if(selectMenu == 1)
				  {
					  	if(num == 1500)
					  	{
					  		LPUART1_transmit_string("\n\r Thank yyyyyy\n\r"); /* Transmit char string */
					  	}
					  	else if(num > 1500)
					  	{
					  		LPUART1_transmit_string("\n\rChanges for you\n\r"); /* Transmit char string */
					  	}
					  	else if(num < 1500)
					  	{
					  		LPUART1_transmit_string("\n\rGive me more money.\n\r"); /* Transmit char string */
					  		sig_switch--;
					  	}

				  }
				  else if(selectMenu == 2)
				  {
					  	if(num == 2000)
					  	{
					  		LPUART1_transmit_string("\n\r Thank yyyyyy\n\r"); /* Transmit char string */
					  	}
					  	else if(num > 2000)
					  	{
					  		LPUART1_transmit_string("\n\rChanges for you\n\r"); /* Transmit char string */
					  	}
					  	else if(num < 2000)
					  	{
					  		LPUART1_transmit_string("\n\rGive me more money.\n\r"); /* Transmit char string */
					  		sig_switch--;
					  	}
				  }
				  else if(selectMenu == 3)
				  {
					  	if(num == 3500)
					  	{
					  		LPUART1_transmit_string("\n\r Thank yyyyyy\n\r"); /* Transmit char string */
					  	}
					  	else if(num >3500)
					  	{
					  		LPUART1_transmit_string("\n\rChanges for you\n\r"); /* Transmit char string */
					  	}
					  	else if(num < 3500)
					  	{
					  		LPUART1_transmit_string("\n\rGive me more money.\n\r"); /* Transmit char string */
					  		sig_switch--;
					  	}
				  }
				  else if(selectMenu == 4)
				  {
					  	if(num == 3500)
					  	{
					  		LPUART1_transmit_string("\n\r Thank yyyyyy\n\r"); /* Transmit char string */
					  	}
					  	else if(num > 3500)
					  	{
					  		LPUART1_transmit_string("\n\rChanges for you\n\r"); /* Transmit char string */
					  	}
					  	else if(num < 3500)
					  	{
					  		LPUART1_transmit_string("\n\rGive me more money.\n\r"); /* Transmit char string */
					  		sig_switch--;
					  	}
				  }


			  }

		  }

	  }
}


void LPIT0_Ch0_IRQHandler (void)
{
	lpit0_ch0_flag_counter++; /* Increment LPIT0 timeout counter*/
	if (sig_switch == 2)dc_counter++;
	if (sig_switch == 3)servo_counter++;
	if (sig_switch == 4)paz_counter++;
	if((sig_switch == 2) && (dc_counter <= 8))
	{
		StepForward();
	}
	if ((sig_switch == 2) && (dc_counter == 8))
	{
		PTD-> PCOR |= 1<<PTD0;
		LPUART1_transmit_string("\n\rExtract Coffee with hot water.\n\r"); /* Transmit char string */
		LPUART1_transmit_string("Please Wait.......\n\r"); /* Transmit char string */
		servo_counter = 0;

		PTD-> PDOR |= 1<<PTD15;
		sig_switch ++;
	}
	if ((sig_switch == 3) && (servo_counter == 8))
	{
		PTD-> PCOR |= 1<<PTD15;
		LPUART1_transmit_string("\n\rYour Coffee is ready.\n\r"); /* Transmit char string */
		LPUART1_transmit_string("Thank you.\n\r"); /* Transmit char string */
		paz_counter = 0;
		PTD-> PDOR |= 1<<PTD16;
		sig_switch ++;
	}
	if ((paz_counter == 4) && (paz_counter == 8))
	{
		PTD-> PCOR |= 1<<PTD16;
	}
	if (sig_switch >= 2)
	{
		LPUART1_transmit_string("0");
	}
	LPIT0 ->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
}

void PORTC_IRQHandler(void)
{
	/**/
	sig_switch++;
	if (sig_switch == 1)
	{

		LPUART1_transmit_string("Thank you for your selection\n\r"); /* Transmit char string */
	    if(send == '1')
	    {
	    	LPUART1_transmit_string("You selected an Espresso.\n\r"); /* Transmit char string */
	    	LPUART1_transmit_string("The Espresso is 1500 won. \n\r"); /* Transmit char string */
	    	selectMenu = 1;
	    }
	    else if(send == '2')
	    {
	    	LPUART1_transmit_string("You selected an Americano.\n\r"); /* Transmit char string */
	    	LPUART1_transmit_string("The Americano is 2000 won. \n\r"); /* Transmit char string */
	    	selectMenu = 2;
	    }
	    else if(send == '3')
	    {
	    	LPUART1_transmit_string("You selected an Cafe latte.\n\r"); /* Transmit char string */
	    	LPUART1_transmit_string("The Cafe latte is 3500 won.\n\r"); /* Transmit char string */
	    	selectMenu = 3;
	    }
	    else if(send == '4')
	    {
	    	LPUART1_transmit_string("You selected an Cappuccino.\n\r"); /* Transmit char string */
	    	LPUART1_transmit_string("The Cappuccino is 3500 won. \n\r"); /* Transmit char string */
	    	selectMenu = 4;
	    }
	    LPUART1_transmit_string("Please Input the Cash!.\n\r"); /* Transmit char string */
	}
	else if(sig_switch == 2)
	{
		LPUART1_transmit_string("\n\rStart blending coffee beans.\n\r"); /* Transmit char string */
		LPUART1_transmit_string("Please Wait.......\n\r"); /* Transmit char string */
		dc_counter = 0;
		PTD-> PSOR |= 1<<PTD0;
	}
	PORTC->PCR[12] |= 0x01000000; // clear ISF bit
	PORTC->PCR[13] |= 0x01000000; // clear ISF bit
}



