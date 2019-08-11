/*!Copyright(c) 2019 Shenzhen TP-LINK Technologies Co.Ltd.
*
*\file linkedlist.c
*\brief A general linked list C source file.
*
*\author Zhao Haodong
*\version 1.0
*\date 2019 Aug 07
*
*\history
*
*
*/

/***************************************************************************/
/*INCLUDE_FILES                                                                     */
/***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linkedlist.h"

/***************************************************************************/
/*TYPES                                                                             */
/***************************************************************************/
typedef struct node{
    Node *prior;
    Node *next;
    void *data;
}Node;

struct link{
    Node *head;
    int length;
    pthread_rwlock_t lock;
};

/*********************************************************************/
/*                           LOCAL PROTOTYPES                        */
/*********************************************************************/
static void list_init(Link *list);
static Node * node_create(void* data);
static Node* list_get(Link *list, int index);
static int partition(Link *list, int low, int high, int (*compare)(void *, void *));
static void quick_sort(Link *list, int low, int high, int (*compare)(void *, void *));
static void initLock(pthread_rwlock_t *rwlock);
static void rdLock(pthread_rwlock_t *rwlock);
static void wrLock(pthread_rwlock_t *rwlock);
static void unLock(pthread_rwlock_t *rwlock);
static void delLock(pthread_rwlock_t *rwlock);

/***************************************************************************/
/*LOCAL_FUNCTIONS                                                                   */
/***************************************************************************/

/*!
*\fn           static void list_init(Link *list)
*
*\brief        Initialize a list.
*
*\param[out]    list    The linked list.
*
*\return       void
*
*/
static void list_init(Link *list){
    list->head = NULL;
    list->length=0;
    initLock(&(list->lock));
};

static Node * node_create(void* data){
    Node *node = malloc(sizeof(Node));
    if(NULL != node){
        node->prior = NULL;
        node->next = NULL;
        node->data = data;
    }
    return node;
}
/*!
*\fn           static Node* list_get(Link *list, int index)
*
*\brief        Get a Node from a linked list by a index.
*
*\param[out]    list    The linked list.
*\param[in]    index   The index of the node.
*
*\return       Node*   The Node of the index.
*
*/
static Node* list_get(Link *list, int index){
    if(list == NULL) return NULL;
    int  i;
    Node *tmp = list->head;
    for(i=1; i<index && tmp != NULL; i++){
        tmp=tmp->next;
    }
    return tmp;
}

/*!
*\fn           static int partition(Link *list, int low, int high,
*                                   int (*compare)(void *, void *))
*
*\brief        Partition of the quick sort.
*
*\param[out]   list     The linked list.
*\param[in]    low      The low position of partition.
*\param[in]    high     The high position of partition.
*\param[in]    compare  The compare function pointer.
*
*\return       the index of pivot
*
*/
static int partition(Link *list, int low, int high, int (*compare)(void *, void *)){
    void* pivot = list_get(list, low)->data;
    while(low < high){
        while(low < high && compare(pivot, list_get(list, high)->data)) high--;
        list_get(list, low)->data = list_get(list, high)->data;
        while(low < high && compare(list_get(list, low)->data, pivot)) low++;
        list_get(list, high)->data = list_get(list, low)->data;
    }
    list_get(list, low)->data = pivot;
    return low;
}
static void quick_sort(Link *list, int low, int high, int (*compare)(void *, void *)){
    if(low<high){
        int pivotkey = partition(list, low, high, compare);
        quick_sort(list, low, pivotkey-1, compare);
        quick_sort(list, pivotkey+1, high,compare);
    }
}

static void initLock(pthread_rwlock_t *rwlock)
{
	int res = 0;
	res = pthread_rwlock_init(rwlock, NULL);
	if(0 != res)
	{
		printf("INIT LOCK FAILURE\n");
        exit(0);
	}
}

static void rdLock(pthread_rwlock_t *rwlock)
{
	
	pthread_rwlock_rdlock(rwlock);
	
}

static void wrLock(pthread_rwlock_t *rwlock)
{
	pthread_rwlock_wrlock(rwlock);
}

static void unLock(pthread_rwlock_t *rwlock)
{
	pthread_rwlock_unlock(rwlock);
}

static void delLock(pthread_rwlock_t *rwlock)
{
	int res = 0;
	res = pthread_rwlock_destroy(rwlock);
	if(0 != res)
	{
		printf("%d ",res);
		printf("DESTORY LOCK FAILURE\n");
        exit(0);
	}
}
/***************************************************************************/
/*PUBLIC_FUNCTIONS                                                                  */
/***************************************************************************/

/*!
*\fn           Link* list_create(void)
*
*\brief        Create a linked list.
*
*
*\return       the linked list
*
*/
Link* list_create(void){
    Link * list = (Link*)malloc(sizeof(Link));
    list_init(list);
    return list;
}

/*!
*\fn           list_destroy(Link *list)
*
*\brief        Destroy a linked list.
*
*\param[out]   list     The list you want to destroy.
*
*/
void list_destroy(Link *list){
    if(list == NULL) return;
    Node *cur = NULL;
    Node *next = NULL;
    wrLock(&(list->lock));
        for (cur = list->head; cur != NULL; cur = next)
        {
            next = cur->next;
            cur->next=NULL;
            cur->prior=NULL;
            free(cur);
        }
     list->length=0;
     list->head=NULL;
     unLock(&(list->lock));
     delLock(&(list->lock));
     free(list);
     list = NULL;

}

