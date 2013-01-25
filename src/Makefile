CC = gcc
CFLAGS =-std=c99 -lbcm2835 -lrt
DEPS = 
OBJ = get_temp.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

get_temp: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o get_temp 
