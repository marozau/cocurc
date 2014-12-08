#ifndef _COCURC_PROCESSOR_LOGIC_H_
#define _COCURC_PROCESSOR_LOGIC_H_

namespace cocurc
{
	class logic
	{
	public:
		virtual ~logic() {}
		//
		virtual bool process() = 0;
	};
}

#endif // _COCURC_PROCESSOR_LOGIC_H_
