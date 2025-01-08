# TP synthese-TFTP client

The aim of this tutorial is to create a client program capable of exchanging files using the
TFTP (Trivial File Transfer Protocol).

To do this, we first coded the RRQ request sent to the server. 
Once this request has been sent, the server responds with data packets (DAT). 
Each DAT packet contains :
- a 2-byte block number
- File data (up to 512 bytes per packet)
The client must receive these packets and send an acknowledgement (ACK) for each packet. This allows the server to know that the client has received the packet.
The ACK packet contains :
- the block number of the received data packet 

