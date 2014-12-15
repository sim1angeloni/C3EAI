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

#ifndef STDAFX_H
#define STDAFX_H
#pragma once

#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#endif

/* GLOBAL ENVIRONMENT */
#include "util/gEnv/gEnv.h"

/* TRASLATIONS FILE */
#include "interface/traslations.h"

/* LOG UTILS */
#include "util/logger/logFunction.h"
#include "util/logger/logSystem.h"

#define LOGAlways(message) LogAlways(Q_FUNC_INFO, QString("%1").arg(message));
inline void LogAlways(QString function, QString message)
{
    if (gEnv && gEnv->pLog)
    {
        gEnv->pLog->LogAlways(function, message);
    }
}

#define LOGInfo(message) LogInfo(Q_FUNC_INFO, QString("%1").arg(message));
inline void LogInfo(QString function, QString message)
{
    if (gEnv && gEnv->pLog)
    {
        gEnv->pLog->LogInfo(function, message);
    }
}

#define LOGWarning(message) LogWarning(Q_FUNC_INFO, QString("%1").arg(message));
inline void LogWarning(QString function, QString message)
{
    if (gEnv && gEnv->pLog)
    {
        gEnv->pLog->LogWarning(function, message);
    }
}

#define LOGError(message) LogError(Q_FUNC_INFO, QString("%1").arg(message));
inline void LogError(QString function, QString message)
{
    if (gEnv && gEnv->pLog)
    {
        gEnv->pLog->LogError(function, message);
    }
}

#define LOGFatalErrorANDExit(message) LogFatalErrorANDExit(Q_FUNC_INFO, QString("%1").arg(message));
inline void LogFatalErrorANDExit(QString function, QString message)
{
    if (gEnv && gEnv->pLog)
    {
        gEnv->pLog->LogFatalError(function, message);
    }

    LogInfo(function, "APPLICATION SHUTTING DOWN...");

    QCoreApplication::exit(-1);
}

#ifdef DEBUG_MODE
#define FUNCTION_LOG { if (gEnv->bDebug) CLogFunction functionLogger(Q_FUNC_INFO); }
#else
#define FUNCTION_LOG
#endif
inline void LogFunction(QString function, bool start)
{
    if (gEnv && gEnv->pLog && start)
    {
        gEnv->pLog->LogFunction(function, "START");
    }
    else if (gEnv && gEnv->pLog && !start)
    {
        gEnv->pLog->LogFunction(function, "END");
    }
}

#define LOGQt(message) LogQt(message);
inline void LogQt(QString message)
{
    if (gEnv && gEnv->pLog)
    {
        gEnv->pLog->LogQt(message);
    }
}

inline bool isFieldEmpty(QString text, QString field_description, QString field_description_traslation, QWidget * parent)
{
    if (!text.isEmpty())
        return false;

    LOGError(QString("Field '%1' is empty").arg(field_description));
    QMessageBox::critical(parent, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_EMPTYFIELD.arg(field_description_traslation));
    return true;
}

inline bool isFieldText(QString text, QString field_description, QString field_description_traslation, QWidget * parent)
{
    if (text.contains(QRegExp("^[A-Za-z0-9_]+$")))
        return true;

    LOGError(QString("Field '%1' is empty").arg(field_description));
    QMessageBox::critical(parent, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_AZ09FIELD.arg(field_description_traslation));
    return false;
}

inline bool isFieldPortOverflow(quint32 value, QString field_description, QString field_description_traslation, QWidget * parent)
{
    if (value <= 0xffff)
        return false;

    LOGError(QString("Overflow field '%1'").arg(field_description));
    QMessageBox::critical(parent, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_PORTOVERFLOW.arg(field_description_traslation));
    return true;
}

inline bool isFieldPortZero(quint32 value, QString field_description, QString field_description_traslation, QWidget * parent)
{
    if (value > 0)
        return false;

    LOGError(QString("Overflow field '%1'").arg(field_description));
    QMessageBox::critical(parent, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_PORTOVERFLOW.arg(field_description_traslation));
    return true;
}

#endif /* STDAFX_H */
