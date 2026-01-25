MUTEX LOCKS IN SEQUENCE OF EXECUTION

(Mandatory Part)

***PHASE 1: THREAD INITIALIZATION***

1\. meal_mutex (start.c:22, create_threads)

└─ pthread_mutex_lock(&rules-\>philos\[philo_idx\].meal_mutex)

WHY: Initialize each philosopher\'s last_meal time with thread-safe
write

Sets synchronization starting point before threads spawn

PHASE 2: PHILOSOPHER ROUTINE LOOP

(Each philosopher executes this cycle concurrently)

2\. Fork Mutex (Left) (forks.c:30, take_forks)

└─ pthread_mutex_lock(first) → acquired by even/odd philosophers
alternately

WHY: Prevent multiple philosophers from grabbing the same fork

Ordered locking (even IDs take right then left, odd IDs take left then
right) prevents circular deadlock

3\. Fork Mutex (Right) (forks.c:32, take_forks)

└─ pthread_mutex_lock(second)

WHY: Grab second fork only after first is secured

Ensures philosopher has both forks before eating

Ordered acquisition prevents deadlock

4\. meal_mutex (meal.c:17, update_meal_time)

└─ pthread_mutex_lock(&philo-\>meal_mutex)

WHY: Update last_meal timestamp with atomic write

Monitor thread reads this concurrently to detect starvation

Must be protected to avoid race conditions

5\. meal_mutex (meal.c:24, increment_meals)

└─ pthread_mutex_lock(&philo-\>meal_mutex)

WHY: Safely increment meal counter

Monitor checks this to determine if all philosophers are satisfied

Prevents lost updates or torn reads

6\. stop_mutex (print.c:19, print_state)

└─ pthread_mutex_lock(&philo-\>rules-\>stop_mutex)

WHY: Check if simulation should stop before printing

Avoids printing after simulation ends (cleaner output)

Prevents race between checking stop flag and printing

7\. print_mutex (print.c:23, print_state)

└─ pthread_mutex_lock(&philo-\>rules-\>print_mutex)

WHY: Ensure only one philosopher prints at a time

Prevents interleaved/corrupted output from concurrent printf

Critical section protects output buffer

***PHASE 3: MONITOR LOOP (Main Thread)***

(Runs concurrently with philosopher routines)

8\. meal_mutex (monitor.c:26, all_ate_enough)

└─ pthread_mutex_lock(&rules-\>philos\[philo_idx\].meal_mutex)

WHY: Read meal count safely to check if philosopher has eaten enough

Each philosopher updates this, so needs protection

Prevents stale reads or missed updates

9\. meal_mutex (monitor.c:46, check_death)

└─ pthread_mutex_lock(&rules-\>philos\[philo_idx\].meal_mutex)

WHY: Read last_meal timestamp to check if philosopher is starving

Philosopher writes this during eating

Must read atomically to avoid partial/corrupted values

10\. stop_mutex (sync.c:19, get_stop)

└─ pthread_mutex_lock(&rules-\>stop_mutex)

WHY: Safely read stop flag in monitor loop condition check

Philosopher threads also check this flag

Ensures consistent view of stop state

PHASE 4: TERMINATION (Main Thread)

11\. stop_mutex (sync.c:27, set_stop)

└─ pthread_mutex_lock(&rules-\>stop_mutex)

WHY: Set stop flag when death detected or all philosophers satisfied

Multiple sources might set this (monitor or various checkers)

Protects flag writes from race conditions

MUTEX DEPENDENCY SUMMARY:

┌─ Fork Mutexes (N forks for N philosophers)

│ Used: take_forks → eat phase

│ Reason: Prevent fork sharing (Dining Philosophers problem)

│

├─ meal_mutex (1 per philosopher)

│ Used: update_meal_time, increment_meals, monitor checks

│ Reason: Protect shared philosopher state (last_meal, meals)

│

├─ stop_mutex (shared by all)

│ Used: print_state, monitor, get_stop/set_stop

│ Reason: Graceful shutdown signal (write once, read many)

│

└─ print_mutex (shared by all)

Used: printf critical section

Reason: Prevent interleaved output from concurrent threads

TOTAL LOCK ACQUISITIONS PER CYCLE (per philosopher):

• Fork locks: 2x (left + right)

• meal_mutex: 2x (update time + increment count)

• stop_mutex: 1x (before printing)

• print_mutex: 1x (printf)

─────────────

Total: 6 locks per eating cycle

\+ Monitor thread adds: 2x meal_mutex reads per cycle

(one for all_ate_enough, one for check_death)
