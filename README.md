# Threading
Temperature sensor simulation done with threading

The program simulates a distributed temperature sensor using threads and mutex/conditional variables.

The main thread creates a global linked list, then creates all the sensor threads one by one and passes to each sensor thread a struct that contains an integer seed for the random number and an ID number. The number of sensors, and thus threads, can be an argument passed on the command line to main or read in by main from the user.

When all threads complete a cycle (which is placing the temperature in the linked list), the main thread prints each temperature in the linked list of temperatures and the average temperature. Each thread does this 10 times and terminates.

Each thread can only put a number in the shared linked list only once during each of the 10 cycles using mutual exclusion.

By using a conditional varaible/mutex mix, the linked list is ensured to update correctly. The sensor thread places its temperature in the global linked list, and waits (by the use mutex).
