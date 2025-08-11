# Echo Watchdog
**Echo Watchdog** is the Linux shell application that echos user's input text.
It can detect both user's input text and user's key presses.
The latter works only if the user is `root`, since the key press detection needs the system file access priviledges by the `root`.
The program terminates after three seconds if it does not detect any user's key actions.

## Technical Skills
* Mutex (`pthread_mutex_t`): A synchronization mechanism to avoid any race conditions for a multithread process. It uses lock-and-unlock mechanism for specific statements.
* Atomic Integer Type (`sig_atomic_t`): An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
* Thread Cancelation Cleaner: It works like a stack which pushes and pops specific actions. It is used to perform any actions that need to be done when the thread is terminated, but actually fail to do because of the early thread cancelation.

## Synchronization Techniques
### Mutex
`pthread_mutex_t`

### `sig_atomic_t`

### Thread Calcelation Cleaner
Thread cancelation cleaner works like a stack that pushes and pops specific actions.
#### `pthread_cleanup_push`
Pushes `routine` onto the top of the stack of clean-up handlers.

#### `pthread_cleanup_pop`
Optionally executes if the argument is nonzero.
