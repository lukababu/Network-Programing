/* ------------------------------------------------------------------------- 
 *
 * Name              : qos.c  (Quality of Service, queue calculator)
 * Authors           : Luke Iremadze
 * Language          : ANSI C
 * Latest Revision   : Nov 9, 2018
 * Compile with      : gcc qos.c
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>                         

#define DEBUG         0           /*  1 = true, 0 = false   */
#define EMPTY         0.000000
#define FILENAME      "test.txt"  /*  input data file       */

int bufferSIZE = 100;
int simulatedBUFFERSIZE[] = {0, 1, 5, 10, 20, 35, 50, 65, 85, 100};

int droppedPKT;
/*  read an arrival time  */
double GetArrival(FILE *fp) { 
  double a;

  fscanf(fp, "%lf", &a);
  return (a);
}

/* read a service time */
double GetSize(FILE *fp) { 
  double s;

  fscanf(fp, "%lf\n", &s);
  return (s);
}

/* Get the next array index number in the opposite direction */
int getLastIndex(int index) {
  int lastIndex = index-1;

  if (lastIndex == -1 ) return bufferSIZE-1;
  else return lastIndex;
}

/* Get the next array index number */
int getNextIndex(int index) {
  int nextIndex = index +1;
  if (nextIndex > bufferSIZE-1 ) {
    return nextIndex-bufferSIZE;
  }
  else return nextIndex;
}

int find_minimum(double a[], int n) {
  int c, index;
  double min;
 
  min = a[0];
  index = 0;
 
  for (c = 1; c < n; c++) {
    if (min > a[c]) {
       index = c;
       min = a[c];
    }
  }

  if (DEBUG == 1) printf("Min: %f\n", min);
  return index;
}

/**
  * Get transmission time in seconds
  * speed - in Mbps
  * size  - in bytes
  * @returns time in seconds
  */
double GetTransmissionTime(double speed, int size) {
  double bitSize;
  double bitspeed;
  double transmissionTime;

  bitSize = ((double)size)*8; // convert to bits
  bitspeed = speed * 1000000; //1048570; // convert to bps

  transmissionTime = bitSize/bitspeed;

  return transmissionTime;
}


/**
  *Simulate at diffrent buffer sizes
  *speed - in Mbps
  */
double simulateBuffer(int speed, int newBufferSize) {
  bufferSIZE = newBufferSize;
  double  buffer[bufferSIZE];           /*   Buffer / Queeu       */
  FILE    *fp;                          /*   Input data file      */
  long    pktNumber         =     0;    /*   Number of packets    */
  double  time              = EMPTY;    /*   Elapsed time since start of simulation    */
  double  packetArrivalTime = EMPTY;    /*   The time it takes for a packet to arrive  */
  double  queueDelay;                   /*   delay in queue       */
  int     index;                        /*   Front position in the queue  */
  int     nextIndex;                    /*   Rear position in the queue  */
  double  timer;                        /*   The time tracker for how long a packet has been held in queue  */
  double  lastTime;                     /*   Previous time reading  */
  long     size;                         /*   Packet size in bytes  */
  struct {                        /*   SUM OF...          */
    double queueDelay;            /*   queueDelay times   */
    int dropped;                  /*   interarrival times */
    long size;                    /*   Packet size in bytes  */
  } sum = {0.0, 0, 0};

  // Initialize queue
  for(int i = 0; i < bufferSIZE; i++) {
    buffer[i] = EMPTY;
  }

  // Start Reading File
  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return -1.0;
  }

  // Read file until end
  while (!feof(fp)) {
    lastTime          =   time;               /*   Previous time reading  */
    time              =   GetArrival(fp);     /*   The current time reading  */
    packetArrivalTime =   time - lastTime;    /*   The time it takes for a packet to arrive  */
    size              =   GetSize(fp);        /*   Packet size in bytes  */
    sum.size          +=  size;               /*   Agrigate the size of packets for speed calculation  */
    timer             +=  packetArrivalTime;  /*   The time tracker for how long a packet has been held in queue  */

    // Make sure there is space in buffer
    if (bufferSIZE != 0) {
      // Initialize
      if (pktNumber == 0) {
        buffer[0] = GetTransmissionTime(speed, size);
        index = 0;
        nextIndex = getNextIndex(index);
        sum.size -= size; // offset the initial value
      } else {

        //Queue
        if (buffer[getLastIndex(index)] == EMPTY) {       // If buffer not full, add on top of queue      
          buffer[nextIndex] = GetTransmissionTime(speed, size);
          if (nextIndex>index) {
            for (int i = nextIndex; i > index; i--) {
            sum.queueDelay = sum.queueDelay + buffer[i];
            }
          } else {
            for (int i = index; i < (index+nextIndex); i++) {
            sum.queueDelay = sum.queueDelay + buffer[getNextIndex(i)];
            }
          } 

          nextIndex = getNextIndex(nextIndex);   
        }
        // Otherwise drop the packet
        else sum.dropped++;

        //Transmit
        while (timer != EMPTY && (timer > buffer[index]) ) { // Has enough time passed for the packet to be transmited from the queue?
          timer -= buffer[index];                            // simulating time passage

          // Clear buffer
          buffer[index] = 0.000000;

          //Check to see if queue is empty, if not move to next entry otherwise reset the timer
          if (buffer[getNextIndex(index)] != 0) index = getNextIndex(index);
          else timer = EMPTY;
        }
      }
    } else sum.dropped++;

    pktNumber++;
  }

  fclose(fp);
  droppedPKT = sum.dropped;
  if (bufferSIZE == 0) return EMPTY;
  else return (sum.queueDelay/(pktNumber-sum.dropped));
}

