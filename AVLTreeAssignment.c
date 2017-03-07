#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct llnode{
    int data;
    struct occurrence *next;
};
typedef struct llnode lnode;

struct list{
	struct llnode *head;
	struct llnode *tail;
	int size;
};
typedef struct list linked;

struct AVLTreeNode{
    struct AVLTreeNode *left;
    struct AVLTreeNode *right;
    char *hash;
    int height;
    linked *occ;
};
typedef struct AVLTreeNode node;
 
linked *create_ll(){
	linked *mylist;
	mylist = (linked*)malloc(sizeof(linked));
	mylist->head = malloc(sizeof(lnode));
	mylist->head->next = NULL;
	mylist->tail = mylist->head;
	mylist->size = 0;
	
	return mylist;
}

void insert_pos(int data,linked *l){
	if(l->head->next == NULL){
		lnode *temp;
		temp = malloc(sizeof(lnode));
		temp->data = data;
		temp->next = NULL;
		l->head->next = temp;
		l->tail = temp;
		l->size++;
	}
	else{
		lnode *tempo;
		tempo = malloc(sizeof(lnode));
		tempo->data = data;
		tempo->next = NULL;
		l->tail->next = tempo;
		l->tail = tempo;
	}
}

node* create_node(){
   
    node* t = (node*)malloc(sizeof(node));
    t->height = 0;
    t->occ = create_ll();
    t->left = NULL;
    t->right = NULL;
   
    return t;
}
 
void destroy_tree(node *t){
    if(t!=NULL){
        destroy_tree(t->left);
        destroy_tree(t->right);
        free(t);
    }
}

int find_in_list(linked *l,int index){//this function checks whether a hashtag is used twice in
	if(l->head->next == NULL){	  // a tweet and if so, doesnt let the tweet number to be inserted more than once
		return 0;
	}
	lnode *search;
	search = l->head;
	while(search->next != NULL){
		if(search->data == index) return 1;
		search = search->next;
	}
	return 0;
}

int height(node *n){
    if(n == NULL){
        return -1;
    }
    else return n->height;
}
 
int max(int a, int b){
    return a > b ? a : b;
}
 
node* single_rotate_with_left(node *k2){
    node *k1 = NULL;
   
    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
   
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    return k1; /* new root */
}
 
node* single_rotate_with_right(node *k1){
    node *k2;
   
    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
   
    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->height = max( height( k2->right ), k1->height ) + 1;
 
    return k2;  /* New root */
}
 
node* double_rotate_with_left( node* k3 ){
    /* Rotate between k1 and k2 */
    k3->left = single_rotate_with_right( k3->left );
 
    /* Rotate between K3 and k2 */
    return single_rotate_with_left( k3 );
}
 
node* double_rotate_with_right( node* k1 ){
    /* rotate between K3 and k2 */
    k1->right = single_rotate_with_left( k1->right );
 
    /* rotate between k1 and k2 */
    return single_rotate_with_right( k1 );
}
 
node* insert(node **t,char label[], int index){

    if(*t == NULL){
        /* Create and return a one-node tree */
       
        *t = create_node();
       
        (*t)->hash = (char *)malloc(sizeof(char) * (strlen(label) + 1));
        strcpy((*t)->hash,label);
       
    }
    else if(strcmp(label,(*t)->hash) < 0){
        (*t)->left = insert(&(*t)->left,label,index);
        if( height( (*t)->left ) - height( (*t)->right ) >= 2 )
            if(strcmp(label,(*t)->left->hash) < 0)
                (*t) = single_rotate_with_left( (*t) );
            else
                (*t) = double_rotate_with_left( (*t) );
    }
    else if(strcmp(label,(*t)->hash) > 0){
        (*t)->right = insert(&(*t)->right,label,index);
        if( height( (*t)->left ) - height( (*t)->right ) >= 2 )
            if(strcmp(label,(*t)->left->hash) > 0)
                (*t) = single_rotate_with_right( (*t) );
            else
                (*t) = double_rotate_with_right( (*t) );
    }
    else{
    	if(find_in_list((*t)->occ,index) == 0);
			insert_pos(index,(*t)->occ); //inserts tweets' numbers into linked list.
	}
    
    (*t)->height = max( height( (*t)->left ), height( (*t)->right ) ) + 1;
	
    return (*t);
}
 
int is_valid(int c, int num_allowed)
{
    if ('A' <= c && c <= 'Z') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('0' <= c && c <= '9') return num_allowed;
    if (c == '_') return 1;
    return 0;
}
 
int hashtag(char **p, char buf[], size_t max)
{
    char *q;
 
    if (*p == NULL || **p == '\0') return 0;

    q = strchr(*p, '#');
    while (q && is_valid(q[1], 0) == 0){ /*this loop eliminates the ### case or !'^+%&/()=?\*/
        q = strchr(q + 1, '#');			  /* because such cases cannot be in the hashtag*/	
    }                                     /* the function above allows us to take a ordinary hashtag*/
 
    if (q) {
        size_t n = 0;
 
        q++;                    // skip hash sign
 
        while (n + 1 < max && is_valid(*q, 1)){
            buf[n++] = *q++;
        }
 
        if (max) buf[n] = '\0'; // terminate buffer
        *p = q;                 // remember position so that multiple hashtags can be found.
 
        return 1;               // hashtag found
    }

    return 0;              // nothing found
}
 
 
 
void read_tweet_data(node **t,char name[]){
    FILE *data;
    data = fopen(name,"r");
    if(data == NULL){
    	printf("file couldn't found");
	}
    char line[140]; // More if lines can be longer
 
    while(fgets(line, sizeof(line), data) != NULL)
    {
        int tweetn;
        fscanf(data,"%d",&tweetn);
        char *p = line;
        char tag[32];
        int n=0;
        while(hashtag(&p, tag, sizeof(tag))){
            //printf("[%d] %s\n", tweetn, tag);//insert functions come here...
            //strlwr(tag); //if you want to see the hashtags in lower case...but twitter treats them as they are different
			insert(t,tag,tweetn);
        }
    }
   
}
void DisplayTreeStructure(node *t, int depth)
{
   int i;
   
   if (t !=NULL)
   {
        DisplayTreeStructure(t->right, depth + 1);
         
        for (i = 0; i < depth; i++)
            printf("     ");
        printf("%s",t->hash);
         
        DisplayTreeStructure(t->left, depth + 1);
   }
}

void print_index(node *t){
	lnode *temp;
	if(t != NULL){
		temp = t->occ->head;
		printf("%s:\t",t->hash);
		printf("tweet:");
		while(temp->next != NULL){
			printf("%d ",temp->data);
			temp = temp->next;
		}
		printf("\n");
		print_index(t->left);
		//printf("\n");
		print_index(t->right);
		
		
	}
}


int main(){
    node *t = NULL;
    char name[100];
    
    printf("Enter the file name:");
    gets(name);
   
    read_tweet_data(&t,name);
   
    //DisplayTreeStructure(t,0);
	print_index(t); 

	
    return 0;
}
