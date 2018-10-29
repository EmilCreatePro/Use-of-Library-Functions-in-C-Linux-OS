#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h> //for working with directories
#include <fcntl.h> //for working with files
#include <unistd.h>
#include <string.h>

int main(int nrArgs, char **args)
{

	DIR *myDir;
	int myFile;

	struct dirent *infoDir;

	struct stat infoFile;

	if(nrArgs != 3)
	{
		printf("\nERROR!\nUsage: <exe> <dir> <file>\n");
		exit(1);
	}

	if((myDir = opendir(args[1])) == NULL)
	{
		printf("\n%s is not a Directory!\n", args[1]);
		exit(2);
	}

	if((myFile = open(args[2], O_RDWR)) == -1)
	{
		printf("\n%s does not exist!\n", args[2]);
		exit(3);
	}
	else
	{
		if((lstat(args[2], &infoFile)) == -1)
		{
			printf("\nCould not get info from '%s' file!\n", args[2]);
			exit(4);
		}
		else
		{
			if(!(S_ISREG(infoFile.st_mode)))
			{
				printf("\n%s is not a Regular File!\n", args[2]);
				exit(5);
			}
			else
			{
				while((infoDir = readdir(myDir)) != NULL)
				{
					if((strcmp(infoDir->d_name, ".")) && (strcmp(infoDir->d_name, "..")))
					{	
						char path[256];

						strcpy(path, args[1]);
						strcat(path, "/");
						strcat(path, infoDir->d_name);

						//sprintf(path,"%s/%s",args[1],infoDir->d_name);

						if(lstat(path, &infoFile) == -1)
						{
							printf("\nCould not get info from '%s'!\n", infoDir->d_name);
							exit(6);
						}
						else
						{
							if(S_ISREG(infoFile.st_mode))
							{
								printf("\n%s has %ld blocks allocated.", infoDir->d_name, infoFile.st_blocks);

								if((infoFile.st_blocks) >= 2)
								{
									char content[100];
									char blocksChar[10];

									strcpy(content,infoDir->d_name);
									strcat(content, " has ");
									sprintf(blocksChar,"%ld", infoFile.st_blocks);
									strcat(content, blocksChar);
									strcat(content, " blocks allocated.\n");

									//sprintf(content, "%s has %ld blocks allocated \n", infoDir->d_name, infoFile.st_blocks);

									if(write(myFile, content, strlen(content)) == -1)
									{
										printf("Error when writing in file!");
										exit(7);
									}
								}
								else
								{
								
								}
							}
						}	
						
					}
				}
				printf("\n");
			}

		}
	}

	closedir(myDir);
	close(myFile);
	return 0;
}
