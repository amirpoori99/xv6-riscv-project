#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int pid1, pid2;

  printf("Starting test_prio...\n");

  pid1 = fork();
  if (pid1 == 0) {
    setpriority(getpid(), 10); 
    for(int i = 0; i < 50000000; i++) {} // High priority task
    printf("Process 1 (Priority 10) finished.\n");
    exit(0);
  }

  pid2 = fork();
  if (pid2 == 0) {
    setpriority(getpid(), 90);
    for(int i = 0; i < 50000000; i++) {} // Low priority task
    printf("Process 2 (Priority 90) finished.\n");
    exit(0);
  }

  wait(0);
  wait(0);
  printf("test_prio completed.\n");
  exit(0);
}
