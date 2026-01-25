# Philosophers - Concurrent Programming Project

A multithreaded/multiprocess simulation of the classic Dining Philosophers problem with two implementations: one using POSIX threads and mutexes (mandatory), and another using processes and semaphores (bonus).

## Project Overview

The Philosophers project simulates a dining philosophers scenario where N philosophers sit around a table, each with a fork on both sides. Philosophers alternate between thinking and eating. To eat, a philosopher must acquire both adjacent forks. The challenge is to implement concurrent access to shared resources without deadlock or starvation.

### Problem Statement

- **Input**: Number of philosophers, time to die, time to eat, time to sleep, (optional) minimum meals required
- **Output**: Timestamped log of each philosopher's state changes
- **Goal**: Ensure no philosopher dies and (optionally) all philosophers eat the required number of meals

## Building the Project

### Mandatory Implementation (Threads + Mutexes)

```bash
cd philo
make
```

### Bonus Implementation (Processes + Semaphores)

```bash
cd philo_bonus
make
```

### Clean Compiled Files

```bash
make clean    # Remove .o files
make fclean   # Remove .o files and executable
make re       # Rebuild from scratch
```

## Running the Simulations

### Mandatory Version

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### Bonus Version

```bash
./philo_bonus <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### Parameters

| Parameter                                 | Type           | Description                                         |
| ----------------------------------------- | -------------- | --------------------------------------------------- |
| number_of_philosophers                    | int            | Number of philosophers (and forks)                  |
| time_to_die                               | int            | Milliseconds before philosopher dies without eating |
| time_to_eat                               | int            | Milliseconds to eat (forks held)                    |
| time_to_sleep                             | int            | Milliseconds to sleep after eating                  |
| number_of_times_each_philosopher_must_eat | int (optional) | Meals required per philosopher (bonus only)         |

### Example Usage

```bash
# 4 philosophers, 410ms to die, 200ms to eat, 200ms to sleep
./philo 4 410 200 200

# 5 philosophers, each must eat 7 times
./philo_bonus 5 800 200 200 7
```

## Project Structure

```
philo_new/
├── philo/                    # Mandatory implementation (threads)
│   ├── Makefile
│   ├── include/
│   │   └── philo.h
│   └── src/
│       ├── main.c
│       ├── init.c
│       ├── philo_init.c
│       ├── start.c
│       ├── routine.c
│       ├── monitor.c
│       ├── meal.c
│       ├── forks.c
│       ├── sync.c
│       ├── print.c
│       ├── cleanup.c
│       ├── mutex.c
│       ├── time.c
│       ├── utils.c
│       └── error.c
│
├── philo_bonus/              # Bonus implementation (processes)
    ├── Makefile
    ├── include/
    │   └── philo_bonus.h
    └── src/
        ├── main_bonus.c
        ├── init_bonus.c
        ├── start_bonus.c
        ├── routine_bonus.c
        ├── monitor_bonus.c
        ├── meal_bonus.c
        ├── forks_bonus.c
        ├── sem_init_bonus.c
        ├── sem_name_bonus.c
        ├── print_bonus.c
        ├── cleanup_bonus.c
        ├── time_bonus.c
        ├── parsing_bonus.c
        ├── number_utils_bonus.c
        ├── string_utils_bonus.c
        ├── utils_bonus.c
        └── error_bonus.c
