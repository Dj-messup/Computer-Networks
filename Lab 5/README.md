# Lab 5: Hostname to IP Address Lookup

## Overview

For this lab, I wrote a small C program that looks up the IP addresses for a hostname. The program uses `getaddrinfo()` so it can return both IPv4 and IPv6 addresses when they are available.

I tested it with different hostnames to make sure the program was actually resolving names instead of only working for one example.

## Files

| File | Purpose |
|---|---|
| `getipaddr.c` | Main C program |
| `Makefile` | Builds the program |
| `Screenshots/` | Shows the program compiling and running |

## How to Compile

```bash
make
```

This creates the program:

```bash
./getipaddr
```

## How to Run

Run the program with a hostname:

```bash
./getipaddr example.com
```

Another example:

```bash
./getipaddr google.com
```

## What the Program Does

The program takes one hostname from the command line. It passes that hostname into `getaddrinfo()`, then loops through the results. For each result, it checks whether the address is IPv4 or IPv6 and prints the IP address in readable form.

## Screenshots

My screenshots show:

| Screenshot | What it Shows |
|---|---|
| `Make.png` | The program compiling |
| `RunExample.png` | The program resolving a hostname |
| `RunGoogle.png` | Another test with a different hostname |

## What I Learned

This lab helped me understand how hostname resolution works in C. Instead of hardcoding an IP address, the program asks the system to look up the address records for a hostname. I also got more practice with command line arguments, structs, and checking whether an address is IPv4 or IPv6.

## Conclusion

The program successfully takes a hostname, resolves it, and prints the IP addresses that are returned.
