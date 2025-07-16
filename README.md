# Echo Watchdog
**Echo Watchdog** is the Linux shell application that echos user's input text. It terminates after three seconds if it does not detect any user's input, or the user does not type anything. (Only if the user is `root`, getting the system file access priviledges. Because the key press detections need such priviledges.)

## Technical Skills
* Mutex (`pthread_mutex_t`): A synchronization mechanism to avoid any race conditions for a multithread process.
* Atomic Integer Type (`sig_atomic_t`): An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
* Thread Cancelation Cleaner: It works like a stack which pushes and pops specific actions. It is used to perform any actions that need to be done when the thread is terminated, but actually fails to do because of the thread cancelation before doing the action.
