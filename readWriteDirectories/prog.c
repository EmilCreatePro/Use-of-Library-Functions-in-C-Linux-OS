#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h> //for working with directories
#include <fcntl.h> //for working with files
#include <unistd.h>
#include <string.h>

/*
	Requirement:
	-You give two arguments from the command line: <fileInput> <directoryOutput> 
	-You then need to write something in the <fileInput> if it's NOT already created
	-<fileInput> will have at least two rows -> you need to save each row on an individual file in the <directoryOutput>

	Obs: the output files can have any name you want
*/

int main(int nrArgs, char **args)
{
	/*
	for(int i = 0; i < nrArgs; i++)
	{
		printf("\n%s", args[i]);
	}
	printf("\n");
	*/

	struct stat buf;
	int myFile;
	DIR *myDir;
	int option;
	int len;
	char text[4096];

	if(nrArgs != 3)
	{
		printf("\nUsage: <exe> <fileInput> <directoryOutput> ");
		exit(1);
	}

	if(lstat(args[1], &buf) == -1)
	{
		printf("\nFile '%s' doesn't exist, it will be created!\n", args[1]);
		if(creat(args[1], S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) == -1)
		{
			printf("\nError! File could not be created!\n");
			exit(2);
		}
		else
		{
			printf("\nFile was created with succes!\n");
		}
	}
	else
	{
		printf("\nFile '%s' already exists!\n", args[1]);
	}

	if(lstat(args[2], &buf) == -1)
	{
		printf("\nFolder '%s' doesn't exist, it will be created!\n", args[2]);
		if(mkdir(args[2], S_IRWXU | S_IRWXG) == -1)
			{
				printf("\nError when creating folder!\n");
				exit(4);
			}
		printf("\nFolder was created with succes!\n");
	}
	else
	{
		printf("\nFolder '%s' already exists!\n", args[2]);
	}

	//open file
	if((myFile = open(args[1], O_RDWR)) == -1)
	{
		printf("Error when opening file!");
		exit(5);
	}

	//open directory
	if((myDir = opendir(args[2])) == NULL)
	{
		printf("Error when opening file!");
		exit(5);
	}

	printf("\nDo you want to write something in this file? (0 -> no, 1 -> yes)\nA: ");
	scanf("%d", &option);

	if(option == 1)
	{
		getchar();
		printf("\nWrite the text:\n");
		fgets(text, 4096, stdin);

		text[strlen(text) - 1] = '\0';// -> get read of the last new line character

		if(write(myFile, text, strlen(text)) == -1)
		{
			printf("\nError when writing in file!\n");
			exit(6);
		}
	}
	else
	{
		//do nothing
	}

	lseek(myFile, 0, SEEK_SET); //go back at the beginning of the file!

	printf("\n");

	int nrFilesCnt = 0;
	int bufferFile;
	char nrFilesCntChr[10];
	char path[100];
	char textBuffer[255];
	int textBufferIndex = 0;

	strcpy(path, args[2]);
	strcat(path,"/");

	printf("\npath: %s\n", path);


	while((len = read(myFile, text, 1)) > 0)
	{
		
			if(text[len -1] == '\n')//len - 1 beacuse a byte was read and len = 1;
			{
				//printf("am intrat aici!");
				nrFilesCntChr[0] = '\0';//reset string
				path[strlen(args[2]) + 1] = '\0';//get rid of the old file
				
				sprintf(nrFilesCntChr, "%d", nrFilesCnt++);//convert integer to string

				//create the path or creating the files
				strcat(path,"fis");
				strcat(path, nrFilesCntChr);
				strcat(path, ".txt");
				//***************************************

				printf("\npath: %s\n", path);

				if(creat(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) == -1)
				{
					printf("\nError! File could not be created!\n");
					exit(7);
				}
				else
				{
					printf("\nFile was created with succes!\n");
				}

				if((bufferFile = open(path, O_RDWR)) == -1)
				{
					printf("Error when opening file!");
					exit(8);
				}

				if(write(bufferFile, textBuffer, strlen(textBuffer)) == -1)
				{
					printf("\nError when writing in file!\n");
					exit(9);
				}

				textBuffer[0] = '\0';//reset the string
				textBufferIndex = 0;//reset length of string

				close(bufferFile);
			}
			else
			{
				//printf("am intrat aici de %d ori!\n", cnt++);
				textBuffer[textBufferIndex++] = text[len-1];
				textBuffer[textBufferIndex] = '\0';
			}
		
		
	}

	close(myFile);
	closedir(myDir);
	return 0;
}

