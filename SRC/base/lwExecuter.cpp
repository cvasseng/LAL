#include "../../INCLUDE/lwExecuter.h"

#include <sstream>

using namespace lw;



lw::cExecuter::cExecuter( cState &state )
{
	m_pushed = 0;
	m_state = &state;
	m_hasPrepared = false;
}

lw::cExecuter::~cExecuter()
{

}

void lw::cExecuter::loadAndRunScript( std::string filename )
{
	if (!m_state)
	{
		if (!m_state->open())
		{
			std::string debug0 = "damn..:(";
			return;
		}
	}

	std::string debug = filename;
	int s = luaL_loadfile(m_state->m_state, debug.c_str());
	if (s == 0)
	{
			lua_pcall(m_state->m_state, 0, -1, 0);
	}
	m_state->reportErrors(s);
}

bool lw::cExecuter::prepareCall( const std::string &funcName, const std::string &tableName, bool expectReturn )
{
	m_preparedName = funcName;
	m_expectReturn = expectReturn;

	if (funcName.empty())
		return false;

	if (tableName.empty())
	{
		m_isTable = false;
		lua_getglobal(m_state->m_state, funcName.c_str());
		if(!lua_isfunction(m_state->m_state, -1))  
		{  
			lua_pop(m_state->m_state,1);  
		//	log(funcName + " is not a Lua function!");
			return false;  
		}
	}
	else
	{	
		m_tablename = tableName;
		m_isTable = true;

		std::stringstream ss(tableName);
		std::string table = m_tablename;
		bool first = true;
		while(!ss.eof())
		{
			ss >> table;
	
			if (table != " ")
			{
				if (first)
				{
					lua_getglobal(m_state->State(), table.c_str());
				}
				else
				{
					lua_pushstring(m_state->m_state, table.c_str());
					lua_gettable(m_state->m_state, -2);//LUA_GLOBALSINDEX
				}
			}

			first = false;
		}
		
		lua_pushstring(m_state->m_state, funcName.c_str());
		lua_gettable(m_state->m_state, -2); 
	}

	m_hasPrepared = true;
	m_pushed = 0;
	return true;
}

bool lw::cExecuter::call()
{	
	std::string result;
	return call(result);
}

bool lw::cExecuter::call( std::string &result )
{
	
	if (m_preparedName.empty())
		return false;

	if (!m_hasPrepared)
	{
		m_state->log( "Attempting to preform a Lua function call without preparing it first!" );
		return false;
	}

	m_hasPrepared = false;

	int ret = 0;
	if (m_expectReturn) ret = 1;

	/* do the call (2 arguments, 1 result) */  
	if (lua_pcall(m_state->m_state, m_pushed, ret, 0) != 0) 
	{  
		m_state->log( "Error running Lua function \"" + m_preparedName + "\": " + std::string(lua_tostring(m_state->m_state, -1)) );
		return false;  
	}  

	if (m_expectReturn)
	{
		result = "";
		if (lua_isstring(m_state->m_state, -1))
			result = lua_tostring(m_state->m_state, -1);
		if (lua_isboolean(m_state->m_state, -1))
		{
			if (lua_toboolean(m_state->m_state, -1) == 1)
				result = "true";
		}
	}

	return true;
}

cState* lw::cExecuter::getState() const
{
	return m_state;
}

void lw::cExecuter::push( const std::string& val )
{
	lua_pushstring(m_state->m_state, val.c_str());
	m_pushed++;
}

void lw::cExecuter::push( float val )
{
	lua_pushnumber(m_state->m_state, val);
	m_pushed++;
}

void lw::cExecuter::push( int val )
{
	

	lua_pushnumber(m_state->m_state, val);
	m_pushed++;
}

