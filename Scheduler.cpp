/*
 * Scheduler.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "Scheduler.h"
#include "Task.h"
#include "SchedulingAlgorithms.h"

Scheduler::Scheduler() :
Thread("Scheduler"),
tickCounter(0) {

	sem_init(&scheduleSem, 0, 0);
}

Scheduler::~Scheduler() {
	// delete all the allocated task objects
	while(taskSet.size() > 0) {
		delete taskSet.back();
		taskSet.pop_back();
	}
	sem_destroy(&scheduleSem);
}

void Scheduler::createTask(std::string name, int computeTime, int period,
int deadline) {
	taskSet.push_back(new Task(name, computeTime, period, deadline, &scheduleSem));
}

void* Scheduler::run() {
	this->setPriority(TP_SCHEDULER);

	// calibrate nanospin_ns before the tasks do it.
	nanospin_calibrate(0);

	//When we first are told to start, start the tasks...
	for(std::size_t i; i < taskSet.size(); i++) {
		taskSet[i]->setState(TP_READY);
		taskSet[i]->start();
		sem_wait(&scheduleSem);
		taskSet[i]->setPriority(TP_READY);
	}

	for(std::size_t i; i < taskSet.size(); i++) {
		taskSet[i]->startDeadlineTimer();
	}

#if TRACE_EVENT_LOG_DEBUG
	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_SCHEDULING_START, "Scheduling");
#endif

	scheduleTasks();

#if TRACE_EVENT_LOG_DEBUG
	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_SCHEDULING_END, "Done Scheduling");
#endif

	for (std::size_t i = 0; i < taskSet.size(); i++) {
		Task *task = taskSet[i];
		task->postSem();
	}

	//then perform the scheduling
	while (!killThread) {
		sem_wait(&scheduleSem);
#if TRACE_EVENT_LOG_DEBUG
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_SCHEDULING_START, "Scheduling");
#endif
		scheduleTasks();
#if TRACE_EVENT_LOG_DEBUG
		TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_SCHEDULING_END, "Done Scheduling");
#endif
	}

	return NULL;
}
