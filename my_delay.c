#include "my_delay.h"

volatile 	uint16_t 	ms 						=		0;
volatile 	static 		uint16_t	start_ms;
uint8_t 	timeout_flag						=		0;
uint8_t 	timeout_enable					=		0;

uint16_t get_start_ms(void);

void 			init_my_delay(void){
	SysTick_Config( (SystemCoreClock/1000)-1 );
}

void		reset_start_ms(void)
{
	start_ms	=	get_ms();
}

uint16_t get_start_ms(void)
{
	return start_ms;
}

uint8_t get_timeout_flag(void)
{
	return timeout_flag;
}

void	 stop_timeout(void)
{
	timeout_flag	=	0;
}
void	 start_timeout(void)
{
	timeout_flag	=	1;
	reset_start_ms();
}

uint16_t get_ms(void)
{
	return ms;
}



uint8_t	timeout(uint16_t time)
{
	if(get_ms()>=time+get_start_ms())
	{
		return 1;
	}
	else
	{
		return 	0;
	}
}

void 			delay_ms(uint16_t n){
	uint16_t current = ms;
	while((ms-current)<n);
}

void 			SysTick_Handler(void){
	ms++;
	check_button1(GPIOA,5);
	check_button2(GPIOA,6);
}

