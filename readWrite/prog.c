#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

/*
	Requirement:
	you give 2 parameters, <nameFile> the name of a file and <number> (which will be number of bytes);
	create that file and write something in it
	after you did this, read <number> of bytes from the file that was created and if you read
		all the file then write a message "Succes", otherwise write "Fail to read all the bytes,
		there were an extra of '<number> - bytesRead' on the file "

*/

int main(int nrArgs, char **args)
{
	char buffer[4096];

	if(nrArgs != 3)
	{
		perror("Usage: exe <file> <nbytes>");
		exit(1);
	}

	int nrBytes = atoi(args[2]);

	//printf("Ati dat %d octeti\n", nrBytes);

	int myFile = creat(args[1], S_IRUSR | S_IWUSR);//on failre return -1

	if(myFile == -1)
	{
		printf("Error when creating the file!");
		exit(2);
	}
	else
		printf("File was created!\n");


	printf("\n What do you want to write in this file?\n");
	fgets(buffer, 4096, stdin);//4096 is the max size of a block in linux

	buffer[strlen(buffer)-1] = '\0';//put the null character at the end of the word, we get rid of the '\n' character

	if(write(myFile, buffer, strlen(buffer)) == -1)//on failre return -1
	{
		printf("Error when writing in the file!");
		exit(3);
	}

	myFile = open(args[1], O_RDWR);//need to open the file again after creating

	if(myFile == -1)//on failre return -1
	{
		printf("Error after opening the new File!");
		exit(4);
	}

	if(nrBytes > read(myFile, buffer, nrBytes))
	{
		printf("\nSucces! The file was read completely!\n");
		printf("\nContent read:\n%s\n\n", buffer);
	}
	else
	{
		lseek(myFile, 0, SEEK_SET);
		printf("\nFailure! The file was not read completely!\nThere were another %ld bytes left to read.\n", read(myFile, buffer, 4096) - nrBytes);
	}

	close(myFile);

	//printf("\nread -> %ld\n", read(myFile, buffer,nrBytes));//--?concluzie: dupa ce ai creat fisierul trebuie si deschis

	/*
	close(myFile);

	myFile = open(args[1], O_RDWR);

	buffer[0] = '\0';

	if((read(myFile, buffer,nrBytes)) < nrBytes)
		printf("\nThe file has been read completely!\n");
	else
	{
		lseek(myFile, -nrBytes, SEEK_CUR);
		printf("\nThe file has NOT been read completely!\nThere are %ld bytes left!\n", nrBytes - (read(myFile, buffer,nrBytes))) ;
	}

	lseek(myFile, 0, SEEK_SET);
	//printf("There were %ld bytes read!\n", (read(myFile, buffer,nrBytes)));
	*/
	

	//printf("S-a citit %s", buffer);

	return 0;
}
