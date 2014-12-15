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
#include "numericValue.h"

static const int MIN_DEFAULT = 0;
static const int MAX_DEFAULT = 99;

CNumericValue::CNumericValue(QObject *parent):
	QObject(parent),
	m_value(0),
	m_previous_value(0),
	m_undefined(true),
	m_minimum(MIN_DEFAULT),
	m_maximum(MAX_DEFAULT)
{
}

CNumericValue::CNumericValue(int _value, QObject *parent):
	QObject(parent),
	m_value(_value),
	m_previous_value(_value),
	m_undefined(false),
	m_minimum(MIN_DEFAULT),
	m_maximum(MAX_DEFAULT)
{
}

CNumericValue::CNumericValue(const QString & _text, QObject *parent):
	QObject(parent),
	m_minimum(MIN_DEFAULT),
	m_maximum(MAX_DEFAULT)
{
	bool ok = false;
	m_value = _text.toInt(&ok);
	m_previous_value = m_value;
	m_undefined = !ok;
}

CNumericValue::CNumericValue(const CNumericValue & other, QObject *parent):
	QObject(parent),
	m_value(other.m_value),
	m_previous_value(other.m_previous_value),
	m_undefined(other.m_undefined),
	m_minimum(other.m_minimum),
	m_maximum(other.m_maximum)
{
}

CNumericValue::~CNumericValue()
{
}

QString CNumericValue::text() const
{
	if (m_undefined) return QString("");
	return QString::number(m_value);
}

int CNumericValue::value() const
{
	if (m_undefined) return 0;
	return m_value;
}

void CNumericValue::setValue( int _value)
{
	if (_value <= m_maximum && _value >= m_minimum)
	{
		m_value = _value;
		m_undefined = false;
		checkChanged();
	}
}

void CNumericValue::setValue( const QString & _text)
{
	bool ok = false;
	int value = _text.toInt(&ok);

	if (ok)
	{
		setValue(value);
	}
	else
	{
		m_value = 0;
		m_previous_value = 0;
		m_undefined = true;
	}
}

CNumericValue & CNumericValue::operator=(const CNumericValue & other)
{
	m_value = other.m_value;
	m_undefined = other.m_undefined;
	m_previous_value = other.m_previous_value;
	return *this;
}

bool CNumericValue::checkChanged()
{
	if (m_previous_value != m_value)
	{
		m_previous_value = m_value;
		Q_EMIT valueChanged(m_value);
		Q_EMIT valueChanged(QString::number(m_value));

		return true;
	}

	return false;
}

bool CNumericValue::setMinimum(int _min)
{
	if (_min <= m_maximum)
	{
		m_minimum = _min;

		if (m_value < m_minimum)
		{
			m_value = m_minimum;
			m_undefined = false;
			return checkChanged();
		}
	}

	return false;
}

bool CNumericValue::setMaximum(int _max)
{
	if (_max >= m_minimum)
	{
		m_maximum = _max;

		if (m_value > m_maximum)
		{
			m_value = m_maximum;
			m_undefined = false;
			return checkChanged();
		}
	}

	return false;
}
