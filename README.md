# Echo Watchdog
**Echo Watchdog** is the Linux shell application that echos user's input text.
It can detect both user's input text and user's key presses.
The latter works only if the user is `root`, since the key press detection needs the system file access priviledges by the `root`.
The program terminates after three seconds if it does not detect any user's key actions.

## Structure
```
Echo Watchdog
├── Makefile             # Makefile for the application EchoWatchdog 
├── ewdshell.c           # C source for ewdshell
├── ewdshell.h           # C header for ewdshell
└── main.c               # Main source
```

## Technical Skills
### Synchronization Techniques
#### Mutex
**Mutex** (`pthread_mutex_t`) is a synchronization mechanism to avoid any race conditions for a multithread process.
It uses lock-and-unlock mechanism for specific statements.

#### Atomic Integer Type
**Atomic Integer Type** (`sig_atomic_t`) is an integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.

#### Thread Calcelation Cleaner
**Thread cancelation cleaner** works like a stack that pushes and pops specific actions.
It is used to perform any actions that need to be done when the thread is terminated, but actually fail to do because of the early thread cancelation.

##### `pthread_cleanup_push`
Pushes `routine` onto the top of the stack of clean-up handlers.

##### `pthread_cleanup_pop`
Pops `routine` and optionally executes if the argument is nonzero.
