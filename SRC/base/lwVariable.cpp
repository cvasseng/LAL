#include "../../INCLUDE/lwVariable.h"

using namespace lw;

lw::cVariable::cVariable( cState &state )
{
	m_state = &state;
}

void lw::cVariable::writeString( const std::string& varName, const std::string& val )
{
	lua_pushstring(m_state->State(), val.c_str()); 
	lua_setfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
}

void lw::cVariable::writeInt( const std::string& varName, int val )
{
	lua_pushinteger(m_state->State(), (lua_Integer)val);
	lua_setfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
}

void lw::cVariable::writeFloat( const std::string& varName, float val )
{
	lua_pushnumber(m_state->State(), (lua_Number)val);
	lua_setfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
}

void lw::cVariable::writeBool( const std::string& varName, bool val )
{
	lua_pushboolean(m_state->State(), val);
	lua_setfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
}

std::string lw::cVariable::readString( const std::string& varName )
{
	lua_getfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
	if (lua_isstring(m_state->State(), -1))
		return lua_tostring(m_state->State(), -1);
	m_state->log("Tried to read string from non-string variable (" + varName + ")");
	return "";
}

int lw::cVariable::readInt( const std::string& varName )
{
	lua_getfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
	if (lua_isnumber(m_state->State(), -1))
		return (int)lua_tointeger(m_state->State(), -1);
	m_state->log("Tried to read integer from non-integer variable (" + varName + ")");
	return 0;
}

float lw::cVariable::readFloat( const std::string& varName )
{
	lua_getfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
	if (lua_isnumber(m_state->State(), -1))
		return (float)lua_tonumber(m_state->State(), -1);
	m_state->log("Tried to read float from non-float variable (" + varName + ")");
	return 0;
}

bool lw::cVariable::readBool( const std::string& varName )
{
	lua_getfield(m_state->State(), LUA_GLOBALSINDEX, varName.c_str());
	if (lua_isboolean(m_state->State(), -1))
		return ((int)lua_toboolean(m_state->State(), -1) == 1);
	m_state->log("Tried to read bool from non-bool variable (" + varName + ")");
	return 0;
}
