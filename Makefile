OBJS := q2.o test/q2_tests.o test/unity/src/unity.o
INC=-Itest/unity/src/ -Itest/../
CFLAGS=-Wall

# link
q2_tests: $(OBJS)
	gcc $(OBJS) -o q2_tests

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
%.o: %.c
	gcc -c $(CFLAGS) $(INC) $*.c -o $*.o
	gcc -MM $(CFLAGS) $(INC) $*.c > $*.d

# remove compilation products
clean:
	rm -f build *.o *.d