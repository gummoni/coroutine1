#include <stdio.h>
#include "scheduler.h"


//prototype--------------------------------------------------
static void f1(void);
static void f2(void);
static void f3(void);

static task tasks1[] = {
	task_create(f1),
	task_create(f2),
};

static task task3 = task_create(f3);

static scheduler sc1;
// ----------------------------------------------------------

static void f1(void) {
	for (;;) {
		printf("-f1:1-");
		task_yield;

		printf("-f1:2-");
		task_yield;

		printf("-f1:3-");
		task_yield;

		printf("-f1:4-");
		task_yield;
	}
}

static void f2(void) {
	for (;;) {
		printf("-f2:1-\n");
		task_yield;

		printf("-f2:2-\n");
		task_call(&task3);

		printf("-f2:33333333333333-\n");
		task_yield;
	}
}

static void f3(void) {

	printf("f3:1\n");
	task_yield;

	printf("f3:2\n");
	task_return;
}


// try~catch
void try_catch_example(int i) {
	try {
		throw(i);
	}
	catch(1) {
		printf("throw:1\n");
	}
	catch(2) {
		printf("throw:2\n");
	}
	catch_other {
		printf("THROW:%d\n", i);
	}
}



int main(void) {
	try_catch_example(1);
	try_catch_example(2);
	try_catch_example(3);
	try_catch_example(4);

	scheduler_init(&sc1, tasks1, sizeof(tasks1) / sizeof(task));
	schrduler_start(&sc1);
}
