#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "SchedulingAlgorithms.h"
#include "RMAScheduler.h"
#include "EDFScheduler.h"
#include "SCTScheduler.h"

int main(int argc, char *argv[]) {
	//for the scheduler
	Scheduler* sc;
	int choice = -1;

	//for creating tasks
	string name;
	int computeTime, deadline, period;

	if (argc == 3) {
		ifstream file;
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

		//create the tasks
		while (!(file.eof() || file.fail())) {
			file >> name >> computeTime >> deadline >> period;
			if (!(file.eof() || file.fail())) {
				cout << "Making task: " << name << " " << computeTime << " "
						<< period << " " << deadline << endl;
				sc->createTask(name, computeTime, period, deadline);
			}
		}

		cout << "Starting..." << endl;

		//start the simulation
		sc->start();
		sc->join();					//this will stick because the thread never gets stop()'d

		cout << "Exiting..." << endl;

		//cleanup
		delete sc;
	} else {
		cerr << "Usage: " << argv[0] << " Filename SchedulerInt" << endl;
	}

	return EXIT_SUCCESS;
}
