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

#ifndef LOGCOMMON_H
#define LOGCOMMON_H
#pragma once

/* LOG TYPES */
static const QString LOGTYPE_ALWAYS("ALWAYS");
static const QString LOGTYPE_INFO("INFO");
static const QString LOGTYPE_WARNING("WARNING");
static const QString LOGTYPE_ERROR("ERROR");
static const QString LOGTYPE_FATAL_ERROR("FATAL_ERROR");
static const QString LOGTYPE_FUNCTION("FUNCTION");
static const QString LOGTYPE_QT("QT");

/* LOG OUTPUT */
static const QString LOGOUTPUT_FILE("FILE");
static const QString LOGOUTPUT_CONSOLE("CONSOLE");
static const QString LOGOUTPUT_WINDOW("WINDOW");

/* SETTINGS KEYS */
static const QString SETTING_LOGTYPES("LOG_TYPES");
static const QString SETTING_LOGOUTPUT("LOG_OUTPUT");

#endif /* LOGCOMMON_H */
