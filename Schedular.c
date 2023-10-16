#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/types.h>

int ncpu;
int tslice ;
const int COMMAND_SIZE = 500;
#define maxprocess 1000 
typedef struct Process {
    int pid;
    struct Process* next; 
} process;


int create_process_and_run(char* command) {
    char command_copy[COMMAND_SIZE];
    strcpy(command_copy, command);
    char* command_vector[100];
    int i = 0;
    command_vector[i] = strtok(command_copy, " ");
    while(command_vector[i] != NULL) {
        command_vector[++i] = strtok(NULL, " ");
    }
    int pid = fork();
    time_t start = time(NULL);
    if(pid < 0) {
        printf("ERROR in fork\n");
    } else if(pid == 0) {
        execvp(command_vector[0], command_vector);
    } else {
        FILE* history = fopen("history.txt", "a");
        fputs(" Process ID : ", history);
        fprintf(history, "%d", pid);
        waitpid(pid, NULL, 0);
        time_t end = time(NULL);
        double diff = difftime(end, start);
        fputs(" Total Execution Time : ", history);
        fprintf(history, "%lf", diff);
        fclose(history);

}

return 1;
}

int launch (char *command) {
    int status;
    status = create_process_and_run(command);
    return status;
}
typedef struct Node {
    int data;
    struct Node *next;
    char path[512]; 
    char executable_name[256];  
}node;

typedef struct LinkedList {
    struct Node *head;
}readyqueue;
readyqueue  readyq = { .head = NULL };
int sizechecker(readyqueue *readyq) {
    node *current  = readyq->head ;
    int size = 0 ;
    while(current!=NULL)
    {
        current = current->next ;
        size ++ ;
    }
    if(size==maxprocess)
    {
        return 0  ;
        printf("Ready queue is full cant add ") ;
    }
    else{
        return 1 ;
    }
}
void add(readyqueue *readyq, int pid, const char* name) {
    node* mover = (node*)malloc(sizeof(node));
    mover->data = pid;
   // mover->executable_name  = name ;
  //  printf("%s",mover->executable_name) ; error 
  // printf("hello from the add function") ;
    strncpy(mover->executable_name, name, sizeof(mover->executable_name));
    //strncpy(mover->executable_path, path, sizeof(mover->executable_path));

   // printf("%s",mover->executable_name) ;
    mover->next = NULL;
     if (readyq->head == NULL) {
        readyq->head = mover;
        return;
    }
    
    struct Node *current = readyq->head;
    int counter ;
    while (current->next != NULL) {
        current = current->next;
        counter ++ ;
    }

    current->next = mover ;
    free(mover) ;
   
}

void remover(readyqueue *readyq ) {
    if (sizechecker(readyq)!=0)
    {
    node *running = readyq->head;
    readyq->head = running->next;
    free(running); 
    }
}

int checker(const char* command) {
    // null 
    int counter = 0;

    if (strcmp(command, "") == 0) {
        return 0; 
    } 

    else {
        char* token = strtok((char*)command, " ");
        while (token != NULL) {
            counter++;
            token = strtok(NULL, " ");
        }
    }
     if (counter > 2 ) 
     {
        return 1; 
    }
    else 
    {
    return 0;
    }
}
void read_user_input(char* command) {
    fgets(command, COMMAND_SIZE, stdin);
    command[strlen(command) - 1] = '\0';  
}
void pidprinter(int counter , readyqueue *readyq)
{
    node *dataprinter  = readyq->head  ;
    int i = 0 ;
    while(i!=counter)
    {
        printf("%d\n",dataprinter->data) ;
        printf("%s\n",dataprinter->executable_name) ;
        dataprinter  = dataprinter->next ;
        i++ ;
    }
}

void printer(readyqueue *readyq) {
    node *mover = readyq->head;
    int counter = 0;

    while (mover != NULL) {
        counter++;
        mover = mover->next;
    }

    printf("counter %d\n", counter);
    pidprinter(counter  , readyq) ;
}




