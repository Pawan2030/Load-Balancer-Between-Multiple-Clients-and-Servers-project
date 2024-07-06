# Load-Balancer-Between-Multiple-Clients-and-Servers

## Overview

This project demonstrates a simple load balancer using the round-robin algorithm to distribute client requests across multiple servers. The setup includes three servers and six clients managed by a central load balancer.

## Project Structure

- **load_balancer/**
  - `load_balancer.cpp`: Implements the load balancer that distributes client requests to the servers.
  
- **server/**
  - `server1.cpp`, `server2.cpp`, `server3.cpp`: Each server listens on different ports (8080, 8081, 8082 respectively) to handle incoming requests.
  
- **client/**
  - `client1.cpp` to `client6.cpp`: Simulate clients that connect to the load balancer and send requests.

## How to Run

### Servers

Compile and run each server (`server1.cpp`, `server2.cpp`, `server3.cpp`):
```bash
g++ -o server1 server/server1.cpp -lws2_32
./server1

g++ -o server2 server/server2.cpp -lws2_32
./server2

g++ -o server3 server/server3.cpp -lws2_32
./server3


## Load Balancer
Compile and run load_balancer.cpp:

g++ -o load_balancer load_balancer/load_balancer.cpp -lws2_32
./load_balancer

## Clients
Compile and run each client (client1.cpp to client6.cpp):

g++ -o client1 client/client1.cpp -lws2_32
./client1


Example Output
See the example output screenshots in the assets/ directory of this repository.

Future Scope
Database Integration
Connect servers to a database for dynamic content serving based on client requests.

Advanced Load Balancing Algorithms
Implement algorithms like Least Connection, Weighted Response Time for optimized performance.


![q1](https://github.com/Pawan2030/Load-Balancer-Between-Multiple-Clients-and-Servers-project/assets/136910101/fdcf2a03-24f2-48e0-b878-fd7582bce6d0)

## Contributing
Feel free to contribute to this project by forking it and submitting a pull request.

## Authors
Pawan Mehta




