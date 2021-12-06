/*Name:Samantha Craig
 * Class:4280
 * Scanner from P1 
 * */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"token.h"
#include"scanner.h"
//the numebr that tells what character we are at in the given line
int charNum;
//list of errors
enum errors {ERROR=-2};
//list of differnt states
enum states {START,LOWERLETTER_STATE,UPPERLETTER_STATE,NUM_STATE,GREATER_STATE,LESSER_STATE,EQUAL_STATE,DOUBLEEQ_STATE,COLON_STATE,COLONEQ_STATE,PLUS_STATE,MINUS_STATE,ASTERISK_STATE,SLASH_STATE,PERCENT_STATE,PERIOD_STATE,LEFTPAREN_STATE,RIGHTPAREN_STATE,COMMA_STATE,LEFTBRACE_STATE,RIGHTBRACE_STATE,SEMICOLON_STATE,LEFTBRACKET_STATE,RIGHTBRACKET_STATE};
//list of different final values starting at 101
enum finals {ID_FINAL=101,NUM_FINAL,WS_FINAL,EOL_FINAL,GREATER_FINAL,LESSER_FINAL,EQ_FINAL,DOUBLEEQ_FINAL,COLON_FINAL,COLONEQ_FINAL,PLUS_FINAL,MINUS_FINAL,ASTERISK_FINAL,SLASH_FINAL,PERCENT_FINAL,PERIOD_FINAL,LEFTPAREN_FINAL,RIGHTPAREN_FINAL,COMMA_FINAL,LEFTBRACE_FINAL,RIGHTBRACE_FINAL,SEMICOLON_FINAL,LEFTBRACKET_FINAL,RIGHTBRACKET_FINAL,EOF_FINAL};
//list of column values 
enum columns {lowerLetter,upperLetter,number,ws,dollarSign,eol,greater,lesser,equal,colon,plus,minus,asterisk,slash,percent,period,leftParen,rightParen,comma,leftBrace,rightBrace,semiColon,leftBracket,rightBracket,eof};
//initializing the FSA table where each column correspounds to columns and every row correspounds to states
int table[24][25]={
		{LOWERLETTER_STATE,ERROR,NUM_STATE,WS_FINAL,LOWERLETTER_STATE,EOL_FINAL,GREATER_STATE,LESSER_STATE,EQUAL_STATE,COLON_STATE,PLUS_STATE,MINUS_STATE,ASTERISK_STATE,SLASH_STATE,PERCENT_STATE,PERIOD_STATE,LEFTPAREN_STATE,RIGHTPAREN_STATE,COMMA_STATE,LEFTBRACE_STATE,RIGHTBRACE_STATE,SEMICOLON_STATE,LEFTBRACKET_STATE,RIGHTBRACKET_STATE,EOF_FINAL},
		{LOWERLETTER_STATE,UPPERLETTER_STATE,LOWERLETTER_STATE,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL},
 		{LOWERLETTER_STATE,UPPERLETTER_STATE,LOWERLETTER_STATE,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL,ID_FINAL},
		{NUM_FINAL,NUM_FINAL,NUM_STATE,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL,NUM_FINAL},
		{GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL,GREATER_FINAL},
		{LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL,LESSER_FINAL},
		{EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,DOUBLEEQ_STATE,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL,EQ_FINAL},
		{DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL,DOUBLEEQ_FINAL},
		{COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLONEQ_STATE,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL,COLON_FINAL},
		{COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL,COLONEQ_FINAL},
		{PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL,PLUS_FINAL},
		{MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL,MINUS_FINAL},
		{ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL,ASTERISK_FINAL},	
		{SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL,SLASH_FINAL},
		{PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL,PERCENT_FINAL},
		{PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL,PERIOD_FINAL},
		{LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL,LEFTPAREN_FINAL},
		{RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL,RIGHTPAREN_FINAL},
		{COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL,COMMA_FINAL},
		{LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL,LEFTBRACE_FINAL},
		{RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL,RIGHTBRACE_FINAL},
		{SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL,SEMICOLON_FINAL},
		{LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL,LEFTBRACKET_FINAL},
		{RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL,RIGHTBRACKET_FINAL}
		};
