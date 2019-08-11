#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "linkedlist.h"
#define MAXNAME 41

typedef struct employee{
    unsigned int number;
    char name[MAXNAME];
    char ip[16];
    char seat[10];
    char group[MAXNAME];
}Stuff;

static void print_stuff(void* data){
    int * tmp = data;
    printf("%-10d  ",((Stuff*)data)->number);
    printf("%-20s  ",((Stuff*)data)->name);
    printf("%-20s  ",((Stuff*)data)->ip);
    printf("%-10s  ",((Stuff*)data)->seat);
    printf("%-10s\n",((Stuff*)data)->group);

}

static int equal_number(void* data1, void* data2){
    if(((Stuff*)data1)->number == ((Stuff*)data2)->number)
        return 1;
    else return 0;
}
static int less_number(void* data1, void* data2){             //工号升序
    if(((Stuff*)data1)->number < ((Stuff*)data2)->number)
        return 1;
    else return 0;
}
static int larger_number(void* data1, void* data2){            //工号降序
    if(((Stuff*)data1)->number > ((Stuff*)data2)->number)
        return 1;
    else return 0;
}
static int less_ip(void* data1, void* data2){                       //IP升序
    struct in_addr ip1;
    struct in_addr ip2;
    int result = 0;
    inet_pton(AF_INET, ((Stuff*)data1)->ip, &ip1);
    inet_pton(AF_INET, ((Stuff*)data2)->ip, &ip2);

    result = memcmp(&ip1, &ip2, sizeof(struct in_addr));
    if(result < 0) return 1;
    else return 0;

}



int main(void){


    Stuff soft[40];
    Stuff hard[50];
    int i;

    //创建40个软件组成员
    for(i=0; i<40; i++){
        soft[i].number = 999+2*(i+1);
        strcpy(soft[i].name, "zhaohaodong");
        sprintf(soft[i].ip, "123.123.123.%d", 2*i);
        sprintf(soft[i].seat, "542-%d",i+1);
        strcpy(soft[i].group,"software");
    }

    //创建50个硬件组成员
    for(i=0; i<50; i++){
        hard[i].number = 1000+2*i;
        strcpy(hard[i].name, "zhaohaodong");
        sprintf(hard[i].ip, "123.123.123.%d", 2*i+1);
        sprintf(hard[i].seat, "543-%d",i+1);
        strcpy(hard[i].group,"hardware");
    }

    Link * softlist = list_create();
    Link * hardlist = list_create();

    //将成员放入链表
    if(softlist != NULL){
        for(i=0; i<40; i++)
            list_insert(softlist, &soft[i], 1);
    }

    if(hardlist != NULL){
        for(i=0; i<50; i++)
            list_insert(hardlist, &hard[i], 1);
    }

    printf("software group list:\n");
    list_print(softlist, print_stuff);
    printf("\nhardware group list:\n");
    list_print(hardlist, print_stuff);

    list_sort(softlist, less_number);   //工号升序排序
    list_sort(hardlist, less_number);
    printf("\nAfter sorting by ascending numbers:\n");
    printf("\nsoftware group list:\n");
    list_print(softlist, print_stuff);
    printf("\nhardware group list:\n");
    list_print(hardlist, print_stuff);

    Link* mergedlist = list_merge(softlist, hardlist); //合并两个链表
    printf("\nMerge two lists :\n");
    list_print(mergedlist, print_stuff);

    list_sort(mergedlist, larger_number);      //工号降序排序
    printf("\nAfter sorting by descending numbers:\n");
    list_print(softlist, print_stuff);

    list_sort(mergedlist, less_ip);             //ip升序排序
    printf("\nAfter sorting by ascending IP address:\n");
    list_print(mergedlist, print_stuff);

    list_destroy(mergedlist);  //销毁链表
    
    return 0;
}
