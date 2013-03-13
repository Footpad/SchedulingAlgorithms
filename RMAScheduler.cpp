/*
 * RMAScheduler.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "RMAScheduler.h"
#include <algorithm>
#include <cassert>
#include "Task.h"

bool comparePeriod(Task *t1, Task *t2) {
	return t1->getPeriod() < t2->getPeriod();
}

RMAScheduler::RMAScheduler() : taskPrioritiesSet(false) {
}

RMAScheduler::~RMAScheduler() {
}

void RMAScheduler::scheduleTasks() {
	if (!taskPrioritiesSet) {
		assert(taskSet.size() < TP_SCHEDULER);

		std::sort(taskSet.begin(), taskSet.end(), comparePeriod);

		for (std::size_t i = 0; i < taskSet.size(); i++) {
			Task *task = taskSet[i];
			task->setPriority(TP_SCHEDULER - (i + 1));
		}

		taskPrioritiesSet = true;
	}
}
