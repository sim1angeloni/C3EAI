/****************************************************************************************
** Copyright (C) 2011, 2015 Simone Angeloni
** This file is part of C3 External Admin Interface.
**
** C3 External Admin Interface is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** C3 External Admin Interface is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with C3 External Admin Interface. If not, see <http://www.gnu.org/licenses/>
**
****************************************************************************************/

#ifndef SYSTEMGLOBALENVIRONMENT_H
#define SYSTEMGLOBALENVIRONMENT_H
#pragma once

#include "interface/splash.h"

class CLogSystem;
class CSettingsFile;
class CIPUtils;

struct SUtils
{
    CIPUtils * pIPStringUtils;
    SUtils():pIPStringUtils(NULL) {}
};

struct SSystemGlobalEnvironment
{
    SUtils * pUtils;
    CLogSystem * pLog;
    CSettingsFile * pSettings;
    CSplashScreenDialog * pSplashScreen;

    bool bDebug;    // Used to tell if the software is running in debug mode

	SSystemGlobalEnvironment():
		pLog(NULL),
		pSettings(NULL),
		pSplashScreen(NULL)
	{
		pUtils = new SUtils();

		#ifdef DEBUG_MODE
			bDebug = true;
		#else
			bDebug = false;
		#endif
	}
};

extern SSystemGlobalEnvironment *gEnv;

#endif /* SYSTEMGLOBALENVIRONMENT_H */