volatile int scheduling_started = 0;  
void start_handle(int signum) {
    scheduling_started = 1;
  //  printf("Hello form the singal handler") ;
}
int filecheck(node *head , const char *executable_name) {
    printf("Hi form the file checker") ;
   // printf("hello fornm the file checker") ;
    node *current = head;
    while (current != NULL) {
        if (strcmp(current->executable_name, executable_name) == 0) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}
int crosscounter(const readyqueue* queue) {
    const node* current = queue->head;

    if (current == NULL) {
        return 0;
    } else {
     return 1;
    }
}


void shell_loop(char* command, int ncpu, int tslice) {
    int status;
    char pathname[50];            
    int submitchecker = 0;
    struct sigaction sa;
    sa.sa_handler = start_handle;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask) ;
    printf("Welcome:~$ ");
            
    do {
        read_user_input(command);
        int flag = 0;
        if (checker(command) == 1) {
            printf("The command has parameters.\n");
            printf("Try again\n\n");
            printf("Welcome:~$ ");
        }
        else
        {
            const char* calls[] = {
                "read", "write", "sleep", "usleep",
                "nanosleep", "select", "poll", "accept",
                "wait", "waitpid", "pthread_mutex_lock", "pthread_cond_wait",
                "recv", "send", "open", "fopen"
            };

            for (int i = 0; i < sizeof(calls) / sizeof(calls[0]); i++) {
                char* executable = command + 7;
                if (strstr(executable, calls[i]) != NULL) {
                    printf("Error Blocking sys call found (%s).\n", calls[i]);
                }
            }
           
            if (strncmp(command, "submit", 6) == 0) {
                if (submitchecker) {
                    printf("The process is already in the queue.\n");
                } 
                else 
                {
                    char* executable = command + 7;
                    if(strcmp(executable , "exit")==0)
                    {
                        break  ;
                    }
                    int pid = fork();
                    if (pid < 0) 
                    {
                        printf("Fork failed\n");
                    } else if (pid == 0) 
                    {
                       if (sizechecker(&readyq) == 1) {
                            add(&readyq, pid, executable);
                            printf("process addeed ") ;
                        }
                        else {
                            printf("The ready queue is full. Cannot add more items.\n");
                            printer(&readyq);
                        }
                        submitchecker = 1;
                        kill(getpid(), SIGUSR1);
                    } else 
                    {
                        int status;
                        waitpid(pid, &status, 0);                  
                    }
                }
            }
         if (sizechecker(&readyq) > 0) {
    int i = 0;
    pid_t c_pid[ncpu];

    while (i < ncpu) {
        node* currentmover = readyq.head;

        if (currentmover == NULL) {
            break;
        }

        if (filecheck(currentmover, currentmover->executable_name) == 1) {
            printf("This is already in the queue\n");
        }

        pid_t child_pid = fork();

        if (child_pid < 0) {
            printf("Fork failed\n");
        } else if (child_pid == 0) {
            char *const args[] = {currentmover->executable_name, NULL};
            execl(currentmover->executable_name, currentmover->executable_name, (char *)NULL);
            perror("execl"); // Handle execl errors
            exit(1); // Terminate the child process on error
        }

        c_pid[i] = child_pid;
        i++;

        printf("Number of processes running: %d\n", i);

        for (int j = 0; j < i; j++) {
            waitpid(c_pid[j], NULL, 0);
        }

        if (i >= ncpu) {
            sleep(tslice);
        }

        readyq.head = currentmover->next;
        free(currentmover);
    }
}

            else 
            {
                scheduling_started = 0;
            }

            FILE* history = fopen("history.txt", "a");
            char hist[10] = "history";
            char letter;
            if (!strcmp(command, hist)) {
                char hist[10] = "history";
                char letter;
                printf("\n");
                letter = fgetc(history);
                while (letter != EOF) {
                    printf("%c", letter);
                    letter = fgetc(history);
                }
            }
            fputs(command, history);
            fclose(history);
            status = launch(command);
            history = fopen("history.txt", "a");
            fputs("\n", history);
            fclose(history);
        }
    } while (status);
}

void my_handler(int signum) {
    FILE* history = fopen("history.txt", "r");
    char hist[10] = "history";
    char letter;
    printf("\n");
    letter = fgetc(history);
    while(letter != EOF) {
        printf("%c", letter);
        letter = fgetc(history);
    }
    exit(0);
}

int main() {
    while(1)    
    {
   
    printf("enter NCPU ") ;
    scanf("%d",&ncpu) ;
    if(ncpu<1)
    {
        printf("Invalid NCPU") ;
        return 0 ;
    }
    else 
    {
    printf("enter TSLICE ") ;    
    scanf("%d",&tslice) ;
    if(tslice<0)
    {
        printf("TSLICE cannot be negative ") ;
        return 0 ;
    }
   else 
   {
   
     
    struct sigaction sig;
    sig.sa_handler = my_handler;
    sigaction(SIGINT, &sig, NULL);
    char command[COMMAND_SIZE];
    FILE* history = fopen("history.txt", "w");
    fclose(history);
    shell_loop(command , ncpu , tslice);
    return 0;
   }
}
}
}
