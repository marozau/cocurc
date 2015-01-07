#ifndef COCURC_BQUEUE_CSYNC_H_
#define COCURC_BQUEUE_CSYNC_H_

#include <Windows.h>

namespace cocurc
{

	class sync
	{
	private:
		CRITICAL_SECTION  m_cs;
	public:
		sync()  { ZeroMemory( &m_cs, sizeof( m_cs ) ); InitializeCriticalSection( &m_cs ); }
		~sync()  { DeleteCriticalSection( &m_cs );   ZeroMemory( &m_cs, sizeof( m_cs ) ); }
		inline void       Lock()   { EnterCriticalSection( &m_cs ); }
		inline void       Unlock() { LeaveCriticalSection( &m_cs ); }
	};

	class mutex
	{
		sync& sync_;

	public:
		explicit mutex( sync& sync )
			: sync_( sync )
		{
			sync_.Lock();
		}
		~mutex()
		{
			sync_.Unlock();
		}
	};
}
#endif // COCURC_BQUEUE_CSYNC_H_
