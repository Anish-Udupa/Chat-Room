#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

void remove_newline(char *str){
	for(int i=0; str[i] != '\0'; i++){
		if(str[i] == '\n'){
			str[i] = '\0';
			break;
		}
	}
	printf("rmv newline: %s", str);
}

void close_socket(int socket_desc){
	close(socket_desc);
}

int authenticate(int new_socket){
    write(new_socket, "Enter your username: ", 21);
    char username[100];
    read(new_socket, username, 100);
    remove_newline(username);
    
    write(new_socket, "Enter your password: ", 21);
    char password[100];
    read(new_socket, password, 100);
    //remove_newline(password);
    
    printf("123 %s %s", username, password); 
    
    if(strcmp(username, "admin\n") == 0 && strcmp(password, "admin\n") == 0){
        write(new_socket, "Authentication successful.\n", 30);
        printf("Success");
        return 1;
    }
    else{
        write(new_socket, "Authentication failed.\n", 25);
        return 0;
    }
}

int main(){
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1){
        printf("Error: Could not create socket");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Binding the socket
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        printf("Error: Could not bind socket");
        close_socket(socket_desc);
        return 1;
    }

    listen(socket_desc, 3);

    printf("Socket initialized successfully. Waiting for connections.\n");


    // Accepting connections
    int c, new_socket;
    struct sockaddr_in client;
    c = sizeof(struct sockaddr_in);
    while(new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)){
        printf("Connection accepted.\n");
        // char *message = "Hello, client!\n";
        // send(new_socket, message, strlen(message), 0);
        int val = authenticate(new_socket);
        if (val == 1){
            // Auth successful
        }
        close(new_socket);
    }

    if(new_socket < 0){
        printf("Error: Could not accept connection");
	    close_socket(socket_desc);
        return 1;
    }
    printf("Server terminated.\n");
    close_socket(socket_desc);
}