#ifndef _SETJMP_H_
#define _SETJMP_H_

// longjmp.s
void longjmp(jmp_buf env, int val);
// setjmp.s
int setjmp(jmp_buf envbuf);

#endif // _SETJMP_H_

