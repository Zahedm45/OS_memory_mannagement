#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct MemoryList
{
  // doubly-linked list
  struct MemoryList *previous;
  struct MemoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};

strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct MemoryList *head;
static struct MemoryList *next;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

	/* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */

    if (head != NULL) {
        struct MemoryList *trav;
        for (trav = head; trav->next != NULL; trav = trav->next) {
            free(trav->previous);
            free(trav);
        }
    }


	myMemory = malloc(sz);
	
	/* TODO: Initialize memory management structure. */

    head = (struct MemoryList*) malloc(sizeof (struct MemoryList));
    head->previous = NULL;
    head->next = NULL;
    head->size = sz;
    head->alloc = 0;
    head->ptr = myMemory;

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested) {

	assert((int)myStrategy > 0);
	
	switch (myStrategy) {
	  case NotSet: 
	            return NULL;
	  case First:
	            return NULL;
	  case Best:
	            return NULL;
	  case Worst:
	            return NULL;
	  case Next:
	            return NULL;
	}



    if (head != NULL) {
        struct MemoryList *currentNode;


        for (currentNode = head; currentNode->next != NULL; currentNode = currentNode->next) {

            if ((currentNode->alloc != 1) && (currentNode->size > requested)) {
                struct MemoryList *restMem =(struct MemoryList*)malloc(sizeof(struct MemoryList));

                restMem->size = currentNode->size - requested;
                restMem->alloc = 0;
                restMem->ptr = currentNode->ptr + requested;

                if (currentNode->next == NULL) {
                    restMem->next = NULL;
                } else {
                    restMem->next = currentNode->next;
                    currentNode->next->previous = restMem;
                }

                currentNode->next = restMem;
                restMem->previous = currentNode;

                currentNode->alloc = 1;
                currentNode->size = requested;
                return currentNode->ptr;

//                restMem->previous = currentNode;                              // assigning the current node to previous node of the temporary node(restMem)
//                restMem->next = currentNode->next;

//                if (restMem->next != NULL) {
//                    currentNode->next = restMem;
//                    restMem->next->previous = restMem;
//                }
//                restMem->size = requested;
//                restMem->ptr = currentNode->ptr + requested;
//                restMem->alloc = 1;
            }
        }
    }
	return NULL;
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block) {

    struct MemoryList *trav = head;
    while (trav != NULL) {

        if (trav == block) {
            trav->alloc = 0;

            // if previous block is free
            if ( (trav->previous != NULL) && (trav->previous->alloc == 0)) {      // It is not the head and not allocated.
                trav->previous->size += trav->size;                                 // two empty memory blocks become one block

                struct MemoryList *temp = trav;
                if (temp->next != NULL) {                                       // not tail
                    temp->previous->next = temp->next;
                    temp->next->previous = temp->previous;
                    free(temp);
                }
            }

            // if next block is also free, become one block
            if ((trav->next != NULL) && (trav->next->alloc == 0)) {
                trav->previous->size += trav->size;

                struct MemoryList *temp = trav;
                if (temp->previous != NULL) {
                    temp->previous->next = temp;
                    temp->next->previous = temp->previous;
                    free(temp);
                }
            }
        }
    }

    return;
}




/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes() {
    if (head == NULL) {
        printf("Memory in not initialized")
        return;
    }
    struct MemoryList *curr = head;
    int counter = 0;
    while (curr != NULL) {
        if (curr->alloc == 0) {
            counter++;
        }

        curr = curr->next;
    }

	return counter;
}

/* Get the number of bytes allocated */
int mem_allocated() {

    if (head == NULL) {
        printf("Memory in not initialized")
        return;
    }
    struct MemoryList *curr = head;
    int counter = 0;
    while (curr != NULL) {
        if (curr->alloc == 1) {
            counter++;
        }

        curr = curr->next;
    }

    return counter;
}

/* Number of non-allocated bytes */
int mem_free() {

    if (head == NULL) {
        printf("Memory in not initialized")
        return;
    }
    struct MemoryList *curr = head;
    int size = 0;
    while (curr != NULL) {
        if (curr->alloc == 0) {
            size += curr->size;
        }
        curr = curr->next;
    }
    return size;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	return 0;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	return 0;
}       

char mem_is_alloc(void *ptr)
{
        return 0;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory() {


    struct MemoryList *temp = head;

    if (head == NULL){
        printf("Node is empty\n");
        return;
    }

    printf("Hello, World\n");
    while (temp != NULL) {
        printf("%d ", temp->size);
        temp = temp->next;
    }

	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;
	
	
	/* A simple example.  
	   Each algorithm should produce a different layout. */
	


	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);

	print_memory_status();
	
}

//int main () {
//    initmem(strat,500);
//    print_memory();
//
//}