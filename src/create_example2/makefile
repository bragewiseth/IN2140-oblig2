.PHONY: all clean run



create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/create_2

run: create_2
	../bin/create_2


clean:
	rm -f ../bin/*