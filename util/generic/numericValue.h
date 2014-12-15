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

#ifndef NUMERICVALUE_H
#define NUMERICVALUE_H

#include <QtCore/QString>
#include <QtCore/QObject>

class CNumericValue : public QObject
{
	Q_OBJECT

	public:
        CNumericValue(QObject *parent=0);
        CNumericValue(int _value, QObject *parent=0);
        CNumericValue(const QString & _text, QObject *parent=0);
        CNumericValue(const CNumericValue & other, QObject *parent=0);
        virtual ~CNumericValue();

		QString text() const;
		int value() const;
		void setValue( int );
		void setValue( const QString & );

        int maximum () const { return m_maximum; }
		bool setMaximum ( int );

        int minimum () const { return m_minimum; }
		bool setMinimum ( int );

        CNumericValue &operator=(const CNumericValue & other);

        inline CNumericValue & operator++();
        inline CNumericValue operator++(int);
        inline CNumericValue & operator--();
        inline CNumericValue operator--(int);
        inline CNumericValue & operator+=(int j);
        inline CNumericValue & operator-=(int j);
        inline CNumericValue operator+(int j) const;
        inline CNumericValue operator-(int j) const;

	Q_SIGNALS:
		void valueChanged(int);
		void valueChanged(const QString &);

    private:
        bool checkChanged();

    protected:
        int m_value;
        int m_previous_value;
        bool m_undefined;
        int m_minimum;
        int m_maximum;
};

/* ============================================ */

inline CNumericValue & CNumericValue::operator++()
{
    if (m_undefined)
    {
        m_value = m_minimum; m_undefined = false;
        checkChanged();
    }
    else
    if (m_value < m_maximum)
    {
        ++m_value; m_undefined = false;
        checkChanged();
    }
    return *this;
}

inline CNumericValue CNumericValue::operator++(int)
{
    if (m_undefined)
    {
        m_value = m_minimum; m_undefined = false;
        checkChanged();
    }
    else
    if (m_value < m_maximum)
    {
        ++m_value; m_undefined = false;
        checkChanged();
    }
    return CNumericValue(*this);
}

inline CNumericValue & CNumericValue::operator--()
{
    if (m_undefined)
    {
        m_value = m_minimum; m_undefined = false;
        checkChanged();
    }
    else
    if (m_value > m_minimum)
    {
        --m_value; m_undefined = false;
        checkChanged();
    }
    return *this;
}

inline CNumericValue CNumericValue::operator--(int)
{
    if (m_undefined)
    {
        m_value = m_minimum; m_undefined = false;
        checkChanged();
    }
    else
    if (m_value > m_minimum)
    {
        --m_value; m_undefined = false;
        checkChanged();
    }
    return CNumericValue(*this);
}

inline CNumericValue & CNumericValue::operator+=(int j)
{
    int result = m_value + j;
    if (m_undefined)
    {
        m_value = m_minimum; m_undefined = false;
        checkChanged();
    }
    else
    if (result <= m_maximum)
    {
        m_value = result; m_undefined = false;
        checkChanged();
    }
    return *this;
}

inline CNumericValue & CNumericValue::operator-=(int j)
{
    int result = m_value - j;
    if (m_undefined)
    {
        m_value = m_minimum; m_undefined = false;
        checkChanged();
    }
    else
    if (result >= m_minimum)
    {
        m_value = result; m_undefined = false;
        checkChanged();
    }
    return *this;
}

inline CNumericValue CNumericValue::operator+(int j) const
{
    CNumericValue result(*this);
    result += j;
    return result;
}

inline CNumericValue CNumericValue::operator-(int j) const
{
    CNumericValue result(*this);
    result -= j;
    return result;
}

#endif /* NUMERICVALUE_H */
