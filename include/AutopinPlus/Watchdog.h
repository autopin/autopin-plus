/*
 * This file is part of Autopin+.
 * Copyright (C) 2015 Technische Universität München - LRR
 *
 * This file is licensed under the GNU General Public License Version 3
 */

#pragma once

#include <AutopinPlus/AutopinContext.h>
#include <AutopinPlus/ControlStrategy.h>
#include <AutopinPlus/DataLogger.h>
#include <AutopinPlus/Error.h>
#include <AutopinPlus/ObservedProcess.h>
#include <AutopinPlus/StandardConfiguration.h>
#include <AutopinPlus/OS/OSServices.h>
#include <QCoreApplication>
#include <QTimer>
#include <list>
#include <memory>

namespace AutopinPlus {

/*!
 * \brief Basic class of autopin+
 *
 * This class manages all resources for running exactly one process in the context of autopin+
 * It is responsible for allocating all the resources for running a process and also actually
 * starts the process, as soon as it receives a signal on the slot slot_watchdogRun.
 *
 */
class Watchdog : public QObject {
	Q_OBJECT
  public:
	/*!
	 * \brief Constructor of Watchdog
	 *
	 * \param[in] config  Config of the process, which is run by this class
	 */
	explicit Watchdog(std::unique_ptr<const Configuration> config);

	/*!
	 * \brief Destructor of Watchdog
	 */
	~Watchdog();

  public slots:
	/*!
	 * Runs the watchdog
	 */
	void slot_watchdogRun();

signals:
	/*!
	 * Emitted, when the watchdogs stop, either because there is an error or
	 * the observed process has terminated
	 */
	void sig_watchdogStop();

	/*!
	 * \brief Emitted, when the watchdog has finished initalization.
	 */
	void sig_watchdogReady();

  private:
	/*!
	 * Counts the instances of Watchdog
	 */
	static std::atomic_int counter;

	/*!
	 * \brief Factory function for creating the runtime context
	 */
	void createContext();

	/*!
	 * \brief Factory function for performance monitors
	 *
	 * Reads the configuration and creates all requested
	 * performance monitors.
	 *
	 */
	void createPerformanceMonitors();

	/*!
	 * \brief Factory function for the control strategy
	 *
	 * Reads the configuration and creates the requested
	 * control strategy.
	 */
	void createControlStrategy();

	/*!
	 * \brief Factory function for data loggers
	 *
	 * Reads the configuration and creates all requested data loggers.
	 */
	void createDataLoggers();

	/*!
	 * \brief Factory function for creating an instance of ObservedProcess
	 *
	 * Reads the configuration and creates an instance of ObservedProcess
	 */
	void createObservedProcess();

	/*!
	 * \brief Factory function for the os services
	 *
	 * Creates the os services.
	 *
	 */
	void createOSServices();

	/*!
	 * \brief Creates all global connections between Qt signals and slots
	 */
	void createComponentConnections();

	/*!
	 * Stores a unique pointer to current context.
	 */
	std::unique_ptr<AutopinContext> context;

	/*!
	 * Stores a unique pointer to an instance of a subclass of Configuration.
	 */
	std::unique_ptr<const Configuration> config;

	/*!
	 * Stores a unique pointer to an instance of a subclass of OSServices.
	 */
	std::unique_ptr<OS::OSServices> service;

	/*!
	 * Stores a unique pointer to an instance of ObservedProcess.
	 */
	std::unique_ptr<ObservedProcess> process;

	/*!
	 * Stores a list of unique pointers to performance monitors.
	 */
	PerformanceMonitor::monitor_list monitors;

	/*!
	 * Stores a unique pointer to an instance of a subclass of ControlStrategy.
	 */
	std::unique_ptr<ControlStrategy> strategy;

	/*!
	 * Stores a list of unique pointers to data loggers.
	 */
	std::list<std::unique_ptr<DataLogger>> loggers;
};

} // namespace AutopinPlus
