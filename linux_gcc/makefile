#makefile for zengl language

CC = gcc

CFLAGS = -g3 -ggdb -O0
CDEFINES = -D ZL_LANG_EN_WITH_CH
SHARED_CFLAGS = $(CFLAGS) -shared -fvisibility=hidden

ZENGL_LIB_CFLAGS_LIB = -lzengl

OBJS = zengl_main.c zengl_parser.c zengl_symbol.c zengl_locals.c zengl_assemble.c zengl_ld.c zenglrun_main.c zenglrun_func.c zenglApi.c
RUNOBJS = main.o

zengl: $(RUNOBJS) libzengl.so
		$(CC) $(CFLAGS) $(ZENGL_LIB_CFLAGS_LIB) -o zengl $(RUNOBJS)

libzengl.so : $(OBJS) zengl_global.h zengl_locals.h zengl_exportfuns.h zengl_exportPublicDefs.h
		$(CC) $(CDEFINES) $(SHARED_CFLAGS) -o libzengl.so $(OBJS)
		cp libzengl.so /usr/lib
		
main.o: main.c zengl_exportfuns.h zengl_exportPublicDefs.h
		$(CC) $(CFLAGS) -c main.c

clean:
		rm -v libzengl.so
		rm -v zengl
		rm -v main.o

all: zengl libzengl.so
