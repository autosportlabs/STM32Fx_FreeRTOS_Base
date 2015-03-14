#ifndef _SHELL_H_
#define _SHELL_H_

struct shell_cmd {
	char *command;
	void(*action)(int argc, char **argv);
	void(*help)(void);
};

#define SHELL_MAX_COMMANDS 10

void shell_init(void);
void shell_process(void *params);
int shell_register_command(struct shell_cmd *command);
#endif
