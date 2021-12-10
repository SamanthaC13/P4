/*Name: Samantha Craig
Class:CS4280
   Project P4
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "parser.h"
#include "codegen.h"
int main(int argc, char**argv)
{
	char* filename; 
	char* outputfilename;
	outputfilename=malloc(sizeof(char)*15);
	//This option if the user only entered the Program name without a file
	if(argc==1)
	{
		char character;
		FILE* temp;
		temp=fopen("temp.txt","w");
		//Takes in the Keyboard input and writes it to a file I call temp.txt
		while((character=getc(stdin))!=EOF)
		{
			fputc(character,temp);
		}
		//Writes the end of file to the temp file I created for the keyboard input
		fputc('\n',temp);
		fputc('\0',temp);
		fclose(temp);
		filename=malloc(sizeof(char)*10);
		strcpy(filename,"temp.txt");
		strcpy(outputfilename,"kb");
        }
	else
	{
		//filename is set to the file the user entered with the implict extension .fs
		filename=argv[1];
		strcpy(outputfilename,filename);
		filename=strcat(filename,".fs");
	}
	struct node_t* root;
	root=parser(filename);
	FILE* output;
	outputfilename=strcat(outputfilename,".asm");
	output=fopen(outputfilename,"w");
	traverseTree(root,output);
	printf("\n%s",outputfilename);
	fclose(output);
}
