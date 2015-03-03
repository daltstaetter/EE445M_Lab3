#include "OS.h"

/*
Adds a tcb to a linked list
Could be called by OS_AddThread, OS_Signal, OS_bSignal
Inputs: first - pointer to a pointer to the first element in the linked list
        insert - pointer to linked list to be inserted
				last - pointer to a pointer to the last element in the linked list
*/
void LLAdd(tcbType** first, tcbType* insert, tcbType** last){
	if(first==NULL){
		*first=insert;
		*last=insert;
		insert->next=insert;
		insert->previous=NULL;
	} else if((*last)->previous==NULL){
		(*last)->next=insert;
		(*last)->previous=insert;
		(*first)->previous=insert;
		insert->next=*first;
		insert->previous=*last;
		*last=insert;
	} else{
		(*last)->next=insert;
		(*first)->previous=insert;
		insert->next=*first;
		insert->previous=*last;
		*last=insert;
	}
}

/*
Removes a tcb from a linked list
Could be called by OS_Kill, OS_Signal, OS_bSignal
Inputs: first - pointer to a pointer to the first element in the linked list
        insert - pointer to element to be inserted
				last - pointer to a pointer to the last element in the linked list
Outputs: 1 if the linked list is empty after removal
				 0 if the linked list is not empty after removal
*/
int LLRemove(tcbType** first, tcbType* insert, tcbType** last){
	if(insert->next==insert){  //If the element is the last element in the list, set all pointers to null
		*first=NULL;
		*last=NULL;
		insert->next=NULL;
		insert->previous=NULL;
		return 1;
	}else{
		if(*last==insert){					//If the tcb being removed is the last tcb added, change the last pointer to point to
			*last=insert->previous;		//the previous tcb
		}
		insert->previous->next=insert->next;				//remove from linked list
		insert->next->previous=insert->previous;
	}
	return 0;
}