//list of keywords in the language
char* keywords[16]={"start","stop","loop","while","for","label","exit","listen","talk",
			"program","if","then","assign","declare","jump","else"};
struct tokenType scanner(char* line,int lineNum, int startChar)
//the starting function for the scanner that takes in the line, what character we are starting at in the line, and the line number 
{
	charNum=startChar;
	struct charType currChar;
	struct tokenType token;
	//getting and setting the character at the index sent as startChar
	currChar=getNextChar(line,lineNum);
	//send the chracter to the FSA driver with the line number and line
	token=FADriver(currChar,line,lineNum);
	//get the token from the FSA Driver and return the token
	return token;
}
struct charType getNextChar(char* line,int lineNum)
//when given the line and the line number get the character at the global varaible index charNum and return  the struct charType that includes what line the character is on, the chracter, and the charNum
{
	struct charType c;
	c.character=line[charNum];
	c.charNum=charNum;
	c.lineNum=lineNum;
	charNum++;
	return c;
}
int convertToColumnNum(struct charType c)
//converts all the approved character inputs into their specific column number for FSA Table
{
	if(isalpha(c.character)!=0)
	{
		if(isupper(c.character))
		{
			return upperLetter;
		}
		else
		{
			return lowerLetter;
		}
	}
	else if(c.character==' ')
	{
		return ws;
	}
	else if(c.character=='$')
	{
		return dollarSign;
	}
	else if(isdigit(c.character)!=0)
	{
		return number;
	}
	else if(c.character=='\n')
	{
		return eol;
	}
	else if(c.character=='>')
	{
		return greater;
	}
	else if(c.character=='<')
	{
		return lesser;
	}
	else if(c.character=='=')
	{
		return equal;
	}
	else if(c.character==':')
	{
		return colon;	
	}
	else if(c.character=='+')
	{
		return plus;
	}
	else if(c.character=='-')
	{
		return minus;
	}
	else if(c.character=='*')
	{
		return asterisk;
	}
	else if(c.character=='/')
	{
		return slash;
	}
	else if(c.character=='%')
	{
		return percent;
	}
	else if(c.character=='.')
	{
		return period;
	}
	else if(c.character=='(')
	{
		return leftParen;
	}
	else if(c.character==')')
	{
		return rightParen;
	}
	else if(c.character==',')
	{
		return comma;
	}
	else if(c.character=='{')
	{
		return leftBrace;
	}
	else if(c.character=='}')
	{
		return rightBrace;
	}
	else if(c.character==';')
	{
		return semiColon;
	}
	else if(c.character=='[')
	{
		return leftBracket;
	}
	else if(c.character==']')
	{	
		return rightBracket;
	}
	else if(c.character=='\0')
	{
		return eof;
	}
	else
	//if none of the approved inputs are entered then the column number is set to a negative 1
 	{
		return -1;
	}
}
int isKeyword(char* string)
//this function checks is the string given is a keyword and return 1 if it is and 0 if it not
{
	int i=0;
	for(i=0;i<16;i++)
	{
		if(strcmp(string,keywords[i])==0)
		{
			return 1;
		}
	}
	return 0;
} 
struct tokenType FADriver(struct charType c,char* line,int lineNum)
//this function traverese the table and takes in the charType c and the lineNum and line and returns a token
{
	//All start at the START state
	int currentState=START;
	int nextState=0;
	//get the column index of the first character in the token
	int columnIndex=convertToColumnNum(c);
	struct tokenType token;
	//make a empty string to fill in with the token instance
	char* string=(char*)malloc(sizeof(char));
	int stringCount=0;
	while(currentState<100)
	//while the current state is not at a final state
	{
		if(columnIndex<0)
		//if the chracter recieved was not in an accepted column
		{
			token.tokenID=-1;
			//return token in an error state
			return token;
		}
		//set look ahead to the table position it will occupy
		nextState=table[currentState][columnIndex];
		if(nextState>100)
		//if lookahead is a final state then check which final state it is and set specific tokenID 
		{
			if(nextState==ID_FINAL)
			//if lookahead is an ID token then you need to check if it is a keyword 
			{
				token.tokenID=IDTK;
				if(isKeyword(string)==1)
				//if it is a keyword then change the tokenID
				{
					token.tokenID=KEYWRDTK;
				}
			}
			if(nextState==NUM_FINAL)
			{
				token.tokenID=NUMTK;
			}
			if(nextState==EOF_FINAL)
			{
				stringCount=1;
				token.tokenID=EOFTK;
			}
			if(nextState==EOL_FINAL)
			//this reaches the final state while in START so the string count needs to incremented so the charCount is 1 and not 0
			{
				stringCount=1;
				token.tokenID=EOLTK;
			}
			if(nextState==WS_FINAL)
			//this reaches the final state while in START so the string count needs to incremented so the charCount is 1 and not 0
			{
				stringCount=1;
				token.tokenID=WSTK;
			}
			if(nextState==GREATER_FINAL)
			{
				token.tokenID=GTTK;
			}
			if(nextState==LESSER_FINAL)
			{
				token.tokenID=LTTK;
			}
			if(nextState==EQ_FINAL)
			{
				token.tokenID=EQTK;
			}
			if(nextState==DOUBLEEQ_FINAL)
			{
				token.tokenID=DBEQTK;
			}
			if(nextState==COLON_FINAL)
			{
				token.tokenID=CLNTK;
			}
			if(nextState==COLONEQ_FINAL)
			{
				token.tokenID=CLNEQTK;
			}
			if(nextState==PLUS_FINAL)
			{
				token.tokenID=PSTK;
			}
			if(nextState==MINUS_FINAL)
			{
				token.tokenID=MINTK;
			}
			if(nextState==ASTERISK_FINAL)
			{
				token.tokenID=ASKTK;
			}
			if(nextState==SLASH_FINAL)
			{
				token.tokenID=SLTK;
			}
			if(nextState==PERCENT_FINAL)
			{
				token.tokenID=PCTTK;
			}
			if(nextState==PERIOD_FINAL)
			{
				token.tokenID=PERTK;
			}
			if(nextState==LEFTPAREN_FINAL)
			{
				token.tokenID=LPTK;
			}
			if(nextState==RIGHTPAREN_FINAL)
			{
				token.tokenID=RPTK;
			}
			if(nextState==COMMA_FINAL)
			{
				token.tokenID=CMATK;
			}
			if(nextState==LEFTBRACE_FINAL)
			{
				token.tokenID=LBCTK;
			}
			if(nextState==RIGHTBRACE_FINAL)
			{
				token.tokenID=RBCTK;
			}
			if(nextState==SEMICOLON_FINAL)
			{
				token.tokenID=SMCLNTK;
			}
			if(nextState==LEFTBRACKET_FINAL)
			{
				token.tokenID=LBKTK;
			}
			if(nextState==RIGHTBRACKET_FINAL)
			{
				token.tokenID=RBKTK;
			}
			//the tokenInstance is set to the string and the charCount and line Count in the token are set and the token is returned
			string[stringCount]='\0';
			token.tokenInstance=string;
			token.charCount=stringCount;
			token.lineCount=c.lineNum;
			return token;	
		}
		else if(nextState<0)
		//if the lookahead character returns an error state then the tokenID is set to that error state and the token is returned
		{
			token.tokenID=nextState;
			return token;
		}		
		else
		//if the lookahead is not a final state then the current state is set to the next state, the current character is added to the string, the string count is incremented, the string is reallocated,
		//and a new character is gotton at the next charNum then changed to its correponding column number  
		{
			currentState=nextState;
			string[stringCount]=c.character;
			stringCount++;
			string=realloc(string,stringCount*sizeof(char));
			c=getNextChar(line,lineNum);
			columnIndex=convertToColumnNum(c);	
		}
	}
}
