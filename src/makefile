.PHONY: all clean run_check_disk run_create_fs



check_disk: allocation.c
	gcc -g -Wall -Wextra -std=gnu11 check_disk.c allocation.c -o ../bin/check_disk

create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/create_fs


run_check_disk: check_disk
	../bin/check_disk

run_create_fs: create_fs
	../bin/create_fs


clean:
	rm -f ../bin/*