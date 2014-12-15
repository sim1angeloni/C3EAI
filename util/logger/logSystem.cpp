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
#include <iostream>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include "logSystem.h"
#include "logCommon.h"
#include "programinfo.h"

/* CONFIGURATION FILE */
static const QString SETTING_FILENAME = QString("%1_settings.ini").arg(PROG_NAME_SHORT);

static const QString LOG_SPLIT_KEY("|");

#include <QtCore/QVariant>

CLogSystem::CLogSystem()
{
    m_logTypesMap.insert(eLogAlways, LOGTYPE_ALWAYS);
    m_logTypesMap.insert(eLogInfo, LOGTYPE_INFO);
    m_logTypesMap.insert(eLogWarning, LOGTYPE_WARNING);
    m_logTypesMap.insert(eLogError, LOGTYPE_ERROR);
    m_logTypesMap.insert(eLogFatalError, LOGTYPE_FATAL_ERROR);
    m_logTypesMap.insert(eLogFunction, LOGTYPE_FUNCTION);
    m_logTypesMap.insert(eLogQt, LOGTYPE_QT);

    m_logOutputMap.insert(eLogConsole, LOGOUTPUT_CONSOLE);
    m_logOutputMap.insert(eLogFile, LOGOUTPUT_FILE);
    m_logOutputMap.insert(eLogWindow, LOGOUTPUT_WINDOW);

	/* ON INIT IT SHOWS ALL THE LOG TYPES*/
	m_types |= eLogAlways;
	m_types |= eLogInfo;
	m_types |= eLogWarning;
	m_types |= eLogError;
	m_types |= eLogFatalError;
	m_types |= eLogFunction;
	m_types |= eLogQt;

	/* ON INIT IT PRINTS LOGS ONLY IN CONSOLE
	 * (window and file can't exist
	 */
	m_outputs |= eLogConsole;

	m_pFile = NULL;
}

CLogSystem::~CLogSystem()
{

}

void CLogSystem::setLogTypes(QString types)
{
	if (types.isEmpty())
	{
		LOGError("Input argument was empty. Need at least 1 value.");
		return;
	}

	QStringList typeList = types.split(LOG_SPLIT_KEY, QString::SkipEmptyParts, Qt::CaseInsensitive);

	if (typeList.isEmpty())
	{
		LOGError("Input argument was wrong. Need at least 1 value.");
		return;
	}

	LogTypes enumType;
	for (int i = 0; i < typeList.size(); ++i)
	{
		const QString value = typeList.at(i).simplified();

		ELogType currentType = m_logTypesMap.key(value);
		switch (currentType)
		{
			case eLogAlways:
			case eLogInfo:
			case eLogWarning:
			case eLogError:
			case eLogFatalError:
			case eLogFunction:
			case eLogQt:
                {
                    enumType |= currentType;
                    break;
                }

			default:
                {
                    LOGFatalErrorANDExit(QString("Unexpected value '%1' for '%2' in '%3'").arg(value).arg(SETTING_LOGTYPES).arg(SETTING_FILENAME))
                }
		}
	}

	setLogTypes(enumType);
}

void CLogSystem::setLogOutputs(QString output)
{
	if (output.isEmpty())
	{
		LOGError("Input argument was empty. Need at least 1 value.");
		return;
	}

	QStringList outputList = output.split(LOG_SPLIT_KEY, QString::SkipEmptyParts, Qt::CaseInsensitive);

	if (outputList.isEmpty())
	{
		LOGError("Input argument was wrong. Need at least 1 value.");
		return;
	}

	LogOutputs enumType;
	for (int i = 0; i < outputList.size(); ++i)
	{
		const QString value = outputList.at(i).simplified();

		ELogOutput currentOutput = m_logOutputMap.key(value);
		switch (currentOutput)
		{
			case eLogConsole:
			case eLogFile:
			case eLogWindow:
                {
                    enumType |= currentOutput;
                    break;
                }

			default:
                {
                    LOGFatalErrorANDExit(QString("Unexpected value '%1' for '%2' in '%3'").arg(value).arg(SETTING_LOGOUTPUT).arg(SETTING_FILENAME))
                }
		}
	}

	setLogOutputs(enumType);
}

