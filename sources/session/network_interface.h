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
	};
}

#endif // _COCURC_PROCESSOR_NETWORK_INTERFACE_H_
