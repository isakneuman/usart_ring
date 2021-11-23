#include "usart_better.h"

uint8_t 		TBUFF_bussy	=	0;

RING_BUFF_t	*RBUFF;
RING_BUFF_t	*TBUFF;

uint8_t 	get_TBUFF_bussy(void)
{
	return TBUFF_bussy;
}
void			set_TBUFF_bussy(uint8_t data)
{
	TBUFF_bussy	= data;
}

void 			set_element(	RING_BUFF_t*,
												uint8_t);
	
uint8_t		get_element(	RING_BUFF_t*);
	
void 			get_element2(	RING_BUFF_t* ring,
												uint8_t* data);


RING_BUFF_t	init_ring_buff(		uint8_t 	head,
															uint8_t 	tail,
															uint8_t*	buff,
															uint8_t		size)
{
	RING_BUFF_t ring;
	ring.head = head;
	ring.tail = tail;
	ring.buff = buff;
	ring.size = size;
	ring.sent	=	0;
	return ring;
}

volatile uint8_t 	flag_TXE	=	0;

void			set_TBUFF(RING_BUFF_t* ring)
{
	TBUFF = ring;
}
void			set_RBUFF(RING_BUFF_t* ring)
{
	RBUFF = ring;
}

RING_BUFF_t*	get_RBUFF(void)
{
	return RBUFF;
}


void			set_TXE(void)
{
	flag_TXE	=		 1;
	USART1->CR1		|=	USART_CR1_TXEIE;
}
void 			reset_TXE(void)
{
	flag_TXE			=		0;
	USART1->CR1		&=	~USART_CR1_TXEIE;
}
//

void			recieve(void)
{
	set_element(RBUFF,USART1->DR);
}


static uint8_t i = 0;
void			send_array(void)
{
	USART1->DR	=	array[i++];
	if(i >= END_ARRAY){
		i = 0;
		reset_TXE();
	}
}
	
void			send(void)
{
/*
*----------------------------------
*|	Checks if there is anything to send.
*|	If there is something to send, it sends to USART1->DR.
*|	If there is nothing to send, stops the interrupt and sets
*|	the buffer flag as sent and sets a flag that the send buffer
*|	is free.
*----------------------------------	
*/
	if( !has_unread(TBUFF) )
	{
		reset_TXE();
		TBUFF->sent	=	1;
		set_TBUFF_bussy(0);
	}
	else
	{
		get_element2(TBUFF,&USART1->DR);
	}
} // end send

uint8_t		has_unread(	RING_BUFF_t* ring)
{
	uint8_t temp = get_count(ring);
	if(temp)
	{
		return 1;
	}
	return 0;
} // end has_unread

void			write_data(	RING_BUFF_t* ring,
											uint8_t data)
{
	set_element(ring,data);
}
// end write_data


uint8_t get_TXE(void)
{
	return flag_TXE;
}
// 

uint8_t 	get_count(		RING_BUFF_t* ring)
{
/*
*----------------------------------
*| Returns, the number of unused elements.
*----------------------------------	
*/
	uint8_t temp = 0;
	if(ring->tail<=ring->head)
	{
		temp = ring->head - ring->tail;
	}
	else
	{
		temp = MAX_8BYTE_VALUE - ring->tail + ring->head;
	}
	return temp;
}
// end get_count

void 			set_element(	RING_BUFF_t* ring,
												uint8_t data)
{
	ring->buff[ ring->head++ % ring->size ] = data;
}
// end set_element

void 			get_element2(	RING_BUFF_t* ring,
												uint8_t* data)
{
	*data = ring->buff[ ring->tail++ % ring->size];
}
// end get_element2

uint8_t 	read_element(	RING_BUFF_t* ring)
{
/*
*------------------------------------
*|Return the element without deleting it
*------------------------------------
*/
	return ring->buff[ (ring->tail) % ring->size ];
}
// end read_element

uint8_t		get_element(	RING_BUFF_t* ring)
{
	uint8_t temp;
	temp	=	ring->buff[ ring->tail++ % ring->size ];
	return temp;
}
// end get_element
 
void 			USART1_IRQHandler(void){
	
	if ( USART1->SR & USART_SR_RXNE ){
		recieve();
	}
 	if ( (USART1->SR & USART_SR_TXE) && get_TXE()){
	
		send();
	}
	
}
//	END IRQHandler

uint16_t 	BRR(uint16_t BAUDR){
	
	uint16_t DIV_Mantissa			=	( (SystemCoreClock/BAUDR/16)	<<	4 );
	uint16_t DIV_Fraction			=	( ((SystemCoreClock/BAUDR)%16)*16	);
	return ( (uint16_t)(DIV_Mantissa|DIV_Fraction) );
}
//	END BRR

void 			init_usart(uint16_t brr){

	RCC->APB2ENR	|= 	RCC_APB2ENR_USART1EN;
	USART1->BRR		= 	BRR(brr);
	USART1->CR1  	|= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
	NVIC_EnableIRQ (USART1_IRQn);
	init_gpio(GPIOA,TX_PIN,OUTPUT_ALLTER_PUSH_PULL,OUTPUT_MODE_10MHz);
	init_gpio(GPIOA,RX_PIN,INPUT_FLOAT,INPUT_MODE);

}
//	END INIT