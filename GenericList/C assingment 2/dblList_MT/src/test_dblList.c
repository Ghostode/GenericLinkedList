#define UNIT_TESTING
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "dblList.h"
#include "producer_consumer.h"

static int testNum[90] = {8366,1879,2375,6730,2549,5158,3257,1373,
                    4048,1460,5074,2510,1259,714,9678,61,6013,
                    3452,2969,6911,8992,4079,3593,7834,2835,
                    5049,3481,4334,1364,8485,7525,6082,6715,
                    6252,2811,9263,1409,2419,6987,5457,230,2061,
                    4318,7840,2774,3995,4252,5138,3798,7221,
                    2048,2789,1299,1993,623,485,7041,4103,4818,
                    4757,8939,8695,838,5653,4946,10000,1267,2706,
                    2418,8253,1956,8940,8797,194,5060,4872,648,4844,
                    6726,1376,5620,3918,1470,7628,1469,1480,6687,
                    4804,6954,1516};

static void test_dblListInit(void **state)
{
    dblListInit();
}

static void test_dblListAppend(void **state)
{
    dblList list = dblListInit();
    int ele = 1;
    assert_true(dblListAppend(list,dblListGetFirst(list),&ele)==TRUE);
}

static void delFuncT(void *pElement)
{

}

static void test_dblListDelNode(void **state)
{
    dblList list = dblListInit();
    int ele = 1;
    DEL_FUNC delFunc = delFuncT;
    dblListAppend(list,dblListGetFirst(list),&ele);
    dblListDelNode(list,dblListGetFirst(list),delFunc);
    assert_true((dblListIsEmpty(list)==TRUE));
}

static void test_delList(void **state)
{
    dblList list = dblListInit();
    DEL_FUNC delFunc = delFuncT;
    int ele = 1;
    dblListAppend(list,dblListGetFirst(list),&ele);
    assert_true(dblListDelList(list, delFunc)==TRUE);
}

static void test_get(void **state)
{
    dblList list = dblListInit();
    int i = 0;
    for(i=0; i<90; i++)
    {
        dblListAppend(list,dblListGetLast(list),&testNum[i]);
    }
    int firstNum = *(int*)dblListGetData(list,dblListGetFirst(list));
    int lastNum = *(int*)dblListGetData(list,dblListGetLast(list));
    int secNum = *(int*)dblListGetData(list,dblListGetNext(list,dblListGetFirst(list)));
    int lastSecNum = *(int*)dblListGetData(list,dblListGetPrv(list,dblListGetLast(list)));
    assert_true(firstNum == 8366);
    assert_true(1516 == lastNum);
    assert_true(1879 == secNum);
    assert_true(6954 == lastSecNum);
    assert_true(90 == dblListGetLen(list));
    
    DEL_FUNC delFunc = delFuncT;
    dblListDelList(list, delFunc);
}

static bool matchNumT(void *pElement, void *find)
{
    if(*(int*)pElement == *(int*)find)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }    
}

static void test_dblListFind(void **state)
{
    dblList list = dblListInit();
    int i = 0;
    MATCH_FUNC matchFunc= matchNumT;
    int findNum1 = 1409;
    int findNum2 = 0;
    position findPos1 = NULL;
    position findPos2 = NULL;
    findPos1 = dblListFind(list, dblListGetFirst(list), matchFunc, &findNum1);
    findPos2 = dblListFind(list, dblListGetFirst(list), matchFunc, &findNum2);

    for(i=0; i<90; i++)
    {
        dblListAppend(list,dblListGetLast(list),&testNum[i]);
    }
    
    findPos1 = dblListFind(list, dblListGetFirst(list), matchFunc, &findNum1);
    findPos2 = dblListFind(list, dblListGetFirst(list), matchFunc, &findNum2);
    assert_true(*(int*)dblListGetData(list,findPos1) == 1409);
    assert_true(findPos2 == NULL);
   
    DEL_FUNC delFunc = delFuncT;
    dblListDelList(list, delFunc);
}

static void printFuncT(void *pElement)
{
    int tmp = *(int*)pElement;
    printf("%d\n", tmp);
}

