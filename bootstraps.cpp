#include <iostream>

#include <dlfcn.h>

#include "boilerplate.h"

using namespace std;

int main(int argc, char **argv) {

  bool keepgoing = true;

  while (keepgoing) {
    void* handle = dlopen("./boilerplate.so", RTLD_LAZY);
    
    void (*destroy)(Boilerplate*);
    Boilerplate* (*create)();
    
    create = (Boilerplate* (*)())dlsym(handle, "create_Boilerplate");
    destroy = (void (*)(Boilerplate*))dlsym(handle, "delete_Boilerplate");
    
    Boilerplate* bp = (Boilerplate*)create();
    keepgoing = bp->run();
    destroy(bp);

    dlclose(handle);
  }
  
  return 0;
}
