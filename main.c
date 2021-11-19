#include "global.h"
void 			magic_function(void);
uint8_t 	check_for_send(void);

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
	GPIOC->BSRR |= GPIO_BSRR_BS13;
	//GPIOC->BSRR |= GPIO_BSRR_BR13;
	while(1){
		
		//set_TXE();
		
		//USART1->DR = a;
		//test();
		send_str();
		
	}
}
/*
void 		magic_function(void){
	
	uint8_t count = is_new_element_r();
	// if new data
	if(count){
		// if not end string
		if( check_for_send() ){
			
			set_element(tbuff, (get_head_t()%BUFF_SIZE), get_element(rbuff, (get_tail_r()%BUFF_SIZE) ) );	
			set_tail_r(get_tail_r()+1);
			set_head_t(get_head_t()+1);
			
			if(get_count_t()>BUFF_SIZE){
				set_tail_t(get_tail_t()+1);
			}
			
		}
		// if end string
		else{
			
			if(get_head_r()==0){
				set_head_r(MAX_8BYTE_VALUE-2);
			}
			if(get_head_r()==1){
				set_head_r(MAX_8BYTE_VALUE-1);
			}
			else{
				set_head_r(get_head_r()-2);
			}
			set_flag_TXE(TRUE);
			
		}
	}
}

*/