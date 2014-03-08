#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../core/Macro.h"
#include "Task.h"

#include <unistd.h>

#include <vector>

#include "../core/JSCompiler.h"

NS_DRAGON

	enum ProcessType {
		kProcessMaster,
		kProcessWorker
	};

	class Process {
		pid_t id_;
		ProcessType type_;
		std::vector<Task> tasks_;
		
		JavaScriptCompiler jsc_;

		public:
		Process();
		~Process();

		void Run();
	};


NS_END

#endif

