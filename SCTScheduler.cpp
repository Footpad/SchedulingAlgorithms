/*
 * SCTScheduler.cpp
 *
 * Concrete subclass of a Scheduler implementing the
 * Shortest Completion Time scheduling algorithm.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "SCTScheduler.h"
#include "Task.h"

SCTScheduler::SCTScheduler() {
}

SCTScheduler::~SCTScheduler() {
}

void SCTScheduler::scheduleTasks() {
	int shortestIdx = -1;
	int shortestTimeLeft = INT_MAX;

	// Iterate over all the tasks
	for (std::size_t i = 0; i < taskSet.size(); i++) {
		Task *task = taskSet[i];

		//if this task is running, set it back to ready
		if(task->getPriority() == TP_RUNNING) {
			task->setPriority(TP_READY);
		}

		//look at the deadlines of those tasks which are ready or running
		if(task->getState() == TP_READY || task->getState() == TP_RUNNING) {
			//The shortest completion time is that which has the shortest time remaining
			if(task->getRemainingComputeTime() < shortestTimeLeft) {
				shortestIdx = i;
				shortestTimeLeft = task->getRemainingComputeTime();
			}
		}
	}

	//With the earliest deadline known, set that task to running...
	if(shortestIdx != -1) {
		taskSet[shortestIdx]->setPriority(TP_RUNNING);
	}
}
