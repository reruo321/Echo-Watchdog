#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <errno.h>

#define MAX_WARN_CNT 5
#define MAX_LEN 1000
#define EWD_TIMEOUT 3
#define USLEEP_INTV 100000

extern volatile sig_atomic_t f_is_key_pressed;
extern volatile sig_atomic_t f_is_alarm_set;
extern volatile sig_atomic_t f_is_exit;
extern volatile sig_atomic_t f_is_okay_to_cancel_t_kpre;
extern volatile sig_atomic_t f_is_t_kpre_canceled;
extern volatile sig_atomic_t f_is_root;

extern pthread_t t_wdog;
extern pthread_mutex_t mlock;
extern pthread_mutex_t mlock_keypress;
extern struct sigaction sa;

void alarm_handler(int sig);
void init_ewdshell();
void print_prompt();
void read_input();
void echo_input();
void* detect_keypress(void* arg);
void* watchdog_keypress(void* arg);
void exit_shell();
