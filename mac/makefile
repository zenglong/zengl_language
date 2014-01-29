#makefile for zengl language

CC = gcc

CFLAGS = -g3 -ggdb -O0 -std=c99
CDEFINES = -D ZL_LANG_EN_WITH_CH
SHARED_CFLAGS = $(CFLAGS) -dynamiclib -install_name '$(CURDIR)/libzengl.dylib' -current_version 1.0 -fvisibility=hidden  -fPIC

ZENGL_LIB_CFLAGS_LIB = -L '$(CURDIR)' -lzengl

OBJS = ../linux/zengl_main.c ../linux/zengl_parser.c ../linux/zengl_symbol.c ../linux/zengl_locals.c ../linux/zengl_assemble.c ../linux/zengl_ld.c ../linux/zenglrun_main.c ../linux/zenglrun_func.c ../linux/zenglApi.c ../linux/zenglApi_BltModFuns.c ../linux/zenglDebug.c
RUNOBJS = main.o

zengl: $(RUNOBJS) encrypt.o libzengl.dylib
		$(CC) $(CFLAGS) -o zengl $(RUNOBJS) $(ZENGL_LIB_CFLAGS_LIB)
		$(CC) $(CFLAGS) -o encrypt encrypt.o $(ZENGL_LIB_CFLAGS_LIB)

libzengl.dylib : $(OBJS) ../linux/zengl_global.h ../linux/zengl_locals.h ../linux/zengl_exportfuns.h
		$(CC) $(CDEFINES) $(SHARED_CFLAGS) -o libzengl.dylib $(OBJS)
		
main.o: ../linux/main.c ../linux/zengl_exportfuns.h
		$(CC) $(CFLAGS) -c ../linux/main.c

encrypt.o: ../linux/encrypt.c ../linux/zengl_exportfuns.h
		$(CC) $(CFLAGS) -c ../linux/encrypt.c

clean:
		rm -fv libzengl.dylib
		rm -fv zengl
		rm -fv encrypt
		rm -fv main.o
		rm -fv encrypt.o

all: zengl libzengl.dylib
