#include "main.h"

job_t jobs[MAX_JOBS];
int job_count = 0;

/* ================= BASIC INPUT ================= */
void scan_input(char *prompt, char *input_string)
{
    printf("%s", prompt);
    fflush(stdout);
    __fpurge(stdin);
    if (fgets(input_string, 1024, stdin) != NULL)
        input_string[strcspn(input_string, "\n")] = '\0';
}

char *get_command(char *input_string)
{
    static char command[128];
    int i = 0;
    while (input_string[i] != ' ' && input_string[i] != '\0')
        command[i] = input_string[i++];
    command[i] = '\0';
    return command;
}

void copy_change(char *prompt, char *input_string)
{
    if (strncmp(input_string, "PS1=", 4) == 0)
        strcpy(prompt, input_string + 4);
}

/* ================= COMMAND TYPE ================= */
int check_command_type(char *command)
{
    char *builtins[] = {"cd", "mkdir", "rmdir", "sleep", "exit", "pwd", "echo", "jobs", "fg", "bg", "mkfifo", NULL};
    for (int i = 0; builtins[i] != NULL; i++)
        if (strcmp(command, builtins[i]) == 0)
            return BUILTIN;

    if (strlen(command) == 0)
        return NO_COMMAND;

    return EXTERNAL;
}

/* ================= BUILTIN COMMANDS ================= */
void echo(char *input_string, int status)
{
    if (strcmp(input_string, "echo $$") == 0)
        printf("%d\n", getpid());
    else if (strcmp(input_string, "echo $?") == 0)
        printf("%d\n", status);
    else if (strcmp(input_string, "echo $SHELL") == 0)
        printf("%s\n", getenv("SHELL") ? getenv("SHELL") : "myshell");
    else
        printf("%s\n", input_string + 5);
}

void execute_internal_commands(char *input_string)
{
    char *command = strtok(input_string, " ");

    if (strcmp(command, "cd") == 0)
    {
        char *path = strtok(NULL, " ");
        if (!path)
            path = getenv("HOME");
        if (chdir(path) != 0)
            perror("cd");
    }
    else if (strcmp(command, "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
    }
    else if (strcmp(command, "mkdir") == 0)
    {
        char *dir = strtok(NULL, " ");
        if (dir && mkdir(dir, 0777) != 0)
            perror("mkdir");
    }
    else if (strcmp(command, "rmdir") == 0)
    {
        char *dir = strtok(NULL, " ");
        if (dir && rmdir(dir) != 0)
            perror("rmdir");
    }
    else if (strcmp(command, "sleep") == 0)
    {
        char *sec = strtok(NULL, " ");
        if (sec)
            sleep(atoi(sec));
    }
    else if (strcmp(command, "jobs") == 0)
    {
        list_jobs();
    }
    else if (strcmp(command, "fg") == 0)
    {
        char *idx = strtok(NULL, " ");
        if (idx)
            bring_fg(atoi(idx));
    }
    else if (strcmp(command, "bg") == 0)
    {
        char *idx = strtok(NULL, " ");
        if (idx)
            resume_bg(atoi(idx));
    }
    else if (strcmp(command, "mkfifo") == 0)
    {
        char *name = strtok(NULL, " ");
        if (name)
            create_named_pipe(name);
    }
    else if (strcmp(command, "exit") == 0)
    {
        exit(0);
    }
}

/* ================= JOB CONTROL ================= */
void add_job(pid_t pid, char *cmd)
{
    if (job_count < MAX_JOBS)
    {
        jobs[job_count].pid = pid;
        strcpy(jobs[job_count].cmd, cmd);
        jobs[job_count].running = 1;
        job_count++;
    }
}

void remove_job(pid_t pid)
{
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].pid == pid)
        {
            for (int j = i; j < job_count - 1; j++)
                jobs[j] = jobs[j + 1];
            job_count--;
            break;
        }
    }
}

void list_jobs()
{
    for (int i = 0; i < job_count; i++)
    {
        printf("[%d] %s %s\n", i, jobs[i].running ? "Running" : "Stopped", jobs[i].cmd);
    }
}

void bring_fg(int job_index)
{
    if (job_index >= 0 && job_index < job_count)
    {
        tcsetpgrp(STDIN_FILENO, jobs[job_index].pid);
        kill(jobs[job_index].pid, SIGCONT);
        waitpid(jobs[job_index].pid, NULL, 0);
        remove_job(jobs[job_index].pid);
    }
}

void resume_bg(int job_index)
{
    if (job_index >= 0 && job_index < job_count)
    {
        kill(jobs[job_index].pid, SIGCONT);
        jobs[job_index].running = 1;
    }
}

/* ================= NAMED PIPE ================= */
void create_named_pipe(char *pipe_name)
{
    if (mkfifo(pipe_name, 0666) == -1)
        perror("mkfifo");
}

/* ================= SIGNAL HANDLER ================= */
void signal_handler(int sig_num)
{
    if (sig_num == SIGINT)
        printf("\nCaught SIGINT (Ctrl+C)\n");
    else if (sig_num == SIGTSTP)
        printf("\nCaught SIGTSTP (Ctrl+Z)\n");
}

/* ================= EXTERNAL COMMAND*/
