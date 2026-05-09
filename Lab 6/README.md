# Lab 6 - Socket Programming Evidence Transfer

## Overview

This lab demonstrates basic TCP socket programming in C using a client and server model. I designed the program around a cyber forensics theme where an investigator client connects to an evidence vault server. The server supports two modes: an echo mode for sending short evidence notes and a file transfer mode for sending a case file across the socket.

The goal of the lab was to show that a server can listen on a port, accept a client connection, receive a request, and respond with either echoed text or file data.

## Files

| File | Purpose |
|---|---|
| `evidence_server.c` | TCP server that listens for client connections |
| `investigator_client.c` | TCP client that connects to the server |
| `case_file.txt` | Sample file sent by the server |
| `received_case_file.txt` | File created by the client after file transfer |
| `Makefile` | Compiles the server and client |
| `README.md` | Explains the lab, commands, and verification |

## How to Compile

```bash
make
