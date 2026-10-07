# Linux Synchronization Case Study

A practical case study on synchronization mechanisms in Linux, focusing on race conditions, mutexes, semaphores, and spinlocks.

## Team Members

| Team Member | Responsibilities |
|---|---|
| **Vaishnav Sunil Nair** | Race condition implementation, mutex implementation, project integration, README and documentation |
| **Shreyas Nair** | Semaphore implementation, synchronization concepts, experimental testing and result collection |
| **Adithyadev B** | Spinlock implementation, benchmarking, performance analysis and result visualization |

## Overview

Modern operating systems allow multiple processes and threads to execute concurrently. When multiple threads access and modify shared resources at the same time, race conditions can occur and produce incorrect results.

This project demonstrates how synchronization mechanisms are used to control concurrent access to shared resources and maintain data consistency.

The case study focuses on:

- Race conditions
- Critical sections
- Mutexes
- Semaphores
- Spinlocks
- Thread synchronization
- Performance comparison
- Synchronization concepts in Linux

## Case Study Question

> How does Linux prevent concurrent threads from incorrectly accessing shared resources, and what are the differences and trade-offs between common synchronization mechanisms?

## Objectives

The project aims to:

1. Understand the need for synchronization in operating systems.
2. Demonstrate how race conditions occur during concurrent execution.
3. Implement mutual exclusion using mutexes.
4. Demonstrate resource control using semaphores.
5. Study the working principle of spinlocks.
6. Compare different synchronization mechanisms.
7. Measure and analyze their execution behavior.
8. Relate the practical implementations to synchronization concepts used in Linux.

## Concepts Covered

### Race Condition

A race condition occurs when multiple threads access shared data concurrently and the final result depends on the order in which the threads execute.

### Critical Section

A critical section is a portion of code that accesses a shared resource and must be protected from simultaneous execution by multiple threads.

### Mutex

A mutex provides mutual exclusion by allowing only one thread to enter a protected critical section at a time.

### Semaphore

A semaphore is a synchronization mechanism that uses a counter to control access to a shared resource or a limited number of resources.

### Spinlock

A spinlock is a synchronization mechanism in which a thread repeatedly checks whether a lock is available instead of immediately blocking.

Spinlocks are particularly useful in situations where the expected waiting time is very short and sleeping is undesirable.

## Project Architecture

```text
                  Concurrent Threads
                         |
             +-----------+-----------+
             |                       |
          Thread 1                Thread 2
             |                       |
             +-----------+-----------+
                         |
                  Shared Resource
                         |
                 Synchronization
                         |
        +----------------+----------------+
        |                |                |
      Mutex          Semaphore        Spinlock
        |                |                |
        +----------------+----------------+
                         |
                  Controlled Access
```

## Experiments

### 1. Race Condition

`race_condition.c` demonstrates what happens when multiple threads modify a shared variable without synchronization.

Multiple threads increment the same counter concurrently.

The expected result is:

```text
Expected counter = number of threads × increments per thread
```

However, the actual result can be lower because multiple threads may read and modify the shared variable at the same time.

This demonstrates the need for synchronization.

---

### 2. Mutex Synchronization

`mutex_demo.c` protects the shared counter using a POSIX mutex.

The basic execution flow is:

```text
Thread
   |
Lock Mutex
   |
Critical Section
   |
Unlock Mutex
```

Only one thread can execute the protected section at a time.

The experiment demonstrates how mutual exclusion prevents the race condition observed in the first experiment.

---

### 3. Semaphore Synchronization

`semaphore_demo.c` demonstrates synchronization using a POSIX semaphore.

Unlike a mutex, a semaphore can allow a controlled number of threads to access a resource simultaneously.

The experiment demonstrates how semaphores can be used for resource management and synchronization.

---

### 4. Spinlock

`spinlock_demo.c` demonstrates the basic concept of spin-based synchronization.

Instead of blocking while waiting for a lock, a thread repeatedly checks whether the lock has become available.

```text
             Lock unavailable
                    |
                    v
              Keep checking
                    |
                    v
             Lock available
                    |
                    v
             Enter critical
                section
```

The experiment helps illustrate the difference between blocking synchronization and busy waiting.

---

### 5. Benchmark

`benchmark.c` is used to compare the execution behavior of the synchronization mechanisms.

