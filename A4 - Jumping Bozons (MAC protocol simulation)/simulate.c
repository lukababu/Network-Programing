/* -------------------------------------------------------------------------
 * Bozon Colony simulator
 *
 * Name              : simulate.c
 * Author            : Luke D Iremadze
 * UCID		           : 10163614
 * Language          : C
 * Latest Revision   : 12-3-2018
 * Compile with      : gcc -o simulate simulate.c -lm
 * -------------------------------------------------------------------------
 */


/* -------------- Libraries ------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* -------------- Global variables ------------------------ */
#define MAX_INT 2147483647 /* Maximum positive integer 2^31 - 1 */
#define SIM_END_TIME 1000000.000
#define SEED 12345
#define M 10			  /* Set number of bozons */
#define S 200.000		/* Set mean sleep duration */
#define Y 40.000		/* Set mean yodel duration */

/* Defining event type indicators */
#define SLEEP 1
#define YODEL 2

/* Debugging flags */
//#define DEBUG 1 //comment this line to inactive debugging

/* -------------- Structures ------------------------------ */
// Events can be defined as structures. Each event has at least two elements: 1.time 2.type
struct Event {
	double time;
	int type;
};


/* -------------- Function declarations ------------------- */
double uniform01(void);
double getExponentialVar(double mean);

