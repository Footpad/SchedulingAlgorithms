/*
 * Task.h
 *
 * Threaded object representing a task for the scheduling
 * test fixture. Tasks are composed their name, compute time,
 * period, and deadline.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <semaphore.h>
#include "Thread.h"
#include "SchedulingAlgorithms.h"

// The maximum number of misses which a task can endure when
// used with a net deadline met/deadline missed counter.
#define MAX_DEADLINE_MISSES (15)

/**
 * Enumeration for the set of task priorities and task states.
 * Task priorities are specified to be the valid QNX priority
 * for which they will run at.
 */
enum TaskPriority {
	TP_FINISHED = 1, TP_READY = 7, TP_RUNNING = 8, TP_SCHEDULER = 9
};

class Task : public Thread {
public:
	/**
	 * @param name - user-specified name of the task.
	 * @param computeTime - user-specified time which the task busy-waits for.
	 * @param period - user-specified interval of when the task must run.
	 * @param deadline - user-specified interval of when the task must finish.
	 * @param scheduler - pointer to the schedler's semaphore.
	 */
	Task(std::string name, int computeTime, int period, int deadline, sem_t* scheduler);
	virtual ~Task();

	/**
	 * Kicks off the timer which runs repeatedly at the deadline interval.
	 */
	void startDeadlineTimer();

	/**
	 * Thread's run method which waits for the deadline to pass,
	 * performs a blocking busy-wait using nanospin, and
	 * notifies the scheduler that it has completed its work.
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
	 * @return The time remaining before the task's deadline, timesliced
	 * to the same value for which a single time tick is.
	 */
	long getRemainingDeadline();

	/**
	 * Increments the amount of time completed.
	 */
	void incrementCompletedTime();

	/**
	 * @return the number of time ticks remaining for task computation.
	 */
	int getRemainingComputeTime();

	/**
	 * @return the period of the task.
	 */
	int getPeriod();

	/**
	 * @return the deadline of the task.
	 */
	int getDeadline();

	/**
	 * @return the task's state as a TaskPriority enum value.
	 */
	int getState();

	/**
	 * Sets the state as a TaskPriority enum value.
	 *
	 * @param newState - the state this task will be in.
	 */
	void setState(int newState);

	/**
	 * Called when the deadline has passed to signal the task it can perform
	 * work again.
	 */
	void postSem();

private:
	/**
	 * Called by the timer to signify the deadline has passed.
	 *
	 * @param sig - contains a pointer to this Task.
	 */
	static void tick(union sigval sig);

	/**
	 * The time it takes to actually complete the task in
	 * our specified timeslice.
	 */
	int computeTime;

	/**
	 * The time computed in the current execution cycle of the task
	 * in our specified timeslice.
	 */
	volatile int completedTime;

	/**
	 * The period of the task; how frequently it restarts in our
	 * specified timeslice.
	 */
	int period;

	/**
	 * The deadline the task must finish by in our specified timeslice.
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

	/**
	 * The timer which the task uses to restart its computation.
	 */
	timer_t deadlineTimer;

	/**
	 * Pointer to the semaphore the scheduler synchronizes on.
	 */
	sem_t* scheduler;

	/**
	 * Cached values to decrease the latency in scheduling the timer.
	 */
	struct itimerspec deadlineTimerSpec;
	struct sigevent deadlineEvent;
	struct itimerspec deadlineRemainingSpec;

	/**
	 * Cached strings to decrease the latency of the trace event logging.
	 */
	std::string deadlineTimerString;
	std::string deadlineMissedString;
	const char *deadlineTimerMsg;
	const char *deadlineMissedMsg;
};

#endif /* TASK_H_ */
