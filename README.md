# Echo Watchdog
**Echo Watchdog** is the Linux shell application that echos user's any input text. It terminates after three seconds if it does not detect any user's input, or the user does not type anything. (Only if the user is `root`)

## Technical Skills
* Mutex (`pthread_mutex_t`): An algorithm to avoid any race conditions for a multithread process.
* Atomic Integer Type (`sig_atomic_t`): An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
* Thread Cancelation Cleaner: It works like a stack which pushes and pops specific actions. It is used to perform any actions that needs to be done when the thread is terminated but actually doesn't because of thread cancelation.
