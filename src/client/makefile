src = $(wildcard *.c)
obj = $(src:.c=.o)

client: $(obj)
	gcc -Wall -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) client
