# Lab 5: getaddrinfo Host Lookup

## Overview

For this lab, I wrote a C program that looks up the IP addresses for a hostname. The assignment was based on section 5.1 of Beej's Guide to Network Programming, but I wrote my own version so I could understand what each part was doing.

The program uses `getaddrinfo()` instead of `gethostbyname()` because `getaddrinfo()` supports both IPv4 and IPv6.

## Files

`getipaddr.c` is the main C program.

`getipaddr.out` is the compiled program.

`Screenshots/` contains proof that the program compiled and ran correctly.

## Compile

To compile the program, run:

```bash
gcc -Wall -Wextra -pedantic -std=c11 getipaddr.c -o getipaddr.out
```

Or, if using the Makefile:

```bash
make clean
make
```

## Run

The program needs a hostname as a command line argument.

Example:

```bash
./getipaddr.out example.com
```

Other test:

```bash
./getipaddr.out google.com
```

## What the Program Does

The program takes the hostname from the command line and passes it into `getaddrinfo()`.

Since `getaddrinfo()` can return more than one address, the program loops through the results and prints each IP address. It checks whether the address is IPv4 or IPv6, then uses `inet_ntop()` to print the address in a readable format.

## Screenshots

My screenshots show:

`Compile.png` shows the program compiling with no errors.

`Make.png` shows the program compiling through the Makefile.

`RunExample.png` shows the program looking up `example.com`.

`RunGoogle.png` shows the program looking up `google.com`.

## What I Learned

This lab helped me understand how hostname lookup works in C. I also got more practice with command line arguments, structs, and looping through linked list results.

The biggest thing I learned is that a hostname can return multiple addresses, so the program cannot assume there is only one result.