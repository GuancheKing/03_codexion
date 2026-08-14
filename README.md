*This project has been created as part of the 42 curriculum by @josjimen.*

# Codexion

## Description

Codexion is a concurrency simulation written in C using POSIX threads.

Multiple coders compete for a limited set of USB dongles. Each coder needs two adjacent dongles to compile, then alternates between compiling, debugging and refactoring.

The main goal of the project is to manage shared resources safely while preventing race conditions, deadlocks and starvation.

Dongle requests are scheduled using one of two policies:

- `fifo`: requests are prioritized by arrival order.
- `edf`: requests are prioritized by the earliest burnout deadline.

The scheduler is implemented using a priority queue (binary heap).

The simulation ends when every coder completes the required number of compiles or when one coder burns out.

## Instructions

### Compilation

```bash
make
```

This creates the executable:

```bash
./codexion
```

Other available Makefile rules:

```bash
make clean
make fclean
make re
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

Example:

```bash
./codexion 5 800 200 100 100 3 20 fifo
```

The scheduler must be either `fifo` or `edf`.

All time values are expressed in milliseconds.

## Blocking cases handled

### Deadlocks

A coder never keeps one dongle while waiting indefinitely for another one. Both required dongles are checked and reserved as part of the same acquisition process.

When two dongle mutexes must be locked, they are always locked in a consistent order based on their IDs. This removes circular lock dependencies and prevents circular wait, one of Coffman's deadlock conditions.

### Starvation and scheduling

Requests are stored in a priority queue.

FIFO gives priority according to request arrival order, while EDF gives priority to the coder with the earliest burnout deadline.

Only requests competing for the same dongles block each other, allowing independent coders to compile concurrently.

### Dongle cooldown

After a dongle is released, its next availability time is stored.

A coder cannot reserve it again until the configured `dongle_cooldown` has elapsed. Waiting threads use the shared condition variable instead of continuously polling the resource.

### Burnout detection

A separate monitor thread checks each coder's last compilation start time against their burnout deadline.

The coder state is read under mutex protection so the monitor always works with a consistent snapshot.

### Log serialization

A dedicated mutex protects the program output.

This prevents messages produced by different threads from being mixed together and prevents normal state messages from being printed after the simulation has finished.

## Thread synchronization mechanisms

The project uses `pthread_mutex_t` and `pthread_cond_t` to synchronize threads.

Each dongle has its own mutex protecting its `in_use` state and availability time.

Additional mutexes protect:

- the request queue;
- each coder's shared state;
- the simulation finished state;
- log output.

A condition variable associated with the request queue allows coders to sleep while they cannot acquire their dongles. Threads are awakened when resources are released or when the simulation finishes.

A separate start condition is used so all coder threads begin from the same simulation start point.

Communication between coder threads and the monitor is performed through shared state protected by mutexes. When the simulation finishes, waiting threads are notified using `pthread_cond_broadcast()` so they can stop safely.

## Testing

The project was tested with different coder counts, FIFO and EDF scheduling, cooldown values and burnout scenarios.

Memory and thread synchronization were also checked using:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./codexion ...
valgrind --tool=helgrind ./codexion ...
```

The tested executions reported no memory leaks and no synchronization errors.

## Resources

- Documentation:

- [POSIX threads documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [`pthread_create()` documentation](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
- [POSIX mutexes documentation](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [POSIX condition variables documentation](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3.html)
- [`gettimeofday()` documentation](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

- Personal notes by @lospina-:
  - [Codexion notes on Notion](https://app.notion.com/p/Codexion-390ef47991ff80a0b82ee5dcd76cf888)

### Use of AI
Artificial intelligence was used exclusively as a learning and mentoring tool throughout the development of this project.

It assisted with:

- understanding concurrency concepts such as threads, mutexes, condition variables, race conditions, deadlocks and starvation;
- discussing synchronization and resource-sharing strategies;
- reviewing the FIFO and EDF scheduling approaches;
- understanding and reviewing the priority queue implementation;
- identifying possible edge cases and concurrency issues;
- analyzing testing results from Valgrind and Helgrind;
- reviewing and improving the project documentation and README.

All final code, architecture and implementation decisions were developed by the author, with AI used only to clarify concepts, review ideas and support the learning process.