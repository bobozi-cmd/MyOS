#include "task.h"
#include "sched.h"
#include "types.h"
#include "gdt.h"
#include "pmm.h"
#include "vargs.h"
#include "vmm.h"
#include "heap.h"
#include "string.h"
#include "debug.h"

pid_t now_pid = 0;

int32_t kernel_thread(int (*fn)(void *), void *arg) {
    struct task_struct *new_task = (struct task_struct *)kmalloc(STACK_SIZE);
    assert(new_task != NULL, "kern_thread: kmalloc error");

    // initialize stack bottom's struct to 0
    bzero(new_task, sizeof(struct task_struct));

    new_task->state = TASK_RUNNABLE;
    new_task->stack = current;
    new_task->pid = now_pid++;
    new_task->mm = NULL;

    uint32_t *stack_top = (uint32_t *)((uint32_t)new_task + STACK_SIZE);
    *(--stack_top) = (uint32_t)arg;
    *(--stack_top) = (uint32_t)kthread_exit;
    *(--stack_top) = (uint32_t)fn;

    new_task->context.esp = (uint32_t)new_task + STACK_SIZE - sizeof(uint32_t) * 3;

    // set new task's eflags not to close interrupt
    new_task->context.eflags = 0x200;
    new_task->next = running_proc_head;

    // insert new task to task tail
    struct task_struct *tail = running_proc_head;
    assert(tail != NULL, "Must init sched!");

    while (tail->next != running_proc_head) {
        tail = tail->next;
    }
    tail->next = new_task;

    return new_task->pid;
}

void kthread_exit() {
    // TODO: move exiting PCB to unsched list and wait for join then clean this PCB
    register uint32_t val asm ("eax");

    printk("Thread exited with value %d\n", val);

    while (1) ;
}
