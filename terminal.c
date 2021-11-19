#include "terminal.h"

uint8_t 	check_timeout(void);
uint8_t 	end_str(RING_BUFF_t*);


void 			create_buff(void);
void			send_with_static_word(void);
uint8_t 	buff_t[BUFF_SIZE];
uint8_t 	buff_r[BUFF_SIZE];
uint8_t 	static_buff[] = "Hello, world: ";	
	

RING_BUFF_t STATIC_WORD;
RING_BUFF_t TBUFF_terminal;
RING_BUFF_t RBUFF_terminal;

void			init_terminal(void)
{
	init_usart(9600);
	
	TBUFF_terminal 			= init_ring_buff(0,0,buff_t,BUFF_SIZE);
	RBUFF_terminal 			= init_ring_buff(0,0,buff_r,BUFF_SIZE);
	STATIC_WORD 				=	init_ring_buff(14,0,static_buff,14);
	set_RBUFF(&RBUFF_terminal);
	set_TBUFF(&TBUFF_terminal);

}

void 			create_buff(void)
{
	uint8_t	temp = get_element(&RBUFF_terminal);
	set_element(&TBUFF_terminal,temp);
	
	if(get_count(&TBUFF_terminal)> TBUFF_terminal.size){
		TBUFF_terminal.tail++;
	}
}

void			send_with_static_word(void)
{
		set_TBUFF(&STATIC_WORD);
		set_TXE();
		STATIC_WORD.tail	=	0;
		set_TBUFF(&TBUFF_terminal);
		set_TXE();
}

void			send_str(void)
{
	if(check_timeout())
	{
		//printf("Hello world: ");
		//printf(ring_buff)
		stop_timeout();
		send_with_static_word();
	}
	else
	{
		if(has_unread(&RBUFF_terminal))
		{
			if( read_element(&RBUFF_terminal) == '\r' )
			{
				(void)get_element(&RBUFF_terminal);
				stop_timeout();
				send_with_static_word();
			}
			else
			{
				create_buff();
				start_timeout();
			}
		}
	}
}	

uint8_t 	check_timeout()
{
	if(!get_timeout_flag())
	{
		return 0;
	}
	return timeout(TIMEOUT);
}