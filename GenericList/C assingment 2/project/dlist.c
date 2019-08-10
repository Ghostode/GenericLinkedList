#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dlist.h"

typedef struct _DListNode              //声明节点
{
	struct _DListNode* prev;
	struct _DListNode* next;

	void* ID_num;
	void* IP_num;
	void* name;
	void* seat_num;
	void* group_num;
}DListNode;

struct _DList                            //声明链表结构     
{
	Locker* locker;
	DListNode* first;
	void* data_destroy_ctx;
	DListDataDestroyFunc data_destroy;
};



static DListNode* dlist_node_create(void* ID_num, void* IP_num,	void* name, void* seat_num, void* group_num)  //创建节点
{
	DListNode* node = malloc(sizeof(DListNode));

	if(node != NULL)
	{
		node->prev = NULL;
		node->next = NULL;
		node->ID_num = ID_num;
		node->IP_num = IP_num;
		node->name = name;
		node->seat_num = seat_num;
		node->group_num = group_num;
	}

	return node;
}

static void dlist_destroy_data(DList* thiz, void* data)         //删除数据
{
	if(thiz->data_destroy != NULL)
	{
		thiz->data_destroy(thiz->data_destroy_ctx, data);
	}
	return;
}

static void dlist_destroy_node(DList* thiz,DListNode* node)      //销毁某节点                                              
{
	if(node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		dlist_destroy_data(thiz, node->ID_num);
		SAFE_FREE(node);
	}
	return;
}


static void dlist_lock(DList* thiz)                               //加锁
{
	if(thiz->locker != NULL)
	{
		locker_lock(thiz->locker);
	}

	return;
}

static void dlist_unlock(DList* thiz)                              //解锁
{
	if(thiz->locker != NULL)
	{
		locker_unlock(thiz->locker);
	}

	return;
}

static void dlist_destroy_locker(DList* thiz)                       //销毁锁    
{
	if(thiz->locker != NULL)
	{
		locker_unlock(thiz->locker);
		locker_destroy(thiz->locker);
	}

	return;
}

DList* dlist_create(DListDataDestroyFunc data_destroy, void* ctx, Locker* locker)  //创建链表
{
	DList* thiz = malloc(sizeof(DList));

	if(thiz != NULL)
	{
		thiz->first  = NULL;
		thiz->locker = locker;
		thiz->data_destroy = data_destroy;
		thiz->data_destroy_ctx = ctx;
	}

	return thiz;
}


static DListNode* dlist_get_node(DList* thiz, size_t index)          //获取指定位置的节点
{
	DListNode* iter = thiz->first;

	while(iter != NULL && iter->next != NULL && index > 0)
	{
		iter = iter->next;
		index--;
	}
	return iter;
}


Ret dlist_insert(DList* thiz, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num)  //将数据插入链表
{
	
	Ret ret = RET_OK;
	DListNode* node = NULL;
	DListNode* cursor = NULL;
	dlist_lock(thiz);

    do
	{
		if((node = dlist_node_create(ID_num,IP_num,name,seat_num,group_num)) == NULL)
		{
			return RET_OOM; 
			break;
		}
		if(thiz->first == NULL)   
		{
			thiz->first = node;
			break;
		}
		cursor = dlist_get_node(thiz, -1);
		node->next = cursor; 
		if(cursor->prev != NULL)
		{
			cursor->prev->next = node;
		}
		cursor->prev = node;
		if(cursor == thiz->first)
		{
			thiz->first = node;
		}
	}while(0);
	dlist_unlock(thiz);
	return ret;
}

Ret dlist_append(DList* thiz, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num)  //将数据依次存入链表
{
	return dlist_insert(thiz, ID_num, IP_num, name, seat_num, group_num);
}

Ret dlist_delete(DList* thiz, size_t index)                  //删除指定节点
{
	Ret ret = RET_OK;
	DListNode* cursor = NULL;
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 
	dlist_lock(thiz);
	cursor = dlist_get_node(thiz, index);
	do
	{
		if(cursor == NULL)
		{
			ret = RET_INVALID_PARAMS;
			break;
		}
		else
		{
			if(cursor == thiz->first)
			{
				thiz->first = cursor->next;
			}
			if(cursor->next != NULL)
			{
				cursor->next->prev = cursor->prev;
			}

			if(cursor->prev != NULL)
			{
				cursor->prev->next = cursor->next;
			}

			dlist_destroy_node(thiz, cursor);
		}

	}while(0);
	dlist_unlock(thiz);
	return ret;
}
Ret dlist_set_by_index(DList* thiz, size_t index, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num)  //改变指定位置节点的数据
{
	DListNode* cursor = NULL;
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);
	dlist_lock(thiz);
	cursor = dlist_get_node(thiz, index);

	if(cursor != NULL)
	{
		cursor->ID_num = ID_num;
		cursor->IP_num = IP_num;
		cursor->name = name;
		cursor->seat_num = seat_num;
		cursor->group_num = group_num;
	}
	dlist_unlock(thiz);
	return cursor != NULL ? RET_OK : RET_INVALID_PARAMS;
}

