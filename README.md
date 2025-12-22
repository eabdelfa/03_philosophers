# Philosophers

This project is a simulation of the classic Dining Philosophers problem, implemented in C. It demonstrates knowledge of concurrent programming, synchronization, threads, processes, mutexes, and semaphores.

## Project Structure

The project is divided into two parts:

1.  **Mandatory Part (`philo/`)**:

    - Implemented using **threads** (`pthread`) and **mutexes**.
    - Each philosopher is a thread.
    - There is one fork between each pair of philosophers.
    - Mutexes are used to protect the forks and shared resources.

2.  **Bonus Part (`philo_bonus/`)**:
    - Implemented using **processes** (`fork`) and **semaphores**.
    - Each philosopher is a separate process.
    - Forks are represented by a semaphore.
    - Semaphores are used for synchronization.

## Global Rules

- **No global variables** are allowed.
- The program takes the following arguments:
  `number_of_philosophers` `time_to_die` `time_to_eat` `time_to_sleep` `[number_of_times_each_philosopher_must_eat]`

  - `number_of_philosophers`: The number of philosophers and also the number of forks.
  - `time_to_die`: Time in milliseconds. If a philosopher doesn't start eating 'time_to_die' ms after the start of their last meal or the start of the simulation, they die.
  - `time_to_eat`: Time in milliseconds it takes for a philosopher to eat.
  - `time_to_sleep`: Time in milliseconds a philosopher spends sleeping.
  - `number_of_times_each_philosopher_must_eat` (optional): If all philosophers have eaten at least this many times, the simulation stops.

## Compilation and Usage

### Mandatory Part

1.  Navigate to the `philo` directory:
    ```bash
    cd philo
    ```
2.  Compile the program:
    ```bash
    make
    ```
3.  Run the simulation:

    ```bash
    ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]
    ```

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
2.  Compile the program:
    ```bash
    make
    ```
3.  Run the simulation:

    ```bash
    ./philo_bonus <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]
    ```

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
  - **Bonus**: Each philosopher process has its own monitor thread. If a philosopher dies, the process exits with a specific status, which the main process detects to terminate the entire simulation.
- **Starvation Prevention**:
  - Implemented a smart thinking delay for odd numbers of philosophers. This ensures that in competitive scenarios (e.g., 3 philosophers), the "odd one out" waits just long enough for the fork rotation to occur, preventing resource starvation.
- **High-Precision Timing**:
  - The custom `ft_usleep` function uses a tighter polling interval (100us) to ensure precise adherence to `time_to_die` and `time_to_eat` constraints, minimizing drift in long-running simulations.

## Visualization

You can visualize the output of the simulation using this tool:
[Philosophers Visualizer](https://rom98759.github.io/Philosophers-visualizer/)
