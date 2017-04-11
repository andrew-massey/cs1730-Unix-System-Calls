#include<stdio.h>

extern char **environ;

int main() {
  //i will be count
  int count = 1;
  //s is out
  char *out = *environ;

  for (; out; count++) {
    printf("%s\n", out);
    out = *(environ+count);
  }

  return 0;
}
