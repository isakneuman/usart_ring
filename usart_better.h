#ifndef _RING_USART
#define _RING_USART

	#include "global.h"
	
	#define TX_PIN					9
	#define RX_PIN					10
	#define TRUE						1
	#define FALSE						0
	#define BUFF_SIZE				16
	#define OWNBUFF_SIZE		6
	#define	ENTER_VALL1			0x0A
	#define ENTER_VALL2			0x0D
	#define MAX_8BYTE_VALUE	255
	
	extern 	uint8_t 	flag_TXE;
	extern 	uint8_t		flag_RXNE;
	
	extern 	uint8_t 	tbuff[BUFF_SIZE];
	extern 	uint8_t 	rbuff[BUFF_SIZE];
	extern 	uint8_t 	ownbuff[];
	extern	uint8_t		count_r;
	extern	uint8_t		tail_r;
	extern	uint8_t		head_r;
	extern	uint8_t 	count_t;
	extern	uint8_t		tail_t;
	extern	uint8_t		head_t;
	extern	uint8_t		counter_ownword;
	
	uint8_t		get_count_r(void);
	uint8_t		get_count_t(void);
	void			set_count_t(uint8_t);
	void			set_count_r(uint8_t);
	uint8_t		send_static_own_word(void);
	
	void 			set_element(uint8_t*, uint8_t, uint8_t);
	uint8_t		get_element(uint8_t*, uint8_t);
	
	uint8_t		is_new_element_r(void);
	uint8_t		is_new_element_t(void);
	
	uint8_t		get_tail_t(void);
	uint8_t		get_tail_r(void);
	uint8_t		get_head_t(void);
	uint8_t		get_head_r(void);
	void			set_tail_t(uint8_t);
	void			set_tail_r(uint8_t);
	void			set_head_t(uint8_t);
	void			set_head_r(uint8_t);
	
	
	void			transfer(void);
	void			recieve(void);
	
	// TXE flag
	uint8_t set_flag_TXE(uint8_t i);
	uint8_t get_flag_TXE(void);
	
	// RXNE flag
	uint8_t set_flag_RXNE(uint8_t i);
	uint8_t get_flag_RXNE(void);
	
	// BAUDRATE
	uint16_t BRR(uint16_t);
	
	// INIT
	void 	init_usart(uint16_t brr);
	
	// USART IRQHandler
	void USART1_IRQHandler(void);
#endif