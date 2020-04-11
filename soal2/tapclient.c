#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <termios.h>

#define PORT 8080

pthread_t tid[1];
pthread_t yid[3];

// Getch Template

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}
 
/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}
 
/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
 
/* 
Read 1 character without echo 
getch() function definition.
*/
char getch(void) 
{
  return getch_(0);
}

// Getch Template


struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;
char username[55], password[55], command[55];
char buffer[1024] = {0};
int isFound = 0, firsttry = 1, health = 100;

void* theTapTap(void *arg)
{
    pthread_t id = pthread_self();

    if(pthread_equal(id, yid[0])) // The tappers
    {
        printf("\e[1;1H\e[2J");
        printf("Game starting in 3 seconds ...");

        int j;

        for(j = 3; j > 0; j--)
        {
            printf("%d\n", j);
            sleep(1);
        }

        printf("Game starts now! Tap tap as fast as you can!\n");
        printf("Press [SPACE] to tap.\n");

        char space;

        while(1) // loop to input the space.
        {
            if(health == 0)
            {
                break;
            }

            space = getch();

            if(space == 32)
            {
                printf("Hit!\n");
                send(sock, "hit", strlen("hit"), 0);
            }
        }

        return 0;
    }
    else if(pthread_equal(id, yid[1])) // Health Counter
    {
        while(1)
        {   
            if(firsttry != 1)
            {
                valread = read(sock, buffer, 0);

                if(strcmp(buffer, "hit") == 0)
                {
                    health = health - 10;
                    printf("You've been hit! Your current health : %d\n", health);
                    memset(buffer, 0, sizeof(buffer));
                }
                else if(strcmp(buffer, "win") == 0)
                {
                    printf("You win!\n");
                    memset(buffer, 0, sizeof(buffer));
                }

                if(health == 0)
                {
                    send(sock, "win", strlen("win"), 0);
                    printf("You lose!\n");
                    break;
                }
            }
            
        }

        return 0;
    }
}

void* theGame(void *arg)
{   
    pthread_t id = pthread_self();

    if(pthread_equal(id, tid[0])) // the game
    {
        char cmd[7];

        while(1)
        {
            printf("\e[1;1H\e[2J"); // clear screen
            printf("Welcome to TapTap!\nHere's a list of command:\n1. find -> to find match.\n2. logout -> to logout and exit the game.\n\n");
            printf("Please enter a command: ");
            scanf("%s", cmd);

            if(strcmp(cmd, "logout") == 0)
            {
                printf("Thank you for playing!\n");
                send(sock, "logout", strlen("logout"), 0); // sending logout message
                sleep(2);
                 
                break;
            }
            else if(strcmp(cmd, "find") == 0)
            {
                while(1)
                {
                    printf("Finding match for you ...\n");
                    send(sock, "find", strlen("find"), 0); // sending find message for server

                    if(isFound == 1)
                    {
                        break;
                    }

                    memset(buffer, 0, sizeof(buffer));
                    sleep(10); 
                }
                

                if(isFound == 1)
                {
                    isFound = 0;
                    printf("Match found! Entering the game ...\n");
                    sleep(2);

                    int error, a = 0;

                    while(a < 2)
                    {
                        error = pthread_create(&(yid[a]), NULL, &theTapTap, NULL); // Creating Threads

                        if(error != 0)
                        {
                            printf("Can't create threads : [%s]", strerror(error));
                        }

                        a++;
                    }
                    
                    pthread_join(yid[0], NULL);
                    pthread_join(yid[1], NULL);
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }

        return 0;
    }
    else if(pthread_equal(id, tid[1]))
    {
        while(1)
        {   
            valread = read(sock, buffer, 0);
            if(strcmp(buffer, "matchfound") == 0)
            {
                printf("Match Found.\n");
                isFound = 1;
                break;
            }
        }

        return 0;
    }
}

int main(int argc, char const *argv[])
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    int err;

    int loginattempt = 0;
    int option = 1;
    char optionstr[5];

    printf("\e[1;1H\e[2J");
    printf("Welcome to the TapTap game.\n");
    printf("This is a very simple TapTap game.\n");

    while(1) // login & register loop
    {   
        if(option == 1) // after login
        {
            if(strcmp(buffer, "true") == 0 && loginattempt > 0) // IF CREDENTIALS ARE TRUE
            {
                printf("Login successful!\n\n\n");
                memset(buffer, 0, sizeof(buffer));
                
                int i = 0;

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

                printf("\e[1;1H\e[2J");
                printf("Welcome to the TapTap game.\n");
                printf("This is a very simple TapTap game.\n");
            }
            else if(loginattempt > 0) // IF CREDENTIALS ARE FALSE
            {
                printf("Login failed. Please try again.\n\n");
                memset(buffer, 0, sizeof(buffer));
            }
        }
        else if(option == 2)
        {
            if(strcmp(buffer, "true") == 0)
            {
                printf("Registration successful! Please login now.\n\n");
                memset(buffer, 0, sizeof(buffer));
                option = 1;
            }
            else
            {
                printf("Registration error. Please try again.\n\n"); // registration error
                memset(buffer, 0, sizeof(buffer));
                option = 2;
            }
        }

        printf("Select option:\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n\n");
        printf("Input your selection (1,2,3): ");
        scanf("%d", &option);

        memset(optionstr, 0, sizeof(optionstr)); // resetting option
        sprintf(optionstr, "%d", option);
        send(sock, optionstr, strlen(optionstr), 0);

        if(option == 1) // login option
        {
            printf("\n----------------L O G I N----------------\n");
            printf("Enter your username: ");
            scanf("%s", username);
            send(sock, username, strlen(username), 0);

            printf("Enter your password: ");
            scanf("%s", password);
            send(sock, password, strlen(password), 0);
            printf("---------------------------------------\n");
            printf("Your credentials are now sent to the server.\n");
            printf(".\n");
            printf(".\n\n");

            sleep(1);
            send(sock, "next", strlen("next"), 0);
            valread = read( sock , buffer, 1024);
            loginattempt++;
        }
        else if(option == 2) // REGISTER
        {
            if(strcmp(buffer, "true") == 0)
            {
                printf("Register successful! Please login now.\n\n\n");
            }

            printf("\n----------------R E G I S T E R----------------\n");
            printf("Enter your username: ");
            scanf("%s", username);
            //printf("%s\n", username);
            send(sock, username, strlen(username), 0);

            printf("Enter your password: ");
            scanf("%s", password);
            send(sock, password, strlen(password), 0);
            printf("---------------------------------------\n");
            printf("Your credentials are now sent to the server.\n");
            printf(".\n");
            printf(".\n\n");

            sleep(1);
            send(sock, "next", strlen("next"), 0);
            valread = read( sock , buffer, 1024);
        }
        else if(option == 3)
        {
            printf("Exiting the program...\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Wrong input. Please try again.\n\n");
        }
    }

    return 0;
}