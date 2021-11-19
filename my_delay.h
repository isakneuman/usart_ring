#ifndef _MY_DELAY
	#define _MY_DELAY
	
	#include "global.h"
	
	uint16_t get_ms(void);
	void 		init_my_delay(void);
	void 		SysTick_Handler(void);
	void 		delay_ms(uint16_t);
	
	void		stop_timeout(void);
	void		start_timeout(void);
	
	void		reset_start_ms(void);
	uint8_t timeout(uint16_t);
	uint8_t get_timeout_flag(void);
	
#endif