/*!
*\fn           list_sort(Link *list, int (*compare)(void *, void *))
*
*\brief        Sort the linked list
*
*\param[out]   list         The linked list.
*\param[in]    compare      The compare function pointer.
*
*
*/
void list_sort(Link *list, int (*compare)(void *, void *)){
    if(list == NULL) return;
    wrLock(&(list->lock));
    quick_sort(list, 1, list->length, compare);
    unLock(&(list->lock));
}

/*!
*\fn           int list_search(Link *list, void *data, int (*equal)(void *, void *))
*
*\brief        Search a node in the linked list.
*
*\param[in]    list         The linked list.
*\param[in]    data         The info you want to search.
*\param[in]    equal        The equal function pointer.
*
*\return       the index of the node.
*/
int list_search(Link *list, void *data, int (*equal)(void *, void *)){
    if(NULL == list)  {printf("empty list!\n"); return 0;}
    rdLock(&(list->lock));
    Node *tmp = list->head;
    int i;
    for(i=1; i<=list->length; i++){
        if(1 == equal(data, tmp->data)){
            unLock(&(list->lock));
            return i;
        }
        tmp = tmp->next;
    }
    unLock(&(list->lock));
    return 0;
}

/*!
*\fn           list_print(Link *list, void (*print)(void* ))
*
*\brief        Print the linked list.
*
*\param[in]    list         The linked list.
*\param[in]    print        The print function pointer.
*
*/
void list_print(Link *list, void (*print)(void* )){
    if(list == NULL) {printf("empty list!\n"); return ;}

    Node *tmp = list->head;
    rdLock(&(list->lock));
    while(tmp != NULL){
        //printf("222\n");
        (*print)(tmp->data);
        //printf("1111\n");
        tmp = tmp->next;
    }
    unLock(&(list->lock));
}

/*!
*\fn           list_insert(Link *list, void *data, int pos)
*
*\brief        Insert the linked list with a node.
*
*\param[out]   list        The linked list.
*\param[in]    data        The info you want to insert.
*\param[in]    pos         The position you want to insert at.
*/
void list_insert(Link *list, void *data, int pos){
    Node *node = node_create(data);
    Node *tmp = list->head; //链表为空则为头结点
    wrLock(&(list->lock));
    if(tmp == NULL){
        list->head = node;
        list->length++;

    }

    else if(pos == 1){                         //插入链表头
        tmp->prior = node;
        node->next = tmp;
        list->head = node;
        list->length++;
    }
    else if(pos > list->length){                 //插入链表尾
        while(tmp){
            if(tmp->next == NULL)
                break;
            tmp=tmp->next;
        }
        tmp->next = node;
        node->prior = tmp;
        list->length++;
    }
    else {
        int i;                                     //插入链表中间
        for(i = 1; i<pos; i++){
            tmp=tmp->next;
        }
        tmp->prior->next = node;
        node->prior = tmp->prior;
        tmp->prior = node;
        node->next = tmp;
        list->length++;
    }
    unLock(&(list->lock));
}

/*!
*\fn           int list_delete(Link *list, int pos)
*
*\brief        Delete a node from the linked list.
*
*\param[out]   list        The linked list.
*\param[in]    pos         The position you want to delete at.
*
*\return
*/
int list_delete(Link *list, int pos){
    if(pos == 1 || pos >= list_length(list)) {
        printf("ILLEGAL NODE POSITION!\n");
        exit(0);
    }
    wrLock(&(list->lock));
    Node *p =list_get(list, pos);
    Node *q;
    if(p!=NULL)
    {
        q = p->prior;
        q->next = p->next;
        q->next->prior = q;

        p->next = NULL;
        p->prior = NULL;
        free(p);   //删除节点
        list->length--;
        return 0;
    }
    unLock(&(list->lock));
    return -1;
}

/*!
*\fn           int list_length(Link* list)
*
*\brief        Get the length of the linked list.
*
*\param[in]    list        The linked list.
*
*
*\return       the length of the linked list.
*/
int list_length(Link* list){
    if(list == NULL) return 0;
    int length = 0;
    rdLock(&(list->lock));
    length = list->length;
    unLock(&(list->lock));
    return length;
}

/*!
*\fn           Link* list_merge(Link* list1, Link* list2)
*
*\brief        Merge two list into a new one.
*
*\param[out]   list1        One of the linked lists.
*\param[out]   list2        Another linked list.
*
*\return       the merged linked list.
*/
Link* list_merge(Link* list1, Link* list2){
    if(list1 == NULL||list2 == NULL) return NULL;
    wrLock(&(list1->lock));
    rdLock(&(list2->lock));
    Node *tmp = list1->head;
    while(tmp->next){
        tmp = tmp->next;
    }
    tmp->next = list2->head;
    list2->head->prior = tmp;
    list1->length = list1->length + list2->length;
    unLock(&(list1->lock));
    unLock(&(list2->lock));
    return list1;
}
