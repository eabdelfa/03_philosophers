flowchart TD
A[Start] --> B[Parse Arguments]
B --> C[Initialize Data & Mutexes]
C --> D{Initialization Success?}
D -- No --> E[Print Error & Exit]
D -- Yes --> F[Create Philosopher Threads]
F --> G[Start Monitor Thread]
G --> MT[Monitor Thread: Check Death]
G --> H[Philosopher Routine Loop]
H --> H1[Wait for Forks - pthread_mutex_lock]
H1 --> H2[Take Forks - print status]
H2 --> H3[Eating - update last_meal_time, print status]
H3 --> H4[Release Forks - pthread_mutex_unlock]
H4 --> H5[Sleeping - print status]
H5 --> H6[Thinking - print status]
H6 --> I{Philosopher Dies or All Ate Enough?}
I -- No --> H1
I -- Yes --> J[Cleanup & Destroy Mutexes]
J --> K[Exit]
