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
#include "numericLineEdit.h"
#include <QtGui/QWheelEvent>

CNumericLineEditWidget::CNumericLineEditWidget(QWidget* parent):
	CAdvancedLineEditWidget(parent),
	m_value(CNumericValue())
{
	init();
}

CNumericLineEditWidget::CNumericLineEditWidget(int value, QWidget* parent):
	CAdvancedLineEditWidget(QString::number(value), parent),
	m_value(CNumericValue(value))
{
	init();
}

CNumericLineEditWidget::~CNumericLineEditWidget()
{
}

void CNumericLineEditWidget::init()
{
	CAdvancedLineEditWidget::setAllowedCharacters(CAdvancedLineEditWidget::eAC_ASCIIdigit | CAdvancedLineEditWidget::eAC_Minus);
	CAdvancedLineEditWidget::setLockDisallowedCharacters(true);

	bool q_assert = false;

	q_assert = QObject::connect(&m_value, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(&m_value, SIGNAL(valueChanged(const QString &)), this, SIGNAL(valueChanged(const QString &)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	refreshTextFromValue();
}

void CNumericLineEditWidget::setText(const QString & _text)
{
	m_value = CNumericValue(_text);

	refreshTextFromValue();
}

void CNumericLineEditWidget::setValue(int _value)
{
	m_value = CNumericValue(_value);

	refreshTextFromValue();
}

void CNumericLineEditWidget::wheelEvent(QWheelEvent * pEvent)
{
    const int _delta = pEvent->delta();

	if (_delta == 0)
		return;

	if (_delta < 0)
		--m_value;
	else
	if (_delta > 0)
		++m_value;

	refreshTextFromValue();
}

void CNumericLineEditWidget::keyPressEvent(QKeyEvent * pEvent)
{
    if(pEvent->key() == Qt::Key_Minus || (pEvent->key() >= Qt::Key_0 && pEvent->key() <= Qt::Key_9))
	{
		const int cursor_pos = cursorPosition();

        if (pEvent->key() == Qt::Key_Minus && cursor_pos != 0)
		{
			return;
		}
		else
        if (pEvent->key() == Qt::Key_0)
		{
            const QChar * pData = text().data();
            if (!pData->isNull())
			{
                if (*pData == QChar('0')) return;
                else if (*pData == QChar('-') && cursor_pos == 1) return;
			}
		}

        int value = text().insert(cursor_pos, pEvent->text()).toInt();
		if (value < m_value.minimum() || value > m_value.maximum()) return;
	}

    CAdvancedLineEditWidget::keyPressEvent(pEvent);

	refreshValueFromText();
}

void CNumericLineEditWidget::refreshTextFromValue()
{
	CAdvancedLineEditWidget::setText(m_value.text());
}

void CNumericLineEditWidget::refreshValueFromText()
{
	m_value.setValue(text());
}

int CNumericLineEditWidget::maximum () const
{
	return m_value.maximum();
}

void CNumericLineEditWidget::setMaximum ( int _max)
{
	if (m_value.setMaximum(_max))
		refreshTextFromValue();
}

int CNumericLineEditWidget::minimum () const
{
	return m_value.minimum();
}

void CNumericLineEditWidget::setMinimum ( int _min)
{
	if (m_value.setMinimum(_min))
		refreshTextFromValue();
}

void CNumericLineEditWidget::setRange ( int _min, int _max)
{
	setMinimum(_min);
	setMaximum(_max);
}

int CNumericLineEditWidget::value () const
{
	return m_value.value();
}
