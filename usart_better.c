#include "usart_better.h"


uint8_t 	tbuff[BUFF_SIZE];
uint8_t 	rbuff[BUFF_SIZE];
uint8_t 	ownbuff[] = {
	'H','e','l','l','o',',',' '
};

uint8_t		count_t	=	0;
uint8_t		tail_t	=	0;
uint8_t		head_t	=	0;
uint8_t 	count_r	=	0;
uint8_t		tail_r	=	0;
uint8_t		head_r	=	0;

uint8_t 	flag_TXE	=	0;
uint8_t		flag_RXNE	=	0;

uint8_t 	counter_ownword = 0;

uint8_t		is_new_element_r(void){
	return !(get_tail_r()==get_head_r());
}

uint8_t		is_new_element_t(void){
	return !(get_tail_t()==get_head_t());
}


uint8_t		get_count_t(void){
	
	if( get_tail_t() <= get_head_t() ){
		return ( get_head_t()-get_tail_t() );
	}
	else{
		return (MAX_8BYTE_VALUE-get_tail_t()+get_head_t());
	}
}
uint8_t		get_count_r(void){

	if( get_tail_r() <= get_head_r() ){
		return ( get_head_r()-get_tail_r() );
	}
	else{
		return (MAX_8BYTE_VALUE-get_tail_r()+get_head_r());
	}
}

void			set_count_t(uint8_t value){
	count_t	=	value;
}
void			set_count_r(uint8_t value){
	count_r	=	value;
}

uint8_t 	set_flag_TXE(uint8_t i){
	return (flag_TXE	=		 i);
}
uint8_t 	get_flag_TXE(void){
	return flag_TXE;
}

uint8_t 	set_flag_RXNE(uint8_t i){
	return (flag_RXNE	=		 i);
}
uint8_t 	get_flag_RXNE(void){
	return flag_RXNE;
}
//
uint8_t	get_tail_t(void){
	return	tail_t;
}
uint8_t	get_tail_r(void){
	return	tail_r;
}
uint8_t	get_head_t(void){
	return	head_t;
}
uint8_t	get_head_r(void){
	return	head_r;
}
void			set_tail_t(uint8_t value){
	tail_t	=	value;
}
void			set_tail_r(uint8_t value){
	tail_r	=	value;
}
void			set_head_t(uint8_t value){
	head_t	=	value;
}
void			set_head_r(uint8_t value){
	head_r	=	value;
}
//

void			recieve(void){
	set_element(rbuff, (get_head_r()%BUFF_SIZE) ,USART1->DR);
	set_head_r(get_head_r()+1);
}

void			transfer(void){
	if(is_new_element_t()){
		USART1->DR	=	get_element(tbuff, (get_tail_t()%BUFF_SIZE) );
		set_tail_t(get_tail_t()+1);
	}
	else{
		USART1->DR	=	0x0A;
		counter_ownword	=	0;
		set_flag_TXE(FALSE);
		USART1->CR1		&=	~USART_CR1_TXEIE;
	}
}
// end transfer


uint8_t		send_static_own_word(void){
	return (counter_ownword<=OWNBUFF_SIZE);
}

void 			set_element(uint8_t* buff, uint8_t ordinal_num, uint8_t data){
	*(buff+ordinal_num)	=	data;
}

uint8_t		get_element(uint8_t* buff, uint8_t ordinal_num){
	return *(buff+ordinal_num);
}

// end get set element

void 			USART1_IRQHandler(void){
	
	if ( USART1->SR & USART_SR_RXNE ){
		recieve();
	}
 	if ( (USART1->SR & USART_SR_TXE) && get_flag_TXE()){
		if(send_static_own_word()){
			USART1->DR	=	get_element(ownbuff,counter_ownword++);
		}else{
			transfer();
		}
		
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