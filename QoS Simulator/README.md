# QoS simulator

This program analyses a network trace which is then used to find the number of packets lost, the average queueing delay and generate a table of values to suggest a buffer size.

## Content

* qoc.c file written in ANSI C which is used to simulate.
* soccert.txt a sample trace file which has the time of arrival in the first column and the size in bytes in the next column

## Executing

The input trace file is hardcoded and to adjust modify the source file.
The program was tested with GCC compiler on the Linux machine in the CPSC lab, to run the input to the file compile it in the terminal with ```$ gcc qos.c QoS``` and then run by ```$ ./qos```

## Features

Allows for any number of speed and buffer size variations to be tested, just modify ```simulatedBUFFERSIZE[]``` in the global variable by adding or removing buffer size possibilities, likewise the ```speed[]```  variable located in the main works the same.
Calculate the best buffer size with the values passed in the ```simulatedBUFFERSIZE[]```

## Mechanics

Average queuing delay:
The value is calculated upon a packet entering the buffer. Once the packet arrives in the buffer, a loop looks at all the other packets in the queue and adds the values to the ```double queueDelay```. The final value is then divided by the total number of packets minus the number of dropped packets.

Suggested buffer size:
The ratio of packet loss and average queuing delay is calculated for each individual output value. the ratio of packet loss and average queueing delay at a given buffer size is added to an array and the suggested buffer window is the min of these array values.  

## Drawbacks

The program is a bit messy at its output section which is due to time constraints.
There is some code repeat in ```simulateBuffer()```, it could be made more efficient.
Nothing is done about packets that remain in the queue after no more packets are received.