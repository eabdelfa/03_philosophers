# Main Function Relationships in philo_bonus (Section Flow Diagrams)

## 1. Initialization & Setup Flow

```mermaid
flowchart TD
    MAIN([main])
    INIT_RULES([init_rules])
    SEM_INIT([sem_init_bonus])
    SEM_NAME([sem_name_bonus])
    INIT_PHILOS([init_philos])
    CLEANUP([cleanup_rules])
    PRINT_USAGE([print_usage])

    MAIN --> INIT_RULES
    INIT_RULES --> SEM_INIT
    INIT_RULES --> SEM_NAME
    INIT_RULES -->|on error| CLEANUP
    INIT_RULES -->|usage error| PRINT_USAGE
    MAIN --> INIT_PHILOS
    INIT_PHILOS -->|on error| CLEANUP
```

**Details:**
- `main` initializes rules and philosophers using semaphores instead of mutexes
- `init_rules` calls `sem_init_bonus` to create named semaphores and `sem_name_bonus` to generate semaphore names
- Semaphores are used for inter-process synchronization (different from mandatory part which uses threads)
- Errors trigger full cleanup via `cleanup_rules`
- Usage errors display help via `print_usage`

---

## 2. Process Management & Monitoring Flow

```mermaid
flowchart TD
    MAIN([main])
    START_SIM([start_simulation])
    FORK_PROCESSES([fork processes])
    PHILO_PROCESS([child process: philo_routine])
    MONITOR_ROUTINE([monitor_routine])
    EXIT_PROCESS([exit])
    WAIT_CHILDREN([wait_children])
    KILL_ALL([kill_all])
    CLEANUP([cleanup_rules])

    MAIN --> START_SIM
    START_SIM --> FORK_PROCESSES
    FORK_PROCESSES -->|parent| WAIT_CHILDREN
    FORK_PROCESSES -->|child| PHILO_PROCESS
    PHILO_PROCESS --> MONITOR_ROUTINE
    PHILO_PROCESS --> EXIT_PROCESS
    WAIT_CHILDREN -->|death detected| KILL_ALL
    WAIT_CHILDREN -->|completion| CLEANUP
    MAIN --> CLEANUP
```

**Details:**
- `start_simulation` forks child processes instead of creating threads (uses `fork()`)
- Each child process runs `philo_routine` with its own `monitor_routine` thread
- Parent process calls `wait_children` to monitor child exit statuses
- `kill_all` terminates remaining children if one dies
- Uses inter-process communication via named semaphores
- Process-based model provides better isolation than thread-based

---

## 3. Philosopher Routine Flow

```mermaid
flowchart TD
    PHILO_ROUTINE([philo_routine])
    SETUP_CHILD([setup_child_philo])
    TAKE_FORKS([take_forks])
    EAT([eat])
    RELEASE_FORKS([release_forks])
    PHILO_SLEEP([philo_sleep])
    PHILO_THINK([philo_think])
    UPDATE_MEAL([update_meal_time])
    INCR_MEALS([increment_meals])
    PRINT_STATE([print_state])
    USLEEP([ft_usleep])

    PHILO_ROUTINE --> SETUP_CHILD
    PHILO_ROUTINE --> TAKE_FORKS
    PHILO_ROUTINE --> EAT
    PHILO_ROUTINE --> RELEASE_FORKS
    PHILO_ROUTINE --> PHILO_SLEEP
    PHILO_ROUTINE --> PHILO_THINK
    EAT --> UPDATE_MEAL
    EAT --> INCR_MEALS
    EAT --> USLEEP
    PHILO_SLEEP --> USLEEP
    PHILO_THINK --> USLEEP
    PHILO_ROUTINE --> PRINT_STATE
```

**Details:**
- `philo_routine` runs in child process and manages the eat-sleep-think cycle
- `setup_child_philo` initializes philosopher with meal mutex and spawns monitor thread
- `take_forks` acquires semaphores for fork access
- `eat` updates meal time, increments meal count, and sleeps
- `philo_sleep` and `philo_think` are explicit phases with optimized think time
- `release_forks` releases semaphores after eating
- Each philosopher has its own monitor thread within the child process
- `print_state` logs state changes with semaphore-protected printing

---

## 4. Utility, Error & Process Management Flow

```mermaid
flowchart TD
    GET_TIME([get_time_ms])
    USLEEP([ft_usleep])
    PRINT_STATE([print_state])
    PRINT_ERROR([print_error])
    ERROR_EXIT([error_exit])
    SEM_POST([sem_post])
    SEM_WAIT([sem_wait])
    
    PHILO_ROUTINE[philo_routine]
    EAT[eat]
    MONITOR[monitor_routine]
    TAKE_FORKS[take_forks]
    RELEASE_FORKS[release_forks]

    PHILO_ROUTINE --> GET_TIME
    PHILO_ROUTINE --> USLEEP
    PHILO_ROUTINE --> PRINT_STATE
    EAT --> GET_TIME
    EAT --> USLEEP
    MONITOR --> USLEEP
    TAKE_FORKS --> SEM_WAIT
    RELEASE_FORKS --> SEM_POST
    PRINT_ERROR --> ERROR_EXIT
```

**Details:**
- `get_time_ms()` provides current time in milliseconds for meal tracking
- `ft_usleep()` sleeps with microsecond precision
- `print_state()` outputs philosopher state changes with semaphore protection
- `sem_wait()` acquires semaphores for fork/print access
- `sem_post()` releases semaphores after access
- `print_error()` logs errors to stderr
- `error_exit()` returns error code
- Semaphores replace mutex operations in the bonus version

---

## 5. Complete Execution Flow (Process-Based)

```mermaid
flowchart TD
    INIT["Initialization Phase<br/>main → init_rules → sem_init<br/>→ sem_name → init_philos"]
    FORK["Process Forking Phase<br/>start_simulation → fork()<br/>spawn child processes"]
    PARENT["Parent Process<br/>wait_children() loop<br/>monitor child exits"]
    CHILD["Child Process<br/>philo_routine with<br/>embedded monitor_routine"]
    MONITOR["Monitor Thread<br/>within each child:<br/>check_death & exit"]
    CLEANUP["Cleanup Phase<br/>cleanup_rules<br/>sem_close & sem_unlink"]
    ERROR["Error Handling<br/>Death detected:<br/>kill_all → cleanup"]

    INIT --> FORK
    FORK -->|parent path| PARENT
    FORK -->|child path| CHILD
    CHILD --> MONITOR
    MONITOR -->|death| MONITOR
    PARENT -->|child dies| ERROR
    PARENT -->|all eaten| CLEANUP
    ERROR -->|kill children| CLEANUP
```

**Details:**
- **Initialization Phase:** Create named semaphores for inter-process synchronization
- **Process Forking:** Create child processes instead of threads (each philosopher is a separate process)
- **Parent Process:** Monitors all child processes with `wait_children()` loop
- **Child Process:** Runs `philo_routine` with embedded `monitor_routine` thread
- **Monitor Thread:** Each child process has its own monitor thread to check for starvation
- **Error Handling:** If any child dies, parent kills all remaining children immediately
- **Cleanup Phase:** Close and unlink all named semaphores, free allocated memory
- **Key Difference:** Process-based model (bonus) vs thread-based (mandatory) provides better isolation and separate memory spaces
