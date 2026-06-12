#define NPROC_MAX 64

struct pinfo {
  int pid[NPROC_MAX];
  int state[NPROC_MAX];
  int sz[NPROC_MAX];
  char name[NPROC_MAX][16];
  int nproc;
};
