#include "ewdshell.h"

int main(){

	init_ewdshell();

	while(!f_is_exit){
		print_prompt();
		read_input();
	}

	exit_shell();
	return 0;
}
