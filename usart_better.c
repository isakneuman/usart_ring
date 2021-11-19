#include "usart_better.h"


volatile RING_BUFF_t	*RBUFF;
volatile RING_BUFF_t	*TBUFF;

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
	return ring;
}

uint8_t 	flag_TXE	=	0;
uint8_t		flag_RXNE	=	0;

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
	flag_TXE	=		 0;
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
	if( !has_unread(TBUFF) )
	{
		reset_TXE();
	}
	else
	{
		GPIOC->ODR ^= GPIO_ODR_ODR13;
		get_element2(TBUFF,&USART1->DR);
	}
//	USART1->DR = get_element(TBUFF);
	
}
// end transfer

uint8_t		has_unread(	RING_BUFF_t* ring)
{
	volatile uint8_t temp = get_count(ring);
	if(temp)
	{
		return 1;
	}
	return 0;
}

void			write_data(	RING_BUFF_t* ring,
											uint8_t data)
{
	set_element(ring,data);
}
//

void			read_data2(	RING_BUFF_t* ring,
											uint8_t* data)
{
	*data = get_element(ring);
}
//

uint8_t 	read_data(	RING_BUFF_t* ring)
{
	uint8_t temp = 0;
	temp = get_element(ring);
	return temp;
}
//

uint8_t get_TXE(void)
{
	return flag_TXE;
}
//

uint8_t 	get_count(		RING_BUFF_t* ring)
{
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
//

void 			set_element(	RING_BUFF_t* ring,
												uint8_t data)
{
	ring->buff[ ring->head++ % ring->size ] = data;
}
//

void 			get_element2(	RING_BUFF_t* ring,
												uint8_t* data)
{
	*data = ring->buff[ ring->tail++ % ring->size];
}
//

uint8_t 	read_element(	RING_BUFF_t* ring)
{
	return ring->buff[ (ring->tail) % ring->size ];
}

uint8_t		get_element(	RING_BUFF_t* ring)
{
	uint8_t temp;
	temp	=	ring->buff[ ring->tail++ % ring->size ];
	return temp;
}
// end get set element
 
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