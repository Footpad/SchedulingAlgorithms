#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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

	if (argc == 2) {
		ifstream file;
		file.open(argv[1], ios::in);

		//select the scheduling algorithm...
		do {
			file >> choice;
		} while (choice < 1 || choice > 3);

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
			cout << "RM Scheduler." << endl;
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
		sc->setPriority(TP_SCHEDULER);
		sc->join();					//this will stick because the thread never gets stop()'d

		cout << "Exiting..." << endl;

		//cleanup
		delete sc;
	} else {
		cerr << "Usage: " << argv[0] << " Filename" << endl;
	}

	return EXIT_SUCCESS;
}
