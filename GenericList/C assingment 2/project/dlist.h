#include "locker.h"

#ifndef DLIST_H
#define DLIST_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

struct _DList;            
typedef struct _DList DList; 

typedef void     (*DListDataDestroyFunc)(void* ctx, void* data);
//typedef int      (*DListDataCompareFunc)(void* ctx, void* data);


DList* dlist_create(DListDataDestroyFunc data_destroy, void* ctx, Locker* locker);      //创建链表
Ret dlist_insert(DList* thiz, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num);//插入链表
Ret dlist_append(DList* thiz, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num);//将数据依次存入链表
Ret dlist_delete(DList* thiz, size_t index);                                                                       //删除指定节点
Ret dlist_set_by_index(DList* thiz, size_t index, void* ID_num, void* IP_num,	void* name, void* seat_num, void* group_num);//改变指定位置节点的数据
size_t   dlist_length(DList* thiz);          //计算链表长度
void dlist_find_ID(DList* thiz, void* ctx);  //找到某个指定ID的节点并打印
void dlist_print(DList* thiz);               //打印整个链表
int dlist_isEmpty(DList* thiz);              //判断是否为空
void dlist_destroy(DList* thiz);             //销毁整个链表
void dlist_upSort_ID(DList* thiz);           //按照工号升序排序
void dlist_upSort_IP(DList* thiz);           //按照IP地址升序排序
void dlist_unite(DList* a, DList* b);        //将两个列表合并
#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*DLIST*/
