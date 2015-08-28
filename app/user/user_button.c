
#include "espressif/esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "espressif/esp8266/gpio_register.h"
#include "espressif/esp8266/pin_mux_register.h"
#include "gpio.h"
#include "user_button.h"

void init_button()
{
	//select gpio function
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO5);  //gpio4, D5
	//config io mode
	GPIO_ConfigTypeDef gpiox;
	gpiox.GPIO_Pin = GPIO_Pin_5;
	gpiox.GPIO_Mode = GPIO_Mode_Output;
	gpiox.GPIO_Pullup = GPIO_PullUp_DIS;
	gpiox.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;
	gpio_config(gpiox);
	//set to default value
	GPIO_OUTPUT_SET(5,1);

	//select gpio function
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO4);  //gpio13, D6
	//config io mode
	gpiox.GPIO_Pin = GPIO_Pin_4;
	gpiox.GPIO_Mode = GPIO_Mode_Output;
	gpiox.GPIO_Pullup = GPIO_PullUp_DIS;
	gpiox.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;
	gpio_config(gpiox);
	//set to default value
	GPIO_OUTPUT_SET(4,1);

	//select gpio function
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO13);  //gpio13, on_off
	//config io mode
	gpiox.GPIO_Pin = GPIO_Pin_13;
	gpiox.GPIO_Mode = GPIO_Mode_Output;
	gpiox.GPIO_Pullup = GPIO_PullUp_DIS;
	gpiox.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;
	gpio_config(gpiox);
	//set to default value
	GPIO_OUTPUT_SET(13,0);


	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14); //gpio14,powerdown_time
	//config io mode
	gpiox.GPIO_Pin = GPIO_Pin_14;
	gpiox.GPIO_Mode = GPIO_Mode_Output;
	gpiox.GPIO_Pullup = GPIO_PullUp_DIS;
	gpiox.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;
	gpio_config(gpiox);
	//set to default value
	GPIO_OUTPUT_SET(14,0);


	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);  //gpio12,heat_tune
    //config io mode
	gpiox.GPIO_Pin = GPIO_Pin_12;
	gpiox.GPIO_Mode = GPIO_Mode_Output;
	gpiox.GPIO_Pullup = GPIO_PullUp_DIS;
	gpiox.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;
	gpio_config(gpiox);
	//set to default value
	GPIO_OUTPUT_SET(12,0);


	gpio16_output_conf();  //gpio16, temp_tune
	//set to default value
    gpio16_output_set(0);

}


void press_button(type_button button)
{
	switch(button){
		case BUTTON_ON_OFF:
			GPIO_OUTPUT_SET(13,1);
			vTaskDelay(200/portTICK_RATE_MS);
			GPIO_OUTPUT_SET(13,0);
			vTaskDelay(200/portTICK_RATE_MS);
			printf("BUTTON_ON_OFF actioned\n\r");
			break;

		case BUTTON_TEMP_TUNE:
			gpio16_output_set(1);
			vTaskDelay(200/portTICK_RATE_MS);
			gpio16_output_set(0);
			vTaskDelay(200/portTICK_RATE_MS);
			printf("BUTTON_TEMP_TUNE actioned\n\r");
			break;
		case BUTTON_AUTO_MODE:

			break;
		case BUTTON_POWERDOWN_TIME:
			GPIO_OUTPUT_SET(14,1);
			vTaskDelay(200/portTICK_RATE_MS);
			GPIO_OUTPUT_SET(14,0);
			vTaskDelay(200/portTICK_RATE_MS);
			printf("BUTTON_POWERDOWN_TIME actioned\n\r");
			break;
		case BUTTON_HEAT_TUNE:
			GPIO_OUTPUT_SET(12,1);
			vTaskDelay(200/portTICK_RATE_MS);
			GPIO_OUTPUT_SET(12,0);
			vTaskDelay(200/portTICK_RATE_MS);
			printf("BUTTON_HEAT_TUNE actioned\n\r");
			break;
	}
}
