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

#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QtCore/QString>

enum EGameMode
{
    eGameMode_Crash		 	= 0x01,
    eGameMode_CTR 			= 0x02,
    eGameMode_TDM 			= 0x04,
    eGameMode_DM 			= 0x08,
    eGameMode_Ext		 	= 0x10,
    eGameMode_Assault 		= 0x20,
    eGameMode_Hunter 		= 0x40,
    eGameMode_Spears		= 0x40,
};
Q_DECLARE_FLAGS(GameModes, EGameMode)

struct SGameMode
{
    QString 	m_human_readable;
    QString		m_name;
    QString		m_description;
    EGameMode	m_type;

    SGameMode(QString _human_readable, QString _name, QString _description, EGameMode _type)
    {
        m_human_readable = _human_readable;
        m_name = _name;
        m_type = _type;
        m_description = _description;
    }
};
Q_DECLARE_OPERATORS_FOR_FLAGS(GameModes)

#endif // GAMEMODE_H