/* -------------- Function definitions ------------------- */
int main(void) {

  	/* *************** Defining Variables *************************** */
  	// You should define simulator variables (e.g. time, event list, entities, statistical counters and control variables)
    struct Event curr_event;    /* For storing the most imminent event fetched from the event_list in each iteration */
    struct Event next_event;    /* A dummy variable for creating next event in each iteration */
    struct Event event_list[M]; /* An array of Events */
    int curr_event_index; /* Shows the index of the current event in the event_list */
    double curr_event_time; /* Holds the time of the most imminent event in each iteration */
    double curr_time; /* Represents the current time of the simulator */
    double response_time; /* Holds the generated response time for Bozons */
    double total_silent_time;
    double total_melodious_time;
    double total_screech_time;
    int total_attempted_yodels;
    int total_perfect_yodels;
    int yodels;
  	/* ************************************************************** */

  	/* *************** Initialization ******************************* */
    /* Statistical counters */
    total_silent_time = 0.0;
    total_melodious_time = 0.0;
    total_screech_time = 0.0;
    total_attempted_yodels = 0;
    total_perfect_yodels = 0;

  	curr_time = 0.0;
    curr_event_index = 0;
    //srandom(SEED);

    /* Initialize Bozon events and place them in an array*/
    for (int i = 0; i < M; i++) {
      response_time = getExponentialVar(S);
      next_event.time = curr_time + response_time;
      next_event.type = SLEEP;
      event_list[i] = next_event;
    }

  	/* ************************************************************** */

  	/* Main loop */
  	while (curr_time < SIM_END_TIME) {

  		// Get the most imminent bozon in the event list by finding the smallest durration
      // store it in curr_event for execution
      double min = SIM_END_TIME;
      for (int i = 0; i < M; i++) {
        if (event_list[i].time < min) {
          min = event_list[i].time;
          curr_event_index = i;
        }
        #ifdef DEBUG
          printf("for loop index: %d, current event index: %d, value at index: %f, value of min: %f, type at index: %d\n",
                    i, curr_event_index, event_list[i].time, min, event_list[i].type);
        #endif
      }    

      
      yodels = 0;
      curr_event = event_list[curr_event_index];
      curr_time += curr_event.time;
      curr_event_time = curr_event.time;
      #ifdef DEBUG
            printf("Current_time = %lf index: %d\n", curr_time, curr_event_index);  // FOR DEBUGGING //
      #endif

  		switch (curr_event.type) {
        case SLEEP:
          #ifdef DEBUG
            printf("Entered to SLEEP event.\n"); // FOR DEBUGGING //
          #endif
          for (int i = 0; i < M; i++) {
            event_list[i].time -= curr_event_time;  // Part of the algorithm, we substract the duration of imminent bozon with the rest to simulate that time pass
            if (event_list[i].type == YODEL) {  // Count the number of bozons currently yodeling
              yodels++;
            }
          }

          // Update Statistics
          if (yodels == 0) {
            total_silent_time += curr_event_time;
            #ifdef DEBUG
            printf("SILENT! yodels: %d, total silent time: %f, current event time: %f\n", yodels, total_silent_time, curr_event_time);
            #endif
          }
          else if (yodels == 1) {
            total_melodious_time += curr_event_time;
            #ifdef DEBUG
            printf("Melodious! yodels: %d, total silent time: %f, current event time: %f\n", yodels, total_melodious_time, curr_event_time);
            #endif
          }
          else {
            total_screech_time += curr_event_time;
            #ifdef DEBUG
            printf("Screeching! yodels: %d, total silent time: %f, current event time: %f\n", yodels, total_screech_time, curr_event_time);
            #endif
          }

          // Bozon waking up and starting to YODEL
          event_list[curr_event_index].time = getExponentialVar(Y);
          event_list[curr_event_index].type = YODEL;
          #ifdef DEBUG
            printf("curr_event new time: %f, curr_event new type: %d\n", curr_event.time, curr_event.type); // FOR DEBUGGING //
          #endif
        break;

        case YODEL:
          #ifdef DEBUG
            printf("Entered to YODEL event.\n"); // FOR DEBUGGING //
          #endif
          total_attempted_yodels++;
          
          for (int i = 0; i < M; i++) {
            event_list[i].time -= curr_event_time; // Part of the algorithm, we substract the duration of imminent bozon with the rest to simulate that time pass
            if (event_list[i].type == YODEL) yodels++; // Count the number of bozons currently yodeling
          }

          // Update Statistics
          if (yodels == 0) {
            total_silent_time += curr_event_time;
            #ifdef DEBUG
            printf("SILENT! yodels: %d, total silent time: %f, current event time: %f\n", yodels, total_silent_time, curr_event_time);
            #endif
          }
          else if (yodels == 1) {
            total_melodious_time += curr_event_time;
            #ifdef DEBUG
            printf("Melodious! yodels: %d, total silent time: %f, current event time: %f\n", yodels, total_melodious_time, curr_event_time);
            #endif
          }
          else {
            total_screech_time += curr_event_time;
            #ifdef DEBUG
            printf("Screeching! yodels: %d, total silent time: %f, current event time: %f\n", yodels, total_screech_time, curr_event_time);
            #endif
          }

          // Bozon getting tired and starting to go to SLEEP;
          event_list[curr_event_index].time = getExponentialVar(S);
          event_list[curr_event_index].type = SLEEP;

        break;
        
        default:
          printf("Invalid type of event:");
  		}
      #ifdef DEBUG
            printf("End of the main loop.\n"); // FOR DEBUGGING //
      #endif
      //return -1;

  	} // End of simulation

  	// You may need to update your statistical counters one last time based on the most recent state of the system
  	// Output Results
    printf("Total time observing channel: %.3lf\n", SIM_END_TIME);
    printf("Idle time on the channel: %.3lf %.2lf%%\n", total_silent_time, 100 * total_silent_time / curr_time, '%');
    printf("Melodious time on the channel: %.3lf %.2lf%%\n", total_melodious_time, 100 * total_melodious_time / curr_time, '%');
    printf("Screech time on the channel: %.3lf %.2lf%%\n", total_screech_time, 100 * total_screech_time / curr_time, '%');
    printf("Number of attempted yodels: %d\n", total_attempted_yodels);
    printf("Percentage of perfected yodels: %.3lf%%\n", 100 * total_perfect_yodels / (double) total_attempted_yodels, '%');

  	return 0;
}

double uniform01() {
  	double randnum;
  	randnum = (double) 1.0 * random();
  	randnum = randnum / (1.0 * MAX_INT);
  	return randnum;
}


double getExponentialVar(double m) {
    double randnum, ans;
    randnum = uniform01();
    ans = -(m) * log(randnum);
    return ans;
}
