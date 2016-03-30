#ifndef h__lw_functions__
#define h__lw_functions__

#include "lwState.h"

namespace lw
{
	///Loads and runs a script
	bool loadAndRun(lw::cState& state, const std::string& filename);
}

#endif
