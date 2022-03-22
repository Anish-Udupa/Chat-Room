#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define INP_BUF_SIZE 100
#define FILE_PATH_SIZE 30

void close_socket(int socket_desc){
	close(socket_desc);
}

void strip_rn(char *str, int n){
    // n is inclusive of \r\n
    str[n-2] = '\0';
}

int authenticate(int new_socket, char *usrname){
    write(new_socket, "Enter your username: ", 21);
    char username[10];
    int size_usr = read(new_socket, username, 10);
    if(size_usr == -1){
        printf("Error while receiving the username.\n");
        return 0;
    }
    strip_rn(username, size_usr);
    // write(new_socket, "Enter your password: ", 21);
    // char password[100];
    // read(new_socket, password, 5);
    
    // if(strcmp(username, "admin\n") == 0 && strcmp(password, "admin\n") == 0){
    //     write(new_socket, "Authentication successful.\n", 30);
    //     printf("Success");
    //     return 1;
    // }
    // else{
    //     write(new_socket, "Authentication failed.\n", 25);
    //     return 0;
    // }

    // Adding user details to users.csv ledger
    FILE *fp = fopen("./users.csv", "a");
    if(fp == NULL){
        printf("Error opening the file.\n");
        return 0;
    }
    // Add details here....
    fclose(fp);
    write(new_socket, "Authentication Successful!\n", 28);
    strcpy(usrname, username);
    return 1;
}

int private_chat(int new_socket, char *usr, char *with){
	int exit = 0;
	
	// Opening the chat file
	FILE *fp;
	char path1[FILE_INPUT_SIZE];
	char path2[FILE_INPUT_SIZE];
	sprintf(path1, "%s_%s.txt", usr, with);
	sprintf(path2, "%s_%s.txt", with, usr);
	if(fp = fopen(path1, "r")){
		fclose(fp);
		if((fp = fopen(path1, "a")) == NULL){
			printf("%s: Error opening chat file with path: %s\n", usr, path1);
			return 0;
		}
	}
	else if(fp = fopen(path2, "r")){
		fclose(fp);
		if((fp = fopen(path2, "a")) == NULL){
			printf("%s: Error opening chat file with path: %s\n", usr, path2);
			return 0;
		}
	}
	else{
		if((fp = fopen(path1, "a")) == NULL){
			printf("%s: Error opening chat file with path: %s\n", usr, path1);
			return 0;
		}
	}
	
	// Communicating with the users
	while(!exit){
		write()
	}

}

