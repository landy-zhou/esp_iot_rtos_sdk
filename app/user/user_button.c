
#include "user_button.h"

void ICACHE_FLASH_ATTR
init_button()
{
	//select gpio function
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);

	//config io mode
	GPIO_ConfigTypeDef gpiox;
	gpiox.GPIO_Pin = GPIO_Pin_4;
	gpiox.GPIO_Mode = GPIO_Mode_Output;
	gpiox.GPIO_Pullup = GPIO_PullUp_DIS;
	gpiox.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;

	gpio_config(gpiox);

	//set to default value
	GPIO_OUTPUT_SET(4,0);



	
}


void ICACHE_FLASH_ATTR
press_button(type_button)
{
	GPIO_OUTPUT_SET(4,1);
	delay(500);
	GPIO_OUTPUT_SET(4,0);
	delay(500);
}

