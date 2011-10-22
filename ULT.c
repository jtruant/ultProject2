#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#include <ucontext.h>
#include "ULT.h"
struct ThrdCtlBlk *queueHead=NULL;
//number of existing threads
Tid universalTid=0;
//currently running thread
Tid runningThread=0;
struct ThrdCtlBlk *fromQueue(Tid searchTid,struct ThrdCtlBlk **queueHead);

Tid 
ULT_CreateThread(void (*fn)(void *), void *parg)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}


Tid ULT_Yield(Tid wantTid)
{
  
  if(wantTid>ULT_MAX_THREADS)
  {   
       /*The tid does not correspond to a valid thread*/ 
     return ULT_INVALID; 
  }
  else if((queueHead==NULL) && (wantTid==ULT_ANY))//if queue empty and ULT_any then no-op
  {
       return ULT_NONE;
  
  }
  else if(wantTid == ULT_SELF)
  {
	wantTid = runningThread;
  }
   
  /*assert(0);  TBD */
  /*return ULT_FAILED;*/
  ucontext_t currThread;

  /*build TCB*/
  struct ThrdCtlBlk *currBlock;
  /*allocate memory */
  currBlock=(struct ThrdCtlBlk*)malloc(sizeof(ThrdCtlBlk));
  currBlock->tid=universalTid;
  currBlock->tcbPointerTail=NULL;
  currBlock->tcbPointerHead=NULL;
  /*get context and set the context of the tcb to that context*/ 
  getcontext(&currThread);
  
  /*change instruction pointer JUMP*/
  currThread.uc_mcontext.gregs[REG_EIP]=currThread.uc_mcontext.gregs[REG_EIP]+162;
  currBlock->threadContext=currThread;

  /*stick thread(TCB) on the ready queue*/
  if(queueHead!=NULL)
  {
  currBlock->tcbPointerTail=queueHead;  
  }
  queueHead=currBlock;
  
  /*decide on new thread to run*/
  struct ThrdCtlBlk *setBlock;
  setBlock=(struct ThrdCtlBlk*)malloc(sizeof(ThrdCtlBlk));
  
  setBlock=fromQueue(wantTid,&queueHead); 
  currThread=setBlock->threadContext;
  setcontext(&currThread);
  printf("Jump barrier \n");
  runningThread=setBlock->tid;
  
  return runningThread;
  
  
 
}


Tid ULT_DestroyThread(Tid tid)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}

struct ThrdCtlBlk *fromQueue(Tid searchTid,struct ThrdCtlBlk **queueHead)
{
       // struct ThrdCtlBlk *tempBlock;
       struct ThrdCtlBlk *tempBlock;
       /*allocate memory */
       tempBlock=(struct ThrdCtlBlk*)malloc(sizeof(ThrdCtlBlk));
       tempBlock=*queueHead;

	  while(tempBlock!=NULL)
          {
        	if(tempBlock->tid==searchTid)
		{
			return tempBlock; 
		}
    		else
		{
			tempBlock=tempBlock->tcbPointerTail;
		}
          }
  //this will not be a correct tcb. needs fix		
  return tempBlock;
           	
}



