#include<stdio.h>

extern char **environ;
/**
*env.cpp - prints all of the environmental variables that are active for the user
*/
int main() {
  //counter for env variables
  int count = 1;
  //output char*
  char *out = *environ;

  for (; out; count++) {
    printf("%s\n", out);
    out = *(environ+count);
  }

  return 0;
}
