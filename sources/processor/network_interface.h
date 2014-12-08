#ifndef _COCURC_PROCESSOR_NETWORK_INTERFACE_H_
#define _COCURC_PROCESSOR_NETWORK_INTERFACE_H_

namespace cocurc
{
	class network_interface
	{
	public:
		virtual ~network_interface() {}
		//
		virtual bool connect() = 0;
		virtual void disconnect() = 0;
		virtual bool logon() = 0;
		virtual void logout() = 0;
		//
		virtual bool idle() = 0;
		//
		virtual bool ping() = 0;
	};
	class network_listener
	{
	public:
		virtual ~network_listener() {}
		//
		virtual bool recv() = 0;
	};
	class network_speaker
	{
	public:
		virtual ~network_speaker() {}
		//
		virtual bool send() = 0;
	};
}

#endif // _COCURC_PROCESSOR_NETWORK_INTERFACE_H_
