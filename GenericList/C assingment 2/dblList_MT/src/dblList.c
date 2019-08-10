/*!Copyright(c) 2019 Shenzhen TP-LINK Technologies Co.Ltd.
*
*\file dblList.c
*\brief 
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


/*********************************************************************/
/*                           INCLUDE_FILES                           */
/*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "dblList.h"
#include "printError.h"

/*********************************************************************/
/*                           DEFINES                                 */
/*********************************************************************/


/*********************************************************************/
/*                           TYPES                                   */
/*********************************************************************/
struct listNode
{
	struct listNode *pPre;
	struct listNode *pNext;
	void *pElement;
};

struct list
{
	NODE *pHead;
	NODE *pEnd;
	int listLength;
	pthread_rwlock_t lock;
};

/*********************************************************************/
/*                           EXTERN PROTOTYPES                       */          
/*********************************************************************/


/*********************************************************************/
/*                           LOCAL PROTOTYPES                        */
/*********************************************************************/
static bool isHead(dblList list, position pos);
static bool isEnd(dblList list, position pos);
static bool judgeList(dblList list);
static bool judgePos(position pos);
static void initLock(pthread_rwlock_t *rwlock);
static void rdLock(pthread_rwlock_t *rwlock);
static void wrLock(pthread_rwlock_t *rwlock);
static void unlock(pthread_rwlock_t *rwlock);
static void delLock(pthread_rwlock_t *rwlock);

/*********************************************************************/
/*                           VARIABLES                               */
/*********************************************************************/


/*********************************************************************/
/*                           LOCAL FUNCTIONS                         */
/*********************************************************************/

/*********************************************************************
 Function: static void initLock(pthread_rwlock_t *rwlock)
 Description: 初始化锁
 Input: pthread_rwlock_t *rwlock 链表的锁 
 Output: 
 Return: 
 Others: 这里采用读写锁
 *******************************************************************/
static void initLock(pthread_rwlock_t *rwlock)
{
	int res = 0;
	res = pthread_rwlock_init(rwlock, NULL);
	if(0 != res)
	{
		PRINT_ERROR("INIT LOCK FAILURE");
	}
}

/*********************************************************************
 Function: static void rdLock(pthread_rwlock_t *rwlock)
 Description: 读状态上锁
 Input: pthread_rwlock_t *rwlock 链表的锁 
 Output: 
 Return: 
 Others: 
 *******************************************************************/
static void rdLock(pthread_rwlock_t *rwlock)
{
	
	pthread_rwlock_rdlock(rwlock);
	
}

/*********************************************************************
 Function: static void wrLock(pthread_rwlock_t *rwlock)
 Description: 写状态上锁
 Input: pthread_rwlock_t *rwlock 链表的锁 
 Output: 
 Return: 
 Others: 
 *******************************************************************/
static void wrLock(pthread_rwlock_t *rwlock)
{
	pthread_rwlock_wrlock(rwlock);
}

/*********************************************************************
 Function: static void unlock(pthread_rwlock_t *rwlock)
 Description: 解锁
 Input: pthread_rwlock_t *rwlock 链表的锁 
 Output: 
 Return: 
 Others: 
 *******************************************************************/
static void unlock(pthread_rwlock_t *rwlock)
{
	pthread_rwlock_unlock(rwlock);
}

/*********************************************************************
 Function: static void delLock(pthread_rwlock_t *rwlock)
 Description: 删除锁
 Input: pthread_rwlock_t *rwlock 链表的锁 
 Output: 
 Return: 
 Others: 
 *******************************************************************/
static void delLock(pthread_rwlock_t *rwlock)
{
	int res = 0;
	res = pthread_rwlock_destroy(rwlock);
	if(0 != res)
	{
		printf("%d",res);
		PRINT_ERROR("DESTORY LOCK FAILURE");
	}
}

