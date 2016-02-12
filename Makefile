CC=$(CROSS_COMPILE)gcc -EL
CLIENT_CC=gcc

SRCDIR=src
HEADER=inc

CLIENT=pinclient
SERVER=nepico_srv
LIB=nepico

LIBFLAGS= -fPIC -shared -Wl,-soname,libnepico.so

LDFLAGS = -lnepico

SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=%.o)

CFLAGS += -I $(HEADER) -L . -g -Wall   

#confparser.c  gpiocontrol.c  main.c  pinclient.c  pinmesg.c  pinserver.c

LIB_SRC := $(SRCDIR)/gpiocontrol.c \
$(SRCDIR)/confparser.c \
$(SRCDIR)/pinmesg.c 
LIB_OBJ := gpiocontrol.o confparser.o pinmesg.o 

CL_SRC = pinclient.c 
SR_SRC= pinserver.c 
CL_OBJS= pinclient.o 
SR_OBJS= pinserver.o 


all : $(LIB) $(CLIENT) $(SERVER)

#$(OBJECTS): %.o : $(SRCDIR)/%.c
#	$(CC) $(CFLAGS) -c $< -o $@

#$(LIB_OBJ) : $(LIB_SRC)
#	$(CC) $(CFLAGS) -c $< -o $@

$(LIB) : $(LIB_SRC)
	$(CC) $(CFLAGS) $(LIBFLAGS) $^ -o lib$@.so
	#mv mconn libmconn.so
	ln -s libnepico.so libnepico.so.1


$(CL_OBJS) : $(SRCDIR)/$(CL_SRC)
	$(CLIENT_CC) $(CFLAGS) -c $< -o $@

$(SR_OBJS) : $(SRCDIR)/$(SR_SRC)
	$(CC) $(CFLAGS) -c $< -o $@



$(CLIENT): $(CL_OBJS) 
	$(CLIENT_CC) $(CFLAGS)  $^ -o $@ 
		
$(SERVER): $(SR_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean: 
	rm -rfv *.o
	rm -rfv $(CLIENT) $(SERVER)
	rm -rfv libnepico.*

