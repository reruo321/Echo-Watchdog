#include "ewdshell.h"

volatile sig_atomic_t f_is_key_pressed;
volatile sig_atomic_t f_is_alarm_set;
volatile sig_atomic_t f_is_exit;
volatile sig_atomic_t f_is_okay_to_cancel_t_kpre;
volatile sig_atomic_t f_is_t_kpre_canceled;
volatile sig_atomic_t f_is_root;

char * input_line;
pthread_t t_kpre;
pthread_t t_wdog;
pthread_mutex_t mlock;
pthread_mutex_t mlock_keypress;
struct sigaction sa;

static void cleanup_glob(void* arg){
	glob_t* kbddev = (glob_t*)arg;
	globfree(kbddev);
}

static void cleanup_file(void* arg){
	FILE* kbd = (FILE*)arg;
	if(kbd)
		fclose(kbd);
}

void alarm_handler(int sig){
	f_is_exit = 1;
}

void init_ewdshell(){
	f_is_key_pressed = 1;
	f_is_alarm_set = 0;
	f_is_exit = 0;
	f_is_okay_to_cancel_t_kpre = 0;
	f_is_t_kpre_canceled = 0;
	f_is_root = 0;

	if(pthread_mutex_init(&mlock, NULL) != 0){
		perror("ERROR: Failed to pthread_mutex_init mlock");
		exit(1);
	}
	if(pthread_mutex_init(&mlock_keypress, NULL) != 0){
		perror("ERROR: Failed to pthread_mutex_init mlock_keypress");
		exit(1);
	}

	input_line = malloc((MAX_LEN + 1) * sizeof(char));
	if(!input_line){
		perror("ERROR: Failed to malloc input_line");
		exit(1);
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = alarm_handler;
	if(sigaction(SIGALRM, &sa, NULL) != 0 || sigaction(SIGINT, &sa, NULL) != 0){
		perror("ERROR: Failed to sigaction");
		exit(1);
	}

	if(geteuid() != 0){
		fprintf(stderr, "WARNING: Not running as root to properly detect key presses\n");
	}
	else{
		f_is_root = 1;
	}

	if(pthread_create(&t_kpre, NULL, detect_keypress, NULL) != 0){
		perror("ERROR: Failed to pthread_create t_kpre");
		exit(1);
	}
	if(pthread_create(&t_wdog, NULL, watchdog_keypress, NULL) != 0){
		perror("ERROR: Failed to pthread_create t_wdog");
		exit(1);
	}

	f_is_key_pressed = 0;
}

void print_prompt(){
	printf("INPUT >> ");
	fflush(stdout);
}

void read_input(){
	if(fgets(input_line, MAX_LEN + 1, stdin) == NULL){
		if(feof(stdin)){
			f_is_exit = 1;
			return;
		}
		if(errno == EINTR){
			return;
		}
		perror("ERROR: Failed to fgets");
		f_is_exit = 1;
		return;
	}

	input_line[strcspn(input_line, "\r\n")] = '\0';
	if(strlen(input_line) > MAX_LEN){
		fprintf(stderr, "WARNING: Input may have been truncated to %d characters\n", MAX_LEN);
	}

	pthread_mutex_lock(&mlock_keypress);
	f_is_key_pressed = 1;
	pthread_mutex_unlock(&mlock_keypress);
	
	if(!f_is_exit && strlen(input_line) > 0){
		echo_input();
	}
}

void echo_input(){
	printf("%s\n", input_line);
}

void* detect_keypress(void* arg){
	(void)arg;
	
	static int c_warn_glob = 0;
	static int c_warn_fopen = 0;
	static int c_warn_ioctl = 0;

	while(1){
		if(f_is_exit){
			f_is_okay_to_cancel_t_kpre = 1;
			return NULL;
		}

		if(!f_is_root){
			usleep(USLEEP_INTV);
			continue;
		}
		
		glob_t kbddev;

		if(glob("/dev/input/by-path/*-kbd", 0, 0, &kbddev) != 0){
			if(c_warn_glob < MAX_WARN_CNT){
				fprintf(stderr, "WARNING: No keyboard devices found\n");
				++c_warn_glob;
				if(c_warn_glob == MAX_WARN_CNT)
					fprintf(stderr, "Further WARNINGs for inexistent keyboard devices will be ignored\n");
			}
			usleep(USLEEP_INTV);
			continue;
		}
		pthread_cleanup_push(cleanup_glob, &kbddev);

		int keycode = -1;

		for(size_t i=0; i<kbddev.gl_pathc; ++i){
			FILE* kbd = fopen(kbddev.gl_pathv[i], "r");
			if(!kbd){
				if(c_warn_fopen < MAX_WARN_CNT){
					fprintf(stderr, "WARNING: Failed to open %s\n", kbddev.gl_pathv[i]);
					++c_warn_fopen;
					if(c_warn_fopen == MAX_WARN_CNT)
						fprintf(stderr, "Further WARNINGs for opening keyboard devices will be ignored\n");
				}
				continue;
			}

			char key_map[KEY_MAX/8 + 1];
			memset(key_map, 0, sizeof(key_map));
			
			int is_cleanup_needed = 0;
			pthread_cleanup_push(cleanup_file, kbd);

			if(ioctl(fileno(kbd), EVIOCGKEY(sizeof(key_map)), key_map) == -1){
				if(c_warn_ioctl < MAX_WARN_CNT){
					fprintf(stderr, "WARNING: Failed to query keyboard device %s\n", kbddev.gl_pathv[i]);
					++c_warn_ioctl;
					if(c_warn_ioctl == MAX_WARN_CNT)
						fprintf(stderr, "Further WARNINGs for querying keyboard devices will be ignored\n");
				}
				is_cleanup_needed = 1;
			}
			else{
				for(int k=0; k<KEY_MAX/8 + 1 && keycode < 0; ++k){
					for(int j=0; j<8; ++j){
						if(key_map[k] & (1 << j)){
							keycode = 8*k + j;
							break;
						}
					}
				}
			}
			pthread_cleanup_pop(is_cleanup_needed);
			if(!is_cleanup_needed){
				fclose(kbd);
			}

			if(keycode >= 0){
				pthread_mutex_lock(&mlock_keypress);
				f_is_key_pressed = 1;
				pthread_mutex_unlock(&mlock_keypress);
			}
		}
		pthread_cleanup_pop(1);
		usleep(USLEEP_INTV);
	}
	return NULL;
}

void* watchdog_keypress(void* arg){
	(void)arg;

	while(!f_is_exit){
		pthread_mutex_lock(&mlock);
		if(f_is_key_pressed){
			f_is_key_pressed = 0;
			alarm(0);
			f_is_alarm_set = 0;
		}
		else if(!f_is_alarm_set){
			f_is_alarm_set = 1;
			alarm(EWD_TIMEOUT);
		}
		pthread_mutex_unlock(&mlock);
		usleep(USLEEP_INTV);
	}
	if(f_is_okay_to_cancel_t_kpre && !f_is_t_kpre_canceled){
		pthread_cancel(t_kpre);
		f_is_t_kpre_canceled = 1;
	}
	return NULL;
}

void exit_shell(){
	fprintf(stderr, "TIMED-OUT EXIT: No keyboard press or input was detected for %d seconds. Bye!\n", EWD_TIMEOUT);
	free(input_line);
	
	if(!f_is_t_kpre_canceled)
		pthread_cancel(t_kpre);
	pthread_join(t_kpre, NULL);

	pthread_cancel(t_wdog);
	pthread_join(t_wdog, NULL);
	
	pthread_mutex_destroy(&mlock);
	pthread_mutex_destroy(&mlock_keypress);
	
	exit(0);
}
