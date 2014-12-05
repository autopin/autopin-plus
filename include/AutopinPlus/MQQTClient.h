/*
 * Autopin+ - Automatic thread-to-core-pinning tool
 * Copyright (C) 2014 LRR
 *
 * Author:
 * Lukas Fürmetz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact address:
 * LRR (I10)
 * Technische Universitaet Muenchen
 * Boltzmannstr. 3
 * D-85784 Garching b. Muenchen
 * http://autopin.in.tum.de
 */

#pragma once

#include <mosquitto.h>

namespace AutopinPlus {

/*!
 * \brief Singleton, which handles the MQQT Communication
 */

class MQQTClient  {
 public:
	/*!
	 * \brief Get the instance of the MQQTClient
	 */
	static MQQTClient &getInstance() {
		static MQQTClient instance;
		return instance;
	}

	/*!
	 * \brief Initalizes the MQQTClient
	 */
	int init();

 private:
	/*!
	 * Singleton overwrites
	 */
	MQQTClient(){};
	MQQTClient(MQQTClient const &);
	void operator=(MQQTClient const &);

	/*!
	 * Mosquitto instance
	 */
	struct mosquitto *mosq;

	/*!
	 * Gets called by mosquitto, when a message is received
	 */
	static void messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
};

} // namespace AutopinPlus
