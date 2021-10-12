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
        // one of the methods
        /*if(*end == '\n')
        {
                return NULL;
        }

        ch = getchar();
        if(ch == '\n')
        {
                *end = ch;
                return NULL;
        }*/
        ch = getchar();
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
        /*char *word = NULL;
        word = getword(&lastchar);
        do
        {
                if(word[0] != ' ' && word[0] != '\t' && word[0] != '\n')
                {
                        bytes = (size + 1) * sizeof(char*);
                        str = realloc(str, bytes);
                        str[size] = word;
                        size++;
                }
                word = getword(&lastchar);
        }while(lastchar != '\n');*/
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

int main()
{
        char **line = getlist();
        free_line(line);
        return 0;
}
