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

enum TaskPriority {
	TP_FINISHED = 0, TP_READY = 7, TP_RUNNING = 8, TP_SCHEDULER = 9
};

class Task {
public:
	Task(std::string name, int computeTime, int period, int deadline, sem_t* scheduler);
	virtual ~Task();

	/**
	 * Join on the underlying thread.
	 */
	void join();

	/**
	 * Creates a pthread which targets Thread's run() method.
	 */
	virtual void start();

	/**
	 * Sets a flag which means the thread should stop.
	 *
	 * Subclassers must implement the stopping behavior in run().
	 */
	virtual void stop();

	/**
	 * @return the return value when the Thread exits.
	 */
	void* run();

	/**
	 * Returns the priority of the task.
	 *
	 * @return an int representing the priority of the task
	 */
	int getPriority();

	/**
	 * Sets the priority of the task.
	 *
	 * @param prio - the new priority to be set
	 */
	void setPriority(int prio);

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

protected:
	// Flag which will be set when stop() is called.
	bool killThread;

private:
	/**
	 * Static function which calls the Thread's run() method.
	 */
	static void* pthread_entry(void* args);

	// pthread id for this thread.
	pthread_t thread;

	/**
	 * The name of the task; reflected as the name of the underlying thread.
	 */
	std::string name;

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

	/**
	 * Pointer to the semaphore the scheduler synchronizes on.
	 */
	sem_t* scheduler;
};

#endif /* TASK_H_ */
