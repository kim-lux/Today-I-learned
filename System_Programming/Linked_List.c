/*
* 다음과 같이 구조체를 선언해서 프로그램 작성

   struct CalculatorNode {
         int a;
         int b;
         char name[20];
         int (*fptr) (int, int);
         struct CalculatorNode * next;
   };

* Linked List로 다음의 기능을 구현
  - 계산 노드 생성 (add/sub/multiply/divide) 
  - 현재까지 생성된 계산 노드 출력
  - 현재까지 생성된 계산 노드들의 계산 결과를 출력
  - 전체 삭제
  - 프로그램 종료
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add(int, int);
int multiply(int, int);
int sub(int, int);
int divide(int, int);


struct CalculatorNode {
    int a;
    int b;
    char name[20];
    int (*fptr) (int, int);
    struct CalculatorNode* next;
};

void Intro()
{
    printf("숫자를 입력하세요\n");
    printf("1. 계산 노드 생성 (add/sub/multiply/divide) \n");
    printf("2. 현재까지 생성된 계산 노드 출력\n");
    printf("3. 현재까지 생성된 계산 노드들의 계산 결과를 출력\n");
    printf("4. 전체 삭제\n");
    printf("5. 프로그램 종료\n");
}

struct CalculatorNode * createNode (int a, int b, char name[], int mathMode)
{
  struct CalculatorNode* ptr;
  int (*functionptr)(int,int);
  ptr = (struct CalculatorNode*)malloc(sizeof(struct CalculatorNode));

  if(mathMode == 1)
  {
    functionptr=add;
  }
  else if(mathMode == 2)
  {
    functionptr=sub;
  }
  else if(mathMode == 3)
  {
    functionptr=multiply;
  }
  else if(mathMode == 4)
  {
    functionptr=divide;
  }
  else
  {
    printf("유효하지 않은 값입니다.\n");
  }
  strcpy(ptr->name, name);
  ptr->a = a;
  ptr->b = b;
  ptr->fptr = functionptr;
  ptr->next = NULL;

  return ptr;
}

void insertData(struct CalculatorNode **first, int a, int b, char name[], int mathMode)
{
  struct CalculatorNode *ptr;
  ptr = createNode (a,b,name,mathMode);
  ptr->next = *first;
  *first = ptr;
}
void printData(struct CalculatorNode *first)
{
  printf ("\n");
  
  while (first != NULL) 
  {
    char mathMode[20];
    if(first-> fptr == add)
    {
      strcpy(mathMode, "Add");
    }
    else if(first -> fptr == sub)
    {
      strcpy(mathMode, "Sub");
    }
    else if(first -> fptr == multiply)
    {
      strcpy(mathMode, "Multiply");
    }
    else if(first -> fptr == divide)
    {
      strcpy(mathMode, "Divide");
    }
    else
    {
      printf("끝\n\n");
      return;
    }
    printf("%d %d %s %s \n", first->a, first->b, first->name, mathMode);
    first = first->next;
  }
}
void printResultData(struct CalculatorNode *first)
{
  printf ("\n");
  int result;
  while (first != NULL) 
  {
    result = first -> fptr(first ->a,first ->b);
    printf("%d\n",result);
    first = first->next;
    if(first != NULL)break;
  }
}

void deleteData(struct CalculatorNode **first)
{
  *first = NULL;
}

int main()
{
  struct CalculatorNode *top;
  while(1)
  {
    int choiceMode = 0;
    int mathMode = 0;
    int a = 0;
    int b = 0;
    char name[20];
    Intro();
    scanf("%d",&choiceMode);
    switch (choiceMode)
    {
      case 1:
        printf("숫자를 눌러 계산 방식을 선택하시오.\n");
        printf("1.Add 2.Sub 3.Multiply 4.Divide\n");
        scanf("%d",&mathMode);
        printf("숫자 A를 입력하시오.\n");
        scanf("%d",&a);
        printf("숫자 B를 입력하시오.\n");
        scanf("%d",&b);
        printf("문자열 Name을 입력하시오.\n");
        scanf("%s",name);
        insertData (&top, a, b, name, mathMode);
        continue;
      case 2:
        printData(top);
        continue;
      case 3:
        printResultData(top);
        continue;
      case 4:
        deleteData(&top);
        continue;
      case 5:
        break;    
    }
    break;
  }
  printf("프로그램이 종료되었습니다.\n");

    
}

int add(int a, int b)
{
    int result;
    //printf("add 함수 호출\n");
    result = a + b;
    return result;
}

int multiply(int a, int b)
{
    int result;
    //printf("multiply 함수 호출\n");
    result = a * b;
    return result;
}

int sub(int a, int b)
{
    int result;
    //printf("sub 함수 호출\n");
    result = a - b;
    return result;
}

int divide(int a, int b)
{
    int result;
    //printf("divide 함수 호출\n");
    result = a / b;
    return result;
}
