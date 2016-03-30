/********************************************************************
	created:	2008/11/20
	created:	20:11:2008   14:21
	filename: 	d:\documents\_svn\luawrapper\include\lwClass.h
	file path:	d:\documents\_svn\luawrapper\include
	file base:	lwClass
	file ext:	h
	author:		Christer M. Vasseng
	
	purpose:	Legacy, not in use anymore.
*********************************************************************/


#ifndef h__lw_class__
#define h__lw_class__

#include "lwExecuter.h"

namespace lw
{
	class cLWClass
	{
	public:
		///Constructor
		cLWClass(cState &scriptState);
		///Destructor
		virtual ~cLWClass();
	protected:
		///Script state which this object lives in
		cState *m_state;
		///Executer used for executing Lua callbacks
		cExecuter m_scriptExecuter;
	private:
	};
}

#endif
