# Philosophers

This project is a simulation of the classic Dining Philosophers problem, implemented in C. It demonstrates knowledge of concurrent programming, synchronization, threads, processes, mutexes, and semaphores.

## Project Structure

The project is divided into two parts, each with a modern folder structure:

1.  **Mandatory Part (`philo/`)**:

- Source files are in `philo/src/`
- Header files are in `philo/include/`
- Implemented using **threads** (`pthread`) and **mutexes**.
- Each philosopher is a thread.
- There is one fork between each pair of philosophers.
- Mutexes are used to protect the forks and shared resources.

2.  **Bonus Part (`philo_bonus/`)**:

- Source files are in `philo_bonus/src/`
- Header files are in `philo_bonus/include/`
- Implemented using **processes** (`fork`) and **semaphores**.
- Each philosopher is a separate process.
- Forks are represented by a semaphore.
- Semaphores are used for synchronization.
- Clean exit is handled using a shared stop flag.

## Global Rules

- **No global variables** are allowed.
- The program takes the following arguments:
  `number_of_philosophers` `time_to_die` `time_to_eat` `time_to_sleep` `[number_of_times_each_philosopher_must_eat]`

  - `number_of_philosophers`: The number of philosophers and also the number of forks. Must be between 1 and 200 (inclusive).
  - `time_to_die`: Time in milliseconds. If a philosopher doesn't start eating 'time_to_die' ms after the start of their last meal or the start of the simulation, they die. Must be > 60.
  - `time_to_eat`: Time in milliseconds it takes for a philosopher to eat. Must be > 60.
  - `time_to_sleep`: Time in milliseconds a philosopher spends sleeping. Must be > 60.
  - `number_of_times_each_philosopher_must_eat` (optional): If all philosophers have eaten at least this many times, the simulation stops. Must be positive.

### Error Handling

- All arguments are validated at startup. If any argument is invalid, a clear error message is printed to stderr and the program exits with a failure code.
- Error messages include:
  - Usage message for wrong argument count
  - All arguments must be positive and less than INT_MAX
  - Philosopher count must be between 1 and 200
  - Time arguments must be greater than 60 ms
  - Optional eat count must be positive

### Build & Clean

- Use `make` in `philo/` or `philo_bonus/` to build each part.
- Use `make fclean` to remove all build artifacts.

## Allowed System Functions

The following system/library functions are used, in compliance with 42 project rules:

- memset, printf, malloc, free, write
- usleep, gettimeofday
- pthread_create, pthread_detach, pthread_join
- pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
- exit (for error handling and process exit)
- (Bonus) fork, waitpid, WIFEXITED, WEXITSTATUS (process management)
- (Bonus) sem_open, sem_close, sem_unlink, sem_wait, sem_post (semaphores)
- (Bonus) O_CREAT, SEM_FAILED (semaphore flags/constants)

## Compilation and Usage

### Mandatory Part

1.  Navigate to the `philo` directory:

```bash
cd philo
```

2.  Compile the program (Makefile automatically finds sources in `src/` and headers in `include/`):

```bash
make
```

3.  Run the simulation:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]
```

#### Optional Argument: `[must_eat_count]`

The last argument `[must_eat_count]` is optional. If provided, the simulation will stop once every philosopher has eaten at least this many times. If omitted, the simulation will only stop when a philosopher dies.

**Example with optional argument:**

```bash
./philo 5 800 200 200 7
```

(Each philosopher must eat at least 7 times for the simulation to end.)

**Example:**

```bash
./philo 5 800 200 200
```

(5 philosophers, die after 800ms, eat for 200ms, sleep for 200ms)

### Bonus Part

1.  Navigate to the `philo_bonus` directory:

```bash
cd philo_bonus
```

2.  Compile the program (Makefile automatically finds sources in `src/` and headers in `include/`):

```bash
make
```

3.  Run the simulation:

```bash
./philo_bonus <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]
```

#### Optional Argument: `[must_eat_count]`

The last argument `[must_eat_count]` is optional. If provided, the simulation will stop once every philosopher has eaten at least this many times. If omitted, the simulation will only stop when a philosopher dies.

**Example with optional argument:**

```bash
./philo_bonus 5 800 200 200 7
```

(Each philosopher must eat at least 7 times for the simulation to end.)

**Example:**

```bash
./philo_bonus 5 800 200 200
```

## Implementation Details

- **Deadlock Prevention**:
  - **Mandatory**: Uses an odd/even fork locking strategy. Odd-numbered philosophers pick up the left fork first, while even-numbered philosophers pick up the right fork first.
  - **Bonus**: Semaphores naturally handle the resource counting, and the system scheduler manages process execution.
- **Monitoring**:
  - **Mandatory**: A separate monitor thread checks the state of all philosophers to detect death or completion.
  - **Bonus**: Each philosopher process has its own monitor thread. If a philosopher dies, the process exits with a specific status, which the main process detects to terminate the entire simulation. Clean exit is handled using a shared stop flag.
- **Starvation Prevention**:
  - Smart thinking delays and staggered thread/process starts are implemented to prevent starvation, especially for odd numbers of philosophers.
**High-Precision Timing**:
  - The custom `ft_usleep` function uses a tight polling interval (250 microseconds) to ensure precise adherence to timing constraints and fast responsiveness to death detection, minimizing drift and program exit delay in long-running simulations.

## Visualization

### How to Visualize the Output

You can visualize the output of your simulation using the [Philosophers Visualizer](https://rom98759.github.io/Philosophers-visualizer/):

1. **Run your simulation and save the output to a file.**

For example, in the root of your project:

```bash
./philo/philo 5 800 200 200 7 > output.txt
# or for the bonus part
./philo_bonus/philo_bonus 5 800 200 200 7 > output.txt
```

2. **Open the [Philosophers Visualizer](https://rom98759.github.io/Philosophers-visualizer/) in your browser.**

3. **Upload your `output.txt` file** using the upload button on the visualizer page.

4. **Watch the simulation animation** and analyze the philosophers' behavior interactively.

This tool helps you better understand and debug the timing and synchronization of your solution.

---

**Last updated:** December 28, 2025