size_t   dlist_length(DList* thiz)              //计算链表长度
{
	size_t length = 0;
	DListNode* iter = NULL;
	return_val_if_fail(thiz != NULL, 0);
	dlist_lock(thiz);
	iter = thiz->first;

	while(iter != NULL)
	{
		length++;
		iter = iter->next;
	}
	dlist_unlock(thiz);

	return length;
}
void dlist_find_ID(DList* thiz, void* ctx)        //找到某个指定ID的节点并打印
{
	int i = 0;
	DListNode* iter = NULL;
	return_val_if_fail(thiz != NULL, -1);
	dlist_lock(thiz);
	iter = thiz->first;
	while(iter != NULL)
	{
		if((long)(ctx) - (long)(iter->ID_num) == 0)
		{
			break;
		}
		i++;
		iter = iter->next;
	}
	printf("%d ", iter->ID_num);
	printf("%s ", iter->IP_num);
	printf("%s ", iter->name);
	printf("%d ", iter->seat_num);
	printf("%d ", iter->group_num);
	printf("\n");

	dlist_unlock(thiz);
	return;
}

void dlist_destroy(DList* thiz)                   //销毁整个链表
{
	DListNode* iter = NULL;
	DListNode* next = NULL;
	return_if_fail(thiz != NULL);
	dlist_lock(thiz);
	iter = thiz->first;

	while(iter != NULL)
	{
		next = iter->next;
		dlist_destroy_node(thiz,iter);
		iter = next;
	}
	thiz->first = NULL;
	dlist_destroy_locker(thiz);
	SAFE_FREE(thiz);
	return;
}

int dlist_isEmpty(DList* thiz)                    //判断链表是否为空
{
	if(NULL == thiz->first)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
void dlist_print(DList* thiz)                       //打印整个链表
{
	dlist_lock(thiz);
	DListNode* iter = thiz->first;

	while(iter != NULL)
	{
		printf("%d ", iter->ID_num);
		printf("%s ", iter->IP_num);
		printf("%s ", iter->name);
		printf("%d ", iter->seat_num);
		printf("%d ", iter->group_num);
		printf("\n");
		iter = iter->next;
	}
	dlist_unlock(thiz);
}


void dlist_upSort_ID(DList* thiz)			    //按照ID大小进行排序            
{
      dlist_lock(thiz);
	DList* head;
	DListNode *p, *q;
	void* temp;
	void* temp1;
	for(p=thiz->first;p!=NULL;p=p->next)
    {
        for(q=p->next;q!=NULL;q=q->next)
        {
            if(p->ID_num>q->ID_num)
            {
                temp = p->ID_num;
                p->ID_num = q->ID_num;
                q->ID_num = temp;
			    temp = p->IP_num;
                p->IP_num = q->IP_num;
                q->IP_num = temp;
				temp1 = p->name;
                p->name = q->name;
                q->name = temp1;
				temp = p->seat_num;
                p->seat_num = q->seat_num;
                q->seat_num = temp;
				temp = p->group_num;
                p->group_num = q->group_num;
                q->group_num = temp;
			}
        }
    }
	dlist_unlock(thiz);
}
void dlist_upSort_IP(DList* thiz)                        //按照IP大小进行排序              
{
	dlist_lock(thiz);
	DListNode *p, *q;
	void* temp;
	void* temp1;
	for(p=thiz->first;p!=NULL;p=p->next)
    	{
        for(q=p->next;q!=NULL;q=q->next)
        	{
            if(strcmp((p->IP_num),(q->IP_num))>0)
            	{
                temp = p->ID_num;
                p->ID_num = q->ID_num;
                q->ID_num = temp;
			    temp = p->IP_num;
                p->IP_num = q->IP_num;
                q->IP_num = temp;
				temp1 = p->name;
                p->name = q->name;
                q->name = temp1;
				temp = p->seat_num;
                p->seat_num = q->seat_num;
                q->seat_num = temp;
				temp = p->group_num;
                p->group_num = q->group_num;
                q->group_num = temp;
		}
       		}
    	}
	dlist_unlock(thiz);
}

void dlist_unite(DList* thiz, DList* b)                  //将两个链表合并         
{
	dlist_lock(thiz);
	DListNode* left = thiz->first;
	while(left->next != NULL)
	{
		left = left->next;
	}
	left->next = b->first;
	b->first->prev = left;
	dlist_unlock(thiz);
}
