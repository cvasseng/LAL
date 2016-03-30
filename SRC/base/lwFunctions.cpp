#include "../../INCLUDE/lwFunctions.h"

using namespace lw;

bool lw::loadAndRun(lw::cState& state, const std::string& filename)
{
	if (!state.open())
	{
		return false;
	}
	
	std::string debug = filename;
	int s = luaL_loadfile(state.State(), debug.c_str());
	if (s == 0)
	{
		if (state.reportErrors(lua_pcall(state.State(), 0, -1, 0)))
			return false;
	}
	if (state.reportErrors(s))
		return false;
	return true;
}