void CLogSystem::setLogTypes(LogTypes types)
{
	LOGInfo(QString("Changing log types. New value will be: '0x%1'").arg(QByteArray().setNum(types, 2).constData()));
	m_types = types;
}

void CLogSystem::setLogOutputs(LogOutputs output)
{
	LOGInfo(QString("Changing log outputs. New value will be: '0x%1'").arg(QByteArray().setNum(output, 2).constData()));
	m_outputs = output;
}

void CLogSystem::LogAlways(QString function, QString message)
{
	Log(eLogAlways, function, message);
}

void CLogSystem::LogInfo(QString function, QString message)
{
	Log(eLogInfo, function, message);
}

void CLogSystem::LogWarning(QString function, QString message)
{
	Log(eLogWarning, function, message);
}

void CLogSystem::LogError(QString function, QString message)
{
	Log(eLogError, function, message);
}

void CLogSystem::LogFatalError(QString function, QString message)
{
	Log(eLogFatalError, function, message);
}

void CLogSystem::LogFunction(QString function, QString message)
{
	Log(eLogFunction, function, message);
}

void CLogSystem::LogQt(QString message)
{
	Log(eLogQt, "QT Internal", message);
}

void CLogSystem::Log(ELogType type, QString function, QString message)
{
	if (!m_types.testFlag(type))
		return;

	QDateTime current_date = QDateTime::currentDateTime();

	if (m_outputs.testFlag(eLogConsole))
		std::cout<<"LOG("<<current_date.toTime_t()<<") ==> type:'"<<(int)type<<"' - function:'"<<function.toAscii().constData()<<"' - message:'"<<message.toAscii().constData()<<"'"<<std::endl;

	if (m_outputs.testFlag(eLogFile))
		LogOnFile(current_date, type, function, message);

	//if (m_outputs.testFlag(eLogWindow))
        //TODO add graphical console
}

//FIXME \r\n makes ugly log file on Linux
void CLogSystem::LogOnFile(const QDateTime date, const ELogType type, const QString function, const QString message)
{
	if (!m_pFile)
	{
		QString fileName("%1/%2.log");
		fileName = fileName.arg(QCoreApplication::applicationDirPath());
		fileName = fileName.arg(PROG_NAME_SHORT);

		m_pFile = new QFile(fileName);

		if (m_pFile->exists())
		{
			if (!m_pFile->remove())
			{
                //TODO add error
			}
		}

		if (!m_pFile->open(QIODevice::ReadWrite))
		{
            //TODO add error
		}

		QByteArray fileStart = QString(
				"#######################################################\r\n"
				"%1 %2\r\n"
				"#######################################################\r\n")
				.arg(PROG_NAME)
				.arg(PROG_VERSION)
				.toAscii();

		if (m_pFile->write(fileStart) != fileStart.size())
		{
            //TODO add error
		}

		m_pFile->flush();
	}

	if (m_pFile)
	{
		QString text;
		if (gEnv->bDebug)
		{
			text = 	"Date: %1\tStatus: %2\r\n"
					"Function: %3\r\n"
					"Message: %4\r\n"
					"-------------------------------------------------------\r\n";
		}
		else
		{
			text = 	"Date: %1\tStatus: %2\r\n"
					"Message: %3\r\n"
					"-------------------------------------------------------\r\n";
		}

		text = text.arg(date.toString("ddd MMM dd hh:mm:ss.zzz yyyy"));
		text = text.arg((int)type);
		if (gEnv->bDebug)
		{
			text = text.arg(function.simplified());
		}
		text = text.arg(message);

		QByteArray textContent(text.toAscii());

		if (m_pFile->write(textContent) != textContent.size())
		{
            //TODO add error
		}

		m_pFile->flush();
	}
}
