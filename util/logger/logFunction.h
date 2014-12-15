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

#ifndef LOGFUNCTION_H
#define LOGFUNCTION_H

#include <QtCore/QString>

class CLogFunction
{
	public:
        CLogFunction(QString function);
        virtual ~CLogFunction();

    private:
        Q_DISABLE_COPY(CLogFunction)

	private:
        QString m_functionName;
};

#endif /* LOGFUNCTION_H */
