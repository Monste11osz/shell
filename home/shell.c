#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

char *getword(char *end)
{
        int size = 0, bytes;
        char *word = NULL, ch;
        if(*end == '\n')
        {
                return NULL;
        }

        ch = getchar();
        if(ch == '\n')
        {
                *end = ch;
                return NULL;
        }
        while(size == 0 && ch == ' ')
        {
                if(ch == '\n')
                {
                        *end = ch;
                        return NULL;
                }
                ch = getchar();
        }
        //ch = getchar();
        while(ch != ' ' && ch != '\t' && ch != '\n')
       {
                bytes = (size + 1) * sizeof(char);
                word = realloc(word, bytes);
                word[size] = ch;
                size++;
                ch = getchar();
        }
        bytes = (size + 1) * sizeof(char);
        word = realloc(word, bytes);
        word[size] = '\0';
        *end = ch;
        return word;
}



char **getlist()
{
        int size = 0, bytes;
        char **str = NULL, lastchar = 0;
        while(lastchar != '\n')
        {
                bytes = (size + 1) * sizeof(char*);
                str = realloc(str, bytes);
                str[size] = getword(&lastchar);
                size++;
        }
        bytes = (size + 1) * sizeof(char*);
        str = realloc(str, bytes);
        str[size] = NULL;
        return str;
}

void free_line(char **line)
{
        int i;
        for(i = 0; line[i]; i++)
        {
                free(line[i]);
        }
        free(line);
}

int f_fork(char **line)
{
	while(strcmp(line[0], "exit") != 0 && strcmp(line[0], "quit") != 0)
        {
                if(fork() == 0)
                {
                        if(execvp(line[0], line) < 0)
                        {
                                perror("exec failed");
                                free_line(line);
                                return 1;
                        }
                }
                free_line(line);
                wait(NULL);
                line = getlist();
        }
        free_line(line);
        return 0;
}

int main()
{
        char **line = getlist();
	f_fork(line);
        return 0;
}
