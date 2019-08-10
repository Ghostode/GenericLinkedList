//*************************//
//*******c语言作业—2******//   
//******author:陈忠诚******//
//******date:2019.8.5*****//

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "locker_pthread.h"
#include "dlist.h"

	char a1[] = "Geordie  ";
	char a2[] = "Edward   ";
	char a3[] = "Adams    ";
	char a4[] = "Harris   ";
	char a5[] = "Christian";
	char a6[] = "Owen     ";
	char a7[] = "Julius   ";
	char a8[] = "Violet   ";
	char a9[] = "Mathen   ";
	char a10[] = "Yule     ";
	char b1[] = "Brian    ";
	char b2[] = "Jacob    ";
	char b3[] = "Hale     ";
	char b4[] = "Hiram    ";
    	char b5[] = "Elijah   ";
	char b6[] = "Daniel   ";
	char b7[] = "Patrick  ";
	char b8[] = "Joshua   ";
	char b9[] = "Arthur   ";
	char b10[] = "Fank     ";
	char c1[] = "Alex     ";
	char c2[] = "Andy     ";
	char c3[] = "Mary     ";
	char c4[] = "Zyne     ";
	char c5[] = "Coco     ";
	char a11[] = "192.132.34.00";
	char a12[] = "172.234.23.12";
	char a13[] = "234.231.23.12";
	char a14[] = "136.156.20.10";
	char a15[] = "192.232.23.10";
	char a16[] = "176.180.00.60";
	char a17[] = "123.232.00.10";
	char a18[] = "171.115.25.25";
	char a19[] = "210.247.15.45";
	char a110[] = "182.180.23.10";
	char b11[] = "121.177.45.37";
	char b12[] = "192.143.24.87";
	char b13[] = "186.223.32.42";
	char b14[] = "184.233.34.12";
    	char b15[] = "178.223.11.23";
	char b16[] = "122.234.14.22";
	char b17[] = "193.212.23.24";
	char b18[] = "192.123.11.24";
	char b19[] = "186.223.34.23";
	char b110[] = "172.123.38.34";
	char c11[] = "122.192.14.80";
	char c12[] = "132.102.39.31";
	char c13[] = "122.232.24.50";
	char c14[] = "105.131.24.29";
	char c15[] = "248.198.19.35";

static void* producer(void* param)                                      //生产者函数
{
	int i = 0;
	DList* dlist_a = (DList*)param;                                   //向链表中依次添加数据
	printf("producer\n");
	dlist_append(dlist_a, (void*)12021,(void*)a11,(void*)a1,(void*)12,(void*)1);
	printf("producer\n");
                dlist_append(dlist_a, (void*)10021,(void*)a12,(void*)a2,(void*)15,(void*)1);
	printf("producer\n");
	dlist_print(dlist_a);
     	dlist_append(dlist_a, (void*)13722,(void*)a13,(void*)a3,(void*)13,(void*)1);
	printf("producer\n");
	dlist_append(dlist_a, (void*)14687,(void*)a14,(void*)a4,(void*)18,(void*)1);
	printf("producer\n");
     	dlist_print(dlist_a);
	sleep(1);
	dlist_append(dlist_a, (void*)17354,(void*)a15,(void*)a5,(void*)19,(void*)1);
	printf("producer\n");
	dlist_append(dlist_a, (void*)12345,(void*)a16,(void*)a6,(void*)10,(void*)1);
	printf("producer\n");	
	dlist_print(dlist_a);
	dlist_append(dlist_a, (void*)12453,(void*)a17,(void*)a7,(void*)11,(void*)1);
	printf("producer\n");
	dlist_append(dlist_a, (void*)10253,(void*)a18,(void*)a8,(void*)17,(void*)1);
	printf("producer\n");	
	dlist_print(dlist_a);
	sleep(1);
	dlist_append(dlist_a, (void*)10243,(void*)a19,(void*)a9,(void*)16,(void*)1);
	printf("producer\n");
	dlist_append(dlist_a, (void*)12431,(void*)a110,(void*)a10,(void*)14,(void*)1);
	printf("producer\n");
	dlist_print(dlist_a);
	sleep(1);
	return NULL;
}

static void* consumer(void* param)                                        //消费者函数
{
	int i = 0;
	DList* dlist = (DList*)param;                                       //依次取出链表中的节点
	for(i = 0; i < 3; i++)
	{
		usleep(1);
		dlist_delete(dlist, 0);
		printf("consumer\n");
		dlist_print(dlist);
	}
	return NULL;
}

