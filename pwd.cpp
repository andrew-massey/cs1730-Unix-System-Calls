#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>

using namespace std;
/**
* pwd.cpp - finds the absolute path to the current directory and prints it
*/
int main(){
  char *pwd = get_current_dir_name();
  cout << pwd << endl;
  return EXIT_SUCCESS;
}
