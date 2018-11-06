#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define block_size 4096

int main(int nrArgs, char **args)
{
    
    if(nrArgs < 3)
    {
        printf("\nError!\nUsage: <exe> <N> file1 file2 ... \n");
        exit(1);
    }
    
    int N = atoi(args[1]);
    
    int pid, status;
    
    if( ( pid=fork() ) < 0)
        {
            perror("Eroare");
            exit(1);
        }
        if(pid==0)
        {
            for(int i = 0; i < N; i++)
            printf("\nPID %d\n",i + 1);
            exit(2);
        }
        /* codul parintelui */
        
    for(int i = 0; i < nrArgs - 2; i++)
    {
        if( ( pid=fork() ) < 0)
        {
            perror("Eroare");
            exit(3);
        }
        if(pid==0)
        {
            //char* args[]={"wc","-l","/full/path/1.txt",NULL};
            execlp("wc", "wc", "-l",args[i+2], NULL);
            printf("Eror for execv: %d\n", execv);
            exit(i+4);
        }
        
    }
    
    for(int i = 0; i < nrArgs - 1; i++)
    {
        int wExit = wait(&status);
        
        if (wExit == -1) 
        {
            printf("Error for wait!");
            exit(100);
        }
        if (WIFEXITED(status)) 
        {
            printf("\n\nExit code of child=%d\n", WEXITSTATUS(status));
        }
    }
    
    if(pid != 0)
    printf("\nparinte\n");
    
    return 0;
}
