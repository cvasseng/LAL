#include "../../INCLUDE/lwState.h"


#include <stdlib.h>

using namespace lw;

static
void *my_l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) 
{
	(void)ud;
	(void)osize;
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else {
		void *ret = realloc(ptr, nsize);
		return ret;
	}
}

lw::cState::cState()
{
	m_state = 0;
	m_verboseLogging = true;

#ifndef PS3
//	if (MessageBoxEx(0, "Enable verbose Lua logging?", "Lua", MB_YESNO, 0) == IDYES)
	{
	//	m_verboseLogging = true;
	}
#endif
}

lw::cState::~cState()
{
	//TODO: CRASHESES! WHYS??
	//destroy();
}

bool lw::cState::open()
{
	destroy();
	//m_state = lua_newstate(my_l_alloc, 0);
	m_state = lua_open();
	//lua_gc(m_state,  LUA_GCSETPAUSE, 100);
	if (!m_state)
		return false;
	luaopen_base(m_state);
	luaopen_math(m_state);
	return true;
}

bool lw::cState::destroy()
{
	
	if (m_state)
	{
		lua_close(m_state);
		m_state = 0;
	}
	//*/
	return true;
}

void lw::cState::registerFunction( const std::string& name, lua_CFunction func )
{
	lua_register(m_state, name.c_str(), func);
}

std::string lw::cState::getString( unsigned int index )
{
	return std::string(lua_tostring(m_state, index));
}

float lw::cState::getFloat( unsigned int index )
{
	return (float)lua_tonumber(m_state, index);
}

int lw::cState::getInt( unsigned int index )
{
	return (int)lua_tointeger(m_state, index);
}

bool lw::cState::getBool( unsigned int index )
{
	return getInt(index) == 1;
}

bool lw::cState::isOpen()
{
	return m_state != 0;
}

bool lw::cState::reportErrors( int status )
{
	if ( status!=0 ) 
	{
		log("*** Script Error in  : " + std::string(lua_tostring(m_state, -1)));
		lua_pop(m_state, 1); // remove error message
		return true;
	}
	return false;
}

bool lw::cState::checkParCount( unsigned int requiredPars )
{
	unsigned int pars = lua_gettop(m_state);
	if (pars < requiredPars)
	{
		log("*** Script Error: Trying to fetch invalid id from stack!");
		lua_pushnumber(m_state, 0); // return value
		return false;
	}
	return true;
}

lua_State* lw::cState::State()
{
	return m_state;
}

cState* lw::cState::create( lua_State *L )
{
	cState *tmp = new cState;
	tmp->m_state = L;
	//tmp->m_verboseLogging = m_verboseLogging;
	return tmp;
}

void lw::cState::log(  std::string err )
{
	std::string debug = err; //For setting breakpoints..
	if (m_verboseLogging)
		std::cout << err << std::endl;
}

void lw::cState::verboseLogging( bool flag )
{
	m_verboseLogging = flag;
}
