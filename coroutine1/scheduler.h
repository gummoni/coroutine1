#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include <setjmp.h>

//タスク構造体
typedef struct task {
	jmp_buf buf;
	void (*exe)(void);
	struct task* child;
} task;

//スケジューラ構造体
typedef struct scheduler {
	jmp_buf buf;
	task* current;
	task* tasks;
	int length;
} scheduler;

extern scheduler* __sc__;


//try~catch関連
#define try int __eno__; jmp_buf cur; if (0 == (__eno__ = setjmp(cur)))
#define catch(err_no) else if(__eno__ == err_no)
#define catch_other  else 
#define throw(err_no) longjmp(cur, err_no)

//タスク関連
#define task_create(func)	{ { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, func }
#define task_yield			{ if (!setjmp(__sc__->current->buf)) longjmp(__sc__->buf, 1); }
#define task_return			{ *__sc__->current->buf = 0; longjmp(__sc__->buf, 1); }
#define task_call(exe_task) { if (!setjmp(__sc__->current->buf)) { __sc__->current->child = exe_task; __sc__->current = __sc__->current->child; __sc__->current->exe(); } __sc__->current->child = NULL; }

extern void scheduler_init(scheduler* self, task* tasks, int legnth);
extern void schrduler_start(scheduler* self);


#endif//__SCHEDULER_H__
