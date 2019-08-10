/*!Copyright(c) 2019 Shenzhen TP-LINK Technologies Co.Ltd.
*
*\file dblList.h
*\brief 双向链表接口，每一个方法都线程安全
*
*\author Liang Jian
*\version 1.1
*\date 2019.08.05
*
*\history 
*
* 
*/

/*********************************************************************/
/*                           CONFIGURATIONS                          */
/*********************************************************************/
#ifndef DBLLIST_H_
#define DBLLIST_H_

#ifdef _cplusplus
extern 'C'{
#endif

/*********************************************************************/
/*                           INCLUDE_FILES                           */
/*********************************************************************/
#include "bool.h"


/*********************************************************************/
/*                           DEFINES                                 */
/*********************************************************************/
#define FOWARD_DIRCTION 1
#define REVERSE_DIRCTION 2

#define READ_MODE 1
#define WRITE_MODE 2

/*********************************************************************/
/*                           TYPES                                   */
/*********************************************************************/
typedef void (*DEL_FUNC)(void *pElement);
typedef void (*FUNC)(void *pElement);
typedef bool (*MATCH_FUNC)(void *pElement, void *find);
typedef void (*MODIFY_FUNC)(void *pElement, void *mod);
typedef int (*COMPARE_FUNC)(void *pElement1, void *pElement2);

struct listNode;
typedef struct listNode NODE;
typedef NODE* position;

struct list;
typedef struct list LIST;
typedef LIST* dblList;

/*********************************************************************/
/*                           VARIABLES                               */
/*********************************************************************/


/*********************************************************************/
/*                           FUNCTIONS                               */
/*********************************************************************/

dblList dblListInit();
bool dblListIsEmpty(dblList list);
position dblListGetFirst(dblList list);
position dblListGetLast(dblList list);
int dblListGetLen(dblList list);
void* dblListGetData(dblList list, position pos);
position dblListGetNext(dblList list, position pos);
position dblListGetPrv(dblList list, position pos);
bool dblListDelNode(dblList list, position pos, DEL_FUNC delFunc);
bool dblListDelList(dblList list, DEL_FUNC delFunc);
bool dblListAppend(dblList list, position pos, void* newEle);
bool dblListSwap(dblList list, position pos1, position pos2);
void dblListForEach(dblList list, FUNC func, int drct, int mod);
position dblListFind(dblList list, position pos,MATCH_FUNC matchFunc, 
                    void *find);
bool dblListModify(dblList list, position pos, MODIFY_FUNC modifyFunc, 
                void* mod);
void dblListSort(dblList list, COMPARE_FUNC cmpFunc);


#ifdef _cplusplus
}
#endif

#endif
