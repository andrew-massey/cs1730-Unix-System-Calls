#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(){
  char *pwd = get_current_dir_name();
  cout << pwd << endl;
  return EXIT_SUCCESS;
}
