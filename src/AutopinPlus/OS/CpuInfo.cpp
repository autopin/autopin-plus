/*
 * This file is part of Autopin+.
 * Copyright (C) 2015 Technische Universität München - LRR
 *
 * This file is licensed under the GNU General Public License Version 3
 */

#include <AutopinPlus/OS/CpuInfo.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>

namespace AutopinPlus {
namespace OS {

/*!
 * Distances between numa nodes.
 */
static std::vector<std::vector<int>> distances;

/*!
 * Mapping between cpu and numa node.
 */
static std::vector<int> mapping;

void CpuInfo::setupCpuInfo() {
	for (int i = 0; i < get_nprocs(); i++) mapping.push_back(0);

	auto nodes = parseSysRangeFile("/sys/devices/system/node/online");
	for (int node : nodes) {
		// Mapping
		auto cpus = parseSysRangeFile("/sys/devices/system/node/node" + QString::number(node) + "/cpulist");
		for (int cpu : cpus) {
			mapping[cpu] = node;
		}

		// Distance
		auto distance = parseSysNodeDistance("/sys/devices/system/node/node" + QString::number(node) + "/distance");
		distances.push_back(distance);
	}
}

int CpuInfo::getCpuCount() { return get_nprocs(); }

int CpuInfo::getCpuDistance(int cpu1, int cpu2) {
	int node1 = mapping[cpu1];
	int node2 = mapping[cpu2];
	return distances[node1][node2];
}

std::vector<int> CpuInfo::parseSysRangeFile(QString path) {
	std::vector<int> result;
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		auto text = stream.readAll();
		for (auto range : text.split(",")) {
			auto values = range.split("-");
			if (values.size() == 2) {
				int start = values.at(0).toInt();
				int stop = values.at(1).toInt();
				for (int value = start; value <= stop; value++) result.push_back(value);
			} else {
				result.push_back(values.at(0).toInt());
			}
		}
	}
	return result;
}

std::vector<int> CpuInfo::parseSysNodeDistance(QString path) {
	std::vector<int> result;
	QFile file(path);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		auto text = stream.readAll();
		for (auto value : text.split(" ")) {
			result.push_back(value.toInt());
		}
	}
	return result;
}

std::vector<int> CpuInfo::getCpusByNode(int node) {
	std::vector<int> result;
	for (uint i = 0; i < mapping.size(); i++) {
		if (mapping[i] == node) result.push_back(i);
	}
	return result;
}

int CpuInfo::getNodeCount() { return distances.size(); }

int CpuInfo::getNodeByCpu(int cpu) { return mapping[cpu]; }

} // namespace OS
} // namespace AutopinPlus
