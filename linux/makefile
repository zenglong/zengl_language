#makefile for zengl language

CC = gcc
AR = ar

ARFLAGS = rc
CFLAGS = -g3 -ggdb -O0 -std=c99
CDEFINES = -D ZL_LANG_EN_WITH_CH
SHARED_CFLAGS = $(CFLAGS) -shared -fvisibility=hidden -fPIC
STATIC_CFLAGS = $(CFLAGS) -fvisibility=hidden -fPIC

ZENGL_LIB_CFLAGS_LIB = -lzengl

SRCS = zengl_main.c zengl_parser.c zengl_symbol.c zengl_locals.c zengl_assemble.c zengl_ld.c zenglrun_main.c zenglrun_func.c zenglApi.c zenglApi_BltModFuns.c
OBJS = zengl_main.o zengl_parser.o zengl_symbol.o zengl_locals.o zengl_assemble.o zengl_ld.o zenglrun_main.o zenglrun_func.o zenglApi.o zenglApi_BltModFuns.o 
RUNOBJS = main.o

zengl: $(RUNOBJS) encrypt.o libzengl.so
		$(CC) $(CFLAGS) -o zengl $(RUNOBJS) $(ZENGL_LIB_CFLAGS_LIB)
		$(CC) $(CFLAGS) -o encrypt encrypt.o $(ZENGL_LIB_CFLAGS_LIB)

st_zengl: $(RUNOBJS) encrypt.o libzengl.a
		$(CC) $(CFLAGS) -o zengl $(RUNOBJS) libzengl.a
		$(CC) $(CFLAGS) -o encrypt encrypt.o libzengl.a

libzengl.a: $(OBJS)
		$(AR) $(ARFLAGS) libzengl.a $(OBJS)

$(OBJS): $(SRCS)  zengl_global.h zengl_locals.h zengl_exportfuns.h
		$(CC) $(CDEFINES) $(STATIC_CFLAGS) -c $(SRCS)

libzengl.so : $(SRCS) zengl_global.h zengl_locals.h zengl_exportfuns.h
		$(CC) $(CDEFINES) $(SHARED_CFLAGS) -o libzengl.so $(SRCS)
		cp libzengl.so /usr/lib
		
main.o: main.c zengl_exportfuns.h
		$(CC) $(CFLAGS) -c main.c

encrypt.o: encrypt.c zengl_exportfuns.h
		$(CC) $(CFLAGS) -c encrypt.c

clean:
		rm -fv libzengl.so
		rm -fv zengl
		rm -fv encrypt
		rm -fv main.o
		rm -fv encrypt.o
		rm -fv libzengl.a
		rm -fv *.o

all: zengl libzengl.so
static: st_zengl
