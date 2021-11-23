#include "terminal.h"

uint8_t 	check_timeout(void);
uint8_t 	end_str(RING_BUFF_t*);
uint8_t		is_print_ready	=	0;

void 			create_buff(RING_BUFF_t*,RING_BUFF_t*);
void			send_with_static_word(void);
void			print(RING_BUFF_t*);
void			ready_send(void);

uint8_t 	buff_t[BUFF_SIZE];
uint8_t 	buff_r[BUFF_SIZE];
uint8_t 	static_buff[] = "Hello, world: ";	
	
/*
*-------------------------------------------
*|Objects where they are physically located
*-------------------------------------------
*/
RING_BUFF_t STATIC_WORD;
RING_BUFF_t TBUFF_terminal;
RING_BUFF_t RBUFF_terminal;


void			ready_send(void)
{
/*
*-------------------------------------------
*|If the sending conditions are met,
*|the interrupt is enabled and the timeout is stopped.
*-------------------------------------------
*/
	stop_timeout();
	set_TXE();
}// end ready_send

void			print(RING_BUFF_t* buff)
{
/*
*-------------------------------------------
*| First, it checks whether the send buffer is free.
*| If the send buffer is free, looks to see if this buffer has
*| been sent?
*| If the buffer has not been sent yet, it sets it as a send
*| buffer and indicates that the send buffer is busy.
*-------------------------------------------
*/
	if(!get_TBUFF_bussy())
	{
		if(!(buff->sent))
		{
			set_TBUFF(buff);
			set_TBUFF_bussy(1);
			ready_send();
		}
	}
}// end print()

void			init_terminal(void)
{
	init_usart(9600);
	
	TBUFF_terminal 			= init_ring_buff(0,0,buff_t,BUFF_SIZE);
	//TBUFF_terminal2 		= init_ring_buff(0,0,buff_t2,BUFF_SIZE);
	RBUFF_terminal 			= init_ring_buff(0,0,buff_r,BUFF_SIZE);
	STATIC_WORD 				=	init_ring_buff(STATIC_SIZE,0,static_buff,STATIC_SIZE);
	set_RBUFF(&RBUFF_terminal);
	set_TBUFF(&TBUFF_terminal);
}// end init_terminal

void 			create_buff(RING_BUFF_t* ring,RING_BUFF_t* ring2 )
{
/*
*--------------------------------------------
*|Copies, from ring2 to ring.
*--------------------------------------------
*/
	uint8_t	temp = get_element(ring2);
	set_element(ring,temp);
	
	if(get_count(ring)> ring->size){
		ring->tail++;
	}
}// end create_buff

void			prepare_send(void)
{
/*
*--------------------------------------------
*| 1. Checks the timeout
*| 2. Checks the end of the line
*--------------------------------------------
*/	
	if(check_timeout())
	{
		is_print_ready	=	1;
	}
	else
	{
		if(has_unread(&RBUFF_terminal))
		{
			if( read_element(&RBUFF_terminal) == '\r' )
			{
				(void)get_element(&RBUFF_terminal);
				is_print_ready	=	1;
			}
			else
			{
				create_buff(&TBUFF_terminal,&RBUFF_terminal);

				start_timeout();
			}
		}
	}
}// end prepare_send

void			send_str(void)
{
/*
*--------------------------------------------
*| Sends the specified buffer.
*--------------------------------------------
*/
	print(&STATIC_WORD);
	print(&TBUFF_terminal);
	
	if(STATIC_WORD.sent && TBUFF_terminal.sent)
	{
		STATIC_WORD.sent		=	0;
		TBUFF_terminal.sent	=	0;
		is_print_ready = 0;
		if(STATIC_WORD.tail>=STATIC_SIZE)
			STATIC_WORD.tail = 0;
	}
}// end send_str

uint8_t 	check_timeout()
{
	if(!get_timeout_flag())
	{
		return 0;
	}
	return timeout(TIMEOUT);
}// end check_timeout
