# 🚀 xv6-riscv OS: Advanced Kernel Architecture & Scheduling

![OS](https://img.shields.io/badge/OS-xv6-blue?style=for-the-badge&logo=linux)
![Architecture](https://img.shields.io/badge/Architecture-RISC--V-orange?style=for-the-badge&logo=riscv)
![Language](https://img.shields.io/badge/Language-C-brightgreen?style=for-the-badge&logo=c)
![Build](https://img.shields.io/badge/Build-Makefile-lightgrey?style=for-the-badge&logo=gnu)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

> **A deep-dive kernel engineering project based on MIT's xv6 operating system.**
> This repository showcases significant modifications to the xv6 kernel, bridging the gap between user-space applications and kernel-space hardware management. It features a completely redesigned, dynamically switchable CPU scheduling architecture supporting both **Deterministic (Priority)** and **Probabilistic (Lottery)** models.

---

## 📑 Table of Contents
1. [Project Objectives & Scientific Context](#-project-objectives--scientific-context)
2. [Phase 1: Kernel-User Boundary & Telemetry](#-phase-1-kernel-user-boundary--telemetry)
3. [Phase 2: Deterministic Priority Scheduling](#-phase-2-deterministic-priority-scheduling)
4. [Phase 3: Probabilistic Lottery Scheduling](#-phase-3-probabilistic-lottery-scheduling)
5. [Unified Architecture & Compilation](#-unified-architecture--compilation)
6. [Testing & Benchmarks](#-testing--benchmarks)
7. [How to Run](#-how-to-run)

---

## 🔬 Project Objectives & Scientific Context

The xv6 OS is a modern re-implementation of Unix V6 for the RISC-V architecture. By default, it employs a rudimentary Round-Robin scheduler with an $O(N)$ time complexity for process selection. 

**The goal of this project was to:**
* Extend the System Call Interface (SCI) to allow user-space applications to extract deep telemetry data from the Process Control Block (PCB).
* Address the limitations of Round-Robin by implementing a **Strict Preemptive Priority Scheduler**.
* Explore proportional-share resource allocation by engineering a **Lottery Scheduler** driven by a custom in-kernel Pseudo-Random Number Generator (PRNG).
* Master concurrency control by managing spinlocks (`acquire` / `release`) to prevent Race Conditions during context switches.

---

## 📊 Phase 1: Kernel-User Boundary & Telemetry

### The `getpinfo` System Call
To provide system transparency, a new syscall (`SYS_getpinfo`) was engineered. Instead of iterating through processes in user-space, the kernel safely constructs a snapshot of the process table and pushes it across the protection boundary.

**Key Technical Details:**
* **Struct `pinfo`:** A centralized data structure holding parallel arrays for PID, State, Memory Size, and Process Name.
* **Concurrency Lock:** The kernel acquires `p->lock` for each process before reading its state to ensure the data is not mutated by another CPU core during the read operation.
* **Memory Isolation:** The `copyout()` function is utilized to safely transfer the constructed `pinfo` struct from the kernel's physical memory to the user's virtual address space.
* **User Utility (`ps`):** A command-line tool that invokes the syscall and formats the output into a readable tabular matrix.

---

## ⚡ Phase 2: Deterministic Priority Scheduling

The default Round-Robin scheduler treats all processes equally, which is inefficient for real-time or critical tasks. We implemented an **Absolute Priority Scheduler**.

| Feature | Implementation Detail |
| :--- | :--- |
| **Priority Range** | `0` (Highest) to `100` (Lowest). Default: `50`. |
| **Selection Logic** | The scheduler scans the `proc` array, strictly selects the `RUNNABLE` process with the lowest numeric priority value. |
| **Tie-Breaking** | If multiple processes share the highest priority, CPU time is multiplexed among them using Round-Robin. |
| **Preemption** | If `setpriority()` assigns a stronger priority to a process, the current process immediately yields the CPU (`yield()`), triggering a context switch. |

---

## 🎲 Phase 3: Probabilistic Lottery Scheduling

To implement proportional-share scheduling without strict starvation, a **Lottery Scheduler** was introduced. Processes are granted "tickets", and the CPU is raffled off proportionally.

### In-Kernel PRNG (Linear Congruential Generator)
Because standard C libraries do not exist in kernel space, we engineered a fast, lightweight Linear Congruential Generator (LCG). The sequence of pseudo-random numbers is defined by the recurrence relation:

$$X_{n+1} = (a \cdot X_n + c + \text{entropy}) \pmod{m}$$

* **Multiplier ($a$):** `1664525`
* **Increment ($c$):** `1013904223`
* **Entropy:** The kernel's `ticks` variable is injected into the equation to ensure true non-deterministic behavior across boot cycles.

**Algorithm Flow:**
1. Sum all tickets of `RUNNABLE` processes.
2. Generate a random winning ticket: `winning_ticket = PRNG() % total_tickets`.
3. Iterate through processes, maintaining a running sum of tickets. The process that pushes the sum over the threshold wins the CPU.
4. **Inheritance:** Forked children inherently receive a copy of their parent's ticket count.

---

## 🏗️ Unified Architecture & Compilation

Rather than fragmenting the project into separate branches, the kernel features a **Unified Scheduler Loop**. We utilized C Preprocessor Directives (`#ifdef`, `#elif`) combined with Makefile flags to compile specific scheduling logic dynamically.

### Makefile Integration
The build system was extended to pass the `SCHEDULER` variable directly to the GCC compiler via `CFLAGS`:

```makefile
SCHEDULER ?= DEFAULT
CFLAGS += -DSCHEDULER_$(SCHEDULER)


📈 Testing & Benchmarks
The project includes custom user-space benchmarking suites to validate the theoretical models:

1. Priority Test (test_prio)
Spawns two concurrent CPU-bound tasks.

Process A: Priority 10

Process B: Priority 90

Result: The scheduler successfully starves Process B until Process A (the critical task) completes its execution, proving absolute preemption.

2. Lottery Test (test_lottery)
Spawns two concurrent tasks with disparate ticket allocations.

Process A: 100 Tickets (~90.9% win probability)

Process B: 10 Tickets (~9.1% win probability)

Result: Process A receives exponentially more CPU time slices over the execution window and terminates significantly faster, proving the mathematical accuracy of the proportional-share PRNG model.

🚀 How to Run
Ensure you have the RISC-V toolchain (riscv64-linux-gnu-gcc) and qemu-system-riscv64 installed.

Clone the repository:

Bash
git clone [https://github.com/amirpoori99/xv6-riscv-project.git](https://github.com/amirpoori99/xv6-riscv-project.git)
cd xv6-riscv-project
Boot with Default Scheduler (Round-Robin):

Bash
make qemu
Boot with Priority Scheduler:

Bash
make qemu SCHEDULER=PRIORITY
Boot with Lottery Scheduler (Single CPU recommended for accurate testing):

Bash
make qemu CPUS=1 SCHEDULER=LOTTERY
To exit QEMU, press Ctrl + A, release, then press X.

Developed as part of the Operating Systems course curriculum.