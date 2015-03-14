#include <stdio.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <stdbool.h>
#include <util.h>
#include <stdio.h>
#include <shell.h>

char *shell_prompt_text = "> ";
char *shell_banner = "\n\nCommand Shell";

struct shell_state {
	/* +1 for the help command */
	struct shell_cmd commands[SHELL_MAX_COMMANDS + 1];
	int command_count;
	char input_buf[32];
	int input_pos;
	bool initialized;
	xSemaphoreHandle shell_lock;
} shell_state;

static bool shell_is_whitespace(char c)
{
	switch(c) {
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		return true;
	default:
		break;
	}
	return false;
}

static void shell_exec_cmd(int argc, char **argv)
{
	unsigned int cmd_len;
	unsigned int tgt_cmd_len = strlen(argv[0]);
	int i;
	for (i = 0; i < shell_state.command_count; i++) {
		cmd_len = strlen(shell_state.commands[i].command);

		if (cmd_len == tgt_cmd_len && shell_state.commands[i].action &&
		    strncmp(argv[0], shell_state.commands[i].command, tgt_cmd_len) == 0) {
			shell_state.commands[i].action(argc, argv);
			putchar('\n');
			return;
		}
	}
	printf("Invalid Command!\n");
}

static void shell_parse_command(void)
{
	int i;
	int arg_count = 0;
	char *arg_list[8] = {NULL};
	bool in_arg = false;

	/* TODO: Fix the bug where I pass everything as args...*/
	for(i = 0; i < shell_state.input_pos; i++) {
		if (shell_is_whitespace(shell_state.input_buf[i]) && arg_count == 0)
			continue;

		if (shell_is_whitespace(shell_state.input_buf[i])) {
			if (in_arg) {
				shell_state.input_buf[i] = 0x00;
				in_arg = false;
			}
		} else if (!in_arg) {
			arg_list[arg_count] = &shell_state.input_buf[i];
			arg_count++;
			in_arg = true;
		}
	}
	shell_state.input_buf[i] = 0x00;

	if (arg_count)
		shell_exec_cmd(arg_count, arg_list);
}

static int shell_process_input(void)
{
	char c;

	c = (char)getchar();

	switch(c) {
	case '\r': /*Fall through*/
	case '\n':
		putchar('\n');
		/* handle command here */
		shell_parse_command();

		memset(shell_state.input_buf, 0x00, ARRAY_SIZE(shell_state.input_buf));
		shell_state.input_pos = 0;
		printf("%s", shell_prompt_text);
		break;
	case '\b':
		if (shell_state.input_pos > 0) {
			shell_state.input_pos--;
			putchar('\b');
			putchar(' ');
			putchar('\b');
		}
		break;
	default:
		/* we've overrun our buffer */
		if ((unsigned)shell_state.input_pos >= ARRAY_SIZE(shell_state.input_buf)) {
			printf("Invalid command!\n");
			shell_state.input_pos = 0;
			memset(shell_state.input_buf, 0x00, ARRAY_SIZE(shell_state.input_buf));
			return -1;
		}
		shell_state.input_buf[shell_state.input_pos] = c;
		shell_state.input_pos++;
		putchar(c);
		break;
	}
	return 0;
}

static void shell_help_help(void)
{
	printf("help - Show this dialog\n");
}

static void shell_help_handler(int argc, char **argv)
{
	unsigned int i;

	printf("Shell commands: (case sensitive!)\n");

	for(i = 0; i < ARRAY_SIZE(shell_state.commands); i++){

		/* If there is a help command, execute it*/
		if (shell_state.commands[i].help) {
			shell_state.commands[i].help();
			continue;
		}

		/* Otherwise, try to print out the command name*/
		if (shell_state.commands[i].command[0] > 0) { /* i.e: there is a string there */
			printf("%s\n", shell_state.commands[i].command);
		} else {
			/* This is the end of the command array*/
			return;
		}
	}
}

int shell_register_command(struct shell_cmd *cmd)
{
	int ret = 0;

	xSemaphoreTake(shell_state.shell_lock, portMAX_DELAY);

	/* If we're full, there is no action to perform, or the command instruction isn't present,
	 * return an error */
	if (shell_state.command_count >= SHELL_MAX_COMMANDS || !cmd->action || cmd->command[0] == 0x00) {
		ret = -1;
		goto cleanup_and_return;
	}

	/* Copy the command into place and increment command counter */
	memcpy(&shell_state.commands[shell_state.command_count], cmd, sizeof(struct shell_cmd));
	shell_state.command_count++;

cleanup_and_return:
	xSemaphoreGive(shell_state.shell_lock);
	return ret;
}

/* Debug Shell */
void shell_process(void* params)
{
	printf("%s\n%s", shell_banner, shell_prompt_text);
	while(1) {
		shell_process_input();
	}
}

void shell_init(void)
{
	static struct shell_cmd help_cmd = {
		.command = "help",
		.action = shell_help_handler,
		.help = shell_help_help, /* This name is stupid */
	};

	vSemaphoreCreateBinary(shell_state.shell_lock);

	shell_register_command(&help_cmd);

	xTaskCreate(shell_process,
		    (signed portCHAR *) "Shell",
		    configMINIMAL_STACK_SIZE + 100,
		    NULL,
		    (tskIDLE_PRIORITY + 2),
		    NULL);

	shell_state.initialized = true;
}
