/*!Copyright(c) 2019 Shenzhen TP-LINK Technologies Co.Ltd.
*
*\file producer_consumer.c
*\brief 生产者消费者模型
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

#include "producer_consumer.h"
#include "bool.h"


/*********************************************************************/
/*                           DEFINES                                 */
/*********************************************************************/
/*每个生产者(消费者)生产(消费)的数量 */
#define PRODUCTION_NUM 5
#define CONSUPTION_NUM 4

/*生产者消费者个数*/
#define PRODUCER_NUM 2
#define CONSUMER_NUM 2

/*********************************************************************/
/*                           TYPES                                   */
/*********************************************************************/


/*********************************************************************/
/*                           EXTERN PROTOTYPES                       */          
/*********************************************************************/


/*********************************************************************/
/*                           LOCAL PROTOTYPES                        */
/*********************************************************************/


/*********************************************************************/
/*                           VARIABLES                               */
/*********************************************************************/
static int item[10]={1,2,3,4,5,6,7,8,9,10};

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int prdcrThreadNum = 1;
static int cnsmrThreadNum = 1;

/*********************************************************************/
/*                           LOCAL FUNCTIONS                         */
/*********************************************************************/
static void delFuncT(void *pElement)
{

}
/*
static void printFuncT(void *pElement)
{
    int tmp = *(int*)pElement;
    printf("%d\n", tmp);
} */

/*多线程生产者 */
static void *producerMT(void *list)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    int tnum = prdcrThreadNum;
    prdcrThreadNum++;
    pthread_mutex_unlock(&mutex);

    printf("producer%d\n", tnum);
    dblList tmpList = (dblList)list;
    while(i < PRODUCTION_NUM)
    {
        pthread_mutex_lock(&mutex);
        if(TRUE == dblListAppend(tmpList, dblListGetLast(tmpList), &item[i]))
        {
            i++;
            pthread_cond_signal(&cond);
        }
       pthread_mutex_unlock(&mutex);
    }

    printf("end producer%d\n", tnum);
    return NULL;
}

/*多线程消费者*/
static void *consumerMT(void *list)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    int tnum = cnsmrThreadNum;
    cnsmrThreadNum++;
    pthread_mutex_unlock(&mutex);

    printf("consumer%d\n", tnum);
    DEL_FUNC delFunc = delFuncT;
    while(i < CONSUPTION_NUM)
    {
        pthread_mutex_lock(&mutex);
        if(dblListIsEmpty(list))
        {
            printf("list is empty: %d\n", tnum);    
            pthread_cond_wait(&cond, &mutex);

        }
        else
        {
            dblListDelNode((dblList)list, dblListGetFirst((dblList)list), delFunc);
            i++;
        }
        pthread_mutex_unlock(&mutex);   
    }
    printf("end consumer%d\n", tnum);
    return NULL;
}

/*单线程生产者 */
static void producerST(void *list, int tnum)
{
    int i = 0;
 
    printf("producer%d\n", tnum);
    dblList tmpList = (dblList)list;
    while(i < PRODUCTION_NUM)
    {
        if(TRUE == dblListAppend(tmpList, dblListGetLast(tmpList), &item[i]))
        {
            i++;
        }
    }

    printf("end producer%d\n", tnum);
    
}

/*单线程消费者*/
static int consumerST(void *list, int *cnsmrNum, int tnum)
{    
    printf("consumer%d\n", tnum);
    DEL_FUNC delFunc = delFuncT;
    while(*cnsmrNum < CONSUPTION_NUM)
    {
        if(dblListIsEmpty(list))
        {
            printf("list is empty: %d\n", tnum);
            break;    
        }
        else
        {
            dblListDelNode((dblList)list, dblListGetFirst((dblList)list), delFunc);
            (*cnsmrNum)++;
        } 
    }
    if(CONSUPTION_NUM == (*cnsmrNum))
    {
        printf("end consumer%d\n", tnum);
        *cnsmrNum = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*********************************************************************/
/*                           PUBLIC FUNCTIONS                        */
/*********************************************************************/
/*多线程生产者消费者模式 */
void producer_consumer_MT(dblList list)
{
    pthread_t producer1;
    pthread_t producer2;
    pthread_t consumer1;
    pthread_t consumer2;

   
    pthread_create(&consumer1, NULL, consumerMT, (void*)list);
    pthread_create(&consumer2, NULL, consumerMT, (void*)list);
    pthread_create(&producer1, NULL, producerMT, (void*)list);
    pthread_create(&producer2, NULL, producerMT, (void*)list);
   

    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    prdcrThreadNum = 1;
    cnsmrThreadNum = 1;
}

/*单线程生产者消费者模式 */
void producer_consumer_ST(dblList list)
{
    int consumer = 0;
    int producer = 0;
    /*当前消费者消费的数量 */
    int cnsmrNum = 0;
    while (1)
    {
        while(consumer < CONSUMER_NUM)
        {
            if(TRUE == consumerST(list, &cnsmrNum, consumer+1))
            {
                consumer++;
            }
            else
            {
                break;
            }  
        }
        while (producer < PRODUCER_NUM)
        {
            producerST(list, producer+1);
            producer++;
        }
        if((PRODUCER_NUM == producer) && (CONSUMER_NUM == consumer))
        {
            break;
        }
    }
}


/*********************************************************************/
/*                           GLOBAL FUNCTIONS                        */
/*********************************************************************/