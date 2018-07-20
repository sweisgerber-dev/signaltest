#include <iostream>
#include <signal.h>
#include <unistd.h>

// Statically reserved space for 25 Signal handler functions
const size_t SIZE_OLD_SIGNAL_HANDLERS = 25;
void (*old_signal_handlers[SIZE_OLD_SIGNAL_HANDLERS])(int) = { NULL };

/**
 Handler function logs & forwards the signals
 **/
void signal_handler_function (int parameter) {
  printf("%%%%[servicemanager]%%%% Signal Received: %d %%%%\n", parameter);
  if (parameter >= 0 && parameter < (int)SIZE_OLD_SIGNAL_HANDLERS) {
    printf("%%%%[servicemanager]%% Calling `%p?: %%%%\n", old_signal_handlers[parameter]);
    if (old_signal_handlers[parameter] != NULL) {
      old_signal_handlers[parameter](parameter);
    }
  }
}

int main() {
  // Fuzzing //////////////////////////////////////////////////////////////////////////////////////////
  printf(".: signaler :.\n");

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
  // void (*old_signal_handler)(int) = ;
  printf("- Registering `%lu` SignalHandlers\n", (sizeof(CATCH_SIGNALS) / sizeof(int32_t)));
  for (size_t i = 0; i < (sizeof(CATCH_SIGNALS) / sizeof(int32_t)); i++) {
    printf("  - Signal: `%d`\n", CATCH_SIGNALS[i]);
    // ServiceManager: %%[servicemanager]%% Signal Received: 2 %%
    // ServiceManager: %%[servicemanager]% Calling `0x0?: %%
    // ServiceManager: %%[servicemanager]%% Signal Received: 11 %%
    // ServiceManager: %%[servicemanager]% Calling `0x747b9b46bc60?: %%
    old_signal_handlers[CATCH_SIGNALS[i]] = signal(CATCH_SIGNALS[i], signal_handler_function);
  }
  // /Fuzzing /////////////////////////////////////////////////////////////////////////////////////////

  for(;;) {
    sleep(1);
  }

  return 0;
}