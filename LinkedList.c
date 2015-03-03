#include "OS.h"

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

void LLRemove(tcbType** first, tcbType* insert, tcbType** last){
	
}

