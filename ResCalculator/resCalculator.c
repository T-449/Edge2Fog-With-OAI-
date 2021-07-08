#include "functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

# define USIM_API_K_VALUE  	"0c0a34601d4f07677303652c0462535b"
# define OPC                "ba05688178e398bedc100674071002cb"

uint8_t setNibbles(uint8_t original, uint8_t upperNibble, uint8_t lowerNibble)
{
    uint8_t result = original;
    result &= 0x00;
    result |= (lowerNibble & 0x0F);
    result |= ((upperNibble << 4) & 0xF0);
    return result;
}

void emptyArr(char* string, int size)
{
  int i;
  for(i=0; i<size;i++)
    string[i] = '\0';
}

void hextoOctet(uint8_t *outputArray, char *hexString, int hexStringSize)
{
    int i;
    int j=0;
    uint8_t lowerNibble = 0;
    uint8_t upperNibble = 0;
    for(i=0;i<hexStringSize;i+=2)
    {
        if(hexString[i] >= '0' && hexString[i] <= '9')
            upperNibble = hexString[i] - 48;
        else
            upperNibble = hexString[i] - 97 + 10;
        
        if(hexString[i+1] >= '0' && hexString[i+1] <= '9')
            lowerNibble = hexString[i+1] - 48;
        else
            lowerNibble = hexString[i+1] - 97 + 10;

        outputArray[j] = setNibbles(outputArray[j], upperNibble, lowerNibble);
        j++;
    }
}



void print(uint8_t *octetArray, int hexStringSize)
{
    int k;
    char k_str[hexStringSize+1];
    emptyArr(k_str, hexStringSize+1);
    for(k=0; k<(hexStringSize/2);k++)
    {
        sprintf(&k_str[strlen(k_str)], "%02x", octetArray[k]);
    }

    printf("%s\n",k_str);
}


void tokenizehexString(char *hexString, char output[][32])
{
    char* token = strtok(hexString, " ");
    int i=0;
    while (token) {
        strncpy(output[i], token, strlen(token));
        token = strtok(NULL, " ");
        i++;
    }
}

int startServer(uint8_t *RAND_Octet)
{
    int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[32];
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons( 10500 );

    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
	
	//Receive a message from client
	if(read_size = recv(client_sock , client_message , sizeof client_message , 0) > 0 )
	{
        char *AUTN_RAND = client_message;
        hextoOctet(RAND_Octet, AUTN_RAND, 32);
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
}

int startClient(char *response, int size)
{
    int sock;
	struct sockaddr_in server;
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 10600 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
	//Send some data
	if( send(sock , response , size , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
		
	close(sock);
	return 0;
}


int main()
{
    uint8_t K_Octet[strlen(USIM_API_K_VALUE)/2];
    uint8_t RAND_Octet[16];
    uint8_t RES_Octet[8];
    uint8_t OP_Octet[16];
    hextoOctet(K_Octet, USIM_API_K_VALUE, strlen(USIM_API_K_VALUE));
    print(K_Octet, strlen(USIM_API_K_VALUE));
    hextoOctet(OP_Octet, OPC, strlen(OPC));
    print(OP_Octet, strlen(OPC));
    startServer(RAND_Octet);
    print(RAND_Octet, 32);
    //char hexstring[] = "713858544ee54a36703e7937564fb550";
    //hextoOctet(RAND_Octet, hexstring, 32);
    f2345(K_Octet, RAND_Octet, RES_Octet, OP_Octet);
    print(RES_Octet, 16);
    char RES_str[17];
    emptyArr(RES_str, 17);
    int k;
    for(k=0; k<8; k++)
        sprintf(&RES_str[strlen(RES_str)], "%02x", RES_Octet[k]);
    printf("%s\n", RES_str);
    startClient(RES_str, 16);
}


/*int k;
    char k_str[hexStringSize+1];
    emptyArr(k_str, hexStringSize+1);
    for(k=0; k<(hexStringSize/2);k++)
    {
        sprintf(&k_str[strlen(k_str)], "%02x", octetArray[k]);
    }

    printf("%s\n",k_str);*/