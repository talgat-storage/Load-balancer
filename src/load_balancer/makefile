src = $(wildcard *.c)
obj = $(src:.c=.o)

load_balancer: $(obj)
	gcc -Wall -pthread -o $@ $^

.PHONY: clean
clean:
	rm -f $(obj) load_balancer
