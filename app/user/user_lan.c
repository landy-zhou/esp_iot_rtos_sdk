#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/sockets.h"
#include "json/cJSON.h"

#define UDP_FIND_DEVICE_PORT	1200
#define UDP_BUF_SIZE			128
char udp_msg[UDP_BUF_SIZE];

void task4_lan_func(void *pvParameters)
{
	printf("Hello, start task_find_device!\r\n");

	while (1) {
		struct sockaddr_in server_addr,from_addr;
		int server_sock;
		socklen_t sin_size;
		bzero(&server_addr, sizeof(struct sockaddr_in));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(UDP_FIND_DEVICE_PORT);

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
			}
		}
	}
}

void udp_cmd_prase()
{

}


