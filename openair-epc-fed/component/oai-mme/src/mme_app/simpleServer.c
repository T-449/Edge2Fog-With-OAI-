// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "log.h"
#include "intertask_interface.h"
#include "assertions.h"
#include "s6a_messages_types.h"
#include "3gpp_23.003.h"

#define PORT 9999

static void *startServer()
{
    itti_mark_task_ready(TASK_SERVER);
	
    MessageDef *imsiMessage = NULL;
    s6a_auth_info_req_t *auth_info_req = NULL;
    imsiMessage = itti_alloc_new_message(TASK_SERVER, S6A_AUTH_INFO_REQ);
    auth_info_req = &imsiMessage->ittiMsg.s6a_auth_info_req;

    
    int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024];

    // Creating socket file descriptor
    OAILOG_DEBUG(LOG_S6A, "Trying to create socket \n");
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
		OAILOG_ERROR(LOG_S6A, "Socket creation failed \n");
    else
        OAILOG_DEBUG(LOG_S6A, "Socket creation successful \n");
	

	// Forcefully attaching socket to the port 9999
    OAILOG_DEBUG(LOG_S6A, "Trying to set sockopt \n");
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		OAILOG_ERROR(LOG_S6A, "Setting sockopt failed \n");
    else
        OAILOG_DEBUG(LOG_S6A, "Setting sockopt successful \n");


	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("192.168.61.3");
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 9999
    OAILOG_DEBUG(LOG_S6A, "Trying to bind to port %d \n", PORT);
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
        OAILOG_ERROR(LOG_S6A, "Binding failed \n");
    else
        OAILOG_DEBUG(LOG_S6A, "Binding successful \n");


	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

    OAILOG_DEBUG(LOG_S6A, "Waiting for client \n");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
        OAILOG_ERROR(LOG_S6A, "Failed to connect to cliet \n");
    else
        OAILOG_DEBUG(LOG_S6A, "Successfully connected to client \n");

      
    
    OAILOG_DEBUG(LOG_S6A, "Waiting to receive message from IdP \n");
    valread = read( new_socket , buffer, 1024);
    if(close(new_socket) == -1)
        OAILOG_DEBUG(LOG_S6A, "Couldn't close client socket \n");
    else
        OAILOG_DEBUG(LOG_S6A, "Successfully closed client socket\n");

    if(close(server_fd) == -1)
        OAILOG_DEBUG(LOG_S6A, "Couldn't close server socket \n");
    else
        OAILOG_DEBUG(LOG_S6A, "Successfully closed server socket\n");

    if(valread == -1)
        OAILOG_ERROR(LOG_S6A, "Failed to receive message \n");
    else
    {
        buffer[valread] = '\0';
        OAILOG_DEBUG(LOG_S6A, "Received message from Idp ---- %s \n", buffer);

            
        for(int i=0; i <= valread; i++)
            auth_info_req->imsi[i] = buffer[i];

        plmn_t plmn;
        plmn.mcc_digit1 = buffer[0];
        plmn.mcc_digit2 = buffer[1];
        plmn.mcc_digit3 = buffer[2];
        plmn.mnc_digit1 = buffer[3];
        plmn.mnc_digit2 = buffer[4];
        plmn.mnc_digit3 = buffer[5];

        memset(buffer, 0, sizeof buffer);

        auth_info_req->visited_plmn = plmn;
        auth_info_req->imsi_length = strlen(auth_info_req->imsi);
        auth_info_req->nb_of_vectors = 1;
        auth_info_req->re_synchronization = 0;
        memset(auth_info_req->auts, 0, sizeof auth_info_req->auts);
        itti_send_msg_to_task(TASK_S6A, INSTANCE_DEFAULT, imsiMessage);
    }
	return 0;
}


int serverInit()
{
    OAILOG_DEBUG(LOG_S6A, "Initializing Server \n");
    if (itti_create_task(TASK_SERVER, &startServer, NULL) < 0) 
    {
        OAILOG_ERROR(LOG_S6A, "Cannot initialize server\n");
        return -1;
    }
    return 0;
}