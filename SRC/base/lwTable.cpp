#include "../../INCLUDE/lwTable.h"

#include <sstream>

using namespace lw;

lw::cTable::cTable( cState &state, const std::string& tableName )
{
	m_state = &state;
	m_tablename = tableName;
}

std::string lw::cTable::readFieldString( const std::string& fieldName )
{
	
	if (fetchTable())
	{	
		std::string result;
		lua_pushstring(m_state->State(), fieldName.c_str());
		lua_gettable(m_state->State(), -2);  /* get background[key] */
		if (lua_isnil(m_state->State(), -2))
		{
			m_state->log(fieldName + " was NILL in table. Read aborted.");

			lua_pop(m_state->State(), 1);

			closeTable();
			return "";
		}
		
		if (!lua_isstring(m_state->State(), -1))
		{
			m_state->log("Tried to fetch string from non-string table field");

			lua_pop(m_state->State(), 1);

			closeTable();
			return "";
		}
		result = lua_tostring(m_state->State(), -1);
		lua_pop(m_state->State(), 1);  /* remove number */

		//lua_pop(m_state->State(), 1);

		closeTable();
		
		return result;
	}
	return "";
}

int lw::cTable::readFieldInt( const std::string& fieldName )
{
	int debug_before = lua_gettop(m_state->State());
	if (fetchTable())
	{
		int result;
		lua_pushstring(m_state->State(), fieldName.c_str());
		lua_gettable(m_state->State(), -2);  /* get background[key] */
		if (lua_isnil(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1); //pop key name
			m_state->log(fieldName + " was NILL in table. Read aborted.");
			closeTable();
			return 0;
		}
		if (!lua_isnumber(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1); //pop key name
			m_state->log("Tried to fetch integer from non-integer table field");
			closeTable();
			return 0;
		}
		result = (int)lua_tointeger(m_state->State(), -1);
		lua_pop(m_state->State(), 1);  /* remove number */
		//lua_pop(m_state->State(), 1); //pop key name
		closeTable();
		
		int debug_after = lua_gettop(m_state->State());
		return result;
	}
	return 0;
}

float lw::cTable::readFieldFloat( const std::string& fieldName )
{
	if (fetchTable())
	{
		float result;
		lua_pushstring(m_state->State(), fieldName.c_str());
		lua_gettable(m_state->State(), -2);  /* get background[key] */
		if (lua_isnil(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1); //pop key name
			m_state->log(fieldName + " was NILL in table. Read aborted.");
			closeTable();
			return 0.f;
		}
		if (!lua_isnumber(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1); //pop key name
			m_state->log("Tried to fetch float from non-float table field");
			closeTable();
			return 0;
		}
		result = (float)lua_tonumber(m_state->State(), -1);
		lua_pop(m_state->State(), 1);  /* remove number */
		//lua_pop(m_state->State(), 1); //pop key name
		closeTable();
		return result;
	}
	return 0.f;
}

bool lw::cTable::readFieldBool( const std::string& fieldName )
{
	if (fetchTable())
	{
		bool result = false;
		lua_pushstring(m_state->State(), fieldName.c_str());
		lua_gettable(m_state->State(), -2);  /* get background[key] */
		if (lua_isnil(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1); //pop key name
			m_state->log(fieldName + " was NILL in table. Read aborted.");
			closeTable();
			return false;
		}
		if (!lua_isnumber(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1); //pop key name
			m_state->log("Tried to fetch bool from non-bool table field");
			closeTable();
			return false;
		}
		result = (int)lua_tointeger(m_state->State(), -1) == 1;
		lua_pop(m_state->State(), 1);  /* remove number */
		//lua_pop(m_state->State(), 1); //pop key name
		closeTable();
		return result;
	}
	return false;
}

cTable* lw::cTable::readTable( const std::string& fieldname )
{
	cTable* table = new cTable(*m_state, m_tablename);
	for (unsigned int i = 0; i < m_parents.size(); i++)
		table->m_parents.push_back(m_parents[i]);
	table->m_parents.push_back(fieldname);

	if (!table->fetchTable())
	{
		
		delete table;
		return 0;
	}
	table->closeTable();
	return table;
}

bool lw::cTable::fetchTable()
{
	
	lua_getglobal(m_state->State(), m_tablename.c_str());
	if (!lua_istable(m_state->State(), -1))
	{
		//ERROR!
		//m_state->log("Tried to fetch table from non-table (" + m_tablename + ")");
		return false;
	}
	
	for (unsigned int i = 0; i < m_parents.size(); i++)
	{
		lua_pushstring(m_state->State(), m_parents[i].c_str());
		lua_gettable(m_state->State(), -2);

		if (!lua_istable(m_state->State(), -1))
		{
			lua_pop(m_state->State(), 1 + (int)i + 1);
			
			//ERROR!
			//m_state->log("Tried to fetch table from non-table (" + m_parents[i] + ")");
			return false;
		}
	}
	return true;
}

bool lw::cTable::closeTable()
{
	int debug_before = lua_gettop(m_state->State());
	
	lua_pop(m_state->State(), 1 + (int)m_parents.size());

	int debug_after = lua_gettop(m_state->State());

#ifdef WIN32
	if (debug_after != debug_before)
	{
		int lol = 10;
	}
#endif
	return true;
}



