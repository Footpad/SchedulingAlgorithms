/*
 * Scheduler.h
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <string>
#include <vector>
#include <semaphore.h>
#include "Thread.h"
#include <time.h>
#include <sys/siginfo.h>
#include <sys/trace.h>

// The frequency of the timer tick
#define TIMER_PERIOD_NANO	(50000000)
#define TIMER_PERIOD_SEC	(0)

class Task;

typedef std::vector<Task*> TaskSet;

class Scheduler : public Thread {
public:
	Scheduler();
	virtual ~Scheduler();
	virtual void scheduleTasks() = 0;

	void createTask(std::string name, int computeTime, int period, int deadline);

	void* run();

	static void tick(union sigval sig);

	std::vector<Task*> *getTaskSet();

protected:
	TaskSet taskSet;

	unsigned int tickCounter;
private:
	timer_t tickingTimer;

	sem_t scheduleSem;
};

#endif /* SCHEDULER_H_ */
