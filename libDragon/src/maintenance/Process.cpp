#include "Process.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <memory.h>
#include <stdlib.h>
#include <cassert>

#include <boost/foreach.hpp>

#include "Daemon.h"

static void spawn_process();

sig_atomic_t sig_term  = 0;
sig_atomic_t sig_alarm = 0;

static void signal_handler(int signo)
{
	switch(signo)
	{
		case SIGINT:
		sig_term = 1;
		break;

		case SIGALRM:
		sig_alarm = 1;
		break;
	}
}

NS_USING_DRAGON

void Process::master_process_cycle()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
 	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction error!");
		exit(2);
        }

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigaddset(&set, SIGALRM);
	sigaddset(&set, SIGIO);
	sigaddset(&set, SIGINT);

	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
		perror("sigprocmask error!");
		exit(1);
	}
	sigemptyset(&set); 

	if (config_["Maintenance"]["Deamon"] == "1") {

		if (becameDaemon() == -1) {
			perror("bacameDaemon");
			exit(1);
		}
	}

	spawn_process();

	for (;;)
	{
		sigsuspend(&set);
		if (sig_term) 
		{
			kill(worker_pid_, SIGTERM);
			exit(0);
		}
	}
}

void Process::worker_process_cycle()
{
	sigset_t set;
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = signal_handler;
 	if (sigaction(SIGALRM, &sa, NULL) == -1) {
		perror("sigaction error!");
		exit(2);
        }

	itimerval tv;
	tv.it_value.tv_sec  = 5;
	tv.it_value.tv_usec = 0;

	tv.it_interval.tv_sec  = atoi(config_["Maintenance"]["Interval"].c_str());
	tv.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &tv, NULL) == -1)
	{
		perror("set timer error!");
		exit(3);
	}
	sigemptyset(&set);
	for (;;)
	{
		sigsuspend(&set);
		if (sig_alarm) 
		{
			sig_alarm = 0;
			BOOST_FOREACH(Task *t, tasks_)
			{
				assert(t);
				t->Run();
			}
		}
	}
}

void Process::spawn_process()
{
	int pid;
	switch (pid = fork())
	{
		case -1:
		perror("fork errro!");
		exit(1);

		case 0:
		worker_process_cycle();
		break;

		default :
		worker_pid_ = pid;
		break;
	}
}

Process::Process() : config_(jsc_)
{

}

Process::~Process()
{

}

void Process::Init()
{
	// #1
	SetPath();

	// #2
	master_pid_ = getpid();

	// #3
	std::string configPath = path_;
	configPath += "/conf/App.conf";
	config_.Parse(configPath.c_str());
}

void Process::Dispose()
{

}

void Process::Run()
{
	master_process_cycle();
}

void Process::SetPath()
{
	path_ = "";
}

void Process::PushTask(Task *t)
{
	tasks_.push_back(t);
}