bool lw::cTable::readAll( std::vector<sValuePair> &dest )
{
	fetchTable();
	/* table is in the stack at index 't' */
	lua_pushnil(m_state->State());  /* first key */
	while (lua_next(m_state->State(), -2) != 0) 
	{
		//if (!lua_isfunction(m_state->State(), -1))
		{
			sValuePair data;
			

			std::stringstream ss;

			if (lua_isstring(m_state->State(), -1))
			{
				data.value = lua_tostring(m_state->State(), -1);
				data.tp = LW_STRING;
			}
			else if (lua_isnumber(m_state->State(), -1))
			{
				float num = (float)lua_tonumber(m_state->State(), -1);
				ss << num;
				data.value = ss.str();
				data.tp = LW_FLOAT;
			}
			else if (lua_isnumber(m_state->State(), -1))
			{
				int num = (int)lua_tointeger(m_state->State(), -1);
				ss << num;
				data.value = ss.str();
				data.tp = LW_INT;
			}
			else if (lua_isboolean(m_state->State(), -1))
			{
				if((int)lua_toboolean(m_state->State(), -1) == 1)
				{
					data.value = "1";
				}
				else
				{
					data.value = "0";
				}
				data.tp = LW_BOOL;
			}
			else if (lua_isfunction(m_state->State(), -1))
			{
				data.tp = LW_FUNCTION;
			}

			data.name = lua_tostring(m_state->State(), -2);

			dest.push_back( data );
		}

		lua_pop(m_state->State(), 1);
	}
	closeTable();
	return true;
}

bool lw::cTable::readAllStrings(std::vector<std::string> &dest)
{

	fetchTable();
	/* table is in the stack at index 't' */
     lua_pushnil(m_state->State());  /* first key */
     while (lua_next(m_state->State(), -2) != 0) 
	 {
		 if (lua_type(m_state->State(), -1) == LUA_TSTRING)
		 {
			 dest.push_back( lua_tostring(m_state->State(), -1) );
			 //lua_pop(m_state->State(), -1); //pop value
		 }

       lua_pop(m_state->State(), 1);
     }
	 //lua_pop(m_state->State(), 1);

	 closeTable();
	return true;
}

bool lw::cTable::readAllInts(std::vector<int> &dest)
{
	fetchTable();
	/* table is in the stack at index 't' */
     lua_pushnil(m_state->State());  /* first key */
     while (lua_next(m_state->State(), -2) != 0) 
	 {
		 if (lua_isnumber(m_state->State(), -1))
		 {
			 dest.push_back( (int)lua_tointeger(m_state->State(), -1) );
			 // lua_pop(m_state->State(), 1); //pop value
		 }

       lua_pop(m_state->State(), 1);
     }
	 closeTable();
	return true;

}

bool lw::cTable::readAllFloats(std::vector<float> &dest)
{
	fetchTable();
	/* table is in the stack at index 't' */
     lua_pushnil(m_state->State());  /* first key */
     while (lua_next(m_state->State(), -2) != 0) 
	 {
		 if (lua_isnumber(m_state->State(), -1))
		 {
			 dest.push_back( (float)lua_tonumber(m_state->State(), -1) );
			// lua_pop(m_state->State(), 1); //pop value
		 }

       lua_pop(m_state->State(), 1);
     }
	 closeTable();
	return true;
}

bool lw::cTable::readAllBools(std::vector<bool> &dest)
{
	fetchTable();
	/* table is in the stack at index 't' */
     lua_pushnil(m_state->State());  /* first key */
     while (lua_next(m_state->State(), -2) != 0) 
	 {
		 if (lua_isboolean(m_state->State(), -1))
		 {
			 bool val = (int)lua_toboolean(m_state->State(), -1) == 1;
			 dest.push_back( val );
			// lua_pop(m_state->State(), 1); //pop value
		 }

       lua_pop(m_state->State(), 1);
     }
	 closeTable();
	return true;
}



bool lw::cTable::readAllFunctions( std::vector<std::string> &dest )
{
	fetchTable();
	/* table is in the stack at index 't' */
	lua_pushnil(m_state->State());  /* first key */
	while (lua_next(m_state->State(), -2) != 0) 
	{
		if (lua_isfunction(m_state->State(), -1))
		{
			std::string func = lua_tostring(m_state->State(), -2);
			dest.push_back( func );
		}

		lua_pop(m_state->State(), 1);
	}
	closeTable();
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool lw::cTable::writeString( const std::string& name, const std::string& value )
{
	fetchTable();

	lua_pushstring(m_state->State(), name.c_str());
	lua_pushstring(m_state->State(), value.c_str());
	lua_settable(m_state->State(), -3);

	closeTable();

	return true;
}

bool lw::cTable::writeInteger( const std::string &name, int value )
{
	fetchTable();

	lua_pushstring(m_state->State(), name.c_str());
	lua_pushinteger(m_state->State(), value);
	lua_settable(m_state->State(), -3);

	closeTable();

	return true;
}