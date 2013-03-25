/*
 * Scheduler.h
 *
 * Threaded scheduler base class, which is responsible for
 * accepting a task set, scheduling it, and running it.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <string>
#include <vector>
#include <semaphore.h>
#include "Task.h"

class Scheduler : public Thread {
public:
	Scheduler();
	virtual ~Scheduler();

	/**
	 * Pure virtual function requiring subclasses to implement
	 * how to schedule task sets.
	 */
	virtual void scheduleTasks() = 0;

	/**
	 * Creates a task in the scheduler's task set with the specified attributes.
	 *
	 * @param name - user-specified name of the task.
	 * @param computeTime - user-specified time which the task busy-waits for.
	 * @param period - user-specified interval of when the task must run.
	 * @param deadline - user-specified interval of when the task must finish.
	 */
	void createTask(std::string name, int computeTime, int period, int deadline);

	/**
	 * Implementation of the Thread class's run method.
	 */
	void* run();

protected:
	/**
	 * Vector of the tasks for which this scheduler must schedule.
	 */
	std::vector<Task*> taskSet;

private:
	/**
	 * Semaphore for blocking the execution of the scheduler while
	 * the scheduled task is running.
	 */
	sem_t scheduleSem;
};

#endif /* SCHEDULER_H_ */
