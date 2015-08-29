#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/sockets.h"
#include "json/cJSON.h"

#include "user_button.h"

#define UDP_CONTROL_PORT		1200
#define UDP_DEVICE_FIND_PORT	1100
#define UDP_BUF_SIZE			128
char udp_msg[UDP_BUF_SIZE];

void task3_device_find_func(void *pvParameters)
{
	printf("Hello, start task_find_device!\r\n");
	cJSON *json;

	while (1) {
		struct sockaddr_in server_addr,from_addr;
		int server_sock;
		socklen_t sin_size;
		bzero(&server_addr, sizeof(struct sockaddr_in));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(UDP_DEVICE_FIND_PORT);

		int recbytes;

		if (-1 == (server_sock = socket(AF_INET, SOCK_DGRAM, 0))) {
			printf("S > socket error\n");
			vTaskDelay(1000/portTICK_RATE_MS);
			continue;
		}
		printf("S > create socket: %d\n", server_sock);

		if (-1 == bind(server_sock, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr))) {
			printf("S > bind fail\n");
			vTaskDelay(1000/portTICK_RATE_MS);
			continue;
		}
		printf("S > bind port: %d\n", ntohs(server_addr.sin_port));

		while(1){
			int ret;
			int fromlen = sizeof(struct sockaddr_in);
			bzero(&from_addr, sizeof(struct sockaddr_in));
			ret = recvfrom(server_sock,(uint8 *)udp_msg,UDP_BUF_SIZE,0,(struct sockaddr*)&from_addr,(socklen_t *)&fromlen);
			if(ret > 0){
				printf("ESP8266 UDP task > recv %d Bytes from Port %d %s\n",ret,ntohs(from_addr.sin_port), inet_ntoa(from_addr.sin_addr));
				json = cJSON_Parse(udp_msg);
				if(!json){
					printf("cJSON_Parse(%s) Error\n",udp_msg );
					continue;
				}
				else{
					//printf("get json data: %s\n", cJSON_Print(json));
					cJSON *command = cJSON_GetObjectItem(json,"command");

					if( command && !strncmp(command->valuestring,"find_device",11) ){
						uint32 chip_id = system_get_chip_id();
						cJSON *send_json = cJSON_CreateObject();
						cJSON_AddStringToObject(send_json,"command","device_respond");
						cJSON_AddNumberToObject(send_json,"chip_id",chip_id);
						cJSON_AddStringToObject(send_json,"device_type","AOLE-D13XY");

						uint8 *pjson = cJSON_Print(send_json);
						printf("send_json data: %s\n", pjson);
						sendto(server_sock,(uint8*)pjson,strlen(pjson)+1,0,(struct sockaddr *)&from_addr,fromlen);
						free(pjson);

						cJSON_Delete(send_json);
					}
				}
				cJSON_Delete(json);
			}
		}
	}
}







void task4_lan_func(void *pvParameters)
{
	printf("Hello, start task_button_control!\r\n");
	cJSON *json;
	init_button();
	while (1) {
		struct sockaddr_in server_addr,from_addr;
		int server_sock;
		socklen_t sin_size;
		bzero(&server_addr, sizeof(struct sockaddr_in));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(UDP_CONTROL_PORT);

		int recbytes;

		if (-1 == (server_sock = socket(AF_INET, SOCK_DGRAM, 0))) {
			printf("S > socket error\n");
			vTaskDelay(1000/portTICK_RATE_MS);
			continue;
		}
		printf("S > create socket: %d\n", server_sock);

		if (-1 == bind(server_sock, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr))) {
			printf("S > bind fail\n");
			vTaskDelay(1000/portTICK_RATE_MS);
			continue;
		}
		printf("S > bind port: %d\n", ntohs(server_addr.sin_port));

		while(1){
			int ret;
			int fromlen = sizeof(struct sockaddr_in);
			bzero(&from_addr, sizeof(struct sockaddr_in));
			ret = recvfrom(server_sock,(uint8 *)udp_msg,UDP_BUF_SIZE,0,(struct sockaddr*)&from_addr,(socklen_t *)&fromlen);
			if(ret > 0){
				printf("ESP8266 UDP task > recv %d Bytes from Port %d %s\n",ret,ntohs(from_addr.sin_port), inet_ntoa(from_addr.sin_addr));
				json = cJSON_Parse(udp_msg);
				if(!json){
					printf("cJSON_Parse(%s) Error\n",udp_msg );
					continue;
				}
				else{
					uint8 *pjson = cJSON_Print(json);
					printf("get json data: %s\n", pjson);
					free(pjson);

					cJSON *key_button = cJSON_GetObjectItem(json,"button");
					cJSON *key_action = cJSON_GetObjectItem(json,"action");

					if( key_button && key_action && !strncmp(key_action->valuestring,"pressed",7) ){
						type_button button;
						
						if(!strncmp(key_button->valuestring,"on_off",strlen("on_off")))
							button = BUTTON_ON_OFF;
						else if(!strncmp(key_button->valuestring,"temp_tune",strlen("temp_tune")))
							button = BUTTON_TEMP_TUNE;
						else if(!strncmp(key_button->valuestring,"auto_mode",strlen("auto_mode")))
							button = BUTTON_AUTO_MODE;
						else if(!strncmp(key_button->valuestring,"powerdown_time",strlen("powerdown_time")))
							button = BUTTON_POWERDOWN_TIME;
						else if(!strncmp(key_button->valuestring,"heat_tune",strlen("heat_tune")))
							button = BUTTON_HEAT_TUNE;
						
						printf("button %d pressed\n\r",button);
						press_button(button);
					}
				}
				cJSON_Delete(json);
			}
		}
	}
}

void udp_cmd_prase()
{

}


