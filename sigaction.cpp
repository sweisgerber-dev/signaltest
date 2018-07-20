#include <iostream>
#include <unistd.h>

#include "asignal.h"

int main() {
  printf(".: sigaction :.\n");

  signal(SIGSEGV, test_handler_function);

  setup_signal_handling();

  for (;;) {
    sleep(1);
  }

}
