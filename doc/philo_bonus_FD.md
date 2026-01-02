```mermaid
flowchart TD
    A[Start] --> B[Parse Arguments]
    B --> C[Initialize Data & Semaphores]
    C --> D{Initialization Success?}
    D -- No --> E[Print Error & Exit]
    D -- Yes --> F[Create Philosopher Processes]
    F --> G[Each Process: Start Monitor Thread]
    G --> MT[Monitor Thread: Check Death]
    G --> X[Philosopher Life Loop]
    X --> H1[Wait for Forks - sem_wait forks]
    H1 --> H2[Take Forks - print status]
    H2 --> H3[Lock Meal Semaphore - sem_wait meal_sem]
    H3 --> H4[Eating - update last_meal_time, print status]
    H4 --> H5[Unlock Meal Semaphore - sem_post meal_sem]
    H5 --> H6[Release Forks - sem_post forks]
    H6 --> H7[Sleeping - print status]
    H7 --> H8[Thinking - print status]
    H8 --> I{Philosopher Dies or Ate Enough?}
    I -- No --> H1
    I -- Yes --> J[Cleanup & Unlink Semaphores]
    J --> K[Exit Process]
    F --> L[Parent: Wait for Children]
    L --> L1[Parent: Waitpid for Child Exit Status]
    L1 --> M[Parent: Cleanup & Exit]
```
