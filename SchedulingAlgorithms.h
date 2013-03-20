/*
 * SchedulingAlgorithms.h
 *
 *  Created on: Mar 18, 2013
 *      Author: dam7633
 */

#ifndef SCHEDULINGALOGIRTHMS_H_
#define SCHEDULINGALOGIRTHMS_H_

// Configure this define to set the trace event log level.
#define TRACE_EVENT_LOG_LEVEL 			TRACE_EVENT_LOG_LEVEL_DEBUG

/**
 * TRACE_EVENT_LOG_LEVEL
 *
 * The list of available logging levels for which this fixture can use.
 */

// No Trace Events will be logged.
#define TRACE_EVENT_LOG_LEVEL_SILENT 	0

// Missed deadlines and other error messages will be logged.
#define TRACE_EVENT_LOG_LEVEL_CRITICAL	1

// Messages related to the scheduling operations.
#define TRACE_EVENT_LOG_LEVEL_NORMAL	2

// Messages related to performance testing.
#define TRACE_EVENT_LOG_LEVEL_DEBUG		3


/**
 * TRACE_EVENT_LOG
 *
 * Use these macros around the appropriate Trace Event logging.
 */

#define TRACE_EVENT_LOG_EVAL(__x)		(TRACE_EVENT_LOG_LEVEL >= __x)

#define TRACE_EVENT_LOG_CRITICAL 		TRACE_EVENT_LOG_EVAL(TRACE_EVENT_LOG_CRITICAL)

#define TRACE_EVENT_LOG_NORMAL			TRACE_EVENT_LOG_EVAL(TRACE_EVENT_LOG_LEVEL_NORMAL)

#define TRACE_EVENT_LOG_DEBUG			TRACE_EVENT_LOG_EVAL(TRACE_EVENT_LOG_LEVEL_DEBUG)

/**
 * TRACE_EVENT_CODE
 *
 * Enumeration of trace event types used by this fixture.
 */
#define TRACE_EVENT_SCHEDULING_START	10
#define TRACE_EVENT_SCHEDULING_END		11
#define TRACE_EVENT_TASK_START			20
#define TRACE_EVENT_TASK_END			21
#define TRACE_EVENT_DEADLINE_TIMER		30
#define TRACE_EVENT_DEADLINE_MISS		31

#endif /* SCHEDULINGALOGIRTHMS_H_ */
