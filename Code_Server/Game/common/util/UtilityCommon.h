#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x0500 

#include <windows.h>
#include <memory>
#include <assert.h>
#include <crtdbg.h>
#include <tchar.h>
#include "tstring.h"
#include "Event.h"
#include "CriticalSection.h"
#include "CommonDef.h"