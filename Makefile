# This is a red black tree test program.

CFLAGS += -std=c99 -Wall -g -pg
LDFLAGS += -pg

APP := test
OBJS := test.o rbtree.o queue.o

all : $(APP)
	./$(APP)

$(APP) : $(OBJS)

$(OBJS) : rbtree.h

rbtree.o queue.o: queue.h

.PHONY :
clean:
	-rm -f *.o *.exe