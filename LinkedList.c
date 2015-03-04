#include "OS.h"

/*
Adds a tcb to a linked list
Could be called by OS_AddThread, OS_Signal, OS_bSignal
Inputs: first - pointer to a pointer to the first element in the linked list
        insert - pointer to linked list to be inserted
				last - pointer to a pointer to the last element in the linked list
*/
void LLAdd(tcbType** first, tcbType* insert, tcbType** last){
	if(*first==NULL){   //empty linked list
		*first=insert;
		*last=insert;
		insert->next=insert;
		insert->previous=insert;
	} else if((*first==*last)&&(first!=NULL)){		//one element in linked list, adding the second
		(*last)->next=insert;
		(*last)->previous=insert;
		(*first)->previous=insert;
		insert->next=*first;
		insert->previous=*last;
		*last=insert;
	} else{     //2 or greater elements in linked list
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
		}else if(*first==insert){
			*first=insert->next;
		}
		insert->previous->next=insert->next;				//remove from linked list
		insert->next->previous=insert->previous;
	}
	return 0;
}


// insert into the sema4 linked list, insert at back of list
// need to modify this for a priority sema4Add
void Sem4LLAdd(tcbType** ptFrontPt,tcbType* insert,tcbType** ptEndPt)
{
	if(*ptFrontPt == NULL) // when the sem4 LL is initially empty and you add the first element
	{
		(*ptFrontPt) = insert; // set frontpt to the first element
		(*ptEndPt) = insert; // set endpt to first element
		
		// might be necessary if this ever becomes a front or end of the list
		(*ptFrontPt)->next = NULL;
		
		(*ptEndPt)->next = NULL;
		(*ptEndPt)->previous = NULL;
		
	}
	else if (*ptFrontPt == *ptEndPt) // when there is one element in LL and you add the 2nd
	{
		(*ptFrontPt)->next = insert; // update the LL to point to the next element
		(*ptEndPt) = insert; // set the endpt to the element just inserted
		
		// added for the removing of the thread 
		(*ptEndPt)->previous = *(ptFrontPt);
		
	}
	else // general case when there is 2 or more elements and you add to a list, insert at the back for round robin
	{
		(*ptEndPt)->next = insert; // add to end of the LL
		insert->previous = (*ptEndPt); // point the inserted node (the new End) to the old end
		insert->next = NULL; // mght be necessary if this ever becomes the fron of the list & we are checking for null
		
		
		
		
		
		(*ptEndPt) = insert; // update EndPt to be the new back of the list
	}
}



// remove from the sema4 linked list, remove at front of list
// corner case when there are 2 elements, 1 element and no elements

tcbType* Sem4LLARemove(Sema4Type *semaPt)
{
	tcbType* wakeupThread;
	tcbType* temp;
	int32_t tempHighPri;
	
#ifdef PRIORITYSCHEDULER // need to search for the highest priority thread
	// we need to traverse the whole linked list
	
	// ** what if I removed a thread that was at the end
	// what if I removed a thread that was at the beginning
	// what if I keep removing from front s.t. frontpt == endpt
	// what if I keep removing from the end s.t. frontpt == endpt
	
	if(semaPt->Front == NULL)
	{	// LL is empty, return
		return NULL;
	}
	
	temp = semaPt->Front;	
	tempHighPri = temp->Priority;
	while(temp != NULL)
	{
		if(tempHighPri > temp->priority)
		{	// we found a higher priority thread,
			tempHighPri = temp->priority; // update tempHighPri 
			wakeupThread = temp; // wakeupThread(holds highest pri thread we found at current time)
		}
		temp = temp->next;
	}
	// we now have the highest priority thread, return thread & update LL
	// I need it to be doubly linked to get the previous
	
	if(wakeupThread == semaPt->First)
	{ // don't want to set the previous node to the next node since its not circular LL
		semaPt->First = wakeupThread->next; // update First to point to the new head of the LL
		
	}
	else if(
	else
	{
		wakeupThread->previous->next = wakeupThread->next; // prev point to next
		wakeupThread->next->previous = wakeupThread->previous; // next point back to previous
	}
	
#else	// round robin
	if(semaPt->FrontPt == NULL)
	{ // the list is empty, there is nothing to signal
		return NULL;
	}
	else if(semaPt->FrontPt == semaPt->EndPt)
	{ // the list had only one element in it, remove it then set the Front and End ptrs to NULL
		wakeupThread = semaPt->FrontPt;
		semaPt->FrontPt = NULL;
		semaPt->EndPt = NULL;
	}
	else// if(semaPt->FrontPt->next // there are atleast 2 elements in it
	{
		wakeupThread = semaPt->FrontPt;
		semaPt->FrontPt = semaPt->FrontPt->next; // update the new front of the LL after removing the thread we want
	}
#endif
	
	return wakeupThread;
}


