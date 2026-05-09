# Lab 6: Socket Programming Evidence Transfer

## Overview

For this lab, I built a simple TCP client and server in C. I wanted mine to be a little different, so I used a cyber forensics theme. The server acts like an evidence vault, and the client acts like an investigator connecting to it.

The program has two modes:

- Echo mode, where the client sends short evidence notes and the server sends them back.
- File transfer mode, where the client requests a case file and the server sends it over the socket.

## Files

| File | Purpose |
|---|---|
| `evidence_server.c` | Server program |
| `investigator_client.c` | Client program |
| `case_file.txt` | File sent by the server |
| `received_case_file.txt` | File created after the transfer |

## Compile

Run this inside the `Lab 6` folder:

make clean
make


## Run the Server

In Terminal 1, run:
./evidence_server 3490

The server should start listening on port `3490`.

## Run Echo Mode
./investigator_client localhost 3490 echo

I tested it with these messages:

first evidence note
network packet received
close

The server replied back with the same messages, and when I typed `close`, it ended the connection.

## Run File Transfer Mode

Start the server again if needed:

./evidence_server 3490

Then run:

./investigator_client localhost 3490 file

This creates:

received_case_file.txt

## Verify the File Transfer

To make sure the file actually transferred correctly, I used:

diff case_file.txt received_case_file.txt && echo "Files match. Transfer successful."

If the files match, the transfer worked.

## Screenshots

The `Screenshots` folder includes:

| Screenshot | Shows |
|---|---|
| `Make.png` | The programs compiling successfully |
| `ServerRunning.png` | The server running on port 3490 |
| `Client.png` | The client connecting |
| `ClientReply.png` | Echo mode working |
| `FileTransfer.png` | File transfer mode running |
| `FileTransferSuccess.png` | The transferred file matching the original |

## What I Learned

This lab helped me understand how a TCP client and server communicate. The server listens for a connection, the client connects to it, and then they can send data back and forth.

Echo mode showed the basic connection working. File transfer mode helped me understand how a file can be sent in pieces and saved again on the client side. I also learned why checking the transfer matters, because creating a file does not always mean it copied correctly.

## Conclusion

The client and server worked successfully. Echo mode sent messages back correctly, file transfer mode created the received file, and the `diff` check showed that the received file matched the original.