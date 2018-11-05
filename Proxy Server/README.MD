# COMPUTER NETWORK SOCKETS 

## Getting Started
There are two files in the main directory. The main driver program “server.c” and its header file “parser.h”.
Our goal is to run our proxy server, connect our browser and once we go to webpages the returned content will have been modified with bogus content. 

1. Go to terminal and change directory to the folder containing the two files
2. Type in the following command ```$ gcc -pthread -o server server.c``` By now we should have a third file called server (without the name extension
3. Type in the next command ```$ ./ server arg1 arg2``` where arg 1 is the number of errors, and arg2 is the port number. Example: ```$ ./server 5 12345```
4. Our server should be running and listening to the given port, now we need to configure Firefox so that it binds to this port. Go to settings of Firefox, under network proxy click settings next to “Configure how Firefox connects to the internet”, switch to Manual proxy configuration and set the left textfield to “localhost” and the port accordingly. Make sure the “Use this proxy for all protocols” is selected and click OK. 

Our proxy should be all ready to go and confuse the people trying to connect to pages.cpsc domains 

The testing was done in the computer science labs at the university of Calgary, the code was typed on my laptop and then transferred to the labs where it ran in its natural environment. 

## FEATURES  
* Retrieves webpages 
* Alters the webpages characters with n number of errors where n is the first argument of the CLI 
* Handles bigger sized webpages  Deals with incorrectly formatted html documents (missing body tag) 
* Displays GIFs 
* Multithreading 

## LIMITATIONS 
* When clicking the cancel load page on the browser it seems to crash the proxy 
* Sometimes on webpages where there is no <body> tag the random error inserts an error in the html formatting 
 
 