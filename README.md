# 🚀 xv6-riscv OS: Custom System Calls & Unified Schedulers

![OS](https://img.shields.io/badge/OS-xv6-blue)
![Architecture](https://img.shields.io/badge/Architecture-RISC--V-orange)
![Language](https://img.shields.io/badge/Language-C-brightgreen)
![Build](https://img.shields.io/badge/Build-Makefile-lightgrey)

> **A Comprehensive Educational Operating System Project based on MIT's xv6.** > This project extends the core functionalities of the xv6-riscv operating system by introducing new system calls, concurrent process monitoring, and a unified, multi-algorithm CPU scheduling architecture.

---

## 🎯 Project Overview
The primary objective of this project is to dive deep into the kernel space of a Unix-like operating system to understand and manipulate Process Control Blocks (PCBs), concurrency locks, and CPU scheduling algorithms. The default Round-Robin scheduler has been completely overhauled to support **Absolute Priority** and **Probabilistic Lottery** scheduling natively, switchable at compile-time.

---

## 🛠️ Key Features & Implementation Details

### 1. Process Monitoring (The `getpinfo` System Call)
To monitor the system state, a new system call (`SYS_getpinfo`) and a user-space utility (`ps`) were implemented.
* **Architecture:** Uses a centralized `struct pinfo` to gather data across the `proc` array.
* **Concurrency Safety:** Implements safe read-operations by acquiring process-specific spinlocks (`acquire(&p->lock)`) before accessing PCB data to prevent Race Conditions.
* **Memory Isolation:** Uses `copyout` to safely transfer the populated struct from kernel space to user space.

### 2. Priority Scheduling
Replaced the default Round-Robin with a strict Priority-based algorithm.
* **Mechanics:** Processes are assigned a default priority of `50` (Range: 0-100, Lower = Higher Priority).
* **Tie-Breaking:** If multiple processes share the highest priority, the scheduler falls back to Round-Robin among them to ensure fairness.
* **Preemption:** The `setpriority` syscall forces a `yield()` if the newly assigned priority is lower (stronger) than the old one, ensuring immediate CPU handover.

### 3. Lottery Scheduling (Probabilistic Allocation) 🎲
Implemented a purely mathematical scheduling algorithm where CPU time is distributed based on the proportion of "tickets" a process holds.
* **Ticket Inheritance:** Child processes inherit their parent's ticket count upon `fork()`.
* **PRNG Implementation:** Since standard libraries are unavailable in the kernel, a lightweight **Linear Congruential Generator (LCG)** was implemented.
* **Entropy:** The kernel's `ticks` variable is used to inject entropy into the PRNG seed, ensuring true randomness during context switches.

---

## ⚙️ The Unified Scheduler Architecture
One of the major engineering feats of this project is the **Unified Scheduler**. Instead of maintaining different branches, the kernel uses C Preprocessor Directives (`#ifdef`) linked directly to the `Makefile`.

### Compilation Flags
You can compile and boot the OS with your desired scheduler by passing the `SCHEDULER` flag:

```bash
# 1. Default Round-Robin Scheduler
make qemu

# 2. Absolute Priority Scheduler
make qemu SCHEDULER=PRIORITY

# 3. Lottery Scheduler
make qemu SCHEDULER=LOTTERY
