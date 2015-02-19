/*
 * This file is part of Autopin+.
 * Copyright (C) 2015 Technische Universität München - LRR
 *
 * This file is licensed under the GNU General Public License Version 3
 */

#include <AutopinPlus/ProcessTree.h>

namespace AutopinPlus {

ProcessTree ProcessTree::empty(0);

ProcessTree::ProcessTree() {}

ProcessTree::ProcessTree(int pid) : pid(pid) { tasks.insert(pid); }

ProcessTree::ProcessTree(int pid, ProcessTree::autopin_tid_list tasks) : pid(pid), tasks(std::move(tasks)) {
	this->tasks.insert(pid);
}

void ProcessTree::addChildProcess(int ppid, int cpid) {
	if (pid == ppid) {
		ProcessTree new_child_process(cpid);
		child_procs[cpid] = new_child_process;
	} else {
		for (auto &elem : child_procs) elem.second.addChildProcess(ppid, cpid);
	}
}

void ProcessTree::addProcessTask(int pid, int tid) {
	if (this->pid == pid) {
		tasks.insert(tid);
	} else {
		for (auto &elem : child_procs) elem.second.addProcessTask(pid, tid);
	}
}

void ProcessTree::addProcessTaskList(int pid, ProcessTree::autopin_tid_list &tids) {
	if (this->pid == pid) {
		tasks.insert(tids.begin(), tids.end());
	} else {
		for (auto &elem : child_procs) elem.second.addProcessTaskList(pid, tids);
	}
}

ProcessTree::autopin_tid_list ProcessTree::getTasks(int pid) {
	ProcessTree::autopin_tid_list result;

	const ProcessTree &tasknode = findTask(pid);
	if (tasknode != ProcessTree::empty) result = tasknode.tasks;

	return result;
}

ProcessTree::autopin_tid_list ProcessTree::getAllTasks() {
	ProcessTree::autopin_tid_list result;

	result.insert(tasks.begin(), tasks.end());

	for (auto &elem : child_procs) {
		ProcessTree::autopin_tid_list tmp = elem.second.getAllTasks();
		result.insert(tmp.begin(), tmp.end());
	}

	return result;
}

const ProcessTree &ProcessTree::findTask(int tid) {

	if (tasks.find(tid) != tasks.end())
		return *this;
	else {
		for (auto &elem : child_procs) {
			if (elem.second.findTask(tid) != ProcessTree::empty) return elem.second.findTask(tid);
		}
	}

	return ProcessTree::empty;
}

const ProcessTree &ProcessTree::findProcess(int pid) {
	if (this->pid == pid)
		return *this;
	else {
		for (auto &elem : child_procs) {
			if (elem.second.findProcess(pid) != ProcessTree::empty) return elem.second.findProcess(pid);
		}
	}

	return ProcessTree::empty;
}

bool ProcessTree::operator<(const ProcessTree &comp) const {
	if (pid < comp.pid)
		return true;
	else
		return false;
}

bool ProcessTree::operator==(const ProcessTree &comp) const {
	if (pid == comp.pid) return true;

	return false;
}

bool ProcessTree::operator!=(const ProcessTree &comp) const { return !(*this == comp); }

} // namespace AutopinPlus
