#include <stdio.h>
#include <setjmp.h>

jmp_buf schrduler;

// co-routine
#define co_yield(cur)		{ if (!setjmp(cur)) longjmp(schrduler, 1); }
#define co_call(cur, nxt, func)	{ if (!setjmp(cur)) { *nxt = 0; func(); } if (*nxt != 0) longjmp(nxt, 1); }
#define co_end(cur)	{ *cur = 0; longjmp(schrduler, 1); }

// scheduler
#define sc_init(func) { if (!setjmp(schrduler)) func(); }
#define sc_task(nxt) { if (!setjmp(schrduler)) longjmp(nxt, 1); }

// try~catch
#define tc_try(cur) int __eno__; jmp_buf cur; if (0 == (__eno__ = setjmp(cur)))
#define tc_catch(err_no) else if(__eno__ == err_no)
#define tc_throw(cur, err_no) longjmp(cur, err_no)


jmp_buf j1, j2, j3;

int example_try_catch() {
	tc_try(jb) {
		printf("throw1");
		tc_throw(jb, 1);
	}
	tc_catch (1) {
		printf("catch1");
	}
	tc_catch (2) {
		printf("catch2");
	}
}

void co_func1() {
	printf("-*-\n");
	co_yield(j2);

	printf("-**-\n");
	co_yield(j2);

	printf("-***-\n");
	co_end(j2);
}

int co_task1() {

	for (;;) {
		printf("-0-");
		co_yield(j1);

		printf("-1-");
		co_yield(j1);

		printf("-2-");
		co_yield(j1);

		printf("-3-");
		co_yield(j1);
	}
}


int co_task2() {
	for (;;) {
		printf("-A-\n");
		co_yield(j2);

		printf("-B-\n");
		co_yield(j2);

		printf("-C-\n");
		co_call(j2, j2, co_func1);
	}
}


int main(void) {

	example_try_catch();

	sc_init(co_task1);
	sc_init(co_task2);

	while (true) {
		sc_task(j1);
		sc_task(j2);
	}
}
