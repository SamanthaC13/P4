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
//Checks for statuc semantics and Generates Assembly code
{
	int num=0;
	//Temporary labels and arguments placeholders
	char label1[20],label2[20],arg1[20],arg2[20];
	if(p==NULL)
	//the condition to stop the recursion and return the node
	{
		return;
	}
	switch (p->nodeId)
	//The nodes are sorted based on their nodeId which is set based on their nodeName 
	{
		case Vars:
			//Node for ifa variable is declared
			if(blockFlag==0)
			//the variable is not in a block then it is a global variable at the beginning of the programming and loaded and 
			//declared as a variable
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
				//The variable is pushed to the Symbol Table Stack
				push(p->tokens[0]);
			}
			else
			//The varaible is declared in a block then it is a local variable and is put on the stack and not declared 
			//and the variable count is added by one
			{	
				varCount++;
				if(find(p->tokens[0]->tokenInstance)!=-1)
				{
					if(find(p->tokens[0]->tokenInstance)<varCount)
					{
						errorMsg(p->tokens[0],2);
					}
				}
				//the variable is added to the symbol table stack
				push(p->tokens[0]);
				fprintf(out,"\nLOAD %s",p->tokens[1]->tokenInstance);
				fprintf(out,"\nPUSH");
				fprintf(out,"\nSTACKW 0"); 
			}
			traverseTree(p->children[0],out);			
			break;
		case Program:
			//The first child of program traverses the vars node and the second child traverses to the first block node
			traverseTree(p->children[0],out);//vars
			traverseTree(p->children[1],out);//block
			//When program node is returned to the program is completed 
			//and the global and temporary variables are delcared and the stop command is printed
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
			break;
		case Block:
			blockFlag=1;
			//blockFlag set shows that we are in a block 
			if(p->children[0]!=NULL)
			{
				//if the block is declaring varibles then it will need to reset the variable count if is not then it
				//need to conitnue using the variable count from it's outer block or start at sero if it is the starting block
				varCount=0;
			}
			else
			{
				//however if the block is not delcaring variables it need the variable count to be like the block it is in but it personal 
				//variable Count for how many variables it should pop off the stack needs to be set to 0
				p->blockVarCount=0;
			}
			traverseTree(p->children[0],out);//vars
			if(p->children[0]!=NULL)
			{
				//If the bloack is delcaring any variiables it will do so in the ftraversal of the first child and afterword
				//it can set the new varCount to the amount of variables the block needs to pop off the stack after it is completed 
				p->blockVarCount=varCount;
			}
			traverseTree(p->children[1],out);//stats
			int k;
			for(k=0;k<p->blockVarCount;k++)
			{
				pop();
				fprintf(out,"\nPOP");
			}
			blockFlag=0;
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
			//if the idenifier token has not be declared 
			{	
				errorMsg(p->tokens[0],1);
			}
			if(find(p->tokens[0]->tokenInstance)<varCount)
			//if the idenfiter is in the local stope it needs to be Read and loaded on to the stack using a 
			//temporary variable
			{
				num=find(p->tokens[0]->tokenInstance);
				fprintf(out,"\nREAD %s",newName(VAR));
				fprintf(out,"\nLOAD %s",name);
				fprintf(out,"\nSTACKW %d",num);
			}
			else
			//if the variable is not in the local stope then the value is read into the idenifier given
			{
				fprintf(out,"\nREAD %s",p->tokens[0]->tokenInstance);
			}
			break;
		case Out:
			//First the expresssion is evaluated 
			//Then a temporary variable is used to store the result of the expression and 
			//write it to the output
			traverseTree(p->children[0],out);
			strcpy(arg1,newName(VAR));
			fprintf(out,"\nSTORE %s",arg1);
			fprintf(out,"\nWRITE %s",arg1);
			break;
		case IF:
			//The second expression is evaluated then stored in a local variable 
			//the first expression is evaulted then based on the ralation operator given a branch is printed
			//Thne baesd on if there is an else various labels are printed
			traverseTree(p->children[2],out);
			strcpy(arg1,newName(VAR));
			fprintf(out,"\nSTORE %s",arg1);
			traverseTree(p->children[0],out);
			strcpy(label1,newName(LABEL));
			if(p->children[1]->tokens[0]->tokenID==GTTK)
			//Greater than Relational Operator
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZNEG %s",label1);
				traverseTree(p->children[3],out);	
			}
			else if(p->children[1]->tokens[0]->tokenID==LTTK)
			//Less than Relational Operator
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZPOS %s",label1);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==DBEQTK)
			//Double Equal Relational Operator
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRPOS %s",label1);
				fprintf(out,"\nBRNEG %s",label1);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==PCTTK)
			//Percent Relational Operator
			{
				fprintf(out,"\nMULT %s",arg1);
				fprintf(out,"\nBRNEG %s",label1);
				traverseTree(p->children[3],out);

			}
			else if(p->children[1]->tokens[0]->tokenID==LBCTK)
			{
			//Not Equals Relationsl Operator
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZERO %s",label1);
				traverseTree(p->children[3],out);
			}
			if(p->children[4]!=NULL)
			//If a else is present then it is eevaulated and another label is created
			{
				strcpy(label2,newName(LABEL));
				fprintf(out,"\nBR %s",label2);
				traverseTree(p->children[4],out);
				fprintf(out,"\n%s: NOOP",label2);
			}
			else
			//If there is no else then an ending label is printed
			{
				fprintf(out,"\n%s: NOOP",label1);
			}
			break;
		case Loop:
			//Loop Folows the same logic of the IF node but is creates a label at the beginnig and prints at the beginning
			//then creates another labelto brach to if the rational operators are false, Also there is a br command to the first label 
			//printed at the end to make the loop and the second label printed after that
			strcpy(label1,newName(LABEL));
			fprintf(out,"\n%s: NOOP",label1);
			traverseTree(p->children[2],out);
			strcpy(arg1,newName(VAR));
			fprintf(out,"\nSTORE %s",arg1);
			traverseTree(p->children[0],out);
			strcpy(label2,newName(LABEL));
			if(p->children[1]->tokens[0]->tokenID==GTTK)
			//Greather than
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZNEG %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==LTTK)
			//Less Than
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZPOS %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==DBEQTK)
			//Double Eqaul
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRPOS %s",label2);
				fprintf(out,"\nBRNEG %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==PCTTK)
			//Percent
			{
				fprintf(out,"\nMULT %s",arg1);
				fprintf(out,"\nBRNEG %s",label2);
				traverseTree(p->children[3],out);
			}
			else if(p->children[1]->tokens[0]->tokenID==LBCTK)
			//Not Eqaul
			{
				fprintf(out,"\nSUB %s",arg1);
				fprintf(out,"\nBRZERO %s",label2);
				traverseTree(p->children[3],out);
			}
			fprintf(out,"\nBR %s",label1);
			fprintf(out,"\n%s: NOOP",label2);
			break;
		case GoTo:
			//Creats a Branch to the given idenitfer 
			if(find(p->tokens[0]->tokenInstance)==-1)
			//makes sure the given idenifer has been declared
			{
				errorMsg(p->tokens[0],1);
			}
			fprintf(out,"\nBR %s",p->tokens[0]->tokenInstance);
			break;
		case Expr:
			//First traverse it's child then if it has tokens (the plus token) then stores the value in the ACC 
			//traverses its ofher child and adds them together
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
			//Traverses its first child and if it has tokens  if it does saves the value in the ACC
			//then check if it is the slash or asterik token then traverses 
			//its other child then multiplies it or then loades stores the new value and loads the old value and then divides then
			//to preseve the order
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
			//Mush like above but with the minus sign and subtraction
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
			//Traverse the first child then if it has tokens (the period) 
			//the current value in the ACC is multipled by -1 to negate it
			traverseTree(p->children[0],out);
			if(p->tokens[0]!=NULL)
			{
				fprintf(out,"\nMULT -1");	
			}
			break;
		case r:
			//Since it is a leaf there are only three options an ID token, a number tokern or having a child with another expression
			if(p->tokens[0]->tokenID==IDTK)
			//If it is an id token it can be local varibale an dneed to be read from the stack or be
			//a global varibale and just be loaded
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
			//A number can just be loaded into the ACC
			{
				fprintf(out,"\nLOAD %s",p->tokens[0]->tokenInstance);
			}
			else
			//If it is a child with another expression it will need to be traversed
			{
				traverseTree(p->children[0],out);
			}
			break;
		case Assign:
			//First he ID token is read from either the staock or loaded based on if it is local or global in scope then the expression
			//is evaluated the value fron the expression is writen to either the variable or to that spot on the stack
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
			//Creates a label with the ID token given and makes sure the ID tken given has already been declared
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
	struct stackNode* top=currentNode;
	int count=0;
	while(top!=NULL)
	{
		if(strcmp(top->IDtoken->tokenInstance,identifier)==0)
		{
			break;
		}
		count++;
		top=top->next;		
	}	
	return count;
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
