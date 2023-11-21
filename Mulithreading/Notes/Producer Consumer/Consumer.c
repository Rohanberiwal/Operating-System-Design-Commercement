1. pthread_mutex_lock(&mutex);  // Acquire the mutex lock
2. while(task_queue_size() == 0)
3.   pthread_cond_wait(&cond, &mutex);  // Release the lock and wait for a signal
4. }  // The lock is automatically re-acquired after waking up
5. task = pop_task_queue();  // Critical section: Access and modify shared data
6. pthread_mutex_unlock(&mutex);  // Release the lock
7. execute_task(task);  // Perform task outside the critical section
