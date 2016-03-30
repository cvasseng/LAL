/********************************************************************
	created:	2008/11/20
	created:	20:11:2008   14:22
	filename: 	d:\documents\_svn\luawrapper\include\lwExecuter.h
	file path:	d:\documents\_svn\luawrapper\include
	file base:	lwExecuter
	file ext:	h
	author:		Christer M. Vasseng
	
	purpose:	Allows execution of scripts, and calling of lua
				functions.
*********************************************************************/


#ifndef h__lw_executer__
#define h__lw_executer__

#include "lwState.h"

namespace lw
{
	class cExecuter
	{
	public:
		///Constructor
		cExecuter(cState &state);
		///Destructor
		~cExecuter();
		///Loads and runs a script
		void loadAndRunScript(std::string filename);
		///Prepares for a call
		bool prepareCall(const std::string &funcName, const std::string &tableName = "", bool expectReturn = false);
		///Calls a function in the attached state
		bool call();
		///Calls a function in the attached state, and stores the result in result
		bool call(std::string &result);
		///Returns the attached state
		cState* getState()const;
		///Pushes a string to the stack
		void push(const std::string& val);
		///Pushes a float to the stack
		void push(float val);
		///Pushes an int to the stack
		void push(int val);
		///Pushes a class to the stack
		template <class T>
		void push(T* c)
		{
			if (c)
			{
			//	if (m_isTable)
			//		lua_getglobal(m_state->State(), m_tablename.c_str());
				typedef struct { T *pT; } userdataType;

				//	lua_remove(L, 1);   // use classname:new(), instead of classname.new()
				T *obj = c;//new T( *cState::create(L) );  // call constructor for T objects
				userdataType *ud = static_cast<userdataType*>(lua_newuserdata(m_state->State(), sizeof(userdataType)));
				ud->pT = obj;  // store pointer to object in userdata

				luaL_getmetatable(m_state->State(), T::className().c_str());  // lookup metatable in Lua registry
				lua_setmetatable(m_state->State(), -2);
				//lua_pushlightuserdata(m_state->State(), ud);
			}
			else
			{
				lua_pushboolean(m_state->State(), 0);
			}
			m_pushed++;
		}
	protected:
	private:
		std::string m_tablename;
		bool m_isTable;
		///The state which the executer is attached to
		cState *m_state;
		///The filename opened by the executer
		std::string m_filename;
		///Has prepared call?
		bool m_hasPrepared;
		///Prepared function name
		std::string m_preparedName;
		///Keeps track of pushed count
		unsigned int m_pushed;
		///Expecting the current call to return something?
		bool m_expectReturn;
	};

}

#endif
