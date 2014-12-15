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
#include "ipUtils.h"
#include <QtNetwork/QHostAddress>

static const QString IP_SEPARATOR(".");
static const int IP_FIELDS = 4;

QList<QString> CIPUtils::split(const QString & ip_address, bool * ok) const
{
	if (ok) *ok = false;

	QHostAddress addr;
	if (!addr.setAddress(ip_address.simplified()))
	{
		LOGError(QString("Not a valid ip address ('%1')").arg(ip_address));
		return QList<QString>();
	}

    QList<QString> ret = addr.toString().split(IP_SEPARATOR, QString::SkipEmptyParts, Qt::CaseSensitive);
	if (ret.size() != IP_FIELDS)
	{
		LOGError(QString("Not a valid ip address ('%1')").arg(ip_address));
		return QList<QString>();
	}

	if (ok) *ok = true;
	return ret;
}

QList<int> CIPUtils::toInt(const QString & ip_address, bool * ok) const
{
	if (ok) *ok = false;

	QList<QString> address_list = split(ip_address, ok);
	if (!ok)
		return QList<int>();

	QList<int> ret;
	for (int i = 0; i < address_list.size(); ++i)
	{
		bool isNum = false;
		ret << address_list[i].toInt(&isNum);
		if (!isNum)
			return QList<int>();
	}

	if (ret.size() != IP_FIELDS)
	{
		LOGError(QString("Not a valid ip address ('%1')").arg(ip_address));
		return QList<int>();
	}

	return ret;
}

QString CIPUtils::join(int part_a, int part_b, int part_c, int part_d, bool * ok) const
{
	quint32 ip_address = part_a << 24 | part_b << 16 | part_c << 8 | part_d;
	QHostAddress addr(ip_address);

	if (ok) *ok = true;
	return addr.toString();
}

QString CIPUtils::join(const QString & part_a, const QString & part_b, const QString & part_c, const QString & part_d, bool * ok) const
{
	if (ok) *ok = false;

	const QString ip_address = QString("%1-%2-%3-%4").arg(part_a).arg(part_b).arg(part_c).arg(part_d).replace("-", IP_SEPARATOR);

	QHostAddress addr;
	if (!addr.setAddress(ip_address.simplified()))
	{
		LOGError(QString("Not a valid address ('%1','%2','%3','%4')").arg(part_a).arg(part_b).arg(part_c).arg(part_d));
		return QString();
	}

	if (ok) *ok = true;
	return addr.toString();
}

bool CIPUtils::isValid(const QString & ip_address) const
{
	QHostAddress addr;
	if (!addr.setAddress(ip_address.simplified()))
	{
		LOGError(QString("Not a valid ip address ('%1')").arg(ip_address));
		return false;
	}

	return true;
}

bool CIPUtils::isSectionValid(const QString & ip_section, bool skip_empty) const
{
	if (ip_section.isEmpty() && skip_empty)
		return true;

	bool ok = false;
	int value = ip_section.toInt(&ok);
	if (!ok)
	{
        LOGError(QString("'%1' should be a number between %2 and %3").arg(ip_section).arg(getFieldMinValue()).arg(getFieldMaxValue()));
		return false;
	}

    if (value < getFieldMinValue() || value > getFieldMaxValue())
	{
        LOGError(QString("'%1' should be a number between %2 and %3").arg(ip_section).arg(getFieldMinValue()).arg(getFieldMaxValue()));
		return false;
	}

	return true;
}
