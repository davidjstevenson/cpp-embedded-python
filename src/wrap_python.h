#pragma once

#ifdef _DEBUG
/*
    if _DEBUG is defined when Python.h is included, it will attempt to link
    to the debug version of the python library (e.g. python38_d.lib)
*/
#undef _DEBUG
#define _DEBUG_WAS_DEFINED
#endif

#include <Python.h>

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif
