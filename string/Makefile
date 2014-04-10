WORKDIR = ../..
BINDIR = $(WORKDIR)/bin
SRCDIR = $(WORKDIR)/src
LIBDIR = $(WORKDIR)/lib

MAKE = make
CC = gcc
CFLAGS = -Wall -pipe -g -O0
LIBS = 
LDFLAGS = -lpthread
INCLUDE = -I$(SRCDIR)/include -I.
AR = ar
ARFLAGS = cr
RM = -rm -f
TARGETS = $(LIBDIR)/libutil.so
OBJS = av_string.o
SUBDIRS = 

all : subdirs $(TARGETS)

subdirs : 
	@for dir in $(SUBDIRS); \
        do $(MAKE) -C $$dir || exit 1; \
        done

$(TARGETS) : $(LIBS) $(OBJS)
	#$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@ $(LDFLAGS)
	#$(AR) $(ARFLAGS) $@ $^

%.o : %.cpp
	#$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
	$(CC) $(CFLAGS) -fPIC -shared -c $(INCLUDE) $< -o $@
	
%.o : %.c
	#$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
	$(CC) $(CFLAGS) -fPIC -shared -c $(INCLUDE) $< -o $@

clean:
	@for dir in $(SUBDIRS); \
        do $(MAKE) -C $$dir clean || exit 1; \
        done
	$(RM) $(OBJS) $(TARGETS)

.PHONY: all subdirs clean

