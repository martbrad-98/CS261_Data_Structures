/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Bradley Martin
 * Email: martbrad@oregonstate.edu
 */

#include <stdlib.h>
#include "dynarray.h"

#include "pq.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
  struct dynarray* arr;
};

struct pq_node{
  void* val;
  int priority;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq = malloc(sizeof(pq));
  pq->arr = dynarray_create();
  return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  dynarray_free(pq->arr);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  if(dynarray_size(pq->arr)==0){
    return 1;
  }
  else{
    return 0;
  }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
int left_leaf(int index){
index = ((2*index)+1);
return index;
}

int right_leaf(int index){
  index = ((2*index)+2);
  return index;
}

int parent_index(int index){
  if(index%2 == 1){
    index = ((index-1)/2);
  }
  else{
    index = ((index-2)/2);
  }
  return index;
}

void swap(struct dynarray* arr, int index, int pindex){
  struct pq_node* temp1 = dynarray_get(arr, index);
  struct pq_node* temp2 = dynarray_get(arr, pindex);

  dynarray_set(arr, index, temp2);
  dynarray_set(arr, pindex, temp1);
}

void shift_array(struct dynarray* arr, int index){
  struct pq_node* curr = dynarray_get(arr, index);
  struct pq_node* parent = dynarray_get(arr, parent_index(index));
  
    while((curr->priority) < (parent->priority)){
        swap(arr, index, parent_index(index));
        index = parent_index(index);
        curr = dynarray_get(arr,index);
        parent = dynarray_get(arr, parent_index(index));
    }
}

struct pq_node* create_node(void* val, int priority){
  struct pq_node* n = malloc(sizeof(struct pq_node*));
  n->val = val;
  n->priority = priority;
  return n;
}

void pq_insert(struct pq* pq, void* value, int priority) {
  struct pq_node* node = create_node(value, priority);
  dynarray_insert(pq->arr, -1, node);
  shift_array(pq->arr, dynarray_size(pq->arr)-1);

}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct pq_node* n = NULL;
  if(pq_isempty(pq)){
    return NULL;
  }
  else{
    n = dynarray_get(pq->arr, 0);
    return n->val;
  }
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  struct pq_node* n = NULL;
  if(pq_isempty(pq)){
    return 0;
  }
  else{
    n = dynarray_get(pq->arr, 0);
    return n->priority;
  }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void remove_helper_swap(struct dynarray* arr, int index, int nindex){
  struct pq_node* curr = dynarray_get(arr, index);
  struct pq_node* child = dynarray_get(arr, nindex);
  if(curr->priority > child->priority){
    swap(arr, index, nindex);
  }
  else{
    return;
  }
}
void remove_helper(struct pq* pq){
  int index, nindex;
  struct pq_node* temp = dynarray_get(pq->arr, 0);
  
    while(pq_isempty(pq) == 0){
      if(right_leaf(index) >= dynarray_size(pq->arr)){
        if(left_leaf(index)>= dynarray_size(pq->arr)){
          return;
        }
        else{
          nindex = left_leaf(index);
        }
      }
      else{
        if((dynarray_get(pq->arr, left_leaf(index))) <= (dynarray_get(pq->arr, right_leaf(index)))){
          nindex = left_leaf(index);
        }
        else{
          nindex = right_leaf(index);
        }
      }
      struct pq_node* curr = dynarray_get(pq->arr, index);
      struct pq_node* child = dynarray_get(pq->arr, nindex);
      if(curr->priority > child->priority){
        swap(pq->arr, index, nindex);
      }
      else{
        return;
      }
      index = nindex;
    }
}
void* pq_remove_first(struct pq* pq) {
  int* val = pq_first(pq);
  swap(pq->arr,0,-1);
  dynarray_remove(pq->arr,-1);
  remove_helper(pq);
  // free(dynanarry_get(pq->arr, 0));
  return val;
}
