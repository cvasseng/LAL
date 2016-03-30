/********************************************************************
	created:	2008/11/20
	created:	20:11:2008   14:24
	filename: 	d:\DOCUMENTS\_SVN\LuaWrapper\Include\lwVariable.h
	file path:	d:\DOCUMENTS\_SVN\LuaWrapper\Include
	file base:	lwVariable
	file ext:	h
	author:		Christer M. Vasseng
	
	purpose:	Allows for manipulation of global variables in Lua
*********************************************************************/


#ifndef h__lw_dataFetcher__
#define h__lw_dataFetcher__

#include <string>

#include "lwState.h"

namespace lw
{
	///Fetches global data from lua
	/**Global data in lua is defined by assigning values to variables outside the scope of functions and/or tables.*/
	class cVariable
	{
	public:
		///Constructor
		cVariable(cState &state);
		///Writes a string to the lua variable
		void writeString(const std::string& varName, const std::string& val);
		///Writes an integer to the lua variable
		void writeInt(const std::string& varName, int val);
		///Writes a float to the lua variable
		void writeFloat(const std::string& varName, float val);
		///Writes a bool to the lua variable
		void writeBool(const std::string& varName, bool val);
		///Write a class
		template<class T> void writeClass(const std::string& varName, T* c)
		{
			typedef struct { T *pT; } userdataType;

			//	lua_remove(L, 1);   // use classname:new(), instead of classname.new()
			T *obj = c;//new T( *cState::create(L) );  // call constructor for T objects
			userdataType *ud = static_cast<userdataType*>(lua_newuserdata(m_state->State(), sizeof(userdataType)));
			ud->pT = obj;  // store pointer to object in userdata

			luaL_getmetatable(m_state->State(), T::className().c_str());  // lookup metatable in Lua registry
			lua_setmetatable(m_state->State(), -2);

			lua_setfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
		}

		///Fetches a string
		std::string readString(const std::string& varName);
		///Fetches an int
		int readInt(const std::string& varName);
		///Fetches a float
		float readFloat(const std::string& varName);
		///Fetches a bool
		bool readBool(const std::string& varName);
		///Reads a class
		template<class T> T* readClass(const std::string& varName)
		{
			lua_getfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
			T* res = cLWClassRegister<T>::check(m_state->State(), -1);
			lua_pop(m_state->State(), 1);
		}
	protected:
	private:
		///The attached state
		cState *m_state;
	};
}

#endif