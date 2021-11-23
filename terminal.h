#ifndef _TERMINAL
#define _TERMINAL
	
	#include "global.h"
	#define 	STATIC_SIZE		14
	void			init_terminal(void);
	void			send_str(void);
	void			prepare_send(void);
	extern 		uint8_t		is_print_ready;
	
	
#endif