/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/12/1, v1.0 create this file.
*******************************************************************************/
#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

 #include "user_button.h"
 #include "uart.h"

#define server_ip "192.168.101.142"
#define server_port 9669

extern void task3_device_find_func(void *pvParameters);
extern void task4_lan_func(void *pvParameters);

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    
    UART_ConfigTypeDef uart_config;
    uart_config.baud_rate = BIT_RATE_115200;
    uart_config.data_bits = UART_WordLength_8b;
    uart_config.parity = USART_Parity_None;
    uart_config.stop_bits = USART_StopBits_1;
    uart_config.flow_ctrl = USART_HardwareFlowControl_None;
    uart_config.UART_RxFlowThresh = 120;
    uart_config.UART_InverseMask = UART_None_Inverse;
    UART_ParamConfig(UART0, &uart_config);
    
    printf("SDK version:%s\n", system_get_sdk_version());

    /* need to set opmode before you set config */
    wifi_set_opmode(STATIONAP_MODE);

    {
        struct station_config *config = (struct station_config *)zalloc(sizeof(struct station_config));
        sprintf(config->ssid, "NETGEAR46");
        sprintf(config->password, "12345678");

        /* need to sure that you are in station mode first,
         * otherwise it will be failed. */
        wifi_station_set_config(config);
        free(config);
    }

    //xTaskCreate(task2, "tsk2", 256, NULL, 2, NULL);
    //xTaskCreate(task3, "tsk3", 256, NULL, 2, NULL);
    xTaskCreate(task3_device_find_func, "tsk3", 256, NULL, 2, NULL);
    xTaskCreate(task4_lan_func, "tsk4", 256, NULL, 2, NULL);
}

/*
void task2(void *pvParameters)
{
    printf("Hello, welcome to client!\r\n");

    while (1) {
        int recbytes;
        int sin_size;
        int str_len;
        int sta_socket;

        struct sockaddr_in local_ip;
        struct sockaddr_in remote_ip;

        sta_socket = socket(PF_INET, SOCK_STREAM, 0);

        if (-1 == sta_socket) {
            close(sta_socket);
            printf("C > socket fail!\n");
            continue;
        }

        printf("C > socket ok!\n");
        bzero(&remote_ip, sizeof(struct sockaddr_in));
        remote_ip.sin_family = AF_INET;
        remote_ip.sin_addr.s_addr = inet_addr(server_ip);
        remote_ip.sin_port = htons(server_port);

        if (0 != connect(sta_socket, (struct sockaddr *)(&remote_ip), sizeof(struct sockaddr))) {
            close(sta_socket);
            printf("C > connect fail!\n");
            vTaskDelay(4000 / portTICK_RATE_MS);
            continue;
        }

        printf("C > connect ok!\n");
        char *pbuf = (char *)zalloc(1024);
        sprintf(pbuf, "%s\n", "client_send info");

        if (write(sta_socket, pbuf, strlen(pbuf) + 1) < 0) {
            printf("C > send fail\n");
        }

        printf("C > send success\n");
        free(pbuf);

        char *recv_buf = (char *)zalloc(128);
        while ((recbytes = read(sta_socket , recv_buf, 128)) > 0) {
        	recv_buf[recbytes] = 0;
            printf("C > read data success %d!\nC > %s\n", recbytes, recv_buf);
        }
        free(recv_buf);

        if (recbytes <= 0) {
		    close(sta_socket);
            printf("C > read data fail!\n");
        }
    }
}

void task3(void *pvParameters)
{
    while (1) {
        struct sockaddr_in server_addr, client_addr;
        int server_sock, client_sock;
        socklen_t sin_size;
        bzero(&server_addr, sizeof(struct sockaddr_in));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(80);

        int recbytes;

        do {
            if (-1 == (server_sock = socket(AF_INET, SOCK_STREAM, 0))) {
                printf("S > socket error\n");
                break;
            }

            printf("S > create socket: %d\n", server_sock);

            if (-1 == bind(server_sock, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr))) {
                printf("S > bind fail\n");
                break;
            }

            printf("S > bind port: %d\n", ntohs(server_addr.sin_port));

            if (-1 == listen(server_sock, 5)) {
                printf("S > listen fail\n");
                break;
            }

            printf("S > listen ok\n");

            sin_size = sizeof(client_addr);

            for (;;) {
                printf("S > wait client\n");

                if ((client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &sin_size)) < 0) {
                    printf("S > accept fail\n");
                    continue;
                }

                printf("S > Client from %s %d\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));

                char *recv_buf = (char *)zalloc(128);
                while ((recbytes = read(client_sock , recv_buf, 128)) > 0) {
                	recv_buf[recbytes] = 0;
                    printf("S > read data success %d!\nS > %s\n", recbytes, recv_buf);
                }
                free(recv_buf);

                if (recbytes <= 0) {
                    printf("S > read data fail!\n");
                    close(client_sock);
                }
            }
        } while (0);
    }
}
*/


