struct charType{
	//struct to defines a character in the scanner that holds the character, the line the character is on, and the place the character is in the line
	char character;
	int lineNum;
	int charNum;
};
struct tokenType scanner(char*,int,int);
struct charType getNextChar(char*,int);
int convertToColumn(struct charType);
struct tokenType FADriver(struct charType,char*,int);
