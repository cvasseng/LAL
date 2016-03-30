/************************************************************************
This sample demonstrates calling a C function from lua
This sample uses the script executer class.
************************************************************************/

#include <lw.h>
#include <iostream>

//This is the exported C function that will be called from lua.
//You can retreive parameters by using lua_tonumber/lua_tostring/lua_tointeger(L, <index>)
//but remember to check if things are what you expect first! (lua_isnumber/lua_isstring/etc(L, <index>))
int myFunction(lua_State* L)
{
	std::cout << "I am a C function!" << std::endl;
	return 0;
}

int main(int argc, char** argv)
{
	//First, we need to create a state to keep our script. 
	//It is recommended to have only one state accross the program.
	lw::cState state;

	//Lets open our state:
	state.open();

	//Register the function
	state.registerFunction("myCFunction", myFunction);

	//Create an executer so we can load and run a script
	lw::cExecuter executer(state);

	//Run the script
	//Note that running the script is needed to put the byte code on the stack
	executer.loadAndRunScript("script.lua");

	//We're done, destroy the state:
	state.destroy();

	return 0;
}
