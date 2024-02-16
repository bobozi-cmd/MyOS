#include "sched.h"
#include "pmm.h"
#include "task.h"
#include "types.h"
#include "heap.h"
#include "debug.h"

struct task_struct *running_proc_head = NULL;

struct task_struct *wait_proc_head = NULL;

struct task_struct *current = NULL;

void init_sched() {
    // create task struct at the bottom of stack
    current = (struct task_struct *)(kern_stack_top - STACK_SIZE);

    current->state = TASK_RUNNABLE;
    current->pid = now_pid++;
    current->stack = current;
    current->mm = NULL;

    // Unidirectional cyclic linked list
    current->next = current;

    running_proc_head = current;
}

void schedule() {
    if (current) {
        change_task_to(current->next);
    }
}

void change_task_to(struct task_struct *next) {
    if (current != next) {
        struct task_struct * prev = current;
        current = next;
        switch_to(&(prev->context), &(next->context));
    }
}