static void forEachFuncT(void *pElement)
{
    *(int*)pElement = 1;
}
static void forEachFuncT2(void *pElement)
{
    *(int*)pElement = 2;
}

static void test_dblListForEach(void **state)
{
    dblList list = dblListInit();
    int i = 0;
    int num[10] = {1,2,3,4,5,6,7,8,9,10};
    for(i=0; i<10; i++)
    {
        dblListAppend(list,dblListGetLast(list),&num[i]);
    }
    
    FUNC printFunc= printFuncT;
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);
    dblListForEach(list, printFunc, REVERSE_DIRCTION, READ_MODE);
    FUNC func = forEachFuncT;
    dblListForEach(list, func, FOWARD_DIRCTION, WRITE_MODE);
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);
    func = forEachFuncT2;
    dblListForEach(list, func, REVERSE_DIRCTION, WRITE_MODE);
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);
    DEL_FUNC delFunc = delFuncT;
    dblListDelList(list, delFunc);
}

static void test_dblListSwap(void **state)
{
    dblList list = dblListInit();
    int i = 0;
    for(i=0; i<90; i++)
    {
        dblListAppend(list,dblListGetLast(list), &testNum[i]);
    }
    
    position pos1 = dblListGetFirst(list);
    position pos2 = dblListGetLast(list);
    int tmp1 = *(int*)dblListGetData(list, pos1);
    int tmp2 = *(int*)dblListGetData(list, pos2);
    dblListSwap(list, pos1, pos2);
    assert_true(*(int*)dblListGetData(list, pos1) == tmp2);
    assert_true(*(int*)dblListGetData(list, pos2) == tmp1);

    DEL_FUNC delFunc = delFuncT;
    dblListDelList(list, delFunc);
}


int cmpNumT(void *pElement1, void *pElement2)
{
    int tmp1 = *(int*)pElement1;
    int tmp2 = *(int*)pElement2;
    if(tmp1 > tmp2)
    {
        return 1;
    }
    else if (tmp1 == tmp2)
    {
        return 0;
    }
    else
    {
        return -1;
    } 
}

static void test_dblListSort(void **state)
{
    dblList list = dblListInit();
    COMPARE_FUNC cmpFunc = cmpNumT;
    FUNC printFunc= printFuncT;
    int i = 0;

    dblListSort(list, cmpFunc);

    for(i=0; i<90; i++)
    {
        dblListAppend(list, dblListGetLast(list), &testNum[i]);
    }
    
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);
    dblListSort(list, cmpFunc);
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);
    

    DEL_FUNC delFunc = delFuncT;
    dblListDelList(list, delFunc);
}

static void modifyFuncT(void *pElement, void *mod)
{
    *(int*)pElement = *(int*)mod;
}

static void test_dblListModify(void **state)
{
    dblList list = dblListInit();
    int i = 0;
    int num = 1;
    for(i=0; i<90; i++)
    {
        dblListAppend(list,dblListGetLast(list), &testNum[i]);
    }
    MODIFY_FUNC modifyFunc = modifyFuncT;
    assert_true(dblListModify(list, dblListGetFirst(list), modifyFunc, &num)==TRUE);
    assert_true(*(int*)dblListGetData(list, dblListGetFirst(list))==num);
}

static void test_prdcrCnsmr(void **state)
{
    dblList list = dblListInit();
    printf("多线程：\n");
    producer_consumer_MT(list);
    FUNC printFunc= printFuncT;
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);
   
    printf("单线程：\n");
    producer_consumer_ST(list);
    dblListForEach(list, printFunc, FOWARD_DIRCTION, READ_MODE);

    DEL_FUNC delFunc = delFuncT;
    dblListDelList(list, delFunc);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dblListInit),
        cmocka_unit_test(test_dblListAppend),
        cmocka_unit_test(test_dblListDelNode),
        cmocka_unit_test(test_delList),
        cmocka_unit_test(test_get),
        cmocka_unit_test(test_dblListFind),
        cmocka_unit_test(test_dblListForEach),
        cmocka_unit_test(test_dblListSwap),
        cmocka_unit_test(test_dblListSort),
        cmocka_unit_test(test_dblListModify),
        cmocka_unit_test(test_prdcrCnsmr)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
