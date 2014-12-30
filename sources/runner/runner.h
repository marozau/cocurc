#ifndef _COCURC_RUNNER_RUNNER_H_
#define _COCURC_RUNNER_RUNNER_H_

#include <vector>
#include <thread>
#include <functional>

#ifdef _WIN32 // Windows (x64 and x86)
namespace cocurc
{
	typedef DWORD affynity_mask_type;
}
#elif __linux__ // linux
namespace cocurc
{
	typedef DWORD affynity_mask_type;
}
#endif

namespace cocurc
{
	class runnable;
	class runner
	{
	public:
		typedef affynity_mask_type affynity_mask_type;
		static const affynity_mask_type default_affinity_mask = 0xffffffff;
		typedef std::function< void() > callback;

	private:
		typedef std::vector< std::thread > thread_storage;
		thread_storage thread_storage_;
		bool stopping_;

	public:
		explicit runner();
		~runner();
		//
		void start();
		void stop();
		//
		void separate_thread( runnable& to_run, const affynity_mask_type affinity_mask = default_affinity_mask );
		void separate_thread( runnable& to_run, callback cb, const affynity_mask_type affinity_mask = default_affinity_mask );

	private:
		void this_thread_( runnable& to_run );
	};
}

#endif // _COCURC_RUNNER_RUNNER_H_
