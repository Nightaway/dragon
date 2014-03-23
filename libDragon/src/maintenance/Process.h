#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../core/Macro.h"
#include "../core/JSCompiler.h"
#include "../core/modules/Config.h"
#include "../core/modules/Log.h"
#include "Task.h"

#include <unistd.h>
#include <vector>

NS_DRAGON
	class Process {
	protected:
		const char *path_;
		pid_t master_pid_;
		pid_t worker_pid_;
		std::vector<Task *> tasks_;
		
		JavaScriptCompiler jsc_;
		Config config_;
		Log    logInfo_;
		Log    logError_;

		void master_process_cycle();
		void worker_process_cycle();
		void spawn_process();
	public:
		Process();
		virtual ~Process();

		virtual void Init();
		virtual void Dispose();
		virtual void Run();
		virtual void SetPath();

		void PushTask(Task *t);
	};

NS_END

#endif

