struct tokenType{
	//structs for tokens that include a integeras an ID the line the token is on, the amount of characters in the token, and the actual instance of the token
	int tokenID;
	int lineCount;
	int charCount;
	char* tokenInstance;
	struct tokenType* nextToken;
};
// enum values that represent the different tokenIDs
enum tokenID{IDTK,NUMTK,WSTK,KEYWRDTK,EOLTK,GTTK,LTTK,EQTK,DBEQTK,CLNTK,CLNEQTK,PSTK,MINTK,ASKTK,SLTK,PCTTK,PERTK,LPTK,RPTK,CMATK,LBCTK,RBCTK,SMCLNTK,LBKTK,RBKTK,EOFTK};
