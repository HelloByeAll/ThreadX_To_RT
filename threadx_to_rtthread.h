/*
 * @Author: Hello
 * @Date: 2021-01-21 13:33:55
 * @LastEditors: Hello
 * @LastEditTime: 2021-01-21 15:58:58
 * @FilePath: \Threadx_to_rt-thread\port\threadx_to_rtthread.h
 */
#include <rtthread.h>
#include <rthw.h>

#define VOID     void
typedef char     CHAR;
typedef unsigned char UCHAR;
typedef int      INT;
typedef unsigned int  UINT;
typedef long     LONG;
typedef unsigned long ULONG;
typedef short    SHORT;
typedef unsigned short USHORT;


#define TX_INTERRUPT_SAVE_AREA rt_base_t interrupt_save;
#define TX_DISABLE             interrupt_save = rt_hw_interrupt_disable();
#define TX_RESTORE             rt_hw_interrupt_enable(interrupt_save);

/* API return values.  */

#define TX_SUCCESS                      ((UINT) 0x00)
#define TX_DELETED                      ((UINT) 0x01)
#define TX_POOL_ERROR                   ((UINT) 0x02)
#define TX_PTR_ERROR                    ((UINT) 0x03)
#define TX_WAIT_ERROR                   ((UINT) 0x04)
#define TX_SIZE_ERROR                   ((UINT) 0x05)
#define TX_GROUP_ERROR                  ((UINT) 0x06)
#define TX_NO_EVENTS                    ((UINT) 0x07)
#define TX_OPTION_ERROR                 ((UINT) 0x08)
#define TX_QUEUE_ERROR                  ((UINT) 0x09)
#define TX_QUEUE_EMPTY                  ((UINT) 0x0A)
#define TX_QUEUE_FULL                   ((UINT) 0x0B)
#define TX_SEMAPHORE_ERROR              ((UINT) 0x0C)
#define TX_NO_INSTANCE                  ((UINT) 0x0D)
#define TX_THREAD_ERROR                 ((UINT) 0x0E)
#define TX_PRIORITY_ERROR               ((UINT) 0x0F)
#define TX_NO_MEMORY                    ((UINT) 0x10)
#define TX_START_ERROR                  ((UINT) 0x10)
#define TX_DELETE_ERROR                 ((UINT) 0x11)
#define TX_RESUME_ERROR                 ((UINT) 0x12)
#define TX_CALLER_ERROR                 ((UINT) 0x13)
#define TX_SUSPEND_ERROR                ((UINT) 0x14)
#define TX_TIMER_ERROR                  ((UINT) 0x15)
#define TX_TICK_ERROR                   ((UINT) 0x16)
#define TX_ACTIVATE_ERROR               ((UINT) 0x17)
#define TX_THRESH_ERROR                 ((UINT) 0x18)
#define TX_SUSPEND_LIFTED               ((UINT) 0x19)
#define TX_WAIT_ABORTED                 ((UINT) 0x1A)
#define TX_WAIT_ABORT_ERROR             ((UINT) 0x1B)
#define TX_MUTEX_ERROR                  ((UINT) 0x1C)
#define TX_NOT_AVAILABLE                ((UINT) 0x1D)
#define TX_NOT_OWNED                    ((UINT) 0x1E)
#define TX_INHERIT_ERROR                ((UINT) 0x1F)
#define TX_NOT_DONE                     ((UINT) 0x20)
#define TX_CEILING_EXCEEDED             ((UINT) 0x21)
#define TX_INVALID_CEILING              ((UINT) 0x22)
#define TX_FEATURE_NOT_ENABLED          ((UINT) 0xFF)

#define TX_NULL                    ((void                                    *)0)
#define TX_EMPTY                   ((ULONG)0)
#define TX_TIMER                   struct rt_timer
#define TX_THREAD                  struct rt_thread
#define TX_AUTO_START              ((UINT)1)
#define TX_DONT_START              ((UINT)0)
#define TX_AUTO_ACTIVATE           ((UINT)1)
#define TX_NO_ACTIVATE             ((UINT)0)
#define TX_THREAD_GET_SYSTEM_STATE rt_interrupt_get_nest

