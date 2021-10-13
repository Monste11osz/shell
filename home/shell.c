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

int main()
{
        char **line = getlist();
        return 0;
}
