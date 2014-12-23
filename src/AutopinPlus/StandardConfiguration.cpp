/*
 * Autopin+ - Automatic thread-to-core-pinning tool
 * Copyright (C) 2012 LRR
 *
 * Author:
 * Florian Walter
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

#include <AutopinPlus/StandardConfiguration.h>

namespace AutopinPlus {

StandardConfiguration::StandardConfiguration(const QString configText, AutopinContext &context)
	: Configuration(context), configText(configText) {

	this->name = "StandardConfiguration";
}

void StandardConfiguration::init() { parseConfigurationFile(); }

QStringList StandardConfiguration::getConfigOptionList(QString opt) const {
	QStringList result;

	auto it = options.find(opt);
	if (it != options.end()) return it->second; // BIG BIG TODO!!!

	return result;
}

void StandardConfiguration::parseConfigurationFile() {
	QString line;
	QTextStream config(&configText);

	while (!config.atEnd()) {
		line = config.readLine();

		if (line[0] == '#') continue;

		getArgs(line);
	}
}

void StandardConfiguration::getArgs(QString arg) {
	QString argl, argr;
	QStringList optlist;
	int sep_pos = -1;
	arg_pair opt_pair;

	if ((sep_pos = arg.indexOf("+=")) != -1) {
		argl = arg.mid(0, sep_pos);
		argr = arg.mid(sep_pos + 2);

		argl = argl.trimmed();
		optlist = argr.split(' ', QString::SkipEmptyParts);

		opt_pair = arg_pair(argl, optlist);

		addOption(opt_pair);
	} else if ((sep_pos = arg.indexOf("-=")) != -1) {
		argl = arg.mid(0, sep_pos);
		argr = arg.mid(sep_pos + 2);

		argl = argl.trimmed();
		optlist = argr.split(' ', QString::SkipEmptyParts);

		opt_pair = arg_pair(argl, optlist);

		delOption(opt_pair);
	} else if ((sep_pos = arg.indexOf("=")) != -1) {
		argl = arg.mid(0, sep_pos);
		argr = arg.mid(sep_pos + 1);

		// remove white whitspace from the start and the end of the arguments
		argl = argl.trimmed();
		optlist = argr.split(' ', QString::SkipEmptyParts);

		opt_pair = arg_pair(argl, optlist);

		setOption(opt_pair);
	} else
		context.report(Error::BAD_CONFIG, "option_format", "Invalid option format: \"" + arg + "\"");
}

void StandardConfiguration::setOption(StandardConfiguration::arg_pair opt) {
	if (opt.second.isEmpty()) return;

	options[opt.first] = opt.second;
}

void StandardConfiguration::addOption(StandardConfiguration::arg_pair opt) {
	if (opt.second.isEmpty()) return;
	QStringList new_opts = options[opt.first];
	new_opts.append(opt.second);
	new_opts.removeDuplicates();
	options[opt.first] = new_opts;
}

void StandardConfiguration::delOption(StandardConfiguration::arg_pair opt) {
	if (opt.second.isEmpty()) return;

	if (options.find(opt.first) != options.end()) {
		QStringList new_opts = options[opt.first];
		for (int i = 0; i < opt.second.size(); i++) new_opts.removeAll(opt.second[i]);
		options[opt.first] = new_opts;
	}
}

} // namespace AutopinPlus
