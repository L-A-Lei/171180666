/*linklist.h*/
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define BUFFER_SIZE 1024

typedef char ElementType;       
typedef struct ListNode {
    ElementType  Element[BUFFER_SIZE];       
    struct ListNode* Next;        
}Node, *PNode;

PNode CreateList(void);    
//void TraverseList(PNode List);    
void InsertList(PNode List, char newname[BUFFER_SIZE]);        
//void DeleteTheList(PNode List);    
void DeleteList(PNode List, char pos[BUFFER_SIZE]);    
PNode FindList(PNode List, char tar[BUFFER_SIZE]);    
int ListEmpty(PNode List);

PNode CreateList(void) {
    PNode PHead = (PNode)malloc(sizeof(Node));    
    if (PHead == NULL)    
    {
        printf("Failed to create a node\n");
        exit(-1);
    }
    PHead->Next = NULL;
    return PHead;    
}

/*void TraverseList(PNode List) {
    PNode P = List->Next;    
    if (P == NULL)
        printf("The List is empty\n");
    while (P != NULL)       
    {
        P = P->Next;
    }
    printf("\n");
}*/

PNode FindList(PNode List, char tar[BUFFER_SIZE]) {
    PNode P = List->Next;    
    while ((P != NULL)&&(strcmp(P->Element, tar) != 0)) {
        P = P->Next;
    }
    return P;
}

void InsertList(PNode List, char newname[BUFFER_SIZE]) {
    int position = 0;
    int i;
    PNode P = List;   
                    
    while (P->Next != NULL)
    {
        P = P->Next;
    }
    PNode Tmp = (PNode)malloc(sizeof(Node));    
    if (Tmp == NULL)
    {
        printf("Failed to create a new node\n");
        exit(-1);
    }
    for(i = 0; i < BUFFER_SIZE; i++)	
    	Tmp->Element[i] = newname[i];
    Tmp->Next = NULL;
    P->Next = Tmp;
}


/*void DeleteTheList(PNode List) {
    PNode P, Tmp;
    P = List->Next;   
    List->Next = NULL;
    while (P != NULL) {
        Tmp = P->Next;       
        free(P);    
        P = Tmp;    
    }
}*/

void DeleteList(PNode List, char tar[BUFFER_SIZE]) {
    PNode P = List->Next;
    PNode Q = List;    
    while ((P != NULL)&&(strcmp(P->Element, tar) != 0)) {
        Q = Q->Next;
	P = Q->Next;
    }

    if(P != NULL){
    	PNode Tmp = P;    
    	Q->Next = Tmp->Next;    
    	free(Tmp);    
    	Tmp = NULL;    
    }	
}

int ListEmpty(PNode List) {
	if(List->Next == NULL)
		return 1;
	else
		return 0;
}	
