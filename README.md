# Echo Watchdog
**Echo Watchdog** is the Linux shell application that echos user's input text.
It can detect both user's input text and user's key presses.
The latter works only if the user is `root`, since the key press detection needs the system file access priviledges.
It terminates after three seconds if it does not detect any user's key actions.

## Technical Skills
* Mutex (`pthread_mutex_t`): A synchronization mechanism to avoid any race conditions for a multithread process. It uses lock-and-unlock mechanism for specific statements.
* Atomic Integer Type (`sig_atomic_t`): An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
* Thread Cancelation Cleaner: It works like a stack which pushes and pops specific actions. It is used to perform any actions that need to be done when the thread is terminated, but actually fail to do because of the early thread cancelation.
