#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<pthread.h>
typedef struct Node
{
  int _data;
  struct Node *next;
}node,*pnode,**ppnode;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pnode phead=NULL;
pnode alloc(int data)
{
  pnode temp=(pnode)malloc(sizeof(node));
  if(temp==NULL)
  return NULL;
  temp->_data=data;
  temp->next=NULL;
  return temp;
}
void initList(ppnode phead)
{
   *phead=alloc(0);
}
void pushList(pnode phead,int data)
{
    pnode _p=alloc(data);
    _p->next=phead->next;
    phead->next=_p;
}
int  popList(pnode phead)
{

   if(phead->next==NULL)
   {
       return -1;
   }
   else
   {
   pnode temp=phead->next;
   int data=temp->_data;
   phead->next=temp->next;
   free(temp);
   return data;
   temp=NULL;
   }
   return 0;
}
void destroyList(pnode phead)
{
  while(phead->next)
  {
    pnode temp=phead->next;
    phead->next=temp->next;
    free(temp);
  }
}
void showList(pnode phead)
{
  pnode temp=phead->next;
  while(temp)
  {
    printf("%d",temp->_data);
    temp=temp->next;
  }
  printf("\n");
}
void* producter(void* arg)
{
  int data=0;
  while(1)
  {
   sleep(2);
   pthread_mutex_lock(&lock);
   data=rand()%100;
   pushList(phead,data);
   printf("producter:%d\n",data);
   pthread_cond_signal(&cond);
   printf("data has been producted!\n ");
   pthread_mutex_unlock(&lock);
  }
  return (void*)0;
}
void* consumer(void* arg)
{                                                                   
  int data=0;
  while(1)
  {
      sleep(1);
    pthread_mutex_lock(&lock);
    while(phead->next==NULL)
    {
      printf("No data is producted!\n");
      pthread_cond_wait(&cond,&lock);
    }
      data=popList(phead);  
      printf("consumer:%d\n",data);
      pthread_mutex_unlock(&lock);
  }
  return (void*)0;
}
int main()
{
    initList(&phead);
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1,NULL,producter,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    destroyList(phead);
    return 0;
}