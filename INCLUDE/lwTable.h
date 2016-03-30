/********************************************************************
	created:	2008/11/20
	created:	20:11:2008   14:24
	filename: 	d:\DOCUMENTS\_SVN\LuaWrapper\Include\lwTable.h
	file path:	d:\DOCUMENTS\_SVN\LuaWrapper\Include
	file base:	lwTable
	file ext:	h
	author:		Christer M. Vasseng
	
	purpose:	Allows for manipulation of user tables in Lua
*********************************************************************/


#ifndef h__lw__table__
#define h__lw__table__

#include "lwState.h"
#include "lwClassRegister.h"

#include <vector>

namespace lw
{

	enum LW_DATATYPE
	{
		LW_INT,
		LW_FLOAT,
		LW_BOOL,
		LW_STRING,
		LW_FUNCTION,
	};


	///Used to retreive data fields from lua tables
	/**A lua table is defined as follows:
	table = {
		dataString = "hello world",
		dataNumber = 0.3,
		dataInt = 1.0,
	}
	etc.*/
	
	
	class cTable
	{
	public:
		///Used to store functions
		struct sFunction
		{
			///Name
			std::string name;
			///Parameters
			std::vector<std::string> parameters;
		};
		///Value pair
		struct sValuePair
		{
			std::string name;
			std::string value;
			LW_DATATYPE tp;
		};
		///Constructor
		cTable(cState &state, const std::string& tableName);
		///Read a string from a field
		std::string readFieldString(const std::string& fieldName);
		///Read an integer from a field
		int readFieldInt(const std::string& fieldName);
		///Read a float from a field
		float readFieldFloat(const std::string& fieldName);
		///Read a bool from a field
		bool readFieldBool(const std::string& fieldName);
		///Read a class from a field
		template<class T>
		T* readFieldClass(const std::string& fieldName)
		{
			if (fetchTable())
			{
				lua_pushstring(m_state->State(), fieldName.c_str());
				lua_gettable(m_state->State(), -2);
				T* res = cLWClassRegister<T>::check(m_state->State(), -1);
				lua_pop(m_state->State(), 1);
				closeTable();
				return res;
			}
			return 0;
		}
		///Read a nested table
		/**Returns 0 if the table does not excist. If it returns a valid pointer, the application is responsible for 
		deleting the returned table when it is no longer needed.*/
		cTable* readTable(const std::string& fieldname);
		///Retreive everything (except functions) as value pairs
		bool readAll(std::vector<sValuePair> &dest);

		///Retreives all the strings in a given table
		/**NOTE: The ordering is totaly random (uses hashing) unless the data is arranged as an array, i.e.
		myData = {
			"hello",
			"world",
			...
		}*/
		bool readAllStrings(std::vector<std::string> &dest);
		///Retreives all the ints in a given table
		/**NOTE: The ordering is totaly random (uses hashing) unless the data is arranged as an array, i.e.
		myData = {
			1,
			2,
			...
		}*/
		bool readAllInts(std::vector<int> &dest);
		///Retreives all the floats in a given table
		/**NOTE: The ordering is totaly random (uses hashing) unless the data is arranged as an array, i.e.
		myData = {
			1.1,
			2.2,
			...
		}*/
		bool readAllFloats(std::vector<float> &dest);
		///Retreives all the functions in a given table
		bool readAllFunctions(std::vector<std::string> &dest);
		///Retreives all the bools in a given table
		/**NOTE: The ordering is totaly random (uses hashing) unless the data is arranged as an array, i.e.
		myData = {
			1.1,
			2.2,
			...
		}*/
		bool readAllBools(std::vector<bool> &dest);
		///Write a string to a table
		bool writeString(const std::string& name, const std::string& value);
		///Write an integer to a table
		bool writeInteger(const std::string &name, int value);
		///Write a custom class to a table
		template <class T> bool writeCustom(const std::string &name, T* c)
		{
			fetchTable();

			lua_pushstring(m_state->State(), name.c_str());

			if (c)
			{
				typedef struct { T *pT; } userdataType;
				T *obj = c;//new T( *cState::create(L) );  // call constructor for T objects
				userdataType *ud = static_cast<userdataType*>(lua_newuserdata(m_state->State(), sizeof(userdataType)));
				ud->pT = obj;  // store pointer to object in userdata

				luaL_getmetatable(m_state->State(), T::className().c_str());  // lookup metatable in Lua registry
				lua_setmetatable(m_state->State(), -3);

				//lua_pushlightuserdata(m_state->State(), ud);
			}
			else
			{
				lua_pushboolean(m_state->State(), 0);
				lua_settable(m_state->State(), -3);
				
			}

			

			closeTable();
			return true;
		}
	protected:
	private:
		///The name of the table we're working with
		std::string m_tablename;
		///The state we're working with
		cState *m_state;
		///Parent table(s)
		std::vector<std::string> m_parents;
		///Fetch table, used internaly to simplify things
		bool fetchTable();
		///Close the table
		bool closeTable();
	};
}

#endif
