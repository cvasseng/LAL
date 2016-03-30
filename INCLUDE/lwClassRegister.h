/********************************************************************
	created:	2008/11/20
	created:	20:11:2008   14:22
	filename: 	d:\DOCUMENTS\_SVN\LuaWrapper\Include\lwClassRegister.h
	file path:	d:\DOCUMENTS\_SVN\LuaWrapper\Include
	file base:	lwClassRegister
	file ext:	h
	author:		Christer M. Vasseng
	
	purpose:	Registers a class for use in lua.
*********************************************************************/


#ifndef h__lw_classRegister__
#define h__lw_classRegister__

#include "lwState.h"

#include <vector>


namespace lw
{
	#define lwMethod(class, name) {#name, &class::name}

	template <typename T>
	class cLWClassRegister
	{
		typedef struct { T *pT; } userdataType;
	public:
		typedef int (T::*mfp)(lua_State *L);
		typedef struct { std::string name; mfp mfunc; } RegType;

		///Constructor
		cLWClassRegister(cState &state)
		{
			m_state = &state;
		}
		///Destructor
		~cLWClassRegister()
		{

		}

		///Adds a function
		void addFunction(const std::string &name, mfp func)
		{
			RegType *tmp = new RegType;
			tmp->mfunc = func;
			tmp->name = name;
			m_methods.push_back(tmp);
		}
		
		///Register the class 
		void doRegistration()
		{
			lua_newtable(m_state->State());
			int methods = lua_gettop(m_state->State());
			std::string name = T::className();
			luaL_newmetatable(m_state->State(), name.c_str());
			int metatable = lua_gettop(m_state->State());

			// store method table in globals so that
			// scripts can add functions written in Lua.
			lua_pushstring(m_state->State(), name.c_str());
			lua_pushvalue(m_state->State(), methods);
			lua_settable(m_state->State(), LUA_GLOBALSINDEX);

			lua_pushliteral(m_state->State(), "__metatable");
			lua_pushvalue(m_state->State(), methods);
			lua_settable(m_state->State(), metatable);  // hide metatable from Lua getmetatable()

			lua_pushliteral(m_state->State(), "__index");
			lua_pushvalue(m_state->State(), methods);
			lua_settable(m_state->State(), metatable);

			lua_pushliteral(m_state->State(), "__tostring");
			lua_pushcfunction(m_state->State(), tostring_T);
			lua_settable(m_state->State(), metatable);

			lua_pushliteral(m_state->State(), "__gc");
			lua_pushcfunction(m_state->State(), gc_T);
			lua_settable(m_state->State(), metatable);

			lua_newtable(m_state->State());                // mt for method table
			int mt = lua_gettop(m_state->State());
			lua_pushliteral(m_state->State(), "__call");
			lua_pushcfunction(m_state->State(), new_T);
			lua_pushliteral(m_state->State(), "new");
			lua_pushvalue(m_state->State(), -2);           // dup new_T function
			lua_settable(m_state->State(), methods);       // add new_T to method table
			lua_settable(m_state->State(), mt);            // mt.__call = new_T
			lua_setmetatable(m_state->State(), methods);


			for (unsigned int i = 0; i < m_methods.size(); i++)
			{
				lua_pushstring(m_state->State(), m_methods[i]->name.c_str());
				lua_pushlightuserdata(m_state->State(), (void*)m_methods[i]);
				lua_pushcclosure(m_state->State(), thunk, 1);
				lua_settable(m_state->State(), methods);
			}

			lua_pop(m_state->State(), 2);  // drop metatable and method table

		}

		// get userdata from Lua stack and return pointer to T object
		static T *check(lua_State *L, int narg) {
			userdataType *ud = static_cast<userdataType*>(luaL_checkudata(L, narg, T::className().c_str()));
			if(!ud) 
				luaL_typerror(L, narg, T::className().c_str());
			return ud->pT;  // pointer to T object
		}

	protected:
		///The attached state
		cState *m_state;
		///Methods
		std::vector<RegType*> m_methods;
		
	private:
		// member function dispatcher
		static int thunk(lua_State *L) {
			// stack has userdata, followed by method args
			T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
			lua_remove(L, 1);  // remove self so member function args start at index 1
			// get member function from upvalue
			RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
			return (obj->*(l->mfunc))(L);  // call member function
		}

		// create a new T object and
		// push onto the Lua stack a userdata containing a pointer to T object
		static int new_T(lua_State *L) {
			/*
			lua_remove(L, 1);   // use classname:new(), instead of classname.new()
			T *obj = new T;  // call constructor for T objects
			userdataType *ud = static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
			ud->pT = obj;  // store pointer to object in userdata
			luaL_getmetatable(L, T::className().c_str());  // lookup metatable in Lua registry
			lua_setmetatable(L, -2);
			*/
			return 0;  // userdata containing pointer to T object
		}

		// garbage collection metamethod
		//TODO: FIX!! HUGE MEMORY LEAK NOW!
		//This actually works, but will crash if the class is instanced in C++ and not in Lua.
		static int gc_T(lua_State *L) {
			//userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
			//T *obj = ud->pT;
			//delete obj;  // call destructor for T objects
			return 0;
		}

		static int tostring_T (lua_State *L) {
			char buff[32];
			userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
			T *obj = ud->pT;
			sprintf(buff, "%p", obj);
			lua_pushfstring(L, "%s (%s)", T::className().c_str(), buff);
			return 1;
		}

	};
}

#endif
