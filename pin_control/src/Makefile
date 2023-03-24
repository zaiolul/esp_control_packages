BIN:=pin_control
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)

LIB:= -lubus -lubox -lblobmsg_json -lserialport
#LDFLAGS:=-L../sdk/build/lib $(LIB)
INC:= -I./includes
 #-I../sdk/utils -I../sdk/include -I../sdk/interface -I../sdk/libraries/coreJSON/source/include 
CC:=gcc
CFLAGS = -Wall

.PHONY: all clean

all:$(BIN)
	
$(BIN): $(SRCS)
	$(CC) -o $@ $^ $(INC) $(LIB)

clean:
	rm $(BIN) 