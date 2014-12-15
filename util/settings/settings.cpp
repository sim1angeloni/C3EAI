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

#include "stdafx.h"
#include "settings.h"
#include "util/logger/logCommon.h"

#define _T(def) #def

/* SETTINGS DEFAULT VALUES */
static const QString SETTING_DEFVAL_LOGTYPES = QString("%1|%2|%3|%4|%5|%6|%7").arg(LOGTYPE_ALWAYS).arg(LOGTYPE_INFO).arg(LOGTYPE_WARNING).arg(LOGTYPE_ERROR).arg(LOGTYPE_FATAL_ERROR).arg(LOGTYPE_FUNCTION).arg(LOGTYPE_QT);
static const QString SETTING_DEFVAL_LOGOUTPUT = QString("%1|%2|%3").arg(LOGOUTPUT_FILE).arg(LOGOUTPUT_CONSOLE).arg(LOGOUTPUT_WINDOW);

static const QString NULL_OBJECT("%NULL_OBJECT%");

CSettingsFile::CSettingsFile():
	m_settings(NULL)
{
	FUNCTION_LOG

	m_settings = new QSettings();

	if (!m_settings->isWritable())
		LOGFatalErrorANDExit("Not allowed to use settings keys");

	tryCreateFile();
}

CSettingsFile::~CSettingsFile()
{
	FUNCTION_LOG

	m_settings->sync();
	SAFE_DELETE(m_settings);
}

void CSettingsFile::tryCreateFile()
{
	FUNCTION_LOG

    addValue(SETTING_LOGTYPES, SETTING_DEFVAL_LOGTYPES);
    addValue(SETTING_LOGOUTPUT, SETTING_DEFVAL_LOGOUTPUT);
}

QString CSettingsFile::getValueString(const QString key)
{
	FUNCTION_LOG

	if (key.isEmpty())
	{
		LOGError("Input argument was empty");
		return QString();
	}

    QVariant value = m_settings->value(key.simplified(), NULL_OBJECT);
	if (value.toString().compare(NULL_OBJECT) == 0)
	{
		LOGError(QString("There is no value for '%1'").arg(key.simplified()));
		return QString();
	}

	if (!value.canConvert(QVariant::String))
	{
		LOGFatalErrorANDExit(QString("Unable to convert value of '%1' in a string").arg(key));
	}

	return value.toString();
}

int CSettingsFile::getValueInteger(const QString key)
{
	FUNCTION_LOG

	if (key.isEmpty())
	{
		LOGError("Input argument was empty");
		return int();
	}

    QVariant value = m_settings->value(key.simplified(), NULL_OBJECT);
	if (value.toString().compare(NULL_OBJECT) == 0)
	{
		LOGError(QString("There is no value for '%1'").arg(key.simplified()));
		return int();
	}

	if (!value.canConvert(QVariant::Int))
		LOGFatalErrorANDExit(QString("Unable to convert value of '%1' in an integer").arg(key));

	return value.toInt();
}

void CSettingsFile::setValue(QString & key, QVariant & value)
{
	FUNCTION_LOG

	if (!value.canConvert(QVariant::String))
		LOGError(QString("Trying to add an incompatible value. Accepts only '%1'").arg(_T(QVariant::String)));

    addValue(key, value.toString());
}

void CSettingsFile::addValue(const QString & key, const QString & value)
{
    if (!m_settings->contains(key))
    {
        LOGWarning(QString("'%1' missing in config file, now setted to '%2'").arg(key).arg(value))
        m_settings->setValue(key,value);
    }
}
