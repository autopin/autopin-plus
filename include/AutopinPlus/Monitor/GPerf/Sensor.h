/*
 * This file is part of Autopin+.
 * Copyright (C) 2015 Technische Universität München - LRR
 *
 * This file is licensed under the GNU General Public License Version 3
 */

#pragma once

#include <linux/perf_event.h> // for perf_event_attr
#include <qlist.h>			  // for QList
#include <qstring.h>		  // for QString

namespace AutopinPlus {
namespace Monitor {
namespace GPerf {

/*!
 * \brief A struct describing a perf sensor.
 */
struct Sensor {
	/*!
	 * A "perf_event_attr" which can be passed to the "perf_event_open()" syscall to open the sensor.
	 */
	perf_event_attr attr;

	/*!
	 * \brief The name of the sensor.
	 */
	QString name;

	/*!
	 * \brief The list of processors this sensor should monitor.
	 */
	QList<int> processors;

	/*!
	 * \brief The scaling factor which has to be applied to the raw values read from the file descriptor returned by the
	 * "perf_event_open()" syscall.
	 */
	double scale;

	/*!
	 * \brief The unit of the returned values.
	 */
	QString unit;
}; // struct Sensor

} // namespace GPerf
} // namespace Monitor
} // namespace AutopinPlus
