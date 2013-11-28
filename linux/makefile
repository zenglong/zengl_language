#makefile for zengl language

CC = gcc

CFLAGS = -g3 -ggdb -O0 -std=c99
CDEFINES = -D ZL_LANG_EN_WITH_CH
SHARED_CFLAGS = $(CFLAGS) -shared -fvisibility=hidden -fPIC

ZENGL_LIB_CFLAGS_LIB = -lzengl

OBJS = zengl_main.c zengl_parser.c zengl_symbol.c zengl_locals.c zengl_assemble.c zengl_ld.c zenglrun_main.c zenglrun_func.c zenglApi.c zenglApi_BltModFuns.c
RUNOBJS = main.o

zengl: $(RUNOBJS) encrypt.o libzengl.so
		$(CC) $(CFLAGS) -o zengl $(RUNOBJS) $(ZENGL_LIB_CFLAGS_LIB)
		$(CC) $(CFLAGS) -o encrypt encrypt.o $(ZENGL_LIB_CFLAGS_LIB)

libzengl.so : $(OBJS) zengl_global.h zengl_locals.h zengl_exportfuns.h zengl_exportPublicDefs.h
		$(CC) $(CDEFINES) $(SHARED_CFLAGS) -o libzengl.so $(OBJS)
		cp libzengl.so /usr/lib
		
main.o: main.c zengl_exportfuns.h zengl_exportPublicDefs.h
		$(CC) $(CFLAGS) -c main.c

encrypt.o: encrypt.c zengl_exportfuns.h zengl_exportPublicDefs.h
		$(CC) $(CFLAGS) -c encrypt.c

clean:
		rm -fv libzengl.so
		rm -fv zengl
		rm -fv encrypt
		rm -fv main.o
		rm -fv encrypt.o

all: zengl libzengl.so
