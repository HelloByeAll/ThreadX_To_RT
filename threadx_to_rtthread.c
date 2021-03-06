/*
 * @Author: Hello
 * @Date: 2021-01-21 14:57:17
 * @LastEditors: Hello
 * @LastEditTime: 2021-01-21 16:59:50
 * @FilePath: \Threadx_to_rt-thread\port\threadx_to_rtthread.c
 */

#include "rtthread.h"
#include "threadx_to_rtthread.h"

#define DBG_TAG "tx_to_rt"
#define DBG_LVL DBG_ERROR
#include "rtdbg.h"

TX_THREAD* tx_thread_identify(void)
{
    return (TX_THREAD*)rt_thread_self();
}

UINT tx_thread_info_get(TX_THREAD* thread_ptr, CHAR** name, UINT* state, ULONG* run_count,
    UINT* priority, UINT* preemption_threshold, ULONG* time_slice,
    TX_THREAD** next_thread, TX_THREAD** next_suspended_thread)
{
    TX_INTERRUPT_SAVE_AREA

        /* Disable interrupts.  */
        TX_DISABLE

        /* Retrieve the name of the thread.  */
        if (name != TX_NULL)
        {

            *name = thread_ptr->name;
        }

    /* Pickup the thread's current state.  */
    if (state != TX_NULL)
    {

        *state = thread_ptr->stat;
    }

    /* Pickup the number of times the thread has been scheduled.  */
    if (run_count != TX_NULL)
    {

        *run_count = thread_ptr->init_tick - thread_ptr->remaining_tick;
    }

    /* Pickup the thread's priority.  */
    if (priority != TX_NULL)
    {

        *priority = thread_ptr->current_priority;
    }

    /* Pickup the thread's preemption-threshold.  */
    if (preemption_threshold != TX_NULL)
    {

        *preemption_threshold = thread_ptr->current_priority;
    }

    /* Pickup the thread's current time-slice.  */
    if (time_slice != TX_NULL)
    {

        *time_slice = thread_ptr->init_tick;
    }

    /* Pickup the next created thread.  */
    if (next_thread != TX_NULL)
    {

        *next_thread = TX_NULL;
    }

    /* Pickup the next thread suspended.  */
    if (next_suspended_thread != TX_NULL)
    {

        *next_suspended_thread = TX_NULL;
    }

    /* Restore interrupts.  */
    TX_RESTORE

        /* Return completion status.  */
        return (TX_SUCCESS);
}

UINT tx_thread_priority_change(TX_THREAD* thread_ptr, UINT new_priority, UINT* old_priority)
{
    *old_priority = thread_ptr->current_priority;
    LOG_I("tx_thread_priority_change  %s  %d->%d", thread_ptr->name, thread_ptr->current_priority, new_priority);
    rt_thread_control(thread_ptr, RT_THREAD_CTRL_CHANGE_PRIORITY, &new_priority);
    rt_thread_mdelay(1);
    return (TX_SUCCESS);
}

UINT tx_thread_sleep(ULONG ms)
{
    rt_thread_mdelay(ms);
    return (TX_SUCCESS);
}

UINT tx_event_flags_create(TX_EVENT_FLAGS_GROUP* group_ptr, CHAR* name_ptr)
{
    rt_event_init(group_ptr, name_ptr, RT_IPC_FLAG_PRIO);
    LOG_I("tx_event_flags_create %08X  name  %s", group_ptr, name_ptr);
    return (TX_SUCCESS);
}

UINT tx_event_flags_delete(TX_EVENT_FLAGS_GROUP* group_ptr)
{
    rt_event_detach(group_ptr);
    LOG_I("tx_event_flags_delete %08X  name  %s", group_ptr, group_ptr->parent.parent.name);
    return (TX_SUCCESS);
}

UINT tx_event_flags_get(TX_EVENT_FLAGS_GROUP* group_ptr, ULONG requested_flags,
    UINT get_option, ULONG* actual_flags_ptr, ULONG wait_option)
{
    LOG_I("start tx_event_flags_get %08X  name  %s  get_option 0x%02X  wait_option 0x%X", group_ptr, group_ptr->parent.parent.name, requested_flags, wait_option);
    rt_event_recv(group_ptr, requested_flags, get_option, wait_option, actual_flags_ptr);
    LOG_I("end   tx_event_flags_get %08X  name  %s  get_option 0x%02X  wait_option 0x%X", group_ptr, group_ptr->parent.parent.name, *actual_flags_ptr, wait_option);
    return (TX_SUCCESS);
}

