#include "global.h"
 
int main(){
	
	SystemInit();
	// led init
	init_gpio(GPIOC,13,OUTPUT_PUSH_PULL,OUTPUT_MODE_10MHz);
	init_gpio(GPIOA,7,OUTPUT_PUSH_PULL,OUTPUT_MODE_10MHz);
	
	// button init
	init_button(GPIOA,5);
	init_button(GPIOA,6);
	
	// delay iniy
	init_my_delay();
	//init_usart(9600);
	init_terminal();
	
	while(1){
		
		if(is_print_ready)
		{
			send_str();
		}
		
		prepare_send();
		
	}
}