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
	#define TIMEOUT					5000
	#define END_ARRAY				5
	
	void			send_array(void);
	static uint8_t array[END_ARRAY] = "Hello";
	typedef struct {
		uint8_t		head;
		uint8_t		tail;
		uint8_t* 	buff;
		uint8_t 	size;
	}	RING_BUFF_t;
	
	RING_BUFF_t	init_ring_buff(	uint8_t,
															uint8_t,
															uint8_t*,
															uint8_t);
	
	void			set_TBUFF(		RING_BUFF_t*);
	void			set_RBUFF(		RING_BUFF_t*);
	
	void			write_data( 	RING_BUFF_t*,
													uint8_t);
	RING_BUFF_t*	get_RBUFF(void);
	RING_BUFF_t*	get_TBUFF(void);
	
	uint8_t 	read_element(	RING_BUFF_t* ring);
	uint8_t 	read_data(		RING_BUFF_t* );
	
	void			read_data2(		RING_BUFF_t*,
													uint8_t*);
	
	uint8_t 	get_count(		RING_BUFF_t* );
	
	void			send(void);
	void			recieve(void);
	uint8_t		has_unread(		RING_BUFF_t*);
	
	uint8_t 	get_TXE(void);
	void 			set_TXE(void);
	void 			reset_TXE(void);
	
	uint16_t 	BRR(					uint16_t);
	
	void 			init_usart(		uint16_t brr);
	
	void 			USART1_IRQHandler(void);
#endif