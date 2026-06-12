#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int pid1, pid2;
  printf("Starting test_lottery...\n");

  pid1 = fork();
  if (pid1 == 0) {
    settickets(100);
    for(volatile int i = 0; i < 50000000; i++) {} // High chance
    printf("Process 1 (100 tickets) finished.\n");
    exit(0);
  }

  pid2 = fork();
  if (pid2 == 0) {
    settickets(10);
    for(volatile int i = 0; i < 50000000; i++) {} // Low chance
    printf("Process 2 (10 tickets) finished.\n");
    exit(0);
  }

  wait(0);
  wait(0);
  printf("test_lottery completed.\n");
  exit(0);
}
