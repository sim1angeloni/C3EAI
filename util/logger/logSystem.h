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

#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H
#pragma once

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QFlags>
#include <QtCore/QMap>

class QFile;

class CLogSystem
{
	public:
		/**
		 * Log types list.
		 * Used to specify the logs types to show in output (initial setup)
		 */
		enum ELogType
		{
            eLogAlways		= 0x0001,	/* Logs to show always. */
            eLogInfo		= 0x0002,	/* Informations logs. */
            eLogWarning		= 0x0004,	/* Low gravity errors which do not stop the application execution. */
            eLogError		= 0x0008,	/* High gravity errors which do not stop the application execution. */
            eLogFatalError	= 0x0010,	/* Very high gravity errors which stop the application execution. */
            eLogFunction	= 0x0020,	/* Log for function execution. */
            eLogQt			= 0x0040	/* Log from Qt (should happen only if something is wrong). */
		};
        Q_DECLARE_FLAGS(LogTypes, ELogType)

		/**
		 * Log output.
		 * Used to specify the output for the logs (initial setup)
		 */
		enum ELogOutput
		{
            eLogConsole		= 0x0001,	/* Enable console log. */
            eLogFile		= 0x0002,	/* Enable file log. */
            eLogWindow		= 0x0004,	/* Enable file log. */
		};
        Q_DECLARE_FLAGS(LogOutputs, ELogOutput)

		/**
		* Type for the links between log types and their texts.
		*/
		typedef QMap<ELogType, QString> LogTypesText;

		/**
		* Type for the links between log output and their texts.
		*/
		typedef QMap<ELogOutput, QString> LogOutputText;

    public:
        CLogSystem();
        virtual ~CLogSystem();

		/**
		* Allows the user to set the types to show in outputs.
		* @param types List of types as string separated using OR bitwise.
		* @see ELogType
		*/
		void setLogTypes(QString types);

		/**
		* Allows the user to set the outputs for all the logs.
		* @param output List of outputs in OR.
		* @see ELogOutput
		*/
		void setLogOutputs(QString output);

		/**
		* It should be used for very important logs.
		* @param function Function name.
		* @param message Message to print.
		*/
		void LogAlways(QString function, QString message);

		/**
		* Used for information logs, low importance.
		* @param function Function name.
		* @param message Message to print.
		*/
		void LogInfo(QString function, QString message);

		/**
		* Used to log warnings, little errors which will not stop the execution.
		* @param function Function name.
		* @param message Message to print.
		*/
		void LogWarning(QString function, QString message);

		/**
		* Used for errors which stopped the normal function execution but will not stop the program execution.
		* @param function Function name.
		* @param message Message to print.
		*/
		void LogError(QString function, QString message);

		/**
		* Should be used for very important and critical logs and unexpected results.
		* After this function an exit() should be called.
		* @param function Function name.
		* @param message Message to print.
		*/
		void LogFatalError(QString function, QString message);

		/**
		* Should be used only from Qt for Qt Internal Errors.
		* It is considered as a LogError. It is critical but it will not stop the application execution unless an exception has been called from Qt.
		* @param message Message to print.
		* @see LogError
		*/
		void LogQt(QString message);

		/**
		* Used to log start and stop for the input function.
		* @param function Function name.
		* @param message Message to print.
		*/
		void LogFunction(QString function, QString message);

    private:
        Q_DISABLE_COPY(CLogSystem)

    private:
        void Log(ELogType type, QString function, QString message);
        void setLogTypes(LogTypes types);
        void setLogOutputs(LogOutputs output);
        void LogOnFile(const QDateTime date, const ELogType type, const QString function, const QString message);

	private:
        LogTypesText m_logTypesMap;     /* Links between enumeration values and text descriptors for eLogTypes */
        LogOutputText m_logOutputMap;	/* Links between enumeration values and text descriptors for eLogOutput */
        LogTypes m_types;               /* List of types to show */
        LogOutputs m_outputs;           /* List of outputs defined to show the logs */

        QFile * m_pFile;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(CLogSystem::LogTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(CLogSystem::LogOutputs)

#endif /* LOGSYSTEM_H */
