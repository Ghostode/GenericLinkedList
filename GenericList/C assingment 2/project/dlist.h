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


DList* dlist_create(DListDataDestroyFunc data_destroy, void* ctx, Locker* locker);      //��������
Ret dlist_insert(DList* thiz, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num);//��������
Ret dlist_append(DList* thiz, void* ID_num, void* IP_num, void* name, void* seat_num, void* group_num);//���������δ�������
Ret dlist_delete(DList* thiz, size_t index);                                                                       //ɾ��ָ���ڵ�
Ret dlist_set_by_index(DList* thiz, size_t index, void* ID_num, void* IP_num,	void* name, void* seat_num, void* group_num);//�ı�ָ��λ�ýڵ������
size_t   dlist_length(DList* thiz);          //����������
void dlist_find_ID(DList* thiz, void* ctx);  //�ҵ�ĳ��ָ��ID�Ľڵ㲢��ӡ
void dlist_print(DList* thiz);               //��ӡ��������
int dlist_isEmpty(DList* thiz);              //�ж��Ƿ�Ϊ��
void dlist_destroy(DList* thiz);             //������������
void dlist_upSort_ID(DList* thiz);           //���չ�����������
void dlist_upSort_IP(DList* thiz);           //����IP��ַ��������
void dlist_unite(DList* a, DList* b);        //�������б�ϲ�
#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*DLIST*/
