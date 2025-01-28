CC = gcc

shell: shell.c
	$(CC) shell.c -o shell

clean:
	rm -f shell