UINT tx_event_flags_set(TX_EVENT_FLAGS_GROUP* group_ptr, ULONG flags_to_set,
    UINT set_option)
{
    if (set_option == TX_OR)
    {
        LOG_I("tx_event_flags_set TX_OR %08X  name  %s   flags_to_set 0x%02X", group_ptr, group_ptr->parent.parent.name, flags_to_set);
        rt_event_send(group_ptr, flags_to_set);
    }

    else
    {
        LOG_I("tx_event_flags_set TX_AND %08X  name  %s   flags_to_set 0x%02X", group_ptr, group_ptr->parent.parent.name, flags_to_set);
    }
    return (TX_SUCCESS);
}

UINT tx_mutex_create(TX_MUTEX* mutex_ptr, CHAR* name_ptr, UINT inherit)
{
    LOG_I("tx_mutex_create %08X  name  %s", mutex_ptr, name_ptr);
    rt_mutex_init(mutex_ptr, name_ptr, inherit);

    return (TX_SUCCESS);
}

UINT tx_mutex_delete(TX_MUTEX* mutex_ptr)
{
    LOG_I("tx_mutex_delete %08X  name  %s", mutex_ptr, mutex_ptr->parent.parent.name);
    rt_mutex_detach(mutex_ptr);
    return (TX_SUCCESS);
}

UINT tx_mutex_get(TX_MUTEX* mutex_ptr, ULONG wait_option)
{
    rt_err_t result;
    LOG_D("tx_mutex_get %08X  name  %s", mutex_ptr, mutex_ptr->parent.parent.name);

    if (TX_THREAD_GET_SYSTEM_STATE() != ((ULONG)0))
    {
        LOG_E("tx_mutex_get ISR");
        return (TX_CALLER_ERROR);
    }

    result = rt_mutex_take(mutex_ptr, wait_option);

    if (result == RT_EOK)
    {
        return (TX_SUCCESS);
    }
    else
    {
        return (TX_NO_INSTANCE);
    }
}

UINT tx_mutex_put(TX_MUTEX* mutex_ptr)
{
    LOG_D("tx_mutex_put %08X  name  %s", mutex_ptr, mutex_ptr->parent.parent.name);

    /* Check for interrupt call.  */
    if (TX_THREAD_GET_SYSTEM_STATE() != ((ULONG)0))
    {
        LOG_E("tx_mutex_put ISR");
        return (TX_CALLER_ERROR);
    }

    rt_mutex_release(mutex_ptr);
    return (TX_SUCCESS);
}

UINT tx_semaphore_create(TX_SEMAPHORE* semaphore_ptr, CHAR* name_ptr, ULONG initial_count)
{
    rt_sem_init(semaphore_ptr, name_ptr, initial_count, RT_IPC_FLAG_FIFO);
    LOG_W("tx_semaphore_create %08X  name  %s  initial_count %d", semaphore_ptr, name_ptr, initial_count);
    return (TX_SUCCESS);
}

UINT tx_semaphore_delete(TX_SEMAPHORE* semaphore_ptr)
{
    LOG_W("tx_semaphore_delete %08X  name  %s  type %d", semaphore_ptr, semaphore_ptr->parent.parent.name, semaphore_ptr->parent.parent.type);
    rt_sem_detach(semaphore_ptr);
    LOG_W("tx_semaphore_delete1 %08X  name  %s  type %d", semaphore_ptr, semaphore_ptr->parent.parent.name, semaphore_ptr->parent.parent.type);
    return (TX_SUCCESS);
}

UINT tx_semaphore_get(TX_SEMAPHORE* semaphore_ptr, ULONG wait_option)
{
    rt_err_t result;

    result = rt_sem_take(semaphore_ptr, wait_option);

    if (result == RT_EOK)
    {
        return (TX_SUCCESS);
    }
    else
    {
        return (TX_NO_INSTANCE);
    }
}

UINT tx_semaphore_put(TX_SEMAPHORE* semaphore_ptr)
{
    rt_sem_release(semaphore_ptr);
    return (TX_SUCCESS);
}