The benchmark focuses on factors such as:

- Execution time
- Synchronization overhead
- Correctness
- Waiting behavior
- Performance under concurrent execution

The results are stored in the `results/` directory.

## Comparison

| Mechanism | Main Purpose | Waiting Behavior | Typical Use |
|---|---|---|---|
| Mutex | Mutual exclusion | Blocking | Protecting critical sections |
| Semaphore | Resource/access control | Blocking | Managing limited resources |
| Spinlock | Mutual exclusion | Busy waiting | Very short critical sections |

## Project Structure

```text
Linux Synchronization Case Study/
│
├── README.md
├── .gitignore
├── Makefile
│
├── src/
│   ├── benchmark.c
│   ├── mutex_demo.c
│   ├── race_condition.c
│   ├── semaphore_demo.c
│   └── spinlock_demo.c
│
├── scripts/
│   ├── plot_results.py
│   └── run_all.sh
│
└── results/
```

## Technologies Used

- C
- Linux / Ubuntu
- POSIX Threads
- POSIX Semaphores
- GCC
- GNU Make
- Python
- Git

## Building the Project

The project uses a `Makefile` to simplify compilation.

Build the project using:

```bash
make
```

To remove compiled files:

```bash
make clean
```

## Running the Experiments

The individual programs can be executed after compilation.

For example:

```bash
./race_condition
```

```bash
./mutex_demo
```

```bash
./semaphore_demo
```

```bash
./spinlock_demo
```

The complete experiment set can also be executed using:

```bash
./scripts/run_all.sh
```

## Result Visualization

The project includes a Python script for visualizing benchmark results.

```bash
python3 scripts/plot_results.py
```

The generated results and plots are stored in the `results/` directory.

## Experimental Analysis

The experiments are designed to answer the following questions:

1. What happens when shared data is accessed without synchronization?
2. How does a mutex prevent a race condition?
3. How does a semaphore differ from a mutex?
4. Why would a spinlock be used instead of a blocking lock?
5. What synchronization overhead is introduced by each mechanism?
6. Which mechanism is appropriate for different types of workloads?

## Expected Observations

The race-condition experiment is expected to demonstrate that unsynchronized concurrent access can produce inconsistent results.

The mutex experiment should produce a consistent result by ensuring mutual exclusion.

The semaphore experiment should demonstrate controlled access to a shared resource.

The spinlock experiment should demonstrate busy waiting and its associated trade-offs.

The benchmark results will be used to compare the performance characteristics of the different mechanisms.

## Linux Relevance

Synchronization is an important part of operating-system design because the kernel and applications frequently perform concurrent operations.

Linux provides several synchronization primitives for handling concurrent execution, including mutexes, semaphores, spinlocks, and other locking mechanisms.

These mechanisms are used in different situations depending on factors such as:

- Critical-section duration
- Whether sleeping is allowed
- Contention
- Resource availability
- Performance requirements

This project focuses on the underlying concepts and demonstrates them through user-space programs.

## Work Distribution

### Vaishnav Sunil Nair

- Implemented the race-condition demonstration.
- Implemented mutex-based synchronization.
- Integrated the different project components.
- Maintained the project structure and README.
- Assisted with overall testing and documentation.

### Shreyas Nair

- Implemented the semaphore demonstration.
- Studied and documented semaphore-based synchronization.
- Conducted synchronization experiments.
- Collected and organized experimental results.
- Assisted with testing and analysis.

### Adithyadev B

- Implemented the spinlock demonstration.
- Developed the benchmarking component.
- Analyzed performance and synchronization overhead.
- Developed the result visualization script.
- Assisted with experimental analysis.

## Limitations

The implementations in this project are educational demonstrations and do not attempt to reproduce the complete internal implementation of Linux kernel synchronization primitives.

The experiments are performed in user space using POSIX threading and synchronization APIs.

## Conclusion

This case study demonstrates the importance of synchronization in concurrent systems.

By first reproducing a race condition and then applying mutexes, semaphores, and spinlocks, the project connects operating-system theory with practical experimentation.

The benchmark and result analysis provide an additional comparison of the mechanisms and their respective trade-offs.

## Academic Purpose

This project was developed as an Operating Systems case study to demonstrate the practical application of synchronization concepts in Linux.