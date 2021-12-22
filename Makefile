bin/client: home/shell.c
	gcc home/shell.c -o shell -fsanitize=address

clean:
	rm home/shell.c
	rmdir home
