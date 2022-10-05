#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*Linked List form declare*/
struct listNode
{ 
  char data;
  struct listNode *nextPtr;
};

typedef struct listNode ListNode;
typedef ListNode *ListNodePtr;

/*function declare*/
void insert (ListNodePtr *, char);
char deleteData (ListNodePtr *, char);
int isEmpty (ListNodePtr);
void printList (ListNodePtr);
void instructions(void);

int main()
{  
  ListNodePtr startPtr = NULL;
  int choice;
  char item;
  
  instructions();
  
  printf("Let's Fishing A DUCK\n");
  
  /*Select Insert, Delete or end*/
  scanf("%d", &choice);
  while (choice != 3){
    switch(choice){
  //Insert
      case 1:
        printf("Enter a Duck Name: ");
        scanf("\n%c", &item);
        insert(&startPtr, item);
        printList(startPtr);
        break;
  //Delete      
      case 2:
        if(!isEmpty(startPtr)) //list is empty?
        {
          printf("Enter Duck Name to be deleted: ");
          scanf("\n%c", &item);
          if (deleteData(&startPtr, item))
          {
            printf("%c deleted. \n", item);
            printList(startPtr);
          }
          else
            printf("%c not found.\n\n", item);
        }      
  //If the list is empty        
        else
           printf("List is empty.\n\n");
        break;
      default:
        printf("Please Collect Command\n\n");
        break; 
      	        
    }
  //Next Command
    /*Initialization choice*/
    choice = 0;	    
    printf("Next Command Please\n:");
    scanf("%d", &choice);
  }
  
  printf("End of run. \n");
  return 0;
}

void instructions(void)
{  
    printf("Enter your choice:\n 1 : Insert Duck Name\n 2 : Delete Duck Name \n 3 : end\n");
}

void insert(ListNodePtr *sptr, char value)
{  
    ListNodePtr newPtr, previousPtr, currentPtr;
    newPtr = (ListNode*) malloc(sizeof(ListNode));
    if(newPtr != NULL) //Memory is available
    {
        newPtr->data = value;
        newPtr->nextPtr = NULL;
        previousPtr = NULL;
        currentPtr = *sptr;
    
        while(currentPtr != NULL && value > currentPtr->data)
        {
            previousPtr =currentPtr;
            currentPtr = currentPtr->nextPtr;
        }
        if (previousPtr == NULL)
        {
            newPtr->nextPtr = *sptr;
            *sptr = newPtr;
        }
        else
        {
            previousPtr->nextPtr = newPtr;
            newPtr->nextPtr = currentPtr;
        }
    }
    else
        printf("%c not inserted. No memory available. \n", value);

}


char deleteData(ListNodePtr *sPtr, char value){
  ListNodePtr previousPtr, currentPtr, tempPtr;
  if(value == (*sPtr)->data)
  {
    tempPtr = *sPtr;
    *sPtr = (*sPtr) -> nextPtr;
    free(tempPtr);
    return value;
  }
  else
  {
    previousPtr = *sPtr;
    currentPtr = (*sPtr)->nextPtr;
    while(currentPtr  != NULL && currentPtr->data != value)
      {
        previousPtr = currentPtr;
          currentPtr = currentPtr->nextPtr;
      }
    if (currentPtr != NULL)
    {
      tempPtr = currentPtr;
      previousPtr->nextPtr = currentPtr->nextPtr;
      free(tempPtr);
      return value;
    }
  }
  return '\0';
}

int isEmpty(ListNodePtr sPtr)
{  return sPtr == NULL;}

void printList(ListNodePtr currentPtr)
{  if(currentPtr == NULL)
  {
    printf( "List is empty.\n\n" );
  }
  else
  {
    printf("The list is: \n");
    while(currentPtr != NULL)
      {
        printf("%c -->", currentPtr->data);
        currentPtr = currentPtr->nextPtr;
      }
    printf("Sausage\n\n");
  }
}
