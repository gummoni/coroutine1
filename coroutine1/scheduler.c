#include "scheduler.h"

//current schrduler context
scheduler* __sc__;


/**
* scheduler initialize
*/
void scheduler_init(scheduler* self, task* tasks, int length) {
	__sc__ = self;
	self->tasks = tasks;
	self->length = length;

	for (int index = 0; index < self->length; index++) {
		self->current = &self->tasks[index];
		if (!setjmp(self->buf)) {
			self->current->exe();
		}
	}
}

/**
* scheduler dispatch
*/
void schrduler_start(scheduler* self) {
	__sc__ = self;

	for (;;) {
		for (int index = 0; index < self->length; index++) {
			self->current = &self->tasks[index];

			while (NULL != self->current->child) {
				if (0 == *self->current->child->buf) {
					*self->current->child->buf = 0;
					break;
				}
				self->current = self->current->child;
			}

			if (!setjmp(self->buf)) {
				longjmp(self->current->buf, 1);
			}
		}
	}
}
