# Load-balancer

Ultimate Goal: Build a distributed key-value storage system

High-level functions:
- Store/retrieve data as a key-value pair
- Handle bulk requests with I/O multiplexing and load balancing
- Write logs and provide CLIs to interact with users

Operations
- Request delivery to a specific handler according to a Round-robin strategy
- Request delivery to a specific worker and response delivery to the load balancer
- All handlers should eventually know all key-value distributions

1. Run ./install.sh

2. Go to the directory ./bin

3. Open the info_controls.txt

4. Change the number to 0

5. Open the info_workers.txt

6. Change the IP addresses of workers and ports as you wish.
   You can have as many workers as you wish.

7. Open the info_handlers.txt

8. Change the IP addresses of handlers and ports as you wish.
   You can have as many handlers as you wish.

9. Run workers in different terminals with inputs provided as follows:
   ./worker <IP port to listen to> <worker ID>

10. Run handlers in different terminals as follows:
    ./handler

11. Run the load balancer as follows:
    ./load_balancer

12. Run clients in different terminals as follows:
    ./client

    You can have as many clients as you wish.

13. Enjoy!