static void multi_thread_work(void)                                       //多线程模式
{
	pthread_t consumer_tid ;                                            //分配消费者线程
	pthread_t producer_tid ;                                            //分配生产者线程
	DList* dlist = dlist_create(NULL, NULL, locker_pthread_create());   //创建多线程工作链表
	pthread_create(&producer_tid, NULL, producer, dlist);               //创建生产者线程
	pthread_create(&consumer_tid, NULL, consumer, dlist);               //创建消费者线程

     	pthread_join(producer_tid, NULL);                                   //等待执行                                 
     	pthread_join(consumer_tid, NULL);
	return;
}
static void single_thread_work(void)
{
	int n=5;
	int i=0;
	int d;
	DList* dlist_a;
	DList* dlist_b;
	dlist_a = dlist_create(NULL, NULL, locker_pthread_create());              //创建连表dlist_a并输入数据
	dlist_append(dlist_a, (void*)12021,(void*)a11,(void*)a1,(void*)12,(void*)1);
     	dlist_append(dlist_a, (void*)10021,(void*)a12,(void*)a2,(void*)15,(void*)1);
    	dlist_append(dlist_a, (void*)13722,(void*)a13,(void*)a3,(void*)13,(void*)1);
	dlist_append(dlist_a, (void*)14687,(void*)a14,(void*)a4,(void*)18,(void*)1);
	dlist_append(dlist_a, (void*)17354,(void*)a15,(void*)a5,(void*)19,(void*)1);
	dlist_append(dlist_a, (void*)12345,(void*)a16,(void*)a6,(void*)10,(void*)1);
	dlist_append(dlist_a, (void*)12453,(void*)a17,(void*)a7,(void*)11,(void*)1);
	dlist_append(dlist_a, (void*)10253,(void*)a18,(void*)a8,(void*)17,(void*)1);
	dlist_append(dlist_a, (void*)10243,(void*)a19,(void*)a9,(void*)16,(void*)1);
	dlist_append(dlist_a, (void*)12431,(void*)a110,(void*)a10,(void*)14,(void*)1);

	dlist_b = dlist_create(NULL, NULL, locker_pthread_create());			//创建链表dlist_b并输入数据
	dlist_append(dlist_b, (void*)13021,(void*)b11,(void*)b1,(void*)15,(void*)2);
     	dlist_append(dlist_b, (void*)10421,(void*)b12,(void*)b2,(void*)12,(void*)2);
     	dlist_append(dlist_b, (void*)13722,(void*)b13,(void*)b3,(void*)18,(void*)2);
	dlist_append(dlist_b, (void*)14887,(void*)b14,(void*)b4,(void*)19,(void*)2);
	dlist_append(dlist_b, (void*)19354,(void*)b15,(void*)b5,(void*)13,(void*)2);
	dlist_append(dlist_b, (void*)14345,(void*)b16,(void*)b6,(void*)10,(void*)2);
	dlist_append(dlist_b, (void*)12453,(void*)b17,(void*)b7,(void*)11,(void*)2);
	dlist_append(dlist_b, (void*)10953,(void*)b18,(void*)b8,(void*)16,(void*)2);
	dlist_append(dlist_b, (void*)15243,(void*)b19,(void*)b9,(void*)17,(void*)2);
	dlist_append(dlist_b, (void*)11431,(void*)b110,(void*)b10,(void*)14,(void*)2);
	
	if(dlist_isEmpty(dlist_a)&&dlist_isEmpty(dlist_b))                          //判断链表是否为空
	{
		dlist_print(dlist_a);                                                 //打印链表
	    	printf("\n");
		dlist_print(dlist_b);
	    	printf("\n");
		dlist_length(dlist_a);                                                 //计算两链表长度
		dlist_length(dlist_b);
        	                printf("SORT BY ID:\n");             
          	                dlist_upSort_ID(dlist_a);                                              //运用ID信息对链表进行排序
	    	dlist_print(dlist_a);
	    	printf("\n"); 
	    	dlist_upSort_ID(dlist_b);
	    	dlist_print(dlist_b);
	    	printf("\n");
         	                dlist_unite(dlist_a, dlist_b);                                          //合并两个链表
	    	printf("UNITE TWO LINKLIST AND SORT BY IP:\n");
	    	dlist_upSort_IP(dlist_a);                                               //运用IP信息对链表进行排序
        	                dlist_print(dlist_a);
	    	printf("\n");
	    	printf("FIND ID IS 10253 NODE:\n");
		dlist_find_ID(dlist_a, (void*)10253) ;                                  //找到等于指定ID的节点
    	}
	if(dlist_length(dlist_a) > n)
	{
		for(i = 0; i < n; i++)                                                   //删除若干数据
		{
			dlist_delete(dlist_b, 0) ;
			d=dlist_length(dlist_b) ;
			printf("%d",d);
        		printf("\n");
		}
	}
	if(dlist_length(dlist_a) > 10)                                                
	{
		dlist_set_by_index(dlist_a, 1, (void*)13071, (void*)c11, (void*)c1, (void*)11, (void*)3);   //在链表中指定位置插入数据
		dlist_set_by_index(dlist_a, 3, (void*)14091, (void*)c12, (void*)c2, (void*)13, (void*)3);
		dlist_set_by_index(dlist_a, 5, (void*)15671, (void*)c13, (void*)c3, (void*)15, (void*)3);
		dlist_set_by_index(dlist_a, 7, (void*)11431, (void*)c14, (void*)c4, (void*)17, (void*)3);
		dlist_set_by_index(dlist_a, 9, (void*)13251, (void*)c15, (void*)c5, (void*)19, (void*)3);
	}
	dlist_print(dlist_a);
	dlist_destroy(dlist_a);                                                          //销毁整个链表

	printf("===========Test the invial params=======\n");
	dlist_length(NULL);                                                               //测试异常输入
	dlist_delete(NULL, 0) ;
	dlist_find_ID(NULL, NULL) ;
	printf("==================Test end==============\n");

	return;
}

int main(int argc, char* argv[])
{

	multi_thread_work();	                                                            //运行于多线程
	single_thread_work();                                                            //切换单线程
                multi_thread_work();                                                             //切换多线程
	return 0;
}
