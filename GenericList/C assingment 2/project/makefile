OBJS = main.o dlist.o locker_pthread.o
 
main: $(OBJS)
	gcc $(OBJS) -l pthread -o main  
main.o: main.c dlist.h
	gcc -c main.c -o main.o
dlist.o: dlist.c dlist.h locker.h
	gcc -c dlist.c -o dlist.o
locker_pthread.o: locker_pthread.c locker_pthread.h locker.h typedef.h
	gcc -c locker_pthread.c -o locker_pthread.o
clean:
	rm -rf *.o main
