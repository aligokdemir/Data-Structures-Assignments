#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
	char comb[6];
	int mpn;
	int lower;
	int upper;
	struct Node *next;
};

struct MpnList{
	struct Node *head;
	struct Node *tail;
	int size;
};

typedef struct MpnList *list;

list Load_Mpn_Table(FILE *data){
	int mpnn,low,upp; //variables to store the data in the linked list.
	char com[6];  //same as above.
	
	list mylist = malloc(sizeof(struct MpnList)); //Creating list
	mylist->head = malloc(sizeof(struct Node));  

	if(mylist->head == NULL){
		printf("Out of Memory");
		return -1;
	}
	
	mylist->head->next = NULL;
	mylist->tail=mylist->head;
	mylist->size = 0;
	//lines 26 to 34 is to make an empty list
	data = fopen("data.txt","r"); //opening the file.
	//took the values into variables because I got memory error when directly getting them into linked list.
	while(fscanf(data,"%s %d %d %d",&com,&mpnn,&low,&upp) != EOF){
		struct Node *temp = malloc(sizeof(struct Node));
		temp->next = NULL;
		strcpy(temp->comb,com);
		temp->mpn = mpnn; temp->lower=low; temp->upper=upp;
		
		mylist->tail->next = temp;
		mylist->tail = temp;
		
		mylist->size++;		
	}
	
	fclose(data);
	return mylist;
}

void Display_Mpn_Table(list l){
	struct Node *p;
	p = l->head->next;
	if(l->head->next == NULL)
		printf("The list has not been loaded.\n");

	printf("Combination of Positives\tMPN Index/100 ml\t 95 Confidence Limits\n");
	printf("\t\t\t\t\t\t\tLower\t\tUpper\n");
	while(p != NULL){
		printf("%s\t\t\t        %d\t\t\t%d\t\t%d\n",p->comb,p->mpn,p->lower,p->upper);
		p = p->next;
	}
}

void Insert_Mpn_Table(list l){
	struct Node *element = malloc(sizeof(struct Node));
	struct Node *temp = malloc(sizeof(struct Node));
	temp = l->head;

	printf("Enter the combination of positives:");
	scanf("%s",&element->comb);
	printf("Enter the Mpn Index,Lower Value and Upper Value:");
	scanf("%d %d %d",&element->mpn,&element->lower,&element->upper);
	
	for(; temp->next && strcmp(temp->next->comb, element->comb) != 1;)
		temp = temp->next;
	if(!temp->next)
		l->tail = element;
	element->next = temp->next;
	temp->next = element;
	/*The for loop above compares two strings' ASCII codes so that new string 
	can be put in correct place in the linked list.*/
}

int Search_Mpn_Table(list l,char cop[]){
	struct Node *pp;
	int i;
	char cop2[6];
	pp = l->head->next;
	for(i=0;i<l->size;i++){
		if(strcmp(pp->comb,cop) == 0) return i;
		pp = pp->next;
	}
	
	if(i == l->size){
		return -1;
	}

}

int main(){
	char choice,cop[6];
	int exit = 0,posofnode;
	FILE *mpndata;
	list mpn;
	mpn = Load_Mpn_Table(mpndata);
	
	while(!exit){
		printf("Search Element(S)\nInsert Element(I)\nDisplay Table(D)\nExit(E)\nChoice:");
		fflush(stdin);
		scanf("%c",&choice);
		
		switch(choice){
			case 'S':
				printf("Enter the CoP of the element you want to search:");
				fflush(stdin);
				scanf("%s",&cop);
				posofnode = Search_Mpn_Table(mpn,cop);
				if(posofnode != -1){
					int i=0;
					struct Node* node;
					node = mpn->head->next;
					while(i != posofnode){
						node = node->next;
						i++;
					}
					printf("%s %d %d %d\n",node->comb, node->mpn,node->lower,node->upper);
				}
				else{
					printf("Data not found in the list.\n");
				}
				break;
			case 'I':
				Insert_Mpn_Table(mpn);
				break;
			case 'D':
				Display_Mpn_Table(mpn);
				break;
			case 'E':
				exit = 1;
				break;
			default:
				printf("Command not recognized.\n");
				break;
		}
	}

	system("pause");
	return(0);
}
