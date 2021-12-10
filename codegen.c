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
static int labelCount=0,tempVarCount=0;
typedef enum {VAR, LABEL} nameType;
static char name[20];
static char* newName(nameType);
void  traverseTree(struct node_t* p,FILE *out)
//Function that traverses the tree using recursion in a pre-order traversal
{
	int num=0;
	char label1[20],label2[20],arg1[20],arg2[20];
	if(p==NULL)
	{
		return;
	}
	switch (p->nodeId)
	{
		case Vars:
			if(blockFlag==0)
			{
				if(verify(p->tokens[0]->tokenInstance))
				{
					errorMsg(p->tokens[0],2);
				}
				else
				{
					fprintf(out,"\nLOAD %s",p->tokens[1]->tokenInstance);
					fprintf(out,"\nSTORE %s",p->tokens[0]->tokenInstance);
				}
				push(p->tokens[0]);
			}
			else
			{	
				if(find(p->tokens[0]->tokenInstance)!=-1)
				{
					if(find(p->tokens[0]->tokenInstance)>varCount)
					{
						errorMsg(p->tokens[0],2);
					}
				}
				push(p->tokens[0]);
				fprintf(out,"\nLOAD %s",p->tokens[1]->tokenInstance);
				fprintf(out,"\nPUSH");
				fprintf(out,"\nSTACKW 0"); 
				varCount++;
			}
			traverseTree(p->children[0],out);			
			break;
		case Program:
			traverseTree(p->children[0],out);//vars
			traverseTree(p->children[1],out);//block
			fprintf(out,"\nSTOP");
			while(stackCount!=0)
			{
				fprintf(out,"\n%s 0",currentNode->IDtoken->tokenInstance);
				pop();
			}
			int g=0,j=0;
			for(g=0;g<tempVarCount;g++)
			{
				fprintf(out,"\nT%d 0",g);
			}
			for(j=0;j<labelCount;j++)
			{	
				fprintf(out,"\nL%d 0",j);
			}
			break;
		case Block:
			blockFlag=1;
			//fprintf(out,"START BLOCK-%d",varCount);
			if(p->children[0]!=NULL)
			{	
				varCount=0;
			}
			else
			{
				p->blockVarCount=0;
			}
			traverseTree(p->children[0],out);//vars
			if(p->children[0]!=NULL)
			{
				p->blockVarCount=varCount;
			}
			//fprintf(out,"AFTER VARS-%d",varCount);
			traverseTree(p->children[1],out);//stats
			int k;
			for(k=0;k<p->blockVarCount;k++)
			{
				pop();
				fprintf(out,"\nPOP");
			}
			blockFlag=0;
			//fprintf(out,"END BLOCK-%d",varCount);
			break;
		case Stats:
			traverseTree(p->children[0],out);//stat
			traverseTree(p->children[1],out);//mstat
			break;
		case Mstat:
			traverseTree(p->children[0],out);//stat
			traverseTree(p->children[1],out);//mstat
			break;
		case Stat:
			traverseTree(p->children[0],out);//stat option
			break;
		case In:
			if(find(p->tokens[0]->tokenInstance)==-1)
			{	
				errorMsg(p->tokens[0],1);
			}
			if(find(p->tokens[0]->tokenInstance)<varCount)
			{
				num=find(p->tokens[0]->tokenInstance);
				fprintf(out,"\nREAD %s",newName(VAR));
				fprintf(out,"\nLOAD %s",name);
				fprintf(out,"\nSTACKW %d",num);
			}
			else
			{
				fprintf(out,"\nREAD %s",p->tokens[0]->tokenInstance);
			}
			break;
		case Out:
			traverseTree(p->children[0],out);
			strcpy(arg1,newName(VAR));
			fprintf(out,"\nSTORE %s",arg1);
			fprintf(out,"\nWRITE %s",arg1);
			break;
		case IF:
			traverseTree(p->children[2],out);
			strcpy(arg1,newName(VAR));
			fprintf(out,"\nSTORE %s",arg1);
			traverseTree(p->children[0],out);
			strcpy(label1,newName(LABEL));
			if(p->children[1]->tokens[0]->tokenID==GTTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZNEG %s",label1);
				traverseTree(p->children[3],out);	
			}
			else if(p->children[1]->tokens[0]->tokenID==LTTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZPOS %s",label1);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==DBEQTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRPOS %s",label1);
				fprintf(out,"\nBRNEG %s",label1);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==PCTTK)
			{
				fprintf(out,"\nMULT %s",arg1);
				fprintf(out,"\nBRNEG %s",label1);
				traverseTree(p->children[3],out);

			}
			else if(p->children[1]->tokens[0]->tokenID==LBCTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZERO %s",label1);
				traverseTree(p->children[3],out);
			}
			if(p->children[4]!=NULL)
			{
				strcpy(label2,newName(LABEL));
				fprintf(out,"\nBR %s",label2);
				traverseTree(p->children[4],out);
				fprintf(out,"\n%s: NOOP",label2);
			}
			else
			{
				fprintf(out,"\n%s: NOOP",label1);
			}
			break;
		case Loop:
			strcpy(label1,newName(LABEL));
			fprintf(out,"\n%s: NOOP",label1);
			traverseTree(p->children[2],out);
			strcpy(arg1,newName(VAR));
			fprintf(out,"\nSTORE %s",arg1);
			traverseTree(p->children[0],out);
			strcpy(label2,newName(LABEL));
			if(p->children[1]->tokens[0]->tokenID==GTTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZNEG %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==LTTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZPOS %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==DBEQTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRPOS %s",label2);
				fprintf(out,"\nBRNEG %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==PCTTK)
			{
				fprintf(out,"\nMULT %s",arg1);
				fprintf(out,"\nBRNEG %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==LBCTK)
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZERO %s",label2);
				traverseTree(p->children[3],out);
			}
			fprintf(out,"\nBR %s",label1);
			fprintf(out,"\n%s: NOOP",label2);
			break;
		case GoTo:
			if(find(p->tokens[0]->tokenInstance)==-1)
			{
				errorMsg(p->tokens[0],1);
			}
			fprintf(out,"\nBR %s",p->tokens[0]->tokenInstance);
			break;
		case Expr:
			traverseTree(p->children[0],out);
			if(p->tokens[0]!=NULL)
			{
				strcpy(arg1,newName(VAR));
				fprintf(out,"\nSTORE %s",arg1);
				traverseTree(p->children[1],out);
				fprintf(out,"\nADD %s",arg1);
			}
			break;
		case n:
			traverseTree(p->children[0],out);
			if(p->tokens[0]!=NULL)
			{
				strcpy(arg1,newName(VAR));
				fprintf(out,"\nSTORE %s",arg1);
				traverseTree(p->children[1],out);
				if(p->tokens[0]->tokenID==SLTK)
				{
					strcpy(arg2,newName(VAR));
					fprintf(out,"\nSTORE %s",arg2);
					fprintf(out,"\nLOAD %s",arg1);
					fprintf(out,"\nDIV %s",arg2);
				}
				if(p->tokens[0]->tokenID==ASKTK)
				{
					fprintf(out,"\nMULT %s",arg1);	
				}
			}
			break;
		case a:
			traverseTree(p->children[0],out);
			if(p->tokens[0]!=NULL)
			{
				strcpy(arg1,newName(VAR));
				fprintf(out,"\nSTORE %s",arg1);
				traverseTree(p->children[1],out);
				strcpy(arg2,newName(VAR));
				fprintf(out,"\nSTORE %s",arg2);
				fprintf(out,"\nLOAD %s",arg1);
				fprintf(out,"\nSUB %s",arg2);
			}
			break;
		case m:
			traverseTree(p->children[0],out);
			if(p->tokens[0]!=NULL)
			{
				fprintf(out,"\nMULT -1");	
			}
			break;
		case r:
			if(p->tokens[0]->tokenID==IDTK)
			{
				if(find(p->tokens[0]->tokenInstance)==-1)
				{
					errorMsg(p->tokens[0],1);
				}
				if(find(p->tokens[0]->tokenInstance)<varCount)
				{
					num=find(p->tokens[0]->tokenInstance);
					fprintf(out,"\nSTACKR %d",num);	
				}
				else
				{
					fprintf(out,"\nLOAD %s",p->tokens[0]->tokenInstance);
				}
			}
			if(p->tokens[0]->tokenID==NUMTK)
			{
				fprintf(out,"\nLOAD %s",p->tokens[0]->tokenInstance);
			}
			else
			{
				traverseTree(p->children[0],out);
			}
			break;
		case Assign:
			if(find(p->tokens[0]->tokenInstance)==-1)
			{
				errorMsg(p->tokens[0],1);
			}
			if(find(p->tokens[0]->tokenInstance)<varCount)
			{
				num=find(p->tokens[0]->tokenInstance);
				fprintf(out,"\nSTACKR %d",num);
				traverseTree(p->children[0],out);
				fprintf(out,"\nSTACKW %d",num);		
			}
			else
			{
				fprintf(out,"\nLOAD %s",p->tokens[0]->tokenInstance);
				traverseTree(p->children[0],out);
				fprintf(out,"\nSTORE %s",p->tokens[0]->tokenInstance);
			}
			break;
		case Label:
			if(find(p->tokens[0]->tokenInstance)==-1)
			{
				errorMsg(p->tokens[0],1);
			}
			fprintf(out,"\n%s: NOOP",p->tokens[0]->tokenInstance);
			break;
	}			
}
static char* newName(nameType what)
{
	if(what==VAR)//creating new temporary variable
	{
		sprintf(name,"T%d",tempVarCount++);
	}
	else
	{
		sprintf(name,"L%d",labelCount++);
	}
	return(name);
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
