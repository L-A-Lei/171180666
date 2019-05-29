#include<stdio.h>  
#include<stdlib.h>  
#include<netinet/in.h>  
#include<sys/socket.h>  
#include<arpa/inet.h>  
#include<string.h>  
#include<unistd.h>  
#include<linklist.h>

#define BACKLOG 5       
#define CONCURRENT_MAX 500  
#define SERVER_PORT 11332  
#define BUFFER_SIZE 1024  
#define QUIT_CMD "quit"  
#define MAX_NAME_LEN 10
int client_fds[CONCURRENT_MAX];  
int main(int argc, const char * argv[])  
{  
    char input_msg[BUFFER_SIZE];  
    char recv_msg[BUFFER_SIZE];  
    char name[CONCURRENT_MAX][BUFFER_SIZE]; 
    fd_set server_fd_set;  
    int max_fd = -1;  
    struct timeval tv;
    int j, i;
    int flag;//For name check
    PNode L = CreateList();
    PNode p = (PNode)malloc(sizeof(Node)); 

    struct sockaddr_in server_addr;  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(SERVER_PORT);  
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  

    bzero(&(server_addr.sin_zero), 8);  

    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if(server_sock_fd == -1)  
    {  
        perror("socket error");  
        return 1;  
    }  
    //bind
    int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));  
    if(bind_result == -1)  
    {  
        perror("bind error");  
        return 1;  
    }  
    //listen  
    if(listen(server_sock_fd, BACKLOG) == -1)  
    {  
        perror("listen error");  
        return 1;  
    }  

    while(1)//Initialize the structure server_fd_set  
    {  
        tv.tv_sec = 20;  
        tv.tv_usec = 0;  
        FD_ZERO(&server_fd_set);  
        FD_SET(STDIN_FILENO, &server_fd_set);  
        if(max_fd <STDIN_FILENO)  
        {  
            max_fd = STDIN_FILENO;  
        }  
        FD_SET(server_sock_fd, &server_fd_set);  
        
	if(max_fd < server_sock_fd)  
        {  
            max_fd = server_sock_fd;  
        }  

        for(i =0; i < CONCURRENT_MAX; i++)  
        {  
            if(client_fds[i] != 0)  
            {  
                FD_SET(client_fds[i], &server_fd_set);  
                if(max_fd < client_fds[i])  
                {  
                    max_fd = client_fds[i];  
                }  
            }  
        }  
        int ret = select(max_fd + 1, &server_fd_set, NULL, NULL, &tv);  
        if(ret < 0)  
        {  
            perror("select errorí\n");  
            continue;  
        }  
        else if(ret == 0)  
        {  
            printf("select overtime\n");  
            continue;  
        }  
        else  
        {  
            if(FD_ISSET(STDIN_FILENO, &server_fd_set)) //Send massage to clients 
            {  
                bzero(input_msg, BUFFER_SIZE);  
                fgets(input_msg, BUFFER_SIZE, stdin);  
		for(i = 0; input_msg[i] != '\n'; )
			i++;
		input_msg[i] = '\0';
                if(strcmp(input_msg, QUIT_CMD) == 0)  
                {  
		    exit(0);  
                }  
                for(i = 0; i < CONCURRENT_MAX; i++)  
                {  
                    if(client_fds[i] != 0)  
                    {  
                        send(client_fds[i], input_msg, BUFFER_SIZE, 0);  
                    }  
                }  
            }  
            if(FD_ISSET(server_sock_fd, &server_fd_set)) //Connect with new clients 
            {  
                struct sockaddr_in client_address;  
                socklen_t address_len;  
                int client_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);  
                if(client_sock_fd > 0)  
                {  
                    int index = -1;  
                    for(i = 0; i < CONCURRENT_MAX; i++)  
                    {  
                        if(client_fds[i] == 0)  
                        {  
                            index = i;  
                            client_fds[i] = client_sock_fd;  
                            break;  
                        }  
                    }  
			read(client_sock_fd, name[i], BUFFER_SIZE);
			p = FindList(L, name[i]);
			if(ListEmpty(L) || p == NULL)
			{
				flag = 1;
				write(client_sock_fd, &flag, sizeof(int));
                        	printf("New client(%d):%s joins\n", index, name[i]);  
		    		InsertList(L, name[i]);//Add the new name
			}
			else
			{
				flag = 0;
				write(client_sock_fd, &flag, sizeof(int));//Tell the client that the name is used
				client_fds[i] = 0;//Client failed to connect
				index = -1;
			}
                    if(index < 0)  
                    {  
                        bzero(input_msg, BUFFER_SIZE);  
                        printf("The name has been used! New client failed to join %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));  
                    }  
                }  
            }  
            for(i = 0; i < CONCURRENT_MAX; i++) //Receive from clients and send the massage to all clients 
            {  
                if(client_fds[i] !=0)  
                {  
                    if(FD_ISSET(client_fds[i], &server_fd_set))  
                    {  
                        bzero(recv_msg, BUFFER_SIZE);  
                        long byte_num = recv(client_fds[i], recv_msg, BUFFER_SIZE, 0);  
                        if (byte_num > 0)  
                        {  
                            if(byte_num > BUFFER_SIZE)  
                            {  
                                byte_num = BUFFER_SIZE;  
                            }  
                            recv_msg[byte_num] = '\0';  
                            printf("client(%d)/%s:%s\n", i, name[i], recv_msg);  
			    // Send the massage to all clients
                       	    for(j = 0; j < CONCURRENT_MAX; j++)
			    {
			    	if(client_fds[j] != 0)
				{
					char msg[BUFFER_SIZE];
					strcpy(msg, name[i]);
					strcat(msg, ": ");
					strcat(msg, recv_msg);
                        		send(client_fds[j], msg, BUFFER_SIZE, 0);  
				}
			    }
		       	}  
                        else if(byte_num < 0)  
                        {  
                            printf("receive from client(%d) error.\n", i);  
                        }  
                        else  
                        {  
                            FD_CLR(client_fds[i], &server_fd_set);  
                            client_fds[i] = 0;  
                            printf("client(%d):%s quit\n", i, name[i]);
			    DeleteList(L, name[i]); 
			    name[i][0] = '\0';//Delete a client's name after it logged out 
                        }  
                    }  
                }  
            }  
        }  
    }  
    return 0;  
}

