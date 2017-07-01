src = $(wildcard *.c)
obj = $(src:.c=.o)

worker: $(obj)
	gcc -Wall -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) worker
