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

#ifndef IPUTILS_H
#define IPUTILS_H

#include <QtCore/QString>
#include <QtCore/QList>

class CIPUtils
{
	public:
        CIPUtils() {}
        virtual ~CIPUtils() {}

		QList<QString> split(const QString & ip_address, bool * ok = NULL) const;
		QString join(int part_a, int part_b, int part_c, int part_d, bool * ok = NULL) const;
		QString join(const QString & part_a, const QString & part_b, const QString & part_c, const QString & part_d, bool * ok = NULL) const;

		bool isValid(const QString & ip_address) const;
		bool isSectionValid(const QString & ip_section, bool skip_empty) const;

        int getFieldMinValue() const { return 0x0; }
        int getFieldMaxValue() const { return 0xff; }

		QList<int> toInt(const QString & ip_address, bool * ok = NULL) const;

    private:
        Q_DISABLE_COPY(CIPUtils)
};

#endif /* IPUTILS_H */