void interact(int new_socket, char *usr){
    write(new_socket, "Enter help for list of all commands.\n", 38);
    int no_exit = 1;
    char input[INP_BUF_SIZE];
    int read_size;
    char *op;
    while(no_exit){
        write(new_socket, ">> ", 4);
        read_size = read(new_socket, input, 100);
        strip_rn(input, read_size);
        op = strtok(input, " ");

        // Define operations
        if(strcmp(op, "help") == 0){
            // Help command
            write(new_socket, "Help command\n", 14);
        }
        else if(strcmp(op, "quit") == 0){
            // Quit command
            write(new_socket, "Quitting...\n", 13);
            no_exit = 0;
        }
        else if(strcmp(op, "msg") == 0){
            char *to = strtok(NULL, " ");
            char *msg = strtok(NULL, "^");  // Gets the rest of the string
            FILE *msg_fp = fopen("./messages.csv", "a");
            if(msg_fp == NULL){
                printf("Failed to open message file.\n");
                return;
            }
            else{
                //printf("Writing message to file.\n");
                //printf("To: %s\n", to);
                //printf("Msg: %s\n", msg);

                fprintf(msg_fp, "%s,%s,%s\n", usr, to, msg);
                write(new_socket, "Message successfully written.\n", 31);
                fclose(msg_fp);
            }
        }
        else if(strcmp(op, "inbox") == 0){
            FILE *disp_fp = fopen("./messages.csv", "r");
            if(disp_fp == NULL){
                printf("Failed to open message file to display.\n");
                return;
            }
            else{
                char *line = NULL;
                size_t len = 0;
                write(new_socket, "Displaying inbox:\n", 19);
                write(new_socket, "From\t To\t Message\n", 19);
                //write(new_socket, "------------------\n", 19);
                while(getline(&line, &len, disp_fp) != -1){
                    // printf("Line: %s", line);
                    char *from = strtok(line, ",");
                    char *to = strtok(NULL, ",");
                    char *msg = strtok(NULL, "\n");  // Gets the rest of the message
                    char out_str[INP_BUF_SIZE];
                    // printf("%s\t %s\t %s\n", from, to, msg);
                    if(strcmp(to, usr) == 0){
                    	sprintf(out_str, "%s\t %s\t %s\n", from, to, msg);
                    	write(new_socket, out_str, strlen(out_str) + 1);
                    }
                }
                fclose(disp_fp);
            }
        }
        else if(strcmp(op, "outbox") == 0){
            FILE *disp_fp = fopen("./messages.csv", "r");
            if(disp_fp == NULL){
                printf("Failed to open message file to display.\n");
                return;
            }
            else{
                char *line = NULL;
                size_t len = 0;
                write(new_socket, "Displaying outbox:\n", 20);
                write(new_socket, "From\t To\t Message\n", 19);
                //write(new_socket, "------------------\n", 19);

                while(getline(&line, &len, disp_fp) != -1){
                    // printf("Line: %s", line);
                    char *from = strtok(line, ",");
                    char *to = strtok(NULL, ",");
                    char *msg = strtok(NULL, "\n");  // Gets the rest of the message
                    char out_str[INP_BUF_SIZE];
                    // printf("%s\t %s\t %s\n", from, to, msg);
                    if(strcmp(from, usr) == 0){
                    	sprintf(out_str, "%s\t %s\t %s\n", from, to, msg);
                    	write(new_socket, out_str, strlen(out_str) + 1);
                    }
                }
                fclose(disp_fp);
            }
        }
        else if(strcmp(op, "clear") == 0){
        	// Clear the messages.csv file
        	if(remove("./messages.csv") != 0){
        		printf("%s: Couldnt delete messages.csv during clear", usr);
        		return;
        	}
        	write(new_socket, "Clearing messages... done\n", 27);
        	FILE *create_fp = fopen("./messages.csv", "w");
        	if(create_fp == NULL){
        		printf("Error creating messages.csv file after clearing.\n");
        		return;
        	}
        	fclose(create_fp);
        }
        else if(strcmp(op, "chat") == 0){
        	char *with = strtok(NULL, " ");
        	private_chat(new_socket, usr, with);
        }
        else{
            write(new_socket, "Invalid command\n", 17);
        }
        write(new_socket, "\n", 3);
    }
}

void * connection_handler(void *socket_desc){
	int new_socket = *(int *)socket_desc;
	char usr[10];
        int val = authenticate(new_socket, usr);
        if (val != 1){
            // Auth failed
            write(new_socket, "Some error occoured. Please try again.\n", 40);
            close(new_socket);
            free(socket_desc);
            return 0;
        }
        printf("User %s has logged in.\n", usr);
        interact(new_socket, usr);
        printf("User %s has logged out.\n", usr);
        close(new_socket);
        free(socket_desc);

}

int main(){
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    int *new_sock;
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
        pthread_t sniffer_thread;
	new_sock = malloc(1);
	*new_sock = new_socket;
		
	if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
	{
		printf("could not create thread");
		close_socket(new_socket);
		continue;
	}
		
	//Now join the thread , so that we dont terminate before the thread
	//pthread_join( sniffer_thread , NULL);
        
        //printf("Connection accepted.\n");
        

    }

    if(new_socket < 0){
        printf("Error: Could not accept connection");
	close_socket(socket_desc);
        return 1;
    }
    printf("Server terminated.\n");
    close_socket(socket_desc);
}
