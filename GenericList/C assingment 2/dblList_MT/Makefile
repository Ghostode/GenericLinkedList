DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c)  
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC})) 

TARGET = test_mt

BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc
CFLAGS = -pthread -g -Wall -I${DIR_INC} -fprofile-arcs -ftest-coverage 
LFLAGS = -pthread -l cmocka -L /user/local/lib -fprofile-arcs -ftest-coverage 

${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) -o $@ $(LFLAGS)
    
${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY:get
get:
	gcov ${DIR_OBJ}/*.c

.PHONY:html
html:
	lcov -c -d . -o ${TARGET}.info 
	genhtml -o result ${TARGET}.info

.PHONY:clean
clean:
	find ${DIR_OBJ} -name *.o -exec rm -rf {} \;
	find ${DIR_OBJ} -name *.gcno -exec rm -rf {} \;
	find ${DIR_OBJ} -name *.gcda -exec rm -rf {} \;
	rm -rf *.gcov *.info
	rm -rf ${DIR_BIN}/${TARGET}


