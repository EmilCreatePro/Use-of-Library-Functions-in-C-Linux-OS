#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define block_size 4096

int main(int nrArgs, char **args)
{
 
    DIR *myDir;
    struct stat infoEntry;
    
    char path[100];
    int initialPathLen;
    
    struct dirent *infoDir;
    
    if(nrArgs != 2)
    {
        printf("\nError!\nUsage: <exe> <dir>\n");
        exit(1);
    }
    
    if((myDir = opendir(args[1])) == NULL)
    {
        printf("\n%s is not a directory!\n", args[1]);
        exit(2);
    }
    
    strcpy(path,args[1]);
    strcat(path,"/");
    
    initialPathLen = strlen(path);
    
    while((infoDir = readdir(myDir)) != NULL) //parcurgere nerecursiva
    {
        if((strcmp(infoDir->d_name, "..")) && (strcmp(infoDir->d_name, ".")))
        {
            
            strcat(path, infoDir->d_name);
            
            if(lstat(path, &infoEntry) == -1)
            {
                printf("\nCould not get info on %s!\n", path);
                exit(3);
            }
            
            if((S_ISREG(infoEntry.st_mode)))
            {
                int myFile;
                int len;
                char chr[2];
                char nameSym[block_size];
                int nameLen = 0;
                //printf("%s\n", infoDir->d_name);
                
                myFile = open(path, O_RDONLY);
                
                if(myFile == -1)
                {
                    printf("\nError when opening FILE!\n");
                    exit(4);
                }
                
                if(lseek(myFile, 4, SEEK_SET) == -1)
                {
                    printf("\n%s could not be offset!\n", infoDir->d_name);
                    exit(4);
                }
                
                
                
                while((len = read(myFile, chr, 1)))
                {
                    if(chr[0] == ' ') //if is space stop, se putea face cu strchr dar acuma lasam
                        break;
                    
                    nameSym[nameLen++] = chr[0];
                    nameSym[nameLen] = '\0';
                    
                }
                
                nameSym[strlen(nameSym) - 1] = '\0';//get rid of \n
                
                if(symlink(path, nameSym) == -1)
                {
                    printf("\nCould not create sym link for %s!\n", path);
                    exit(5);
                }
                
                //printf("\n%s\n", nameSym);
                //printf("\nnameSym = %s", nameSym);
                
                printf("\n%s Symbolic Link created succesfully!\n", nameSym);
                
                close(myFile);
            }
            
            
            path[initialPathLen] = '\0'; //reset path
        }
                
    }
    
    
    closedir(myDir);
    return 0;
}
