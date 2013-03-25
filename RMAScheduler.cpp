/*
 * RMAScheduler.cpp
 *
 * Concrete subclass of a Scheduler implementing the
 * Rate-Monotonic scheduling algorithm.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "RMAScheduler.h"
#include <algorithm>
#include <cassert>
#include "Task.h"

/**
 * Comparator function for sorting tasks based on their
 * period.
 *
 * @param t1 - first task
 * @param t2 - second task
 * @return period of t1 < period of t2
 */
bool comparePeriod(Task *t1, Task *t2) {
	return t1->getPeriod() < t2->getPeriod();
}

RMAScheduler::RMAScheduler() : taskPrioritiesSet(false) {
}

RMAScheduler::~RMAScheduler() {
}

void RMAScheduler::scheduleTasks() {
	// Since RMA is a fixed-priority scheduling algorithm, we
	// only need to perform scheduling at the beginning.
	if (!taskPrioritiesSet) {
		// Assert that there are enough priorities for the task set.
		assert(taskSet.size() < TP_SCHEDULER);

		// Sort the tasks by the smallest period.
		std::sort(taskSet.begin(), taskSet.end(), comparePeriod);

		// Assign priorities to tasks in a descending order from TP_SCHEDULER.
		for (std::size_t i = 0; i < taskSet.size(); i++) {
			Task *task = taskSet[i];
			task->setPriority(TP_SCHEDULER - (i + 1));
		}

		// Set this flag so that scheduling is only done once.
		taskPrioritiesSet = true;
	}
}
