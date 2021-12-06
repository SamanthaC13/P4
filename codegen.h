struct stackNode{
	struct tokenType* IDtoken;
	struct stackNode* next;
};
void push(struct tokenType*);
bool verify(char *);
void errorMsg(struct tokenType*,int); 
void traverseTree(struct node_t*);
void pop();
int find(char*);
