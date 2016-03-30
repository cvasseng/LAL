/********************************************************************
	created:	2008/11/20
	created:	20:11:2008   14:23
	filename: 	d:\DOCUMENTS\_SVN\LuaWrapper\Include\lwState.h
	file path:	d:\DOCUMENTS\_SVN\LuaWrapper\Include
	file base:	lwState
	file ext:	h
	author:		Christer M. Vasseng
	
	purpose:	Wraps a lua_State object and adds additional features
				for C-function registering and other neat stuff.
*********************************************************************/


#ifndef h__lw_state__
#define h__lw_state__

#include <string>
#include <iostream>

#include "../SRC/lwLua.h"

namespace lw
{

	class cExecuter;
	class cState
	{
		friend class cExecuter;
	public:
		///Constructor
		cState();
		///De-constructor
		~cState();
		///Opens a new state
		bool open();
		///Destroys the lua state
		bool destroy();
		///Registers a new function for the state
		void registerFunction(const std::string& name, lua_CFunction func);
		///Enable verbose logging
		void verboseLogging(bool flag);

		///Gets a string from the stack
		std::string getString(unsigned int index);
		///Gets a float from the stack
		float getFloat(unsigned int index);
		///Gets an int from the stack
		int getInt(unsigned int index);
		///Gets a bool from the stack
		bool getBool(unsigned int index);
		///Is the state open?
		bool isOpen();
		///Reports a verbal error based on status
		bool reportErrors(int status);
		///Logs an error 
		void log( std::string err);
		///Returns the actual lua state
		lua_State* State();
		///Creates a new lua state wrapped around L
		static cState* create(lua_State *L);

		///Operator overload
		void operator=(cState &other)
		{
			m_state = other.m_state;
		}
	protected:
		///The actual lua state
		lua_State *m_state;
		///Verbose logging?
		bool m_verboseLogging;

		///Checks the number of things on the stack
		bool checkParCount(unsigned int requiredPars);
	private:
	};
}

#endif
