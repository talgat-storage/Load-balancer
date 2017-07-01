src = $(wildcard *.c)
obj = $(src:.c=.o)

handler: $(obj)
	gcc -Wall -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) handler
