#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/wait.h>
// flag to stop the input 
// since there is no true  / false we can use the binary bit 
#define exitstat  1 
#define exitsucess 0 
#define stdoutfile 1 
int exitchecker = 0;
void exitshell(int signal) {
   exitchecker  = 1 ;
}


int main() {
    signal(SIGINT, exitshell);
    time_t startTime ;
    while (1) {
        time(&startTime);
        printf("\nUnixShell> ");
        char input[1000];
        char identifycommand[1000] ;
        char choice[1000] ;
        char name[1000] ;
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nInvalid Command");
        } 
        else
        {
            size_t length = strlen(input);
            // used the parse method  , calcauted the total number of componets here ;
            // then scanned the inputs   ;
            int parts = sscanf(input, "%s %s %s", identifycommand,choice,name);
            for (int i = 0; i < length; i++) {
                if (input[i] == '\\' || input[i] == '\"' || input[i] == '\'') {
                    printf("\nInvalid Command");
                    break;
                }
            }
            for(int k  =0  ; k<1  ; k++)
            {
                int space = 0 ;
            if (strncmp(input  , "echo ", 5) == 0)
            {
               // printf("yes") ;
               FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input)==0) 
                {
                   printf("Not able to opend the file");
                }
                else 
                {
                fprintf(history, "%s\n", input);
                fclose(history);
                int i = 5  ;
                printf("%s" ,choice) ;
                printf("\n") ;
                }
            }
    else if(parts==3  && strncmp(identifycommand , "grep" , 4)==0)
    {
        FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input)==0) {
                printf("Not able to opend the file");
                fclose(history);
                }
                else 
                {
    int pipearray[2];
    if (pipe(pipearray) <0) {
       printf("pipe creation failed");
       printf("\n") ;
        return 0 ;
    }
    else 
    {
    pid_t pid = fork();

    if (pid < 0) {
        printf("\n") ;
        printf("Fork failed");
        return 0  ;
    } 
    else if (pid == 0)
    {
        close(pipearray[1]);
        dup2(pipearray[0],STDIN_FILENO);
        close(pipearray[0]);
        execlp("grep", "grep",choice , (char *)NULL);
    } 
    else
    {
        close(pipearray[0]);
        int s =1  ;
        FILE *file = fopen(name, "r");
        if (file == NULL) {
            printf("Error opening the file ") ;
            printf("\n");
        }
        else 
        {
        char size[1000];
        while (fgets(size, sizeof(size), file)!=NULL) 
        {
            ssize_t nums  = strlen(size) ;
            write(pipearray[1],size,nums);
        }
        close(pipearray[1]);
        int status;
        wait(&status);
        if(s==1)
        {
        printf("Exited with  status Code %d\n",WEXITSTATUS(status)) ;
        fclose(file);
        }
    }
    }
    }
}
}  
else if (strncmp(identifycommand, "uniq", 4) == 0 && parts == 2) {
    FILE *history = fopen("history.txt", "a");
    if (history == NULL || strlen(input ) == 0) {
        perror("Error opening history.txt");
    } 
    else 
    {
        pid_t pid = fork();
        if (pid < 0) 
        {
            printf("Error Process id cannot be negative") ;
            return 0 ;
        } 
        else if (pid == 0) 
        {
            fprintf(history, "%s\n", input);
            fclose(history);
            execlp("uniq", "uniq",choice, NULL);
        } 
        else 
        {
            int status;
            wait(&status);

            if (WIFEXITED(status)) {
                printf("Exited with Status code  %d\n", WEXITSTATUS(status));
            }
        }
    }
}
    // here the parse of the string and comapred the indivisult parts
    // the divided string is stored in identifycommand , option  , filename 
    else if(parts ==2 && strncmp(input , "sort" , 4)==0)
    {
        FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                printf("Not able to opend the file");
                fclose(history);
                }
                else 
                {
            pid_t pid = fork();
            fprintf(history, "%s\n", input);
            fclose(history);
            if (pid < 0 ) {
                printf("Error  , Fork Failed ") ;
                return  0 ;
            }
            else if (pid == 0) { 
                // used the exec part here ;
                // drieclty no use of the pipe array[0]  and the pipe array[1] 
                execlp("sort", "sort", choice , NULL);
            } 
            else
            { 
                int status;
                wait(&status);

                if (WIFEXITED(status)) 
                {
                    printf("Exited with Status Code  %d\n", WEXITSTATUS(status));
                }
            }
        }

    }
    else if(strncmp(input  , "ls -l" , 5)==0 ||  strlen(input)==5)
       {
        // stroring thr command in the file for history 
        FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                printf("Not able to opend the file");
                fclose(history);
                }
                else 
                {
                fprintf(history, "%s\n", input);
                fclose(history);
                int pipearray[2];
                if (pipe(pipearray) < 0) {
                    printf("Pipe failed") ;
                    return 0 ;
                }
                else 
                {
                pid_t pid = fork();
                if (pid < 0) 
                {
                    printf("Error Pid cannot be negative") ;
                    return 0 ;
                }

                else if (pid == 0) { 
                    close(pipearray[1]);
                    dup2(pipearray[0], STDIN_FILENO) ;
                    close(pipearray[0]);
                    execlp("ls", "ls", "-l", NULL); 
                } 
                else
                { 
                    close(pipearray[0]);
                    close(pipearray[1]);
                    int status;
                    wait(&status);

                    if (WIFEXITED(status)) {
                        printf("Exited with Status Code  %d\n", WEXITSTATUS(status));
                    }
                    else 
                    {
                        printf("The child Failed to exit ") ;

                    }
                }
                }
                }
       }
       else if(parts==3 && strcmp(identifycommand, "wc") == 0 && strcmp(choice, "-c") == 0)
       {
            //printf("yes");
            FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                   printf("Not able to opend the file");
                }
                else 
                {
                fprintf(history, "%s\n", input);
                fclose(history);
                // Create a child process to execute the 'wc -l filename ' command
                pid_t pid = fork();
                if(pid < 0)
                {
                    printf("Error pid cannot be negative") ;
                }
                else if (pid == 0) { 
                    execlp("wc", "wc", "-c",name, NULL); // child is execing the other code now not the parent one 
                    exit(exitstat);
                } 
                else 
                {
                    int status;
                    wait(&status);

                    if (WIFEXITED(status)) {
                        printf("Child process exited with status %d\n", WEXITSTATUS(status));
                    }
                }
            }

       }
        else if (parts == 3 && strcmp(identifycommand, "wc") == 0 && strcmp(choice, "-l") == 0) {
            //printf("yes");
            FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                   printf("Not able to opend the file");
                }
                else 
                {
                fprintf(history, "%s\n", input);
                fclose(history);
                // Create a child process to execute the 'wc -l filename ' command
                pid_t pid = fork();
                if(pid < 0)
                {
                    printf("Error pid cannot be negative") ;
                }
                else if (pid == 0) { 
                    execlp("wc", "wc", "-l",name, NULL); // child is execing the other code now not the parent one 
                    exit(exitstat);
                } 
                else 
                {
                    int status;
                    wait(&status);

                    if (WIFEXITED(status)) {
                        printf("Child process exited with status %d\n", WEXITSTATUS(status));
                    }
                }
            }
        } 
        else if(strncmp(input,  "ls -R" , 5)==0  || strlen(input)==5)
        {
            // read after the ls -R  :
                FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                   printf("Not able to opend the file");
                   fclose(history);
                }
                else 
                {
                fprintf(history, "%s\n", input);
                fclose(history);
                printf("\n") ;
                char* dictname = input + 6 ; 
                // pipe creation  , pipe[0] = readpointer and pipe[1]  = writepointer
                int array[2];
                if(pipe(array) < 0)
                {
                    printf("Error") ;
                    return 0 ;
                }
                // child going on  
                else 
                {
                pid_t pid = fork();
                // error checking 
                if (pid< 0) {
                    printf("Error Occured during Fork");
                    return 0 ;
                }
                else if (pid == 0) { 
                    close(array[0]) ;
                    dup2(array[1], STDOUT_FILENO); // duplicated the valus of writer(array[1]) into the file 
                    // array to store the  in the array  
                    close(array[1]) ;
                    char extrarray[1000] ;
                    if (strlen(extrarray) > 0) {
                        snprintf(extrarray  , sizeof(extrarray) , " ls -R %s" , dictname) ;
                    
                    }
                    else 
                    {
                        snprintf(extrarray, sizeof(extrarray), "ls -R /home ");
                    }

                    system(extrarray); 
                    exit(exitsucess); // exit sucess is 0 ;
                }  
                // parent process  
                else 
                { 
                    close(array[1]); 
                    size_t outputsize;
                    char size[1000];

                    while ((outputsize = read(array[0], size, sizeof(size))) > 0) {
                        write(stdoutfile, size, outputsize);
                    }

                    close(array[0]);
                }
                }
            }
        }
        // the use of input strlen is that if we put white spaces between the ls and /home 
        // this command will help us detect that both ls /home are there and executer ls / home 
        else if(strncmp(input ,  "ls /home" ,8)==0 || strlen(input)==8)
        {
                char arrayout[1000];
               // printf("\nline 234\n") ;
                FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                   printf("Not able to opend the file");
                }
                else 
                {
              //  printf("\nline 241 \n") ;
                fprintf(history, "%s\n", input);
                fclose(history);
                //pipe creation in the name of array  ;
                int array[2];
                if (pipe(array) < 0) 
                {
                    printf("\n") ;
                    printf("process failed") ;
                    return 0 ;
                }
                else 
                {
                    // created the child 
                pid_t pid = fork();
                if(pid<0)
                {
                    printf("Error cannot fork") ;
                    return 0 ;
                }
                else if (pid == 0) {
                    // basic of the pipe 
                   // printf("\n line 263\n") ;
                    close(array[0]);
                    dup2(array[1], STDOUT_FILENO); //copied the content 
                    close(array[1]); 
                    execlp("ls", "ls" , "/home" , NULL);
                    exit(EXIT_SUCCESS) ;
                }
                else 
                { 
                    //printf("\nlien 271 \n") ;
                    ssize_t size;
                    close(array[1]);
                    while ((size = read(array[0], arrayout, sizeof(arrayout ))) > 0) {
                        //print("Yes") ;
                        write(stdoutfile, arrayout, size);
                    }
                    close(array[0]);
                    int status;
                    wait(&status);
                    if (WIFEXITED(status)) 
                    {
                        //printf("\nline 280\n") ;
                        printf("Exited With status code  %d\n", WEXITSTATUS(status));
                    }
                }  
                }
            }
                
        }
        else if(strncmp(input , "ls" ,2)==0)
            {
               // printf(" ls started ") ;
                pid_t processid ;
                FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input+space)==0) {
                   printf("Not able to opend the file");
                }
                fprintf(history, "%s", input);
                fclose(history);
                
                // using pipe that is used to maintain the inter process communication (IPC) 
                // the array stores the mode of the file descriptor . 
                // pipearray[0] = read end of the pipe that is reading the command . 
                // pipearray[1] = write end of the pipe 
                int pipearray[2];
                if (pipe(pipearray) == -1) {
                    printf("Pipe failed") ;
                }
                // here we fork() to create a child of the parent .
                else 
                {
                processid = fork();
                // here child process is checked 
                if (processid == 0) {
                    // this duplicate command is duplicating the output to the file stdout_fileno 
                    dup2(pipearray[1],1);
                    close(pipearray[0]);
                    close(pipearray[1]);
                    // exec() the child with the ls command , now the child has the code to execte the ls 
                    execlp("ls", "ls", NULL);
                    exit(1) ;
                } 
                else if (processid == -1) 
                {
                    printf("Invalid Process Id ");
                }
                else
                {
                    // Close the write end of the file descriptor
                    close(pipearray[1]);
                    // opend a file to copy the directory names in the lsfile
                    FILE *lsfile = fopen("output.txt", "w");
                    if (lsfile == NULL) {
                        printf("Error opening the file") ;
                        break ;
                    }

                    char array[1000];
                    size_t size;

                    while ((size = read(pipearray[0], array, sizeof(array))) > 0) {
                        // Write the data to the file
                        fwrite(array, sizeof(char), size, lsfile);
                    }

                    // Close the read end of the pipe and the file
                    close(pipearray[0]);
                    fclose(lsfile);

                    // Print the contents of the "output.txt" file
                    lsfile = fopen("output.txt", "r");
                    int counter;
                    while ((counter = fgetc(lsfile)) != EOF) {
                        putchar(counter);
                    }
                    int exitcode;
                    wait(&exitcode);

                    if (WIFEXITED(exitcode))
                    {
                        printf("\n") ;
                        printf("Exited with Status Code  %d\n", WEXITSTATUS(exitcode));
                        printf("\n") ;
                    }
                    else 
                    {
                        printf("Failed to Exit \n");
                    }
                    fclose(lsfile);
                }
            }
            }
            // hisotry command implemenatation using a file 
            else if(strncmp(input , "showhistory" , 11)==0)
            {
                    FILE *history = fopen("history.txt", "r");
                    printf("Your History") ;
                    printf("\n") ;
                    while (fgets(input, sizeof(input), history) != NULL ) {
                                   for (int i = 0; input[i] != '\0'; i++) {
                                    if (input[i] == '\n' || input[i] == '\r') {
                                        input[i] = '\0';
                                        break;
                                    }
                                }
                                if(strlen(input)>0)
                                {
                                printf(">> %s\n",input);
            
                            }
                    }
                    fclose(history);
            }
            else if (strncmp(identifycommand, "cat", 3) == 0 ||  parts == 5) {
                FILE *history = fopen("history.txt", "a");
                if (history == NULL || strlen(input + space) == 0) {
                    printf("Error opening ");
                } 
                else 
                {
                    fprintf(history, "%s\n", input);
                    fclose(history);
                    int com  = system(input);
                    pid_t pid = fork();
                    if (pid < 0) 
                    {
                        printf("Error Process id cannot be negative") ;
                        return 0 ;
                    } 
                    else if (pid == 0) 
                    {
                        execlp("cat" , "cat",choice, NULL);
                    } 
                    else 
                    {
                        int status  ;
                        wait(&status) ;
                        printf(" Exited with Exit code %d  " ,WEXITSTATUS(status)) ;
                    }
                }
            }
            else 
            {
                time_t endTime;
                if (exitchecker == 1) {
                time(&endTime);
                float  executionTime;
                executionTime = difftime(endTime, startTime);
                time_t currentTime;
                time(&currentTime);
                pid_t pid = getpid();
                printf("\nRuntime of the code : %.2f seconds\n", executionTime);
                printf("Current Time: %s", ctime(&currentTime));
                printf("Process ID: %d\n", pid);
                // clearing the history file when the user exits. 
                FILE *history = fopen("history.txt", "w");
                if (history == NULL) {
                    printf("Error") ;
                }
                else
                {
                    fclose(history) ;
                    printf("File cleaned") ;

                }
                exitchecker = 0;
                break;
            }
            else{
                printf("Invalid Command");
            }
            }
            
        }
         
        }
        
    }

    return 0;
}
