
#ifndef SIGNALTEST_ASIGNAL_H
#define SIGNALTEST_ASIGNAL_H

#include <signal.h>

static void sigaction_signalhandler(const int code, siginfo_t* const si, void* const sc);
extern "C" void setup_signal_stack();
extern "C" void setup_signal_handling();

extern "C" void test_handler_function (int parameter);

#endif //SIGNALTEST_ASIGNAL_H
