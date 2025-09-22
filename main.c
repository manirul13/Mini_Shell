#include "main.h"

int main(int argc, char *argv[]) {
    char prompt[64] = ANSI_COLOR_GREEN "myshell> " ANSI_COLOR_RESET; 
    char input_string[1024];
    char *command;
    int cmd_type;
    int status = 0;

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    while (1) {
        scan_input(prompt, input_string);

        if (strlen(input_string) == 0)
            continue;

        copy_change(prompt, input_string);

        command = get_command(input_string);
        cmd_type = check_command_type(command);

        /* Background check */
        int background = 0;
        if (input_string[strlen(input_string) - 1] == '&') {
            background = 1;
            input_string[strlen(input_string) - 1] = '\0';
        }

        /* Pipe check (Named Pipe) */
        int use_pipe = 0;
        char pipe_file[256] = "";
        char *pipe_pos = strchr(input_string, '|');
        if (pipe_pos) {
            use_pipe = 1;
            *pipe_pos = '\0';
            strcpy(pipe_file, pipe_pos + 1);
            while (*pipe_file == ' ') memmove(pipe_file, pipe_file + 1, strlen(pipe_file));
        }

        if (cmd_type == BUILTIN) {
            if (strncmp(command, "echo", 4) == 0)
                echo(input_string, status);
            else
                execute_internal_commands(input_string);
        }
        else if (cmd_type == EXTERNAL) {
            char *args[MAX_ARGS];
            int i = 0;
            char *token = strtok(input_string, " ");
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;
            extract_external_commands(args, background, use_pipe, pipe_file);
        }
    }
    return 0;
}
