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
#include <ctype.h>

static const char *shell_prompt_text = "> ";
static const char *shell_banner = "\n\nCommand Shell";

#define MAX_ARGS 8
#define INPUT_BUF_SIZE 32

static struct {
	/* +1 for the help command */
	struct shell_cmd commands[SHELL_MAX_COMMANDS + 1];
	unsigned int command_count;
	char input_buf[INPUT_BUF_SIZE];
	unsigned int input_pos;
	bool initialized;
	xSemaphoreHandle registration_lock;
} shell_state;

static void shell_exec_cmd(int argc, char **argv)
{
	unsigned int tgt_cmd_len = strlen(argv[0]);
	unsigned int i;
	for (i = 0; i < shell_state.command_count; i++) {
		const struct shell_cmd *shell_cmd = &shell_state.commands[i];
		unsigned int cmd_len = strlen(shell_cmd->command);

		if (cmd_len == tgt_cmd_len &&
		    strncmp(argv[0], shell_cmd->command, tgt_cmd_len) == 0) {
			shell_cmd->action(argc, argv);
			putchar('\n');
			return;
		}
	}
	printf("Invalid Command!\n");
}

static void shell_parse_command(void)
{
	unsigned int i;
	unsigned int arg_count = 0;
	char *arg_list[MAX_ARGS];
	bool in_arg = false;

	/* TODO: Fix the bug where I pass everything as args...*/
	for (i = 0; i < shell_state.input_pos; i++) {
		if (isspace((int)shell_state.input_buf[i])) {
			if (in_arg) {
				shell_state.input_buf[i] = '\0';
				in_arg = false;
			}
		} else if (!in_arg && arg_count < MAX_ARGS) {
			arg_list[arg_count++] = &shell_state.input_buf[i];
			in_arg = true;
		}
	}
	shell_state.input_buf[i] = 0x00;

	if (arg_count)
		shell_exec_cmd(arg_count, arg_list);
}

static void shell_process_input(void)
{
	char c;
	int result;

	result = getchar();
	if (result < 0)
		return;

	c = (char)result;

	switch (c) {
	case '\r': /*Fall through*/
	case '\n':
		putchar('\n');

		/* handle command here */
		if (shell_state.input_pos > 0)
			shell_parse_command();

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
		/* we've overrun our buffer (we leave one extra byte
		 * for string termination) */
		if (shell_state.input_pos >= ARRAY_SIZE(shell_state.input_buf) - 1) {
			printf("Invalid command!\n");
			shell_state.input_pos = 0;
			memset(shell_state.input_buf, 0x00, ARRAY_SIZE(shell_state.input_buf));
			return;
		}
		shell_state.input_buf[shell_state.input_pos] = c;
		shell_state.input_pos++;
		putchar(c);
		break;
	}
}

static void shell_help_help(void)
{
	printf("help - Show this dialog\n");
}

static void shell_help_handler(int argc, char **argv)
{
	unsigned int i;

	printf("Shell commands: (case sensitive!)\n");

	for (i = 0; i < ARRAY_SIZE(shell_state.commands); i++) {

		/* If there is a help command, execute it*/
		if (shell_state.commands[i].help) {
			shell_state.commands[i].help();
			continue;
		}

		/* Otherwise, try to print out the command name*/
		if (shell_state.commands[i].command[0] > 0) { /* i.e: there is a string there */
			printf("%s\n", shell_state.commands[i].command);
		}
	}
}

int shell_register_command(const struct shell_cmd *cmd)
{
	int ret = 0;

	xSemaphoreTake(shell_state.registration_lock, portMAX_DELAY);

	/* If we're full, there is no action to perform, the
	 * command instruction isn't present, or we're not initialized
	 * return an error */
	if (!shell_state.initialized ||
	    shell_state.command_count >= SHELL_MAX_COMMANDS ||
	    !cmd->action ||
	    cmd->command[0] == 0x00) {
		ret = -1;
		goto cleanup_and_return;
	}

	/* Copy the command into place and increment command counter */
	memcpy(&shell_state.commands[shell_state.command_count], cmd, sizeof(struct shell_cmd));
	shell_state.command_count++;

cleanup_and_return:
	xSemaphoreGive(shell_state.registration_lock);
	return ret;
}

/* Debug Shell */
static void shell_process(void* params)
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

	vSemaphoreCreateBinary(shell_state.registration_lock);

	shell_state.initialized = true;

	shell_register_command(&help_cmd);

	xTaskCreate(shell_process,
		    (signed portCHAR *) "Shell",
		    configMINIMAL_STACK_SIZE + 100,
		    NULL,
		    (tskIDLE_PRIORITY + 2),
		    NULL);

}
