#include <stdio.h>
#include <stdlib.h>

typedef struct VertexNode VertexNode;
typedef struct Stack stack;
typedef struct Graph graph;
typedef struct ListNode ListNode;

struct ListNode {
    struct ListNode* next;
    int dest;
    int visited;
};

struct VertexNode{
    struct VertexNode* next;
    ListNode* edgeNode;
    int dest;
};

struct Graph{
    VertexNode* head;
    int size;
};

struct Stack{
    ListNode* node;
    struct Stack* next;
    int size;
};

stack* create_stack(){
        stack* S = (stack*) malloc(sizeof(stack));
        S->next = NULL;
        S->size = 0;
        return S;
}

void push_stack(stack* S, ListNode* p) {
        ListNode* temp = (ListNode*) malloc(sizeof(ListNode));
        temp->dest = p->dest;
        temp->visited = 0;
        temp->next = p->next;
       
        stack* selem = (stack*) malloc(sizeof(stack));
        selem->node = temp;
        selem->next = S->next;
        S->next = selem;
        S->size++;
}

ListNode* pop(stack* S) {
        ListNode* res = (ListNode*) malloc(sizeof(ListNode));
        
		stack* del = S->next;
        
		res->dest = del->node->dest;
        res->visited = del->node->visited;
        res->next = del->node->next;
        S->next = S->next->next;
        free(del);
        
		S->size--;
        
		return res;
}

ListNode* top(stack* S) {
        return S->next->node;
}

int isempty(stack* S) {
        if(!S->next)
            return 1;
        return 0;
}
 
graph* create_list() {
    graph* Graph = (graph*) malloc(sizeof(graph));
    
	Graph->head = (VertexNode*) malloc(sizeof(VertexNode));
	Graph->head->next = NULL;
    // head of ListNodes
    
	Graph->head->edgeNode = NULL;
    
	return Graph;
}
 
void addNodetoNode(ListNode* p, int y) {
    ListNode* temp = p;
    
	while(temp->next)
        temp = temp->next;
    
	ListNode* np = (ListNode*) malloc(sizeof(ListNode));
    
	np->dest = y;
    np->next = temp->next;
    temp->next = np;
}

void addNode(VertexNode* l, int y){
	ListNode* p = (ListNode*) malloc(sizeof(ListNode));
	p->dest = y;
    p->next = NULL; //creating dummy node
	
    l->edgeNode = (ListNode*) malloc(sizeof(ListNode));
    l->edgeNode->dest = -1;
    l->edgeNode->next = p;
}
 
void addList(graph* L, int x, int y) {
    VertexNode* temp = L->head;
       
    while(temp->next && temp->next->dest < x)
        temp = temp->next;
       
    VertexNode* l = (VertexNode*) malloc(sizeof(VertexNode));
    if(l == NULL)
        printf("Memory fail!");
    else {
        if(!temp->next) {
            l->dest = x;
            l->next = temp->next;
            temp->next = l;
                       
            // no ListNode or VertexNode index available, create new ListNode
            addNode(l, y);
        }
        else
            // VertexNode was available, add the ListNode to existing ListNode
            addNodetoNode(temp->next->edgeNode, y);
        }
}

void load_data_points(graph* L, FILE* f){
    while(!feof(f)){
        int x, y;
        int iseof = fscanf(f, "(%d %d) ", &x, &y); // returns number of successful input
        if(iseof == 2)
            addList(L, x, y);
        }
}

void print_list(graph* L) {
    VertexNode* temp = L->head;
	
	while(temp->next) {
        ListNode* ptemp = temp->next->edgeNode;
        
		while(ptemp->next) {
            printf("ListNode from %d to %d\n", temp->next->dest, ptemp->next->dest);
            ptemp = ptemp->next;
        }
        
        temp = temp->next;
    }
}
 
int get_start_point() {
    int res;
    printf("Enter starting point: ");
    scanf("%d", &res);
    return res;
}
 
int get_end_point() {
	int res;
	printf("Enter end point: ");
	scanf("%d", &res);
    return res;
}
 
ListNode* advance_next_node(VertexNode* L, int x) {
        VertexNode* temp = L;
        while(temp->next && temp->dest != x)
            temp = temp->next;

        if(!temp->next && temp->dest != x)
            return NULL;
        return temp->edgeNode->next;
}
 
void delete_ListNode(ListNode* p) {
        ListNode* temp = p;
        while(temp){
                ListNode* del = temp;
                temp = temp->next;
                free(del);
        }
}

void goal_seek(int begin, int end, graph* L) {
    VertexNode* start = L->head->next;
    //starting point may not be the first one, so a while loop needed
    while(start->next && start->dest != begin)
        start = start->next;
    stack* S = create_stack();

    //pushing the start node to the stack below.
	ListNode* fn = (ListNode*) malloc(sizeof(ListNode));
    fn->dest = start->dest;
    fn->next = NULL;
    push_stack(S, fn);
    
    int* resarr;
    int resarrsize;
    int offset = 0;
	
    while(!isempty(S)){
        ListNode* temp = top(S);
        temp->visited = 1;
        ListNode* ret = temp;
        /*chose first node in the stack as visited so that if a choice is made in 
		branches that will be visited and thus will be deleted and move on to next branch*/
		
        if(ret->dest == end){
        	/*if my dest equals to end this means i found what i'm looking for so
        	create a list to print them by popping the values from stack*/
            resarr = (int*) malloc(sizeof(int) * S->size);
            resarrsize = S->size;
            int pos = resarrsize - 1;
            while(!isempty(S)) {
                ListNode* pt = pop(S);
                if(pt->visited){
                    resarr[pos] = pt->dest;
                    pos--;
                }
                else
                    offset++;
                }
        }
        else{
            ListNode* add = advance_next_node(L->head, ret->dest);
            //if(!add) means I came to the leaf node and we can start backtracking.
			if(!add){
    			//pop the visited ones
                while(top(S)->visited){
                    pop(S);
                }
                continue;
            }
            //push_stack pushes the node that is not leaf node.
            push_stack(S, add);
            //this while continues adding if it has more in that path as branches
            while(add->next){
                push_stack(S, add->next);
                add = add->next;
            }
    	}
    }
	
    int i;
	//printf("%d",resarrsize);
	//printf("%d",offset);
	printf("\nPath is: ");
	for(i = offset; i < resarrsize; i++) {
    	printf("%d", resarr[i]);
    	if(i != resarrsize - 1)
        	printf(",");
    }
    
	printf("\n");
    free(resarr);
}

int main(){
    char fname[30];
    
	printf("Filename:");
    gets(fname);
    
	FILE* f = fopen(fname, "r");
    
    if(f == NULL)
        printf("No such file");
	else{
        graph* L = create_list();

        load_data_points(L, f);
		
		//print_list(L); /*to see if the list is created correctly.*/
		
		int start, end;
        start = get_start_point();
        end = get_end_point();         
        goal_seek(start, end, L);

	}
	
	system("pause");
	return 0;
}
