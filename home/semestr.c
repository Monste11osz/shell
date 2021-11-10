#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

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
	int i; //j = type + 1;
	for(i = 0; line[i]; i++)
	{
		free(line[i]);
	}
	free(line);
}


/*void pip_two(char **line, int type)
{
	int fd[2], i;
	i = type + 1;
	pipe(fd);
	if(fork() == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execvp(line[0], line);
	}
	if(fork() == 0)
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execvp(line[i], line);
	}
	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);
}*/

void freee_line(char ***triple)
{
	for(int i = 0; triple[i] != NULL; i++)
	{
		for(int j = 0; triple[i][j] != NULL; j++)
		{
			free(triple[i][j]);
		}
	}
	free(triple);
}

char ***recording(char **line, int n)
{
	int size = 0, out = 0;
	int pos = 0;
	int next_pos = 0;
	char ***triiple = NULL;
	triiple = (char***) malloc((n + 1) * sizeof(char**));
	while(line[size] != NULL)
	{
		if(strcmp(line[size], "|") == 0)
		{
			triiple[out] = (char**) malloc((size - next_pos + 1) * sizeof(char*));
			pos = size;
			for(int size = 0; size < pos - next_pos; size++)
			{
				triiple[out][size] = line[size + next_pos];
			}
			triiple[out][pos - next_pos] = NULL;
			next_pos = pos + 1;
			out++;
		}
		size++;
	}
	triiple[out] = (char**) malloc((size - next_pos + 1) * sizeof(char*));
	pos = size;
	for(int size = 0; size < pos - next_pos; size++)
	{
		triiple[out][size] = line[size + next_pos];
	}
	triiple[out][pos - next_pos] = NULL;
	return triiple;
}


void pipe_N(char **line, int n)
{
	int size;
	int fd[n][2], pid;
	char ***triple = recording(line, n);
	for(size = 0; size < n + 1; size++)
	{
		if(size != n)
		{
			pipe(fd[size]);
		}
		if((pid = fork()) == 0)
		{
			if(size != 0)
			{
				dup2(fd[size - 1][0], 0);
				close(fd[size - 1][0]);
				close(fd[size - 1][1]);
			}
			if(size != n)
			{
				dup2(fd[size][1], 1);
			}
			close(fd[size][0]);
			close(fd[size][1]);

			if(execvp(triple[size][0], triple[size]) < 0);
			{
				perror("exec failed");
				exit(1);
			}
		}
		else
		{
			if(size != 0)
			{
				close(fd[size - 1][0]);
			}
			close(fd[size][1]);
			waitpid(pid, NULL, 0);
		}
	}
}

int f_fork(char **line)
{
	int fd = 0, index = 0, type = 0;
	int size = 0, n = 0;
	char end;
	char *getenv(const char *user);
	int setenv(const char *user, const char *value, int overwrite);
	while(line[size] != NULL)
        {
		if(strcmp(line[size], "cd") == 0)
		{
			if(line[size + 1] == NULL || strcmp(line[size + 1], "~") == 0 || strcmp(line[size + 1], "..") == 0)
			{
				chdir(getenv("home"));
				return 0;
			}
			else
			{
				chdir(line[size + 1]);
				return 0;
			}
		}
                else if(strcmp(line[size], ">") == 0)
                {
			free(line[size]);
			line[size] = getword(&end);
                        fd = open(line[size + 1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
                        if(fd < 0)
                        {
                                perror("Open failed");
				return 1;
                        }
                       	index = 1;
                       	free(line[size + 1]);
                       	line[size] = NULL;
                       	break;
               	}
               	else if(strcmp(line[size], "<") == 0)
               	{
			free(line[size]);
			line[size] = getword(&end);
                       	fd = open(line[size + 1], O_RDONLY|O_TRUNC);
                       	if(fd < 0)
                       	{
                               	perror("Open failed");
                               	return 1;
                       	}
                       	index = 0;
                       	free(line[size + 1]);
                       	line[size] = NULL;
                       	break;
                }
                else if(strcmp(line[size], "|") == 0)
                {
			n++;
                        type = size;
			//free(line[size]);
			//line[size] = getword(&end);
                        //line[size] = NULL;
                	//break;
                }
        	size++;
        }
	size = 0;
	if(type != 0)
	{
		//pip_two(line, type);
		pipe_N(line, n);
	}
	else
	{
		if(fork() == 0)
		{
			if(fd > 0)
			{
				dup2(fd, index);
			}
			if(execvp(line[0], line) < 0)
			{
				perror("exec failed");
				free_line(line);
				return 1;
			}
		}
	}
	if(fd > 0)
	{
		close(fd);
	}
	fd = 0;
	return 0;
}

int main()
{
	char **line = getlist();
	while(strcmp(line[0], "exit") != 0 && strcmp(line[0], "quit") != 0)
	{
		f_fork(line);
		free_line(line);
		wait(NULL);
		line = getlist();
	}
	free_line(line);
	return 0;
}

