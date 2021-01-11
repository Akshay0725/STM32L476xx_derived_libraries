OBJ:=$(patsubst %.c, %.o, $(wildcard *.c))

main:$(OBJ)
	gcc -o $@ $^
	
clean:
	rm *.o main