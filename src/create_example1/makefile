.PHONY: all clean run



create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/create_1

run: create_1
	../bin/create_1


clean:
	rm -f ../bin/*