/*********************************************************************
 Function: bool isHead(dblList list, position pos)
 Description: 判断链表中某一位置是否为链表的头部
 Input: dblList list所在链表， 
		position pos节点位置
 Output: 
 Return: 如果是头部，返回TRUE，否则返回FALSE
 Others: 
 *******************************************************************/
static bool isHead(dblList list, position pos)
{
	if (NULL == pos)
	{
		PRINT_ERROR("POSITION IS NULL");
	}

	if (list->pHead == pos)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*********************************************************************
 Function: isEnd
 Description: 判断链表中某一位置是否为链表的尾部
 Input: dblList list所在链表， 
		position pos节点位置
 Output: 
 Return: 如果是尾部，返回TRUE，否则返回FALSE
 Others: 
 *********************************************************************/
static bool isEnd(dblList list, position pos) 
{
	if (NULL == pos)
	{
		PRINT_ERROR("POSITION IS NULL");
	}

	if (pos == list->pEnd)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*********************************************************************
 Function: bool judgeList(dblList list)
 Description: 判断链表是否合法
 Input: dblList list所在链表， 
 Output: 
 Return: 如果是合法链表，返回TRUE，否则返回FALSE
 Others: 
 ********************************************************************/
static bool judgeList(dblList list)
{
	if(list == NULL)
	{
		return FALSE;
	}
	
	//rdlock(list->listlock)
	if((list->pHead != NULL)&&
		(list->pEnd != NULL))
	{
	//	unlock(list->listlock);
		return TRUE;
	}
	else
	{
	//	unlock(list->listlock);
		return FALSE;
	}	
}

/*********************************************************************
 Function: bool judgePos(position pos)
 Description: 判断结点位置是否合法
 Input: position pos 判断位置， 
 Output: 
 Return: 如果是合法位置，返回TRUE，否则返回FALSE
 Others: 
 *********************************************************************/
static bool judgePos(position pos)
{
	if(NULL == pos)
	{
		return FALSE;
	}
	//rdlock(&(pos->listlock));
	if((pos->pNext != NULL)&&
		(pos->pPre != NULL))
	{
	//	unlock(&(pos->listlock));
		return TRUE;
	}
	else
	{
	//	unlock(&(pos->listlock));
		return FALSE;
	}		
}

/*********************************************************************/
/*                           PUBLIC FUNCTIONS                        */
/*********************************************************************/

/*********************************************************************
 Function: dblList dblListInit()
 Description: 新建一个空的双向链表
 Input: 
 Output: 
 Return: 返回新表的地址
 Others: 创建失败直接退出程序
 ********************************************************************/
dblList dblListInit()
{
	dblList newList = NULL;
	newList = (dblList)malloc(sizeof(LIST));
	if(NULL != newList)
	{
		NODE *pHead = NULL;
		NODE *pEnd = NULL;
		NODE *ptmp = NULL;
		
		
		ptmp = (NODE*)malloc(sizeof(NODE));
		if (NULL == ptmp)
		{
			PRINT_ERROR("MALLOC ERROR");
		}
		pHead = ptmp;
		pHead->pPre = NULL;
		pHead->pNext = NULL;
		pHead->pElement = NULL;

		ptmp = NULL;
		ptmp = (NODE*)malloc(sizeof(NODE));
		if (NULL == ptmp)
		{
			free(pHead);
			pHead = NULL;
			PRINT_ERROR("MALLOC ERROR");
		}
		pEnd = ptmp;
		ptmp = NULL;

		pHead->pNext = pEnd;
		pEnd->pPre = pHead;
		pEnd->pNext = NULL;
		pEnd->pElement = NULL;

		newList->pHead = pHead;
		newList->pEnd = pEnd;
		newList->listLength = 0;
		initLock(&(newList->lock));

		pHead = NULL;
		pEnd = NULL;

		return newList;
	}
	else
	{
		PRINT_ERROR("MALLOC ERROR");
	}		
}

/*********************************************************************
 Function: bool dblListIsEmpty(dblList list)
 Description: 判断某一链表是否为空
 Input: dblList list判断的链表
 Output: 
 Return: 如果为空，返回TRUE，否则返回FALSE
 Others: 
 ********************************************************************/
bool dblListIsEmpty(dblList list)
{
	rdLock(&(list->lock));
	if(judgeList(list))
	{
		if (0 == list->listLength)
		{
			unlock(&((list->lock)));
			return TRUE;
		}
		else
		{
			unlock(&((list->lock)));
			return FALSE;
		}
	}
	else
	{
		PRINT_ERROR("ILLEGAL LIST");
	}
}

/*********************************************************************
 Function: position dblListGetFirst(dblList list)
 Description: 获取链表第一个结点的地址
 Input: dblList list 判断的链表
 Output: 
 Return: 如果为空，返回NULL，否则返回第一个结点地址
 Others: 
 ********************************************************************/
position dblListGetFirst(dblList list)
{
	position pos = NULL;
	rdLock(&(list->lock));
	if(judgeList(list))
	{
		pos = list->pHead->pNext;
		unlock(&(list->lock));
		return pos;
	}
	else
	{
		PRINT_ERROR("ILLEGAL LIST");
	}
}

/*********************************************************************
 Function: position dblListGetLast(dblList list)
 Description: 获取链表最后一个结点的地址
 Input: dblList list 判断的链表
 Output: 
 Return: 如果为空，返回NULL，否则返回最后一个结点地址
 Others: 
 ********************************************************************/
position dblListGetLast(dblList list)
{
	position pos = NULL;
	rdLock(&(list->lock));
	if(judgeList(list))
	{
		pos = list->pEnd->pPre;
		unlock(&(list->lock));
		return pos;
	}
	else
	{
		PRINT_ERROR("ILLEGAL LIST");
	}
}

/*********************************************************************
 Function: int dblListGetLen(dblList list)
 Description: 获取链表存储数据个数
 Input: dblList list 判断的链表
 Output: 
 Return: 返回存储数据个数
 Others: 
 ********************************************************************/
int dblListGetLen(dblList list)
{
	int length = 0;
	rdLock(&(list->lock));
	if(judgeList(list))
	{
		length = list->listLength;
		unlock(&(list->lock));
		return length;
	}
	else
	{
		PRINT_ERROR("ILLEGAL LIST");
	}
}

/*********************************************************************
 Function: void* dblListGetData(dblList list, position pos)
 Description: 获取链表中某一位置存储数据的地址
 Input: dblList list 判断的链表; position pos 当前位置
 Output: 
 Return: 如果为空，返回NULL，否则返回存储数据的地址
 Others: 
 ********************************************************************/
void* dblListGetData(dblList list, position pos)
{
	void *pData = NULL;

	if(dblListIsEmpty(list))
	{
		return NULL;
	}
	else
	{
		rdLock(&(list->lock));
		if(judgePos(pos))
		{
			pData = pos->pElement;
			unlock(&(list->lock));
			return pData;
		}
		else
		{
			PRINT_ERROR("ILLEGAL POSITION");
		}
	}
}

/*********************************************************************
 Function: position dblListGetPrv(dblList list, position pos)
 Description: 获取链表中某一位置的上一个结点
 Input: dblList list 判断的链表; position pos 当前位置
 Output: 
 Return: 如果为空，返回NULL，否则返回上一个结点地址
 Others: 
 ********************************************************************/
position dblListGetPrv(dblList list, position pos)
{
	position res = NULL;
	if(dblListIsEmpty(list))
	{
		return NULL;
	}
	else
	{
		rdLock(&(list->lock));
		if(judgePos(pos))
		{
			res = pos->pPre;
			unlock(&(list->lock));
			return res;
		}
		else
		{
			PRINT_ERROR("ILLEGAL POSITION");
		}
	}
}

/*********************************************************************
 Function: position dblListGetNext(dblList list, position pos)
 Description: 获取链表中某一位置的下一个结点
 Input: dblList list 判断的链表; position pos 当前位置
 Output: 
 Return: 如果为空，返回NULL，否则返回上一个结点地址
 Others: 
 ********************************************************************/
position dblListGetNext(dblList list, position pos)
{
	position res = NULL;
	if(dblListIsEmpty(list))
	{
		return NULL;
	}
	else
	{
		rdLock(&(list->lock));
		if(judgePos(pos))
		{
			res = pos->pNext;
			unlock(&(list->lock));
			return res;
		}
		else
		{
			PRINT_ERROR("ILLEGAL POSITION");
		}
	}
}

/*********************************************************************
 Function: void dblListDelNode(dblList list, position pos, 
                                DEL_FUNC delFunc)
 Description: 删除链表中某一位置的节点
 Input: dblList list所在链表， 
		position pos删除节点的位置
		DEL_FUNC delFunc 删除具体存储数据的方法
 Output: 
 Return: 删除成功返回TRUE，链表为空时返回FALSE
 Others: 不能删除头尾两个节点，非法删除直接退出程序
 ********************************************************************/
bool dblListDelNode(dblList list, position pos, DEL_FUNC delFunc)
{

	wrLock(&(list->lock));
	if(0 == list->listLength)
	{
		printf("LIST IS EMPTY\n");
		unlock(&(list->lock));
		return FALSE;
	}
	else if(judgePos(pos))
	{
		/*不删除头尾的节点或者尾节点*/
		if ((NULL == pos->pPre) || (NULL == pos->pNext))
		{
			PRINT_ERROR("ILLEGAL NODE POSITION");
		}
		/*先将pos前后两个节点连接*/
		pos->pNext->pPre = pos->pPre;
		pos->pPre->pNext = pos->pNext;

		pos->pPre = NULL;
		pos->pNext = NULL;
		/*删除存储的具体数据*/
		//printf("del:%d\n", *(int*)pos->pElement);
		delFunc(pos->pElement);
		pos->pElement = NULL;
		free(pos);
		pos = NULL;

		list->listLength -= 1;
		unlock(&(list->lock));
		
		return TRUE;
	}
	else
	{
		unlock(&(list->lock));
		PRINT_ERROR("ILLEGAL POSITION");
	}
}

/*********************************************************************
 Function: bool dblListDelList(dblList list, DEL_FUNC delFunc)
 Description: 删除一个链表
 Input: dblList list 需要删除的链表
		DEL_FUNC delFunc 删除具体存储数据的方法
 Output: 
 Return: 如果删除成功，返回TRUE
 Others: 非法删除直接退出程序
*********************************************************************/
bool dblListDelList(dblList list, DEL_FUNC delFunc)
{	

	position tempPos = NULL;
	while(FALSE == dblListIsEmpty(list))
	{	
		dblListDelNode(list, tempPos = list->pHead->pNext, delFunc);
	}

	wrLock(&(list->lock));

	list->pHead->pNext = NULL;
	list->pEnd->pPre =NULL;

	free(list->pHead);
	free(list->pEnd);
	list->pHead = NULL;
	list->pEnd =NULL;
	delLock(&(list->lock));
	free(list);
	list = NULL;

	return TRUE;
}

/*********************************************************************
 Function: bool dblListSwap(dblList list, position pos1 , position pos2)
 Description: 交换链表中某两个节点
 Input: dblList list所在链表，position pos1 , position pos2
		需要交换的两个位置
 Output: 
 Return: 如果交换成功，返回TRUE
 Others: 不能交换头尾节点，非法交换直接退出程序
*********************************************************************/
bool dblListSwap(dblList list, position pos1 , position pos2)
{	
	/*头尾不能交换*/
	void* pTmp = NULL;

	wrLock(&(list->lock));
	if(judgeList(list)&&judgePos(pos1)&&judgePos(pos2))
	{
		pTmp = pos1->pElement;
		pos1->pElement = pos2->pElement;
		pos2->pElement = pTmp;
		pTmp = NULL;
		unlock(&(list->lock));
		return TRUE;
	}
	else
	{
		unlock(&(list->lock));
		PRINT_ERROR("ILLEGAL LIST/POSITION");
	}	
}


/*********************************************************************
 Function: bool dblListAppend(dblList list, position pos, void* newEle)
 Description:从某一位置后插入一个新节点 
 Input: dblList list所在链表，
		position pos所要插入的位置
 Output: 
 Return: 插入成功返回TRUE
 Others: 
*********************************************************************/
bool dblListAppend(dblList list, position pos, void* newEle)
{
	if(judgeList(list)){
		NODE *newNode = NULL;

		newNode = (NODE*)malloc(sizeof(NODE));
		if (NULL == newNode)
		{
			PRINT_ERROR("MALLOC ERROR");
		}
		newNode->pPre = NULL;
		newNode->pNext = NULL;
		newNode->pElement = NULL;
			
		newNode->pElement = newEle;

		wrLock(&(list->lock));

		if(pos != NULL &&(pos->pNext != NULL || pos->pPre != NULL))
		{
			if(isEnd(list, pos) == TRUE)
			{
				newNode->pNext = pos;
				newNode->pPre = pos->pPre;
				pos->pPre->pNext = newNode;
				pos->pPre = newNode;
			}
			else
			{
				newNode->pNext = pos->pNext;
				newNode->pPre = pos;
				pos->pNext->pPre = newNode;
				pos->pNext = newNode;
			}
			//printf("add: %d\n", *(int*)newNode->pElement);
			list->listLength += 1;

			unlock(&(list->lock));
			newNode = NULL;	
			return TRUE;
		}
		else
		{
			unlock(&(list->lock));
			newNode->pElement = NULL;
			free(newNode);
			return FALSE;
		}
	}
	else
	{
		PRINT_ERROR("ILLEGAL LIST");
	}
}

/*********************************************************************
 Function: bool dblListModify(dblList list, position pos,
 							MODIFY_FUNC modifyFunc,void* mod)
 Description:修改某一位置的数据
 Input: dblList list所在链表，
		position pos所要修改的位置
		MODIFY_FUNC modifyFunc 修改方法
		void* mod	新的数据
 Output: 
 Return: 插入成功返回TRUE
 Others: 
*********************************************************************/
bool dblListModify(dblList list, position pos, MODIFY_FUNC modifyFunc, 
				void* mod)
{
	wrLock(&(list->lock));
	modifyFunc(pos->pElement, mod);
	unlock(&(list->lock));
	return TRUE;
}

/*********************************************************************
 Function: void dblListForEach(dblList list, FUNC Func, 
                            int drct, int mod)
 Description: 遍历整个链表
 Input: dblList list 需要遍历的链表；
		FUNC func 数据处理方法的函数指针;
		在elementType.h定义；
		int drct 链表遍历方向，顺序还是倒序
		int mod 遍历方法的读写类型
 Output: 
 Return: 
 Others:drct取值：FOWARD_DIRCTION 1 正向遍历
		REVERSE_DIRCTION 2 倒序遍历
		mod取值：READ_MODE 1
				WRITE_MODE 2
 *********************************************************************/
void dblListForEach(dblList list, FUNC func, int drct, int mod)
{
	if(FALSE == dblListIsEmpty(list))
	{
		position tmpPos = NULL;

		switch (drct)
		{
		case FOWARD_DIRCTION:
			tmpPos = list->pHead->pNext;
			if(READ_MODE == mod)
			{
				rdLock(&(list->lock));
				while (FALSE == isEnd(list, tmpPos))
				{
					func(tmpPos->pElement);
					tmpPos=tmpPos->pNext;	
				}
				unlock(&(list->lock));
			}
			else if(WRITE_MODE == mod)
			{
				wrLock(&(list->lock));
				while (FALSE == isEnd(list, tmpPos))
				{
					func(tmpPos->pElement);
					tmpPos=tmpPos->pNext;
				}
				unlock(&(list->lock));
			}
			else
			{
				PRINT_ERROR("ILLEGAL MODE");
			}
			
			break;
		case REVERSE_DIRCTION:
			tmpPos = list->pEnd->pPre;
			if(READ_MODE == mod)
			{
				rdLock(&(list->lock));
				while (FALSE == isHead(list, tmpPos))
				{
					func(tmpPos->pElement);
					tmpPos=tmpPos->pPre;
				}
				unlock(&(list->lock));
			}
			else if(WRITE_MODE == mod)
			{
				wrLock(&(list->lock));
				while (FALSE == isHead(list, tmpPos))
				{
					func(tmpPos->pElement);
					tmpPos=tmpPos->pPre;
				}
				unlock(&(list->lock));
			}
			else
			{
				PRINT_ERROR("ILLEGAL MODE");
			}
			break;
		default:
			PRINT_ERROR("ILLEGAL DIRECTION");
		}
	}
	else
	{
		printf("LIST IS EMPTY\n");
	}
	
}


/*********************************************************************
 Function: position dblListFind(dblList list, position pos,
                                MATCH_FUNC matchFunc, 
                                void *find)
 Description: 从指定位置开始查找，返回找到的第一个位置
 Input: dblList list 查找链表；
		MATCH_FUNC matchFunc 匹配方法的函数指针；
		void *find 被查找的内容
 Output: 
 Return: 返回查找到的第一项的位置，否则返回NULL；
 Others: 
 *********************************************************************/
position dblListFind(dblList list, position pos,
                    MATCH_FUNC matchFunc, 
                    void *find)
{
	position tmpPos = pos;
	
	if(dblListIsEmpty(list))
	{
		printf("LIST IS EMPTY\n");
		return NULL;
	}
	rdLock(&(list->lock));
	if(judgePos(pos))
	{
		while (FALSE == isEnd(list, tmpPos))
		{
			if(matchFunc(tmpPos->pElement, find) == TRUE)
			{
				unlock(&(list->lock));
				return tmpPos;
			}
			else
			{
				tmpPos=tmpPos->pNext;
			}
		}
		unlock(&(list->lock));
		return NULL;
	}
	else
	{
		unlock(&(list->lock));
		PRINT_ERROR("ILLEGAL POSITION");
	}	
}

/*********************************************************************
 Function: void dblListSort(dblList list, COMPARE_FUNC cmpFunc, 
                            int sortType)
 Description: 对链表按照某一类型进行排序
 Input: dblList list 需要排序的链表；
		COMPARE_FUNC cmpFunc 比较方法的函数指针；
 Output: dblList list 排序后的链表
 Return: 
 Others: 
 *********************************************************************/
void dblListSort(dblList list, COMPARE_FUNC cmpFunc)
{	
	if(FALSE == dblListIsEmpty(list))
	{
		wrLock(&(list->lock));
		position maxPos = list->pHead->pNext;
		position cmpPos = maxPos;
		position pos = maxPos->pNext;
		int cmpFlag = 0;

		/*总是插在cmppos的后面 */
		while (pos!= list->pEnd)
		{
			while(cmpPos != list->pHead)
			{
				if(cmpFunc(pos->pElement, cmpPos->pElement) < 0)
				{
					cmpPos=cmpPos->pPre;
					cmpFlag =1;
				}
				else
				{
					break;
				}
			}
			if (cmpFlag == 1)
			{
				pos->pPre->pNext = pos->pNext;
				pos->pNext->pPre = pos->pPre;
				cmpPos->pNext->pPre = pos;
				pos->pNext = cmpPos->pNext;
				cmpPos->pNext = pos;
				pos->pPre = cmpPos;
			}
			else if(cmpFlag == 0)
			{
				maxPos = pos;
			}
			cmpPos = maxPos;
			pos = maxPos->pNext;
			cmpFlag = 0;
		}
		unlock(&(list->lock));
	}
	else
	{
		printf("list is empty\n");
	}	
}

/*********************************************************************/
/*                           GLOBAL FUNCTIONS                        */
/*********************************************************************/