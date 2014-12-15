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
#include "advancedLineEdit.h"
#include <QtGui/QKeyEvent>

static const QRgb ERROR_COLOR(0xb10e16);

CAdvancedLineEditWidget::CAdvancedLineEditWidget(QWidget* parent):
	QLineEdit(parent),
	m_minLength(0),
	m_allowedCharacters(eAC_Any),
	m_lockDisallowedChars(false),
	m_highlightErrors(true),
    m_errorColor(ERROR_COLOR),
	m_valid(true),
	m_currentLength(0)
{
}

CAdvancedLineEditWidget::CAdvancedLineEditWidget(const QString & text, QWidget* parent):
	QLineEdit(text, parent),
	m_minLength(0),
	m_allowedCharacters(eAC_Any),
	m_lockDisallowedChars(false),
	m_highlightErrors(true),
    m_errorColor(ERROR_COLOR),
	m_valid(true),
	m_currentLength(text.size())
{
}

CAdvancedLineEditWidget::~CAdvancedLineEditWidget()
{
}

void CAdvancedLineEditWidget::setMinLength(int _minLength)
{
    m_minLength = qMax(_minLength, 0);
	update();
}

void CAdvancedLineEditWidget::setMaxLength(int _maxLength)
{
    QLineEdit::setMaxLength(qMax(_maxLength, 0));
	update();
}

void CAdvancedLineEditWidget::setAllowedCharacters(AllowedCharacters _allowedCharacters)
{
	m_allowedCharacters = _allowedCharacters;
	update();
}

void CAdvancedLineEditWidget::setLockDisallowedCharacters(bool _lockDisallowedChars)
{
	m_lockDisallowedChars = _lockDisallowedChars;
	update();
}

void CAdvancedLineEditWidget::setErrorColor(QRgb _errorColor)
{
	m_errorColor = _errorColor;
	update();
}

void CAdvancedLineEditWidget::setErrorHighlight(bool _highlightErrors)
{
	m_highlightErrors = _highlightErrors;
	update();
}

void CAdvancedLineEditWidget::setText(const QString & _text)
{
	QLineEdit::setText(_text);
	update();
}

void CAdvancedLineEditWidget::keyPressEvent(QKeyEvent * pEvent)
{
    if (!pEvent->text().isEmpty() &&
        pEvent->key() != Qt::Key_Backspace &&
        pEvent->key() != Qt::Key_Delete)
	{
        if (!testCharacter(pEvent->text().at(0)) && m_lockDisallowedChars)
			return;
	}

    QLineEdit::keyPressEvent(pEvent);

	testBorders();

	update();
}

void CAdvancedLineEditWidget::update()
{
	const QString _text = text();
	m_currentLength = _text.size();

	bool oldValid = m_valid;
	m_valid = true;

	if (m_currentLength < m_minLength)
	{
		m_valid = false;
	}

	if (m_valid && !_text.isEmpty())
	{
        for (int i = 0; i < m_currentLength; ++i)
		{
            if (!testCharacter(_text[i]))
			{
				m_valid = false;
				break;
			}
		}
	}

	refreshColor();

	if (oldValid != m_valid)
		Q_EMIT validityChanged(m_valid);
}

void CAdvancedLineEditWidget::refreshColor()
{
	refreshColor(m_valid);
}

void CAdvancedLineEditWidget::refreshColor(bool _valid)
{
	if (!_valid && m_highlightErrors)
	{
		QPalette current_palette = palette();
		current_palette.setColor(QPalette::Base, QColor(m_errorColor));
		setPalette(current_palette);

		return;
	}

	if (parentWidget())
	{
		setPalette(parentWidget()->palette());
	}
}

void CAdvancedLineEditWidget::testBorders()
{
	const int _true_size = text().size();

	if (_true_size == m_currentLength)
		return;

	if (_true_size <= m_minLength && m_currentLength > m_minLength)
		Q_EMIT minLengthReached();

	if (_true_size >= maxLength() && m_currentLength < maxLength())
		Q_EMIT maxLengthReached();
}

bool CAdvancedLineEditWidget::testCharacter(const QChar & character) const
{
	if (m_allowedCharacters.testFlag(eAC_Any))
	{
		return true;
	}

	if (m_allowedCharacters.testFlag(eAC_ASCIIalphabetic))
	{
		if (character >= QChar('a') && character <= QChar('z'))
			return true;
		else
		if (character >= QChar('A') && character <= QChar('Z'))
			return true;
	}

	if (m_allowedCharacters.testFlag(eAC_ASCIIdigit))
	{
		if (character >= QChar('0') && character <= QChar('9'))
			return true;
	}

	if (m_allowedCharacters.testFlag(eAC_Hex))
	{
		if (character >= QChar('a') && character <= QChar('f'))
			return true;
		else
		if (character >= QChar('A') && character <= QChar('F'))
			return true;
		else
		if (character >= QChar('0') && character <= QChar('9'))
			return true;
	}

	if (m_allowedCharacters.testFlag(eAC_Binary))
	{
		if (character >= QChar('0') && character <= QChar('1'))
			return true;
	}

	if (m_allowedCharacters.testFlag(eAC_Space))
	{
		if (character == QChar(' '))
			return true;
	}

	if (m_allowedCharacters.testFlag(eAC_Underscore))
	{
		if (character == QChar('_'))
			return true;
	}

	if (m_allowedCharacters.testFlag(eAC_Minus))
	{
		if (character == QChar('-'))
			return true;
	}

	return false;
}
