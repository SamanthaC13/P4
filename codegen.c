/*Name: Samantha Craig
 * Project 4
 * Class:CS4280*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"token.h"
#include"parser.h"
#include"statsem.h"
struct stackNode* head;
struct stackNode* currentNode;
struct stackNode** headP;
int stackCount=0,blockFlag=0,varCount=0;
void  traverseTree(struct node_t* p)
//Function that traverses the tree using recursion in a pre-order traversal
{
	int i,varsFlag=0;
	if(strcmp(p->nodeName,"vars")==0)
	//if a vars node is encountered then the varsFlag is set to 1
	{
		varsFlag=1;	
	}
	if(p->startToken!=NULL)
	//In each node all the tokens in that node are looped through
	{
		int j=0;
		struct tokenType* tokenP=p->startToken;
		while(j<p->numOfTokens)
		{   
			if(tokenP->tokenID==IDTK)
			//Looking for all the ID tokens to see if they need to be added to the stack or verified that they have been defined
			{
				if((varsFlag==1)&&(blockFlag==0))
				//this option is for variables declared not in a block (global)
				{
					if(verify(tokenP->tokenInstance))
					{	
						errorMsg(tokenP,2);
					}
					push(tokenP);
				}
				if((varsFlag==0)&&(blockFlag==0))
				//This option is for variables that are used not in a block
				{
					if(!verify(tokenP->tokenInstance))
					{
						errorMsg(tokenP,1);
					}
				}
				if((blockFlag==1)&&(varsFlag==0))
				//this option is for variables that are being used in a block to make sure they were defined in the program
				{
					if(find(tokenP->tokenInstance)==-1)
					{
						errorMsg(tokenP,1);
					}
				}
				if((blockFlag==1)&&(varsFlag==1))
				//this option is for variables defined in a block (local) there is a special case that accounts for variables that are defined with the same name 
				//as a global variable they are allowed in the local scope 
				{
					varCount++;
					if(find(tokenP->tokenInstance)!=-1)
					{
						if(find(tokenP->tokenInstance)>varCount)
						{
							errorMsg(tokenP,2);
						}
					}
					push(tokenP);
				}
			}
			tokenP=tokenP->nextToken;
			j++;
		}
	}
	for(i=0;i<5;i++)
	{
		if((strcmp(p->nodeName,"block")==0)&&(i==0))
		//sets the flag for the start of a block
		{
			blockFlag=1;
		}
		if(p->children[i]!=NULL)
		{
			//the function recursivly calls itself with the children of the node
			traverseTree(p->children[i]);
		}
		if((strcmp(p->nodeName,"block")==0)&&(i==1))
		//gets the end the block when it is being closed it pops all of its local variables off the stack and sets the blockFlag to 0 and the varCount to 0
		{	
			p->blockVarCount=varCount;
			int k=0;
			for(k=0;k<p->blockVarCount;k++)
			{	
				pop();
			}
			blockFlag=0;
			varCount=0;
		}
	}
	return;
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