#define TX_NO_WAIT                 ((ULONG)0)
#define TX_WAIT_FOREVER            ((ULONG)0xFFFFFFFFUL)

#define TX_SEMAPHORE               struct rt_semaphore
#define TX_MUTEX                   struct rt_mutex
#define TX_EVENT_FLAGS_GROUP       struct rt_event
#define TX_AND                     RT_EVENT_FLAG_AND
#define TX_AND_CLEAR               (RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR)
#define TX_OR                      RT_EVENT_FLAG_OR
#define TX_OR_CLEAR                (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR)
#define TX_QUEUE                   struct rt_messagequeue

#define TX_NO_INHERIT              ((UINT)0)


#ifndef ALIGN_TYPE_DEFINED
#define ALIGN_TYPE ULONG
#endif

TX_THREAD* tx_thread_identify(void);
UINT _tx_thread_info_get(TX_THREAD* thread_ptr, CHAR** name, UINT* state, ULONG* run_count,
    UINT* priority, UINT* preemption_threshold, ULONG* time_slice,
    TX_THREAD** next_thread, TX_THREAD** next_suspended_thread);

UINT tx_thread_priority_change(TX_THREAD* thread_ptr, UINT new_priority, UINT* old_priority);
UINT tx_thread_sleep(ULONG ms);
UINT tx_event_flags_create(TX_EVENT_FLAGS_GROUP* group_ptr, CHAR* name_ptr);
UINT tx_event_flags_delete(TX_EVENT_FLAGS_GROUP* group_ptr);
UINT tx_event_flags_get(TX_EVENT_FLAGS_GROUP* group_ptr, ULONG requested_flags,
    UINT get_option, ULONG* actual_flags_ptr, ULONG wait_option);

UINT tx_event_flags_set(TX_EVENT_FLAGS_GROUP* group_ptr, ULONG flags_to_set,
    UINT set_option);

UINT tx_mutex_create(TX_MUTEX* mutex_ptr, CHAR* name_ptr, UINT inherit);
UINT tx_mutex_delete(TX_MUTEX* mutex_ptr);
UINT tx_mutex_get(TX_MUTEX* mutex_ptr, ULONG wait_option);
UINT tx_mutex_put(TX_MUTEX* mutex_ptr);
UINT tx_semaphore_create(TX_SEMAPHORE* semaphore_ptr, CHAR* name_ptr, ULONG initial_count);
UINT tx_semaphore_delete(TX_SEMAPHORE* semaphore_ptr);
UINT tx_semaphore_get(TX_SEMAPHORE* semaphore_ptr, ULONG wait_option);
UINT tx_semaphore_put(TX_SEMAPHORE* semaphore_ptr);
UINT tx_queue_create(TX_QUEUE* queue_ptr, CHAR* name_ptr, UINT message_size,
    VOID* queue_start, ULONG queue_size);
UINT tx_queue_receive(TX_QUEUE* queue_ptr, VOID* destination_ptr, ULONG wait_option);
UINT tx_queue_send(TX_QUEUE* queue_ptr, VOID* source_ptr, ULONG wait_option);

UINT tx_timer_create(TX_TIMER* timer_ptr, CHAR* name_ptr,
    VOID(*expiration_function)(ULONG input), ULONG expiration_input,
    ULONG initial_ticks, ULONG reschedule_ticks, UINT auto_activate);


ULONG tx_time_get(VOID);
UINT tx_timer_delete(TX_TIMER* timer_ptr);
UINT tx_thread_create(TX_THREAD* thread_ptr, CHAR* name_ptr,
    VOID(*entry_function)(ULONG entry_input), ULONG entry_input,
    VOID* stack_start, ULONG stack_size,
    UINT priority, UINT preempt_threshold,
    ULONG time_slice, UINT auto_start);

UINT tx_thread_delete(TX_THREAD* thread_ptr);
UINT tx_thread_resume(TX_THREAD* thread_ptr);
UINT tx_thread_suspend(TX_THREAD* thread_ptr);
UINT tx_thread_terminate(TX_THREAD* thread_ptr);
VOID tx_thread_relinquish(VOID);
// UINT tx_interrupt_control(INT flag);
