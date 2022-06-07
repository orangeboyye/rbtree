# This is a red black tree test program.

CFLAGS += -std=c99 -Wall -g -pg
LDFLAGS += -pg

APP := test
OBJS := test.o rbutils.o rbtree.o queue.o

all : $(APP)

run : $(APP)
	./$(APP)

$(APP) : $(OBJS)

rbtree.o  : rbtree.h
queue.o   : queue.h
rbutils.o : rbutils.h rbtree.h queue.h
test.o    : rbutils.h rbtree.h queue.h


.PHONY :
clean:
	-rm -f *.o *.exe test