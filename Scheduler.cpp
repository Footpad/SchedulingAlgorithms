/*
 * Scheduler.cpp
 *
 * Threaded scheduler base class, which is responsible for
 * accepting a task set, scheduling it, and running it.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include <time.h>
#include <sys/siginfo.h>
#include <sys/trace.h>
#include "Scheduler.h"
#include "Task.h"
#include "SchedulingAlgorithms.h"

Scheduler::Scheduler() :
Thread("Scheduler") {
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

void Scheduler::createTask(std::string name, int computeTime, int period, int deadline) {
	taskSet.push_back(new Task(name, computeTime, period, deadline, &scheduleSem));
}

void* Scheduler::run() {
	// The scheduler must run at its priority - the
	// highest in the test fixture.
	this->setPriority(TP_SCHEDULER);

	// Calibrate nanospin_ns before the tasks do it.
	nanospin_calibrate(0);

	// When we first are told to start, start the tasks...
	for(std::size_t i; i < taskSet.size(); i++) {

		// Initialize task state so that all are ready to run.
		taskSet[i]->setState(TP_READY);

		// Create the pthread for the task.
		taskSet[i]->start();

		// taskSet[i] will post on scheduleSem.
		// This ensures that all task threads are created before
		// they are scheduled.
		sem_wait(&scheduleSem);

		// Lower the priority of the task to not running.
		taskSet[i]->setPriority(TP_READY);
	}

	// Kick off the deadline timer for each task.
	for(std::size_t i; i < taskSet.size(); i++) {
		taskSet[i]->startDeadlineTimer();
	}

#if TRACE_EVENT_LOG_DEBUG
	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_SCHEDULING_START, "Scheduling");
#endif

	// Perform the first scheduling.
	scheduleTasks();

#if TRACE_EVENT_LOG_DEBUG
	TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, TRACE_EVENT_SCHEDULING_END, "Done Scheduling");
#endif

	// Release all of the tasks.
	for (std::size_t i = 0; i < taskSet.size(); i++) {
		Task *task = taskSet[i];
		task->postSem();
	}

	// Continue to perform scheduling when:
	// 1. A Task completes its computation.
	// 2. A Task's deadline timer puts it into a ready state.
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