/**
  *Initiate simulation
  *speed - in Mbps
  */
void simulateSpeed(int speed) {
  double  buffer[bufferSIZE];           /*   Buffer / Queeu       */
  FILE    *fp;                          /*   Input data file      */
  long    pktNumber         =     0;    /*   Number of packets    */
  double  time              = EMPTY;    /*   Elapsed time since start of simulation    */
  double  packetArrivalTime = EMPTY;    /*   The time it takes for a packet to arrive  */
  double  queueDelay;                   /*   delay in queue       */
  int     index;                        /*   Front position in the queue  */
  int     nextIndex;                    /*   Rear position in the queue  */
  double  timer;                        /*   The time tracker for how long a packet has been held in queue  */
  double  lastTime;                     /*   Previous time reading  */
  long     size;                         /*   Packet size in bytes  */
  struct {                        /*   SUM OF...          */
    double queueDelay;            /*   queueDelay times   */
    int dropped;                  /*   interarrival times */
    long size;                    /*   Packet size in bytes  */
  } sum = {0.0, 0, 0};

  // Initialize queue
  for(int i = 0; i < bufferSIZE; i++) {
    buffer[i] = EMPTY;
  }

  // Start Reading File
  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return;
  }

  // Read file until end
  while (!feof(fp)) {
    lastTime          =   time;               /*   Previous time reading  */
    time              =   GetArrival(fp);     /*   The current time reading  */
    packetArrivalTime =   time - lastTime;    /*   The time it takes for a packet to arrive  */
    size              =   GetSize(fp);        /*   Packet size in bytes  */
    sum.size          +=  size;               /*   Agrigate the size of packets for speed calculation  */
    if (sum.size < 0 ) return;
    timer             +=  packetArrivalTime;  /*   The time tracker for how long a packet has been held in queue  */

    // Make sure there is space in buffer
    if (bufferSIZE != 0) {
      // Initialize
      if (pktNumber == 0) {
        buffer[0] = GetTransmissionTime(speed, size);
        index = 0;
        nextIndex = getNextIndex(index);
        sum.size -= size; // offset the initial value
      } else {

        //Queue
        if (buffer[getLastIndex(index)] == EMPTY) {       // If buffer not full, add on top of queue      
          buffer[nextIndex] = GetTransmissionTime(speed, size);
          if (nextIndex>index) {
            for (int i = nextIndex; i > index; i--) {
            sum.queueDelay = sum.queueDelay + buffer[i];
            }
          } else {
            for (int i = index; i < (index+nextIndex); i++) {
            sum.queueDelay = sum.queueDelay + buffer[getNextIndex(i)];
            }
          } 

          nextIndex = getNextIndex(nextIndex);   
        }
        // Otherwise drop the packet
        else sum.dropped++;

        //Transmit
        while (timer != EMPTY && (timer > buffer[index]) ) { // Has enough time passed for the packet to be transmited from the queue?
          timer -= buffer[index];                            // simulating time passage

          // Clear buffer
          buffer[index] = 0.000000;

          //Check to see if queue is empty, if not move to next entry otherwise reset the timer
          if (buffer[getNextIndex(index)] != 0) index = getNextIndex(index);
          else timer = EMPTY;
        }
      }
    } else sum.dropped++;

    pktNumber++;
  }
   
  double averageQueDelay[sizeof(simulatedBUFFERSIZE)/sizeof(int)];
  int droppedPKTArray[sizeof(simulatedBUFFERSIZE)/sizeof(int)];

  // Generate output
  printf("\n===========================================================================\n");
  printf("At %d Mbps For %ld packets at buffer size %d:\n",speed, pktNumber, bufferSIZE);
  printf("   Total number of dropped packets .... = %d\n", sum.dropped);
  printf("   Total number of delivered packets .. = %d\n", pktNumber - sum.dropped);
  printf("   Percentage %% of dropped packets .... = %.2f%%\n", (( (double)sum.dropped / (double)pktNumber)*100) );
  if (DEBUG == 1) printf("   Average incomming speed ............ = %.2f Mbps\n", (double)((sum.size*8)/time*0.000001));
  printf("---------------------------------------------------------------------------\n");
  printf("&  Buffer size   |   Average queueing delay   |   Packet Loss\n");

  for (int i = 0; i < sizeof(simulatedBUFFERSIZE)/sizeof(int); i++) {
    averageQueDelay[i] = simulateBuffer(speed, simulatedBUFFERSIZE[i]);
    droppedPKTArray[i] = droppedPKT;
    if (simulatedBUFFERSIZE[i]>=0 && simulatedBUFFERSIZE[i]<10)
        printf("&  %d             |   %f Seconds         |  %d\n", simulatedBUFFERSIZE[i], averageQueDelay[i], droppedPKTArray[i]);
    if (simulatedBUFFERSIZE[i]>9 && simulatedBUFFERSIZE[i]<100)
        printf("&  %d            |   %f Seconds         |  %d\n", simulatedBUFFERSIZE[i], averageQueDelay[i], droppedPKTArray[i]);
    if (simulatedBUFFERSIZE[i]>99 && simulatedBUFFERSIZE[i]<1000)
        printf("&  %d           |   %f Seconds         |  %d\n", simulatedBUFFERSIZE[i], averageQueDelay[i], droppedPKTArray[i]);
  }
  int suggestedBuffer = 0;
  double pktPercentDif[sizeof(simulatedBUFFERSIZE)/sizeof(int)];
  double avgQuePercentDif[sizeof(simulatedBUFFERSIZE)/sizeof(int)];
  double sumPercent[sizeof(simulatedBUFFERSIZE)/sizeof(int)];

  for (int i = 0; i < sizeof(simulatedBUFFERSIZE)/sizeof(int); i++) {
    pktPercentDif[i] = (double)droppedPKTArray[i] / (  (double)droppedPKTArray[0] -  (double)droppedPKTArray[(sizeof(simulatedBUFFERSIZE)/sizeof(int)) -1]);
    avgQuePercentDif[i] = averageQueDelay[i] / (averageQueDelay[ (sizeof(simulatedBUFFERSIZE)/sizeof(int)) - 1] - averageQueDelay[0]);
    sumPercent[i] = avgQuePercentDif[i] + pktPercentDif[i];
  }

  printf("---------------------------------------------------------------------------\n");
  printf("--------------| Sugessted buffer size: %d\n", simulatedBUFFERSIZE[find_minimum(sumPercent, sizeof(simulatedBUFFERSIZE)/sizeof(int))]);
  printf("---------------------------------------------------------------------------\n");
  fclose(fp);
}



int main(void) {
  int speed[] = {6, 11, 30, 54};  // Speed in Mpbs

  for (int i = 0; i < 4; i++) {
    simulateSpeed(speed[i]);
  }
  
  return (0);
}