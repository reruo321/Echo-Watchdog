# Echo Watchdog
**Echo Watchdog** is the Linux shell application that echos the user's keyboard input. It terminates after three seconds if it does not detect any user input, or the user does not type anything (if the user is `root`).

## Technical Stacks
* Mutex (`pthread_mutex_t`)
* Atomic Integer Type (`sig_atomic_t`): An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
* Thread Cancelation Cleaner
