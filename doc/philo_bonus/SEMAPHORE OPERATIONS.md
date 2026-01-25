SEMAPHORE OPERATIONS IN SEQUENCE OF EXECUTION

(Bonus Part - Process-Based)

PHASE 1: PROCESS INITIALIZATION

SETUP (Before fork):

• forks semaphore created with count = N (number of philosophers)

• print semaphore created with count = 1 (binary)

• Both are named semaphores (shared across processes)

PHASE 2: PHILOSOPHER PROCESS ROUTINE LOOP

(Each philosopher is a separate process, runs concurrently)

1\. forks semaphore (forks_bonus.c:17, take_forks)

└─ sem_wait(philo-\>rules-\>forks)

WHY: Decrement fork counter, waits if no forks available (count = 0)

Only 1 fork available when count = 1

Philosopher blocks here if all forks taken by other philosophers

Prevents N+1 philosophers from eating simultaneously

2\. forks semaphore (forks_bonus.c:19, take_forks)

└─ sem_wait(philo-\>rules-\>forks)

WHY: Acquire second fork (wait for count to become non-zero)

Now philosopher has secured 2 forks for eating

Atomic operations prevent deadlock (semaphore handles ordering)

Unlike mandatory part, no explicit fork ordering needed

3\. print_state semaphore (print_bonus.c:19, print_state)

└─ sem_wait(philo-\>rules-\>print)

WHY: Acquire exclusive access to stdout

Only 1 process can print at a time (print semaphore count = 1)

Prevents interleaved/corrupted output from concurrent processes

Simpler than mandatory part (no nested stop_mutex + print_mutex)

PHASE 3: EATING & FORK RELEASE

4\. forks semaphore (forks_bonus.c:25, release_forks)

└─ sem_post(philo-\>rules-\>forks)

WHY: Release first fork (increment semaphore count by 1)

Makes fork available for other waiting philosophers

Signal waiting processes that resource is available

5\. forks semaphore (forks_bonus.c:26, release_forks)

└─ sem_post(philo-\>rules-\>forks)

WHY: Release second fork (increment semaphore count by 1)

Both forks now available again

Other philosophers can proceed from their sem_wait blocks

PHASE 4: MONITOR THREAD (within child process)

(Runs concurrently with philosopher eating/sleeping)

6\. print_state semaphore (monitor_bonus.c:29, check_philo_death)

└─ sem_wait(philo-\>rules-\>print)

WHY: Acquire print semaphore before printing \"died\" message

Ensures death announcement isn\'t interleaved with other output

Process then immediately exits(1) without releasing semaphore

(Parent process detects non-zero exit code)

PHASE 5: PRINT RELEASES (After critical sections)

7\. print_state semaphore (print_bonus.c:22, print_state)

└─ sem_post(philo-\>rules-\>print)

WHY: Release exclusive print access after printing state

Allows next philosopher to print their message

Happens after printf completes successfully

**SEMAPHORE DEPENDENCY SUMMARY (Bonus):**

┌─ forks semaphore (count = N initially)

│ Used: take_forks (2x sem_wait), release_forks (2x sem_post)

│ Reason: Prevent more than N/2 philosophers from eating simultaneously

│Semaphore atomicity eliminates deadlock risk (no ordering needed)

│Inter-process synchronization (works across fork boundaries)

│

└─ print semaphore (count = 1 initially, binary)

Used: print_state (1x sem_wait, 1x sem_post)

Reason: Ensure only 1 process prints at a time

Single critical section (no nested locks like mandatory part)

Simpler model than stop_mutex + print_mutex combination

TOTAL SEMAPHORE OPERATIONS PER CYCLE (per philosopher):

• forks semaphore: 4x (2 waits to take, 2 posts to release)

• print semaphore: 2x per message (1 wait + 1 post per print_state call)

─────────────────────────────────

Per eating cycle:

• take_forks: 2x sem_wait(forks)

• print_state after fork 1: 1x sem_wait(print) + 1x sem_post(print)

• print_state after fork 2: 1x sem_wait(print) + 1x sem_post(print)

• release_forks: 2x sem_post(forks)

─────────────────────────────────

Total: 4 waits, 4 posts + print overhead

KEY DIFFERENCES FROM MANDATORY PART:

Mandatory (Threads + Mutexes):

• Fork mutexes (one per fork) - explicit ordering to prevent deadlock

• meal_mutex (one per philosopher) - tracks state

• stop_mutex (global) - coordination flag

• print_mutex (global) - output synchronization

────────────────────────────

Total locks per cycle: 6+ critical sections

Bonus (Processes + Semaphores):

• forks semaphore (counting, count = N) - automatic deadlock prevention

• print semaphore (binary, count = 1) - single output lock

• meal_mutex still used (threads within each process need it)

────────────────────────────

Total locks per cycle: 4 sem operations + print overhead

ADVANTAGES OF SEMAPHORE APPROACH (Bonus):

1\. Counting semaphore handles N-way resource sharing elegantly

2\. No explicit fork ordering needed (automatic FIFO by semaphore)

3\. Cross-process synchronization (works with fork\'d processes)

4\. Simpler code (fewer nested critical sections)

5\. No deadlock possible with single semaphore (atomic operations)

DISADVANTAGES:

1\. Each process has separate memory (needs named semaphores)

2\. Harder to debug (multiple processes vs threads)

3\. More overhead (fork is heavier than thread)
