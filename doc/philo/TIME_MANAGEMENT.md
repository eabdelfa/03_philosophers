# Time Capturing and Management in Philosophers

This document provides a comprehensive explanation of how time is captured, managed, and used throughout the Philosophers project.

## Table of Contents

1. [Overview](#overview)
2. [Time Capturing Mechanism](#time-capturing-mechanism)
3. [Time Reference Point](#time-reference-point)
4. [Precise Sleep Implementation](#precise-sleep-implementation)
5. [Time Usage in Different Contexts](#time-usage-in-different-contexts)
6. [Thread Safety Considerations](#thread-safety-considerations)
7. [Performance Considerations](#performance-considerations)

---

## Overview

Time management is **critical** to the Philosophers simulation for three main reasons:

1. **Death Detection**: Tracking when each philosopher last ate to detect starvation
2. **Action Timing**: Ensuring philosophers eat, sleep, and think for the correct durations
3. **Output Logging**: Timestamping every action for debugging and visualization

The time system must be:
- **Precise**: Millisecond accuracy is required
- **Thread-safe**: Multiple threads access time concurrently
- **Efficient**: Time is checked frequently (thousands of times per second)
- **Relative**: All timestamps are relative to simulation start time

---

## Time Capturing Mechanism

### Core Function: `get_time_ms()`

Located in `philo/src/time.c`:

```c
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + ((tv.tv_usec + 500) / 1000));
}
```

#### How It Works

**1. System Call:**
```c
gettimeofday(&tv, NULL);
```
- Retrieves current time from system clock
- Populates `struct timeval` with two fields:
  - `tv.tv_sec`: Seconds since Unix epoch (Jan 1, 1970)
  - `tv.tv_usec`: Microseconds (0-999999)

**2. Conversion to Milliseconds:**
```c
return ((tv.tv_sec * 1000) + ((tv.tv_usec + 500) / 1000));
```

Breaking this down:
- `tv.tv_sec * 1000`: Converts seconds to milliseconds
- `(tv.tv_usec + 500) / 1000`: Converts microseconds to milliseconds with rounding
  - Adding 500 provides **rounding** (not truncation)
  - Example: 1600 microseconds → (1600 + 500) / 1000 = 2 ms ✓
  - Example: 1400 microseconds → (1400 + 500) / 1000 = 1 ms ✓

**3. Why Milliseconds?**
- Project requirements specify times in milliseconds
- Provides sufficient precision (1ms = 1,000 microseconds)
- Balances accuracy with computational efficiency

#### Usage Examples

```c
// Get current time
long current_time = get_time_ms();
// Result: e.g., 1738477467234 (milliseconds since epoch)

// Calculate elapsed time
long start = get_time_ms();
// ... do something ...
long elapsed = get_time_ms() - start;
// Result: elapsed time in milliseconds
```

---

## Time Reference Point

### Simulation Start Time

The simulation establishes a **zero point** at startup to make timestamps human-readable.

Located in `philo/src/start.c`:

```c
int	start_simulation(t_rules *rules)
{
	// ...
	rules->start = get_time_ms();  // ← Capture start time
	threads_created = create_threads(rules);
	// ...
}
```

#### Data Structure

In `philo/include/philo.h`:

```c
struct s_rules
{
	// ...
	long	start;    // Simulation start timestamp (ms since epoch)
	// ...
};
```

### Initial Last Meal Time

When threads are created, each philosopher's `last_meal` is initialized to the start time:

```c
static int	create_threads(t_rules *rules)
{
	int	philo_idx;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		pthread_mutex_lock(&rules->philos[philo_idx].meal_mutex);
		rules->philos[philo_idx].last_meal = rules->start;  // ← Initialize
		pthread_mutex_unlock(&rules->philos[philo_idx].meal_mutex);
		// ...
	}
	// ...
}
```

**Why initialize to start time?**
- Prevents false death detection before first meal
- Simulates that all philosophers just ate at time 0
- Creates fair starting conditions

### Relative Timestamps for Output

All output timestamps are **relative** to start time:

Located in `philo/src/print.c`:

```c
void	print_state(t_philo *philo, char *msg, int force)
{
	long	time;

	pthread_mutex_lock(&philo->rules->stop_mutex);
	if (!philo->rules->stop || force)
	{
		time = get_time_ms() - philo->rules->start;  // ← Relative time
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf("%ld %d %s\n", time, philo->id, msg);
		pthread_mutex_unlock(&philo->rules->print_mutex);
	}
	pthread_mutex_unlock(&philo->rules->stop_mutex);
}
```

**Output Example:**
```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
```
- First action at 0ms (simulation start)
- Easy to read and understand
- Matches project requirements

---

## Precise Sleep Implementation

### The Challenge

Standard `usleep()` has issues:
- **Inaccurate**: Can oversleep due to scheduler
- **Not interruptible**: Can't check stop condition
- **CPU inefficient**: Sleeping too little wastes CPU

### Solution: `ft_usleep()`

Located in `philo/src/time.c`:

```c
void	ft_usleep(t_rules *rules, long ms)
{
	long	start;
	long	now;
	long	remaining;

	start = get_time_ms();
	while (!get_stop(rules))
	{
		now = get_time_ms();
		if (now - start >= ms)
			break ;
		remaining = ms - (now - start);
		if (remaining > 1)
			usleep((remaining / 2) * 1000);
		else
			usleep(100);
	}
}
```

#### How It Works

**1. Capture Start Time**
```c
start = get_time_ms();
```

**2. Check Stop Condition**
```c
while (!get_stop(rules))
```
- Allows early termination if simulation ends
- Critical for responsive shutdown

**3. Check If Target Time Reached**
```c
now = get_time_ms();
if (now - start >= ms)
	break ;
```

**4. Adaptive Sleep Strategy**
```c
remaining = ms - (now - start);
if (remaining > 1)
	usleep((remaining / 2) * 1000);  // Sleep half the remaining time
else
	usleep(100);                      // Busy-wait with tiny sleeps
```

#### Strategy Explanation

**When remaining > 1ms:**
- Sleep for **half** the remaining time
- Safe because we'll wake up and check again
- Prevents oversleeping

**Example:**
```
Target: 200ms
Time: 0ms    → remaining=200ms → sleep 100ms
Time: 100ms  → remaining=100ms → sleep 50ms
Time: 150ms  → remaining=50ms  → sleep 25ms
Time: 175ms  → remaining=25ms  → sleep 12ms
Time: 187ms  → remaining=13ms  → sleep 6ms
Time: 193ms  → remaining=7ms   → sleep 3ms
Time: 196ms  → remaining=4ms   → sleep 2ms
Time: 198ms  → remaining=2ms   → sleep 1ms
Time: 199ms  → remaining=1ms   → sleep 100µs (busy-wait)
Time: 199.1ms → remaining=0.9ms → sleep 100µs
Time: 200ms+ → DONE
```

**When remaining ≤ 1ms:**
- Switch to **busy-wait** mode
- Sleep only 100 microseconds at a time
- Ensures precise timing in the final millisecond

#### Advantages

✅ **Accurate**: Converges to target time precisely  
✅ **Responsive**: Checks stop condition regularly  
✅ **Efficient**: Doesn't waste CPU with continuous polling  
✅ **Safe**: Cannot significantly oversleep  

---

## Time Usage in Different Contexts

### 1. Meal Time Tracking

**Purpose**: Track when each philosopher last ate to detect starvation.

Located in `philo/src/meal.c`:

```c
void	update_meal_time(t_philo *philo, long now)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = now;  // ← Update with absolute time
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	eat(t_philo *philo)
{
	long	now;

	now = get_time_ms();        // ← Capture current time
	update_meal_time(philo, now);
	print_state(philo, "is eating", 0);
	ft_usleep(philo->rules, philo->rules->t_eat);
	increment_meals(philo);
}
```

**Flow:**
1. Philosopher starts eating
2. Capture current time (absolute milliseconds)
3. Update `last_meal` with mutex protection
4. Sleep for eating duration
5. Increment meal counter

**Data Structure:**
```c
typedef struct s_philo
{
	// ...
	long	last_meal;          // Last meal time (ms since epoch)
	pthread_mutex_t	meal_mutex; // Protects last_meal and meals
	// ...
}	t_philo;
```

### 2. Death Detection

**Purpose**: Detect if any philosopher has starved (not eaten within `time_to_die`).

Located in `philo/src/monitor.c`:

```c
static int	check_death(t_rules *rules)
{
	int		philo_idx;
	long	current_time;
	long	last_meal_time;

	philo_idx = 0;
	while (philo_idx < rules->num)
	{
		// Safely read last_meal with mutex
		pthread_mutex_lock(&rules->philos[philo_idx].meal_mutex);
		last_meal_time = rules->philos[philo_idx].last_meal;
		pthread_mutex_unlock(&rules->philos[philo_idx].meal_mutex);
		
		// Skip if not initialized
		if (last_meal_time < 0)
		{
			philo_idx++;
			continue ;
		}
		
		// Check if time since last meal exceeds threshold
		current_time = get_time_ms();
		if (current_time - last_meal_time > rules->t_die)
		{
			set_stop(rules);
			print_state(&rules->philos[philo_idx], "died", 1);
			return (1);
		}
		philo_idx++;
	}
	return (0);
}
```

**Logic:**
```
current_time - last_meal_time > time_to_die
│              │                 │
│              │                 └─ Configured threshold (e.g., 410ms)
│              └─ When philosopher last ate
└─ Current time
```

**Example:**
```
time_to_die = 410ms
last_meal = 1000ms
current_time = 1400ms

Time since last meal = 1400 - 1000 = 400ms
400ms ≤ 410ms → Still alive ✓

current_time = 1420ms
Time since last meal = 1420 - 1000 = 420ms
420ms > 410ms → DEAD ✗
```

**Monitor Loop:**
```c
void	monitor_simulation(t_rules *rules)
{
	while (!get_stop(rules))
	{
		if (check_death(rules))
			return ;
		if (all_ate_enough(rules))
		{
			set_stop(rules);
			return ;
		}
		usleep(1000);  // Check every millisecond
	}
}
```

### 3. Action Timing (Eat, Sleep, Think)

**Purpose**: Ensure philosophers perform actions for the correct duration.

Located in `philo/src/routine.c`:

```c
static void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping", 0);
	ft_usleep(philo->rules, philo->rules->t_sleep);  // ← Sleep for t_sleep
}

static void	philo_think(t_philo *philo)
{
	long	think_time;

	print_state(philo, "is thinking", 0);
	if (philo->rules->num % 2 == 1)
	{
		// Calculate adaptive think time for odd number of philosophers
		think_time = philo->rules->t_die - (philo->rules->t_eat
				+ philo->rules->t_sleep);
		if (think_time > 0)
			ft_usleep(philo->rules, think_time / 2);
	}
}
```

**Timing Parameters:**
```c
struct s_rules
{
	// ...
	long	t_die;     // Time before death without eating (ms)
	long	t_eat;     // Time to eat (ms)
	long	t_sleep;   // Time to sleep (ms)
	// ...
};
```

**Philosopher Cycle:**
```
TAKE FORKS → EAT (t_eat ms) → RELEASE FORKS → SLEEP (t_sleep ms) → THINK → REPEAT
```

### 4. Timestamped Output

**Purpose**: Log every action with precise timestamp for debugging and visualization.

All actions are logged via:

```c
void	print_state(t_philo *philo, char *msg, int force)
{
	long	time;

	pthread_mutex_lock(&philo->rules->stop_mutex);
	if (!philo->rules->stop || force)
	{
		time = get_time_ms() - philo->rules->start;  // ← Relative timestamp
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf("%ld %d %s\n", time, philo->id, msg);
		pthread_mutex_unlock(&philo->rules->print_mutex);
	}
	pthread_mutex_unlock(&philo->rules->stop_mutex);
}
```

**Actions Logged:**
- `"has taken a fork"` - When fork is acquired
- `"is eating"` - Start of eating
- `"is sleeping"` - Start of sleeping  
- `"is thinking"` - Start of thinking
- `"died"` - Death event (forced output even if stopped)

---

## Thread Safety Considerations

### Why Thread Safety Matters

Multiple philosopher threads access time-related data concurrently:
- Reading `last_meal` (monitor thread)
- Writing `last_meal` (philosopher threads)
- Accessing `start` time (all threads)

Without synchronization → **race conditions** and incorrect death detection.

### Protected Data

**1. Last Meal Time**
```c
typedef struct s_philo
{
	// ...
	long	last_meal;          // ← Must protect
	pthread_mutex_t	meal_mutex; // ← Protection mechanism
	// ...
}	t_philo;
```

**Write Operation (Eating):**
```c
void	update_meal_time(t_philo *philo, long now)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = now;  // ← CRITICAL SECTION
	pthread_mutex_unlock(&philo->meal_mutex);
}
```

**Read Operation (Death Check):**
```c
pthread_mutex_lock(&rules->philos[philo_idx].meal_mutex);
last_meal_time = rules->philos[philo_idx].last_meal;  // ← CRITICAL SECTION
pthread_mutex_unlock(&rules->philos[philo_idx].meal_mutex);
```

**2. Start Time**
```c
struct s_rules
{
	// ...
	long	start;  // ← Written once, then read-only
	// ...
};
```

- Written **once** before threads start
- Then **read-only** → No mutex needed
- Safe because no concurrent writes

**3. Stop Flag**
```c
struct s_rules
{
	// ...
	int	stop;
	pthread_mutex_t	stop_mutex;  // ← Protects stop flag
	// ...
};
```

Functions:
```c
int	get_stop(t_rules *rules)
{
	int	result;

	pthread_mutex_lock(&rules->stop_mutex);
	result = rules->stop;
	pthread_mutex_unlock(&rules->stop_mutex);
	return (result);
}

void	set_stop(t_rules *rules)
{
	pthread_mutex_lock(&rules->stop_mutex);
	rules->stop = 1;
	pthread_mutex_unlock(&rules->stop_mutex);
}
```

### Race Condition Example

**Without Mutex:**
```
Thread A (Monitor)          Thread B (Philosopher)
─────────────────          ──────────────────────
read last_meal (1000ms)
                            write last_meal (1400ms)  ← Partial write!
read last_meal (corrupted)
calculate death (WRONG)
```

**With Mutex:**
```
Thread A (Monitor)          Thread B (Philosopher)
─────────────────          ──────────────────────
lock meal_mutex
read last_meal (1000ms)
unlock meal_mutex
                            lock meal_mutex (WAIT)
                            write last_meal (1400ms)
                            unlock meal_mutex
```

---

## Performance Considerations

### Frequency of Time Checks

Time is checked very frequently:

1. **Every philosopher thread:**
   - Before/after each action (eat, sleep, think)
   - ~10-20 times per second per philosopher

2. **Monitor thread:**
   - Every millisecond (usleep(1000))
   - Checks all philosophers

**Example:** 5 philosophers over 10 seconds:
- Philosopher time checks: 5 × 20 × 10 = 1,000 calls
- Monitor time checks: 10,000 calls (1ms interval)
- **Total: ~11,000 calls to `get_time_ms()`**

### Why `gettimeofday()` is Acceptable

✅ **Fast system call**: Typically <1 microsecond  
✅ **No context switch**: User-space access via vDSO (Linux)  
✅ **Monotonic enough**: Sufficient for this use case  

### Optimization: Avoiding Busy-Waiting

The `ft_usleep()` implementation avoids continuous polling:

**Bad Approach (Pure Busy-Wait):**
```c
// DON'T DO THIS
void bad_sleep(long ms)
{
	long start = get_time_ms();
	while (get_time_ms() - start < ms)
		; // Spin loop - wastes CPU!
}
```

**Good Approach (Adaptive Sleep):**
```c
void	ft_usleep(t_rules *rules, long ms)
{
	// ...
	if (remaining > 1)
		usleep((remaining / 2) * 1000);  // Let OS scheduler handle it
	else
		usleep(100);  // Only busy-wait in final millisecond
}
```

**CPU Usage Comparison:**
- Busy-wait: **100% CPU**
- Adaptive sleep: **<5% CPU**

### Memory Access Patterns

Time-related data is accessed frequently but is **cache-friendly**:

```c
struct s_rules
{
	// ...
	long	start;    // ← Read frequently, cache line stays hot
	// ...
};

struct s_philo
{
	// ...
	long	last_meal;  // ← Protected by mutex, accessed less frequently
	// ...
};
```

---

## Summary

### Key Takeaways

1. **Time Capturing**: Uses `gettimeofday()` for millisecond precision
2. **Reference Point**: All timestamps relative to `rules->start` for readability
3. **Precise Timing**: Custom `ft_usleep()` provides accurate, interruptible delays
4. **Death Detection**: Monitors time since last meal vs `time_to_die`
5. **Thread Safety**: Mutexes protect concurrent access to time-related data
6. **Performance**: Efficient implementation suitable for frequent time checks

### Time Flow Diagram

```
Program Start
     │
     ├─→ get_time_ms() ──→ rules->start (reference point)
     │
     ├─→ Initialize philos[].last_meal = rules->start
     │
     ├─→ Start Philosopher Threads
     │       │
     │       ├─→ eat()
     │       │    ├─→ get_time_ms() ──→ now
     │       │    ├─→ update_meal_time(now)
     │       │    └─→ ft_usleep(t_eat)
     │       │
     │       ├─→ sleep()
     │       │    └─→ ft_usleep(t_sleep)
     │       │
     │       └─→ think()
     │            └─→ ft_usleep(think_time)
     │
     └─→ Start Monitor Thread
             │
             └─→ check_death() every 1ms
                  ├─→ get_time_ms() ──→ current_time
                  ├─→ read last_meal
                  └─→ if (current_time - last_meal > t_die) → DEATH
```

### Files Reference

| File | Purpose |
|------|---------|
| `philo/src/time.c` | Core time functions (`get_time_ms`, `ft_usleep`) |
| `philo/src/start.c` | Captures start time, initializes `last_meal` |
| `philo/src/meal.c` | Updates meal time when eating |
| `philo/src/monitor.c` | Death detection using time comparison |
| `philo/src/print.c` | Outputs relative timestamps |
| `philo/src/routine.c` | Uses time for action durations |
| `philo/include/philo.h` | Time-related data structures |

---

*This documentation explains the time management system in the mandatory philo implementation. The bonus implementation (philo_bonus) uses similar concepts but with processes and semaphores instead of threads and mutexes.*
