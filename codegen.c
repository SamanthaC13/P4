/*Name: Samantha Craig
 * Project 4
 * Class:CS4280*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"token.h"
#include"parser.h"
#include"codegen.h"
struct stackNode* head;
struct stackNode* currentNode;
struct stackNode** headP;
int stackCount=0,blockFlag=0,varCount=0;
void  traverseTree(struct node_t* p,FILE *out)
//Function that traverses the tree using recursion in a pre-order traversal
{
	if(p==NULL)
	{
		return;
	}
	switch (p->nodeName)
	{
		case "vars":
			fprintf(out,"LOAD %s",p->tokens[1]);
			fprintf(out,"STORE %s",p->tokens[0]);
			traverseTree(p->children[0],out);			
			break;
		case "program":
			traverseTree(p->children[0],out);//vars
			traverseTree(p->children[1],out);//block
			fprintf(out,"STOP");
			break;
		case "block":
			traverseTree(p->children[0],out);//vars
			traverseTree(p->children[1],out);//stats
			break;
		case "stats":
			traverseTree(p->children[0],out)//stat
			traverseTree(p->chlidren[1],out)//mstat
			break;
		case "mstat":
			traverseTree(p->children[0],out)//stat
			traverseTree(p->children[1],out)//mstat
			break;
		case "stat":
			traverseTree(p->children[0],out)//stat option
			break;
		case "in":
			fprintf(out,"READ %s",p->tokens[0]);
			break;
		case "out":
			break;
		case "if":
			break;
		case "loop":
			break;
		case "goto":
			break;
		case "assign":
			break;
		case "label":
			break;
	}			
}
void push(struct tokenType* addToken)
//function that adds nodes to the stack the data is a token and there is a currentNode to keep track of the current (top) node and the head and head Pointer are represting the start or bootom of the stack
//It also adds to the stack Count 
{
	struct stackNode* temp=malloc(sizeof(struct stackNode));
	temp->IDtoken=addToken;
	temp->next=NULL;
	if(currentNode!=NULL)
	{
		currentNode->next=temp;
		currentNode=temp;	
	}
	else
	{
		currentNode=malloc(sizeof(struct stackNode));
		currentNode=temp;
		head=malloc(sizeof(struct stackNode));
		head=currentNode;
		headP=&head;
	}
	stackCount++;
}
void pop()
//this function takes the top node off the stack it changes the value of the currentNode and frees the memory of the top node
//It also takes one away from the stack count
{
	if(*headP==NULL)
	{
		return;
	}
	struct stackNode* prev=*headP;
	struct stackNode* deleteNode=*headP;
	while(deleteNode->next!=NULL)
	{	
		prev=deleteNode;
		deleteNode=deleteNode->next;	
	}
	free(deleteNode);
	currentNode=prev;
	prev->next=NULL;
	stackCount--;
}
int find(char* identifier)
//this function find how far the node the idenitfer given is in is from the top of the stack
//This is done by finding the node from the bottom of the stack and then taking the stack count minus that value 
//If the identifier is not found then -1 is returned 
{
	struct stackNode* top=*headP;
	int count=1;
	while(top!=NULL)
	{
		if(strcmp(top->IDtoken->tokenInstance,identifier)==0)
		{
			break;
		}
		count++;
		top=top->next;		
	}	
	return (stackCount-count);
}
bool verify(char* identifier)
//This function returns a true or false value based on wether the given identifier is in any of the node on the stack
{
	struct stackNode* stackP;
	if(headP!=NULL)
	{
		stackP=*headP;
	}
	else
	{
		return false;
	}
	while(stackP!=NULL)
	{
		//printf("\n%s",stackP->IDtoken->tokenInstance);
		if(strcmp(stackP->IDtoken->tokenInstance,identifier)==0)
		{
			return true;
		}
		stackP=stackP->next;
	}
	return false;
}
void errorMsg(struct tokenType* token, int errorCase)
//This function print error messages with the token Instance and the line number using the error case given 
{
	if(errorCase==1)
	{
		printf("\nSTATIC SEMENTIC ERROR:%s on line %d has not been defined",token->tokenInstance,token->lineCount); 
	}
	if(errorCase==2)
	{
		printf("\nSTATIC SEMANTIC ERROR:%s on line %d has been defined multiple times",token->tokenInstance,token->lineCount);
	}
	exit(1);
}