UINT tx_queue_create(TX_QUEUE* queue_ptr, CHAR* name_ptr, UINT message_size,
    VOID* queue_start, ULONG queue_size)
{
    rt_mq_init(queue_ptr, name_ptr, queue_start, message_size, queue_size, RT_IPC_FLAG_FIFO);
    return (TX_SUCCESS);
}

UINT tx_queue_receive(TX_QUEUE* queue_ptr, VOID* destination_ptr, ULONG wait_option)
{
    rt_err_t result;
    LOG_W("start tx_queue_receive %#X  name %s", queue_ptr, queue_ptr->parent.parent.name);
    result = rt_mq_recv(queue_ptr, destination_ptr, queue_ptr->msg_size, wait_option);

    if (result == RT_EOK)
    {
        return (TX_SUCCESS);
    }
    else
    {
        return (TX_NO_INSTANCE);
    }
}

UINT tx_queue_send(TX_QUEUE* queue_ptr, VOID* source_ptr, ULONG wait_option)
{
    LOG_W("rt_mq_send %#X  name %s", queue_ptr, queue_ptr->parent.parent.name);
    rt_mq_send(queue_ptr, source_ptr, queue_ptr->msg_size);
    return (TX_SUCCESS);
}

ULONG tx_time_get(VOID)
{
    return rt_tick_get();
}

UINT tx_timer_create(TX_TIMER* timer_ptr, CHAR* name_ptr,
    VOID(*expiration_function)(ULONG input), ULONG expiration_input,
    ULONG initial_ticks, ULONG reschedule_ticks, UINT auto_activate)
{
    if (reschedule_ticks == 0)
        rt_timer_init(timer_ptr, name_ptr, (void*)expiration_function, (void*)expiration_input,
            initial_ticks, RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
    else
        rt_timer_init(timer_ptr, name_ptr, (void*)expiration_function, (void*)expiration_input,
            initial_ticks, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);

    if (auto_activate == TX_AUTO_ACTIVATE)
        rt_timer_start(timer_ptr);

    LOG_I("tx_timer_create %08X  name  %s", timer_ptr, name_ptr);

    return (TX_SUCCESS);
}

UINT tx_timer_delete(TX_TIMER* timer_ptr)
{
    LOG_I("tx_timer_delete %08X  name  %s", timer_ptr, timer_ptr->parent.name);
    rt_timer_detach(timer_ptr);
    return (TX_SUCCESS);
}

UINT tx_thread_create(TX_THREAD* thread_ptr, CHAR* name_ptr,
    VOID(*entry_function)(ULONG entry_input), ULONG entry_input,
    VOID* stack_start, ULONG stack_size,
    UINT priority, UINT preempt_threshold,
    ULONG time_slice, UINT auto_start)
{
    rt_thread_init(thread_ptr, name_ptr, (void*)entry_function, (void*)entry_input,
        stack_start, stack_size, priority, time_slice);

    if (auto_start == TX_AUTO_START)
        rt_thread_startup(thread_ptr);

    LOG_I("tx_thread_create %08X  name  %s  priority %d ", thread_ptr, name_ptr, priority);
    return (TX_SUCCESS);
}

UINT tx_thread_delete(TX_THREAD* thread_ptr)
{
    LOG_I("tx_thread_delete %08X  name  %s", thread_ptr, thread_ptr->name);
    rt_thread_detach(thread_ptr);
    return (TX_SUCCESS);
}

UINT tx_thread_resume(TX_THREAD* thread_ptr)
{

    if (thread_ptr->stat == RT_THREAD_INIT)
        rt_thread_startup(thread_ptr);
    else
        rt_thread_resume(thread_ptr);
    return (TX_SUCCESS);
}

UINT tx_thread_suspend(TX_THREAD* thread_ptr)
{
    rt_thread_suspend(thread_ptr);
    return (TX_SUCCESS);
}

UINT tx_thread_terminate(TX_THREAD* thread_ptr)
{
    LOG_I("tx_thread_terminate %08X  name  %s", thread_ptr, thread_ptr->name);
    rt_thread_suspend(thread_ptr);
    return (TX_SUCCESS);
}

VOID tx_thread_relinquish(VOID)
{
    rt_thread_yield();
}

// UINT tx_interrupt_control(INT flag)
// {
//     if (flag <= 0)
//         return rt_hw_interrupt_disable();

//     rt_hw_interrupt_enable(flag);

//     return 0;
// }
