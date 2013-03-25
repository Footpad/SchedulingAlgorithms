/*
 * SchedulingAlgorithms.cc
 *
 * Driver for the scheduling algorithms test fixture project.
 *
 * This driver expects two command line arguments:
 * 	1. A test file containing the set of tasks on each line
 * 	formatted as:
 * 		<task name> <compute time> <period> <deadline>
 * 		e.g. T2 3 5 5
 *  2. The scheduling algorithm ID as a number:
 *  	Rate-Monotonic: 			1
 *  	Shortest Completion Time: 	2
 *  	Earliest Deadline First:	3
 *
 * Usage:
 * 	./SchedulingAlgorithms <path/to/test/file> <algorithm ID>
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

#include "SchedulingAlgorithms.h"
#include "RMAScheduler.h"
#include "EDFScheduler.h"
#include "SCTScheduler.h"

int main(int argc, char *argv[]) {
	//for the scheduler
	Scheduler* sc;
	int choice = -1;

	// Storage for parsing the task set file.
	string name;
	int computeTime, deadline, period;

	if (argc == 3) {
		std::ifstream file;
		file.open(argv[1], ios::in);

		//select the scheduling algorithm...
		choice = atoi(argv[2]);

		switch (choice) {
		case 3:
			cout << "EDF Scheduler." << endl;
			sc = new EDFScheduler();
			break;
		case 2:
			cout << "SCT Scheduler." << endl;
			sc = new SCTScheduler();
			break;
		case 1:
		default:
			cout << "RMA Scheduler." << endl;
			sc = new RMAScheduler();
			break;
		}

		// Create tasks from the file until it it has been fully read.
		while (!(file.eof() || file.fail())) {
			// Grab the task as a string in the following order
			file >> name >> computeTime >> deadline >> period;

			if (!(file.eof() || file.fail())) {
				cout << "Making task: " << name << " " << computeTime << " "
						<< period << " " << deadline << endl;
				sc->createTask(name, computeTime, period, deadline);
			}
		}

		cout << "Starting..." << endl;

		// Start the simulation
		sc->start();

		// This will stick because the thread never gets stop()'d
		sc->join();

		// Cleanup
		delete sc;

		cout << "Exiting..." << endl;

	} else {
		cerr << "Usage: " << argv[0] << " Filename SchedulerInt" << endl;
	}

	return EXIT_SUCCESS;
}
