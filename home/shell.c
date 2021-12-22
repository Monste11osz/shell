#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define DEVIDED_PID -1

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

int exec_w(char **line)
{
	if(execvp(line[0], line) < 0)
	{
		perror("exec failed");
		free_line(line);
		return 1;
	}
	return 0;
}

/*void pip_mas_N(char **line, int n)
{
	char end;
	int fd[n][2], pid, size, i = 0, type = 0;
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
				close(fd[size][0]);
				close(fd[size][1]);
			}
			exec_w(line);
		}
		else
		{
			if(size != 0)
			{
				close(fd[size - 1][1]);
				close(fd[size - 1][0]);
			}
		}
		while(line[i] != NULL)
		{
			i++;
		}
		i++;
		type = i;
	}
	close(fd[size][1]);
	close(fd[size][0]);
	for(int j = 0; j < n + 1; j++)
	{
		wait(NULL);
	}
}*/

int pipe_N(char **line, int gen)
{
	int size;
	int fd[gen][2], pid;
	char ***triple = recording(line, gen);
	for(size = 0; size < gen + 1; size++)
	{
		if(size != gen)
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
			if(size != gen)
			{
				dup2(fd[size][1], 1);
				close(fd[size][0]);
				close(fd[size][1]);
			}

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
				close(fd[size - 1][1]);
				close(fd[size - 1][0]);
			}
		}
	}
	if(gen > 1)
	{
		close(fd[size][1]);
		close(fd[size][0]);
	}
	for(int i = 0; i < gen + 1; i++)
	{
		wait(NULL);
	}
	return 0;
}

void implementation(char **line, int kol_vo, int size)
{
	int status, k = 0;
	for(int i = 0; i <= kol_vo; i++)
	{
		if(fork() == 0)
		{
			if(execvp(line[k], line) < 0)
			{
				perror("exec failed");
				return;
			}
		}
		else
		{
			wait(&status);
			if(status != 0)
				return;
		}
		for(k; k <= size; k++)
		{
			if(line[k] == NULL)
			{
				//continue;
				k++;
				break;
			}
		}

	}
}

void implementation_or(char **line, int kol_vo_1, int size)
{
	int status, k = 0;
	for(int i = 0; i <= kol_vo_1; i++)
	{
		if(fork() == 0)
		{
			if(execvp(line[k], line) < 0)
			{
				perror("exec failed");
				return;
			}
		}
		else
		{
			wait(&status);
			if(status == 0)
				return;
		}
		for(k; k <= size; k++)
		{
			if(line[k] == NULL)
			{
				//continue;
				k++;
				break;
			}
		}

	}
}

int directory(char **line)
{
	int size = 0;
	char *getenv(const char *user);
	int setenv(const char *user, const char *value, int overwrite);
	while(line[size] != NULL)
        {
		if(strcmp(line[0], "cd") == 0)
		{
			if(line[size + 1] == NULL || strcmp(line[size + 1], "~") == 0)
			{
				chdir(getenv("home"));
				return 1;
			}
			else
			{
				chdir(line[size + 1]);
				return 1;
			}
		}
		size++;
	}
	return 0;
}

int check(char **line,  int *index)
{
	int fd = 0;
	int size = 0;
	while(line[size] != NULL)
	{
		if(strcmp(line[size], ">") == 0)
                {
			free(line[size]);
                        fd = open(line[size + 1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
                        if(fd < 0)
                        {
                                perror("Open failed");
				exit(1);
                        }
                       	*index = 1;
                       	free(line[size + 1]);
                       	line[size] = NULL;
                       	break;
               	}
               	else if(strcmp(line[size], "<") == 0)
               	{
			free(line[size]);
                       	fd = open(line[size + 1], O_RDONLY, S_IRUSR|S_IWUSR);
                       	if(fd < 0)
                       	{
                               	perror("Open failed");
				exit(1);
                       	}
                       	*index = 0;
                       	free(line[size + 1]);
                       	line[size] = NULL;
                       	break;
                }
		size++;
	}
	return fd;
}

int backgraund(char **line, int fon)
{
	int size = 0;
	while(line[size] != NULL)
	{
		if(strcmp(line[size], "&") == 0)
		{
			fon = 1;
			free(line[size]);
			line[size] = NULL;
			break;
		}
		size++;
	}
	return fon;
}

int check_and(char **line)
{
	int kol_vo = 0, size = 0;
	while(line[size] != NULL)
	{
		if(strcmp(line[size], "&&") == 0)
		{
			free(line[size]);
			line[size] = NULL;
			kol_vo++;
		}
		size++;
	}
	return kol_vo;
}

int check_or(char **line)
{
	int size = 0, kol_vo_1 = 0;
	while(line[size] != NULL)
	{
		if(strcmp(line[size], "||") == 0)
		{
			free(line[size]);
			line[size] = NULL;
			kol_vo_1++;
		}
		size++;
	}
	return kol_vo_1;
}

int check_pip(char **line)
{
	int size = 0, kol_vo_2 = 0;
	while(line[size] != NULL)
	{
		if(strcmp(line[size], "|") == 0)
                {
			kol_vo_2++;
                }
        	size++;
	}
	return kol_vo_2;
}

int f_fork(char **line, int *pid_in_phone, int fon_c)
{
	int fd, index, type = 0, pid;
	int size = 0, kol_vo_2, fon = 0, kol_vo, kol_vo_1;
	char end;
	fd = check(line, &index);
	if(directory(line) == 1)
	{
		return 0;
	}
	backgraund(line, fon);
	kol_vo = check_and(line);
	kol_vo_1 = check_or(line);
	kol_vo_2 = check_pip(line);
	if(kol_vo != 0)
	{
		implementation(line, kol_vo, size);
	}
	if(kol_vo_1 != 0)
	{
		implementation_or(line, kol_vo, size);
	}
	if(kol_vo_2 != 0)
	{
		pipe_N(line, kol_vo_2);
	}
	else
	{
		if(pid = fork() != 0)
		{
			if(fon == 0)
			{
				wait(NULL);
			}
			else
			{
				pid_in_phone = realloc(pid_in_phone, fon * sizeof(int));
				pid_in_phone[fon_c] = pid;
				fon_c++;
			}
		}
		else
		{
			if(fd > 0)
			{
				dup2(fd, index);
			}
			exec_w(line);
		}
	}
	if(fd > 0)
	{
		close(fd);
	}
	fd = 0;
	free(pid_in_phone);
	return 0;
}

void handler(int signo)
{
	int pid;
	puts("\nreseived SIGINT");
	if(pid != DEVIDED_PID)
	{
		kill(pid, SIGINT);
		pid = DEVIDED_PID;
	}
}

int main()
{
	int *pid_in_phone = NULL, fon_c = 0;
	char **line = getlist();
	signal(SIGINT, handler);
	while(strcmp(line[0], "exit") != 0 && strcmp(line[0], "quit") != 0)
	{
		f_fork(line, pid_in_phone, fon_c);
		free_line(line);
		wait(NULL);
		line = getlist();
	}
	free_line(line);
	for(int i = 0; i < fon_c; i++)
	{
		waitpid(pid_in_phone[i], NULL, 0);
	}
	return 0;
}
