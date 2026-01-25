# Main Function Relationships in philo (Section Flow Diagrams)

## 1. Initialization & Setup Flow

```mermaid
flowchart TD
    MAIN([main])
    INIT_RULES([init_rules])
    INIT_MUTEXES([init_mutexes])
    INIT_PHILOS([init_philos])
    CLEANUP_PARTIAL([cleanup_partial_init])
    CLEANUP([cleanup_rules])
    PRINT_USAGE([print_usage])

    MAIN --> INIT_RULES
    INIT_RULES --> INIT_MUTEXES
    INIT_RULES -->|on error| CLEANUP
    INIT_RULES -->|usage error| PRINT_USAGE
    INIT_MUTEXES -->|on error| CLEANUP_PARTIAL
    MAIN --> INIT_PHILOS
    INIT_PHILOS -->|on error| CLEANUP
```

**Details:**
- `main` initializes rules, mutexes, and philosophers sequentially
- `init_rules` calls `init_mutexes` internally
- Errors trigger partial or full cleanup
- Usage errors display help message via `print_usage`

---

## 2. Simulation & Monitoring Flow

```mermaid
flowchart TD
    MAIN([main])
    START_SIM([start_simulation])
    CREATE_THREADS([create_threads])
    PHILO_ROUTINE([philo_routine])
    MONITOR([monitor_simulation])
    CHECK_DEATH([check_death])
    ALL_ATE([all_ate_enough])
    CLEANUP([cleanup_rules])

    MAIN --> START_SIM
    START_SIM --> CREATE_THREADS
    CREATE_THREADS --> PHILO_ROUTINE
    START_SIM --> MONITOR
    MONITOR --> CHECK_DEATH
    MONITOR --> ALL_ATE
    START_SIM --> CLEANUP
    MAIN --> CLEANUP
```

**Details:**
- `start_simulation` creates philosopher threads and manages the monitor
- Philosopher threads run concurrent `philo_routine` loops
- Main thread runs `monitor_simulation` to check for death and completion conditions
- Monitor polls `check_death` and `all_ate_enough` in a loop
- After simulation completes, full cleanup occurs

---

## 3. Philosopher Routine Flow

```mermaid
flowchart TD
    PHILO_ROUTINE([philo_routine])
    TAKE_FORKS([take_forks])
    EAT([eat])
    RELEASE_FORKS([release_forks])
    UPDATE_MEAL([update_meal_time])
    INCR_MEALS([increment_meals])
    PRINT_STATE([print_state])
    USLEEP([ft_usleep])
    GET_TIME([get_time_ms])

    PHILO_ROUTINE --> TAKE_FORKS
    PHILO_ROUTINE --> EAT
    PHILO_ROUTINE --> RELEASE_FORKS
    EAT --> UPDATE_MEAL
    EAT --> INCR_MEALS
    EAT --> USLEEP
    PHILO_ROUTINE --> PRINT_STATE
    PHILO_ROUTINE --> USLEEP
    EAT --> GET_TIME
    PHILO_ROUTINE --> GET_TIME
```

**Details:**
- `philo_routine` is the main loop for each philosopher thread
- Philosophers take forks, eat, update meal info, print state, then sleep
- `take_forks` and `release_forks` manage fork mutual exclusion
- `update_meal_time` and `increment_meals` track dining history
- `print_state` outputs philosopher actions
- `ft_usleep` is used for sleeping, which checks stop condition

---

## 4. Utility & Error Handling Flow

```mermaid
flowchart TD
    GET_TIME([get_time_ms])
    USLEEP([ft_usleep])
    PRINT_STATE([print_state])
    PRINT_ERROR([print_error])
    ERROR_EXIT([error_exit])
    
    PHILO_ROUTINE[philo_routine]
    EAT[eat]
    MONITOR[monitor_simulation]

    PHILO_ROUTINE --> GET_TIME
    PHILO_ROUTINE --> USLEEP
    PHILO_ROUTINE --> PRINT_STATE
    EAT --> GET_TIME
    EAT --> USLEEP
    MONITOR --> USLEEP
    PRINT_ERROR --> ERROR_EXIT
```

**Details:**
- `get_time_ms()` provides current time in milliseconds for meal tracking
- `ft_usleep()` sleeps while respecting stop condition (allows early termination)
- `print_state()` outputs philosopher state changes with thread-safe printing
- `print_error()` logs errors to stderr
- `error_exit()` returns error code after printing error message
- Utilities are used throughout philosopher routine and monitor sections

---

## 5. Complete Execution Flow

```mermaid
flowchart TD
    INIT["Initialization Phase<br/>main → init_rules → init_mutexes<br/>→ init_philos"]
    SIM["Simulation Phase<br/>start_simulation → create_threads<br/>spawn philo_routine threads"]
    PHILO["Philosopher Phase<br/>concurrent loop:<br/>take_forks → eat → release_forks"]
    MONITOR["Monitor Phase<br/>concurrent polling:<br/>check_death & all_ate_enough"]
    CLEANUP["Cleanup Phase<br/>cleanup_rules<br/>destroy mutexes & free memory"]
    ERROR["Error Handling<br/>At any point: print_error<br/>→ error_exit or cleanup"]

    INIT --> SIM
    SIM --> PHILO
    SIM --> MONITOR
    PHILO -.->|completion signal| MONITOR
    MONITOR -->|exit condition met| CLEANUP
    ERROR -->|error occurs| CLEANUP
```

**Details:**
- **Initialization Phase:** Validate arguments and setup data structures
- **Simulation Phase:** Create philosopher threads and start main monitor
- **Philosopher Phase:** Concurrent eating-thinking cycle with fork management
- **Monitor Phase:** Main thread checks for death or completion
- **Cleanup Phase:** Destroy all synchronization primitives and free memory
- **Error Handling:** Any error triggers appropriate cleanup and logging

