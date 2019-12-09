all: thread1 thread2 thread3

thread1:thread1.c
	cc -D_REENTERANT thread1.c -o thread1 -lpthread


thread2:thread2.c
	cc -D_REENTERANT thread2.c -o thread2 -lpthread

thread3:thread3.c
	cc -D_REENTERANT thread3.c -o thread3 -lpthread


clean:
	-rm -f thread1 thread2 thread3 *.o

