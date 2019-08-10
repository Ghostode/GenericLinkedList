/*!Copyright(c) 2019 Shenzhen TP-LINK Technologies Co.Ltd.
*
*\file linkedlist.h
*\brief A general linked list head file.
*
*\author Zhao Haodong
*\version 1.0
*\date 2019 Aug 07
*
*\history
*
*
*/
#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

/***************************************************************************/
/*TYPES                                                                             */
/***************************************************************************/
struct node;
typedef  struct node  Node;

struct  link;
typedef  struct link  Link;

/***************************************************************************/
/*FUNCTIONS                                                                         */
/***************************************************************************/
Link*  list_create(void); //创建链表
void list_destroy(Link *list); //销毁链表
void list_sort(Link *list, int (*compare)(void *, void *)); //排序
int list_search(Link *list, void *data, int (*equal)(void *, void *)); //查找
void list_print(Link *list, void (*print)(void*)); //打印链表
void list_insert(Link *list, void *data, int pos); //插入节点
int list_delete(Link *list, int pos); //删除节点
int list_length(Link* list); //获取长度
Link* list_merge(Link* list1, Link* list2); //合并链表
#endif




