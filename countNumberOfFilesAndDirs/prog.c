#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h> //for working with directories
#include <fcntl.h> //for working with files
#include <unistd.h>
#include <string.h>

/*
	Requirement:
	-you give a single parameter <directory> which will contain a number 
		of subfiles and subdirectories, which can contain other subfiles or subdirectories
	-you must say how many subdirectories and subfiles <directory> has
*/

void countLocations(char *path, int *cntFiles, int *cntDirectories)
{
	DIR *myDir;
	
	if((myDir = opendir(path)) == NULL)
	{
		printf("\nError! Could not open '%s' directory!", path);
		exit(3);
	}

	struct dirent *pointerToDir;

	struct stat infoEntry;

	int initialPathLength = strlen(path);

	while((pointerToDir = readdir(myDir)) != NULL )
	{
		if( (strcmp(pointerToDir->d_name,"..") != 0) && (strcmp(pointerToDir->d_name, ".")))//must be different from root folder and self-folder
		{
			strcat(path, pointerToDir->d_name);
			//printf("%s\n", path);
			if( (lstat(path, &infoEntry)) == -1 )
			{
				printf("\nError when obtaining infoEntry!");
				exit(4);
			}

			if(S_ISDIR(infoEntry.st_mode))
			{
				strcat(path, "/");
				(*cntDirectories)++;
				countLocations(path, cntFiles, cntDirectories);
			}
			else
			{
				if(S_ISREG(infoEntry.st_mode))
				{
					(*cntFiles)++;
				}
			}

			path[initialPathLength] = '\0';//reset the path
			
		}
	}
	

	closedir(myDir);
}

int main(int nrArgs, char **args)
{
	if(nrArgs != 2)
	{
		printf("\nUsage: <exe> <directory>\n\n");
		exit(1);
	}

	struct stat buf;

	if(lstat(args[1], &buf) == -1)
	{
		printf("\nError! No such file or directory exists!\n");
		exit(1);
	}
	else
	{
		if((buf.st_mode & S_IFMT) == S_IFDIR) 
		/*
			or you could write:
			if(S_ISDIR(buf.st_mode))
		*/
		{
			//printf("\n%s is a directory!\n", args[1]);

			char *path;
			int cntF = 0, cntD = 0;
			strcpy(path, args[1]);
			path[strlen(args[1])] = '\0';//put the last character null just to be sure :)
			strcat(path,"/");

			countLocations(path, &cntF, &cntD);

			//printf("\npath -> %s\n", path);

			printf("We have %d directories and %d files.\n", cntD, cntF);
		}
		else
		{
			printf("\n%s is NOT a directory!\n", args[1]);
			exit(2);
		}
	}

	return 0;
}