```

## Implementation Details

### Mandatory Version (Threads)

- **Concurrency Model**: POSIX threads (pthread)
- **Synchronization**: Mutexes (pthread_mutex_t)
- **Fork Access**: Protected by mutex array
- **Monitoring**: Separate monitor thread
- **Architecture**: All threads share same memory space

**Key Functions:**

- `philo_routine()` - Each philosopher thread runs this
- `monitor_simulation()` - Watches for death and completion
- `pthread_mutex_lock/unlock()` - Synchronization primitives

### Bonus Version (Processes)

- **Concurrency Model**: Child processes (fork)
- **Synchronization**: POSIX semaphores (sem\_\*)
- **Fork Access**: Named semaphores
- **Monitoring**: Separate monitor process
- **Architecture**: Each process has isolated memory (copy-on-write)
- **Meal Tracking**: Fully implemented and enforced

**Key Functions:**

- `child_process()` - Each philosopher process runs this
- `monitor_simulation_bonus()` - Watches for death
- `sem_wait/sem_post()` - Synchronization primitives
- `make_sem_name()` - Generates unique semaphore names using PID and timestamp

## Algorithm Overview

### Philosopher State Machine

```
THINKING -> (wait for forks) -> EATING -> (sleep) -> THINKING
```

### Resource Management

**Mandatory (Mutexes):**

1. Try to acquire left fork mutex
2. Try to acquire right fork mutex
3. Eat for time_to_eat milliseconds
4. Release both fork mutexes
5. Sleep for time_to_sleep milliseconds
6. Return to thinking

**Bonus (Semaphores):**

1. Wait on left fork semaphore (sem_wait)
2. Wait on right fork semaphore (sem_wait)
3. Eat for time_to_eat milliseconds
4. Post left and right fork semaphores (sem_post)
5. Sleep for time_to_sleep milliseconds
6. Return to thinking

### Death Detection

Monitor checks each philosopher's `last_meal` timestamp:

- If `(current_time - last_meal) > time_to_die`: Philosopher dies
- Death is printed immediately and simulation stops

### Completion Conditions

**Mandatory**: No specific completion (runs until philosopher dies or terminated)

**Bonus**: Simulation completes when all philosophers have eaten `number_of_times_each_philosopher_must_eat` meals

## Key Features

- **Thread Safety**: All shared data protected by mutexes/semaphores
- **Timestamp Output**: Each action logged with millisecond precision
- **Memory Efficient**: Proper cleanup and freeing of all resources
- **Deadlock Prevention**: Philosopher numbering ensures consistent fork ordering
- **Starvation Avoidance**: Fair scheduling through OS kernel

## Output Format

```
[timestamp_ms] [philosopher_id] has taken a fork
[timestamp_ms] [philosopher_id] is eating
[timestamp_ms] [philosopher_id] is sleeping
[timestamp_ms] [philosopher_id] is thinking
[timestamp_ms] [philosopher_id] died
```

## Visualization

Visualize the philosopher interactions in real-time using the 42 Course Philosophers Visualizer:

**[https://42-course.github.io/philosophers_visualizer/](https://42-course.github.io/philosophers_visualizer/)**

### How to Use the Visualizer

1. Build and run the philosophers executable
2. Copy the output (the sequence of philosopher actions)
3. Paste the output into the visualizer
4. Watch the animation showing:
   - Each philosopher's state changes
   - Fork acquisitions and releases
   - Timing of eating and sleeping
   - Death events (if any)

### Visualizer Benefits

- **Interactive Timeline**: Scrub through events step-by-step
- **State Tracking**: See each philosopher's current state in real-time
- **Fork Status**: Visual indication of which forks are in use
- **Performance Analysis**: Identify timing issues and deadlocks
- **Debugging**: Verify correct synchronization and resource management

### Example Visualization Workflow

```bash
# Run simulation and capture output
./philo 4 410 200 200 7 > output.txt

# Copy content from output.txt
# Paste into https://42-course.github.io/philosophers_visualizer/
# Observe the animated dining scenario
```

## Code Quality

- **Compiler Flags**: `-Wall -Wextra -Werror`
- **Style**: 42 School Norm (verified with norminette)
- **Memory Safety**: Validated with Valgrind (zero leaks)
- **Line Limit**: 80 characters per line

## Exit Codes

| Code | Meaning                                          |
| ---- | ------------------------------------------------ |
| 0    | Success                                          |
| 1    | Argument parsing error or initialization failure |
| 84   | Memory allocation failure                        |
| 139  | Segmentation fault                               |

## Edge Cases Handled

- Single philosopher (immediate death at time_to_die)
- Very small timings (microsecond precision with usleep)
- Large number of philosophers (scalable design)
- Optional meal count
- No forks required for thinking

## Technical Considerations

### Mandatory vs Bonus

| Feature | Mandatory   | Bonus            |
| ------- | ----------- | ---------------- |
| Model   | Threads     | Processes        |
| Sync    | Mutexes     | Semaphores       |
| Memory  | Shared heap | Isolated (COW)   |
| Forks   | Mutex array | Named semaphores |
| Meals   | Optional    | Enforced         |

### Performance Notes

- **Mandatory**: Faster due to shared memory, but requires careful synchronization
- **Bonus**: Slower due to process overhead, but provides true isolation

### Known Limitations

- Philosopher count affects fork array allocation
- Very low time values (< 60ms) may cause timing inaccuracies
- Process limit on system affects bonus implementation scalability

## Debugging

### Memory Leaks

```bash
cd philo
make
valgrind --leak-check=full ./philo 2 400 100 100 5
```

### Race Conditions

Use tools like ThreadSanitizer (optional):

```bash
gcc -fsanitize=thread -o philo_san *.c
```

## Author Notes

This project demonstrates:

- Concurrent programming principles
- Synchronization mechanisms
- Resource management
- Process and thread lifecycle management
- POSIX API usage

The implementation prioritizes correctness and clarity while maintaining code quality standards.

## License

42 School Project

## Resources

- [POSIX Threads Documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [POSIX Semaphores](https://man7.org/linux/man-pages/man7/sem_overview.7.html)
- [Valgrind Manual](https://valgrind.org/docs/manual/)
- [Data Flow Documentation](doc/philosophers_FD.md)
