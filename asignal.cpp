//
// Created by weisgerber on 20.07.18.
//

#include "asignal.h"

#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <iostream>

stack_t my_signal_stack;
const static size_t SIZE_sigactions_old = 25;
typedef struct sigaction sigaction_t;
sigaction_t* sigactions_old = NULL;

static void sigaction_signalhandler(const int code, siginfo_t* const si, void* const sc) {
  printf("%%%%[sigaction_signalhandler]%%%% Signal Received: %d %%%%\n", code);
  if (code >= 0 && code < (int)SIZE_sigactions_old) {
    /* Call previous handler. */
    if (sigactions_old[code].sa_sigaction != NULL) {
      printf("-> calling: %p\n", sigactions_old[code].sa_sigaction);
      sigactions_old[code].sa_sigaction(code, si, sc);
    }
  }
}

void test_handler_function (int parameter) {
  printf("%%%%[OldHandler]%%%% Signal Received: %d %%%%\n", parameter);
}

extern "C" void setup_signal_stack() {
  // Allocate memory for the stack struct
  memset(&my_signal_stack, 0, sizeof(my_signal_stack));
  sigactions_old = (sigaction_t*)malloc(SIZE_sigactions_old * (sizeof *sigactions_old));

  /* Reserve the system default stack size.
     We don't need that much by the way. */
  my_signal_stack.ss_size = SIGSTKSZ;
  my_signal_stack.ss_sp = malloc(my_signal_stack.ss_size);
  my_signal_stack.ss_flags = 0;

  /* Install alternate stack size.
     Be sure the memory region is valid until you revert it. */
  if (my_signal_stack.ss_sp != NULL
      && sigaltstack(&my_signal_stack, NULL) == 0) {
    printf("ALT stack works: %p\n", &my_signal_stack);
  } else {
    printf("Could not install ALT stack");
  }
}

extern "C" void setup_signal_handling() {

  printf("setup_signal_handling()\n");

  setup_signal_stack();

  const int32_t CATCH_SIGNALS[] = {
      SIGHUP,
      SIGINT,
      SIGQUIT,
      SIGILL,
      SIGABRT,
      SIGFPE,
      SIGSEGV,
      SIGPIPE,
      SIGALRM,
      SIGTERM
  };
  memset(sigactions_old, 0, SIZE_sigactions_old);
  printf("- Registering `%d` SignalHandlers\n", (int)(sizeof(CATCH_SIGNALS) / sizeof(int32_t)));
  for (uint32_t i = 0; i < (sizeof(CATCH_SIGNALS) / sizeof(int32_t)); i++) {
    printf(" - Signal: `%d`\n", CATCH_SIGNALS[i]);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sigaction_signalhandler;
    sa.sa_flags = SA_SIGINFO;

    int error = sigaction(CATCH_SIGNALS[i], &sa, &sigactions_old[CATCH_SIGNALS[i]]);

    if (error == 0) {
      printf("  - sigaction(): `%d` address: %p DONE\n", CATCH_SIGNALS[i], sigactions_old[CATCH_SIGNALS[i]].sa_sigaction);
    } else {
      printf("  ! sigaction(): `%d` FAILED w/ error: `%d`/`%s` \n", CATCH_SIGNALS[i], errno, strerror(errno));
    }
  }

}