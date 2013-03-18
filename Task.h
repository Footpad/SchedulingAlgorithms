/*
 * Task.h
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef TASK_H_
#define TASK_H_

#include <pthread.h>
#include <string>
#include <semaphore.h>
#include <sys/trace.h>
#include <time.h>
#include <sys/siginfo.h>
#include "Thread.h"
#include "SchedulingAlgorithms.h"

enum TaskPriority {
	TP_FINISHED = 1, TP_READY = 17, TP_RUNNING = 18, TP_SCHEDULER = 19
};

class Task : public Thread {
public:
	Task(std::string name, int computeTime, int period, int deadline, sem_t* scheduler);
	virtual ~Task();

	void startDeadlineTimer();

	/**
	 * @return the return value when the Thread exits.
	 */
	void* run();

	/**
	 * Returns the compute time of the task.
	 *
	 * @return an int - the compute time for the task when no work has been
	 * done
	 */
	int getComputeTime();

	/**
	 * Sets the compute time of the task; the amount of work to do assuming 0
	 * work has been done.
	 */
	void setComputeTime(int computeTime);

	/**
	 * Increments the amount of time completed
	 */
	void incrementCompletedTime();

	int getRemainingComputeTime();

	int getPeriod();
	void setPeriod(int period);

	int getDeadline();
	void setDeadline(int deadline);

	int getState();
	void setState(int newState);

	/**
	 * Called when the deadline has passed to signal the task it can perform
	 * work again.
	 */
	void postSem();

	std::string getName();

	/**
	 * Called by the timer to signify the deadline has passed
	 */
	static void tick(union sigval sig);

private:
	/**
	 * The time it takes to actually complete the task
	 */
	int computeTime;

	/**
	 * The time computed in the current execution cycle of the task.
	 */
	volatile int completedTime;

	/**
	 * The period of the task; how frequently it restarts.
	 */
	int period;

	/**
	 * The deadline the task must finish by.
	 */
	int deadline;

	/**
	 * Semaphore to wait on before executing work; this synchronizes the task
	 * and prevents it from doing work before its period expires.
	 */
	sem_t doWork;

	/**
	 * The state of the task; ready, running, finished. Uses the priority enum
	 */
	int state;

	timer_t deadlineTimer;

	/**
	 * Pointer to the semaphore the scheduler synchronizes on.
	 */
	sem_t* scheduler;

	struct itimerspec deadlineTimerSpec;
	struct sigevent deadlineEvent;

	std::string deadlineTimerString;
	std::string deadlineMissedString;
	const char *deadlineTimerMsg;
	const char *deadlineMissedMsg;
};

#endif /* TASK_H_ */
