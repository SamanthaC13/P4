struct stackNode{
	struct tokenType* IDtoken;
	struct stackNode* next;
};
void push(struct tokenType*);
bool verify(char *);
void errorMsg(struct tokenType*,int); 
void traverseTree(struct node_t*,FILE*);
void pop();
int find(char*);
enum nodeIDs{Program,Vars,Block,Stats,Mstat,Stat,In,Out,Assign,rO,Label,IF,Loop,GoTo,Expr,n,a,m,r};
