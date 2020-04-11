#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
//#include <semaphore.h>

#define PORT 8080

pthread_t tid[2];

int server_fd, new_socket, valread, x, activity, max_clients = 2, client_socket[2], max_sd, sd;
struct sockaddr_in address;
int opt = 1, option;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char username[55], password[55], integer[1000], optionstr[3];
char *truecon = "true", *falsecon = "false", *okay = "Command processed.";
int isHit = 0, isFinding = 0;
//sem_t mutex;

void *theGame(void *arg)
{
    //sem_wait(&mutex);

    pthread_t id = pthread_self();

    memset(buffer, 0, sizeof(buffer));

    if(pthread_equal(id, tid[0])) // From client
    {
        while(1) //SERVER PROGRAM -> MULTI-THREADED
        {
            valread = read( new_socket , buffer, 1024);
            if(strcmp(buffer, "logout") == 0) // logout
            {
                printf("User %d logged out.\n", new_socket);
                memset(buffer, 0, sizeof(buffer));
                return 0;
            }
            else if(strcmp(buffer, "find") == 0) // found match
            {
                printf("User %d is trying to find a match.\n", new_socket);
                isFinding = 1;
                //sem_post(&mutex);
                memset(buffer, 0, sizeof(buffer));
                continue;
            }

            if(strcmp(buffer, "hit") == 0)
            {
                isHit = 1;
                memset(buffer, 0, sizeof(buffer));
            }

            if(strcmp(buffer, "win") == 0)
            {

            }

            memset(buffer, 0, sizeof(buffer));
        }

        return 0;
    }
    else if(pthread_equal(id, tid[1])) // To Client
    {
        //printf("Mutex\n");
        while(1)
        {
            if(isHit == 1)
            {
                send(new_socket, "hit", strlen("hit"), 0);
                isHit = 0;
            }

            if(isFinding == 1)
            {
                //sleep(5);
                //sem_wait(&mutex);
                send(new_socket, "matchfound", strlen("matchfound"), 0);
                isFinding = 0;
                //sem_post(&mutex);
            }
        }

        return 0;
    }


}

int makeFile(char username[55], char password[55], int option) // Function to make a file. Option = Login/Register (1/2). Return 1 if true, 0 if false.
{
    int i;
    FILE *fp; // File Pointer
    char temp[512];
    char str[200];

    if(option == 1) // Login -> Read from file
    {
        fp = fopen("akun.txt", "r"); //r -> read

        strcpy(str, "username=");
        strcat(str, username);
        strcat(str, ";password=");
        strcat(str, password);
        strcat(str, ";\n");

        while(fgets(temp, 512, fp) != NULL) // Comparing the data with one in the textfile
        {
            if((strcmp(temp, str)) == 0) // If match found.
            {
                fclose(fp);
                return 1; // True. Match found.
            }
        }
        fclose(fp);

        return 0; // If no match found.
    }
    else if(option == 2) // Register -> Write to file
    {
        fp = fopen("akun.txt", "a"); //w -> write

        strcpy(str, "username=");
        strcat(str, username);
        strcat(str, ";password=");
        strcat(str, password);
        strcat(str, ";\n");

        printf("%s\n", str);

        for (i = 0; str[i] != '\n'; i++)
        {
          /* write to file using fputc() function */
          fputc(str[i], fp);
        }
        fputs("\n",fp);
        fclose(fp);

        return 1;
    }
    
    return 0; // ERROR
}

int main(int argc, char const *argv[])
{   
    //sem_init(&mutex, 0, 1);  
    fd_set readfds;

    for (x = 0; x < max_clients; x++)   
    {   
        client_socket[x] = 0;   
    }  

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    

    while(1) // WAITING FOR MULTIPLE CONNECTION
    {
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for ( x = 0 ; x < max_clients ; x++)   
        {   
            //socket descriptor  
            sd = client_socket[x];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)  
                FD_SET(sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }

        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }

        if(FD_ISSET(server_fd, &readfds)) // Incoming Connection
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //add new socket to array of sockets
            for (x = 0; x < max_clients; x++)   
            {   
                //if position is empty  
                if( client_socket[x] == 0 )   
                {   
                    client_socket[x] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , x);   
                         
                    break;   
                }   
            }

            while(1) // waiting for credentials
            {
                valread = read( new_socket , buffer, 1024);
                strcpy(optionstr, buffer);
                memset(buffer, 0, sizeof(buffer));
                if(strcmp(optionstr, "1") == 0)
                {
                    printf("User %d selected login option.\n", new_socket);
                    option = 1;
                }
                else if(strcmp(optionstr, "2") == 0)
                {
                    printf("User %d selected register option.\n", new_socket);
                    option = 2;
                }
                else if(strcmp(optionstr, "3") == 0)
                {
                    printf("User %d exitted.\n", new_socket);
                    memset(buffer, 0, sizeof(buffer));
                    exit(EXIT_SUCCESS);
                }

                valread = read( new_socket , buffer, 1024);
                strcpy(username, buffer);
                printf("User %d inputted username.\n", new_socket);
                memset(buffer, 0, sizeof(buffer));

                valread = read( new_socket , buffer, 1024);
                strcpy(password, buffer);
                printf("User %d inputted password.\n", new_socket);
                memset(buffer, 0, sizeof(buffer));

                valread = read(new_socket, buffer, 1024);
                if(strcmp(buffer, "next") == 0) // next step
                {
                    if(option == 1) // login
                    {
                        //printf("Username: %s Password: %s\n", username, password);
                
                        if(makeFile(username, password, 1) == 1) // if true
                        {
                            send(new_socket , truecon , strlen(truecon) , 0 );
                            printf("User %d: Authentication successful.\n\n\n", new_socket);
                            
                            //Creating thread for the game.
                            int i = 0, err;
                            
                            while(i < 2)
                            {
                                err = pthread_create(&(tid[i]), NULL, &theGame, NULL); // Creating Threads

                                if(err != 0)
                                {
                                    printf("Can't create threads : [%s]", strerror(err));
                                }

                                i++;
                            }

                            pthread_join(tid[0], NULL);
                            //pthread_join(tid[1], NULL);
                        }
                        else
                        {
                            send(new_socket, falsecon, strlen(falsecon), 0);
                            printf("User %d: Authentication failed. Trying again...\n", new_socket);
                        }
                    }
                    else if(option == 2) // register
                    {
                        //printf("Username: %s Password: %s\n", username, password);

                        if(makeFile(username, password, 2) == 1) // if true
                        {
                            send(new_socket , truecon , strlen(truecon) , 0 );
                            printf("User %d: Registration successful.\n\n\n", new_socket);
                        }
                        else
                        {
                            send(new_socket, falsecon, strlen(falsecon), 0);
                            printf("User %d: Registration failed. Trying again...\n",new_socket);
                        }
                    }
                    else // error
                    {
                        exit(EXIT_FAILURE);
                    }            
                }
                memset(buffer, 0, sizeof(buffer));
            }
        }
        
        for (x = 0; x < max_clients; x++)   
        {   
            sd = client_socket[x];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {   
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&address ,(socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    client_socket[x] = 0;   
                }   
                     
                //Echo back the message that came in  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    buffer[valread] = '\0';   
                    send(sd , buffer , strlen(buffer) , 0 );   
                }   
            }   
        }   
    }

    //sem_destroy(&mutex);
    return 0;
}