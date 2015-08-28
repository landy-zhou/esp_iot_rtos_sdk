#ifndef __USER_BUTTON_H__
#define __USER_BUTTON_H__

typedef enum{
	BUTTON_ON_OFF = 0,
	BUTTON_TEMP_TUNE = 1,
	BUTTON_AUTO_MODE = 2,
	BUTTON_POWERDOWN_TIME = 3,
	BUTTON_HEAT_TUNE = 4
}type_button;

void init_button(void);
void press_button(type_button button);

#endif //__USER_BUTTON_H__
