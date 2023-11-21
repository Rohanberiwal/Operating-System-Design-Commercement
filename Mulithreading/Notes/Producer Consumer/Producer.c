1. pthread_mutex_lock(&mutex);  // Acquire the mutex lock
2. int queue_size = task_queue_size();  // Critical section: Access shared data
3. push_task_queue(&task);  // Modify shared data
4. if(queue_size == 0) {
5.   pthread_cond_broadcast(&cond);  // Signal waiting consumers
6. }
7. pthread_mutex_unlock(&mutex);  // Release the lock
