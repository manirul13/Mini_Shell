#ifndef MAIN_H
#define MAIN_H

#define SUCCESS 0
#define FAILURE -1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3

#define MAX_JOBS 64
#define MAX_ARGS 64

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

/* Job structure */
typedef struct
{
    pid_t pid;
    char cmd[256];
    int running; // 1 = running, 0 = stopped
} job_t;

/* Global jobs array */
extern job_t jobs[MAX_JOBS];
extern int job_count;

/* Function Prototypes */
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);
void copy_change(char *prompt, char *input_string);
int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands, int background, int use_pipe, char *pipe_file);

/* Job control */
void add_job(pid_t pid, char *cmd);
void remove_job(pid_t pid);
void list_jobs();
void bring_fg(int job_index);
void resume_bg(int job_index);

/* Named pipes */
void create_named_pipe(char *pipe_name);

#endif