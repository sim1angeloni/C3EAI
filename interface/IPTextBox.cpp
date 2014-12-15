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
#include "IPTextBox.h"
#include <QtNetwork/QHostAddress>
#include <QtGui/QKeyEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>

static const int FIELD_SIZE = 3;

CIPTextBoxWidget::CIPTextBoxWidget(QWidget * parent, Qt::WindowFlags f):
	QWidget(parent, f)
{
	const int min_value = gEnv->pUtils->pIPStringUtils->getFieldMinValue();
	const int max_value = gEnv->pUtils->pIPStringUtils->getFieldMaxValue();

	m_pLayout = new QHBoxLayout();
	m_pLayout->setSpacing(2);
	m_pLayout->setMargin(0);

    m_pPartA = new CIPSpinBoxWidget();
	m_pPartA->setMinimum(1);
	m_pPartA->setMaximum(max_value);
	m_pPartA->setButtonSymbols(QAbstractSpinBox::NoButtons);
	m_pLayout->addWidget(m_pPartA);

    m_pPartB = new CIPSpinBoxWidget();
	m_pPartB->setMinimum(min_value);
	m_pPartB->setMaximum(max_value);
	m_pPartB->setButtonSymbols(QAbstractSpinBox::NoButtons);
	m_pLayout->addWidget(m_pPartB);

    m_pPartC = new CIPSpinBoxWidget();
	m_pPartC->setMinimum(min_value);
	m_pPartC->setMaximum(max_value);
	m_pPartC->setButtonSymbols(QAbstractSpinBox::NoButtons);
	m_pLayout->addWidget(m_pPartC);

    m_pPartD = new CIPSpinBoxWidget();
	m_pPartD->setMinimum(min_value);
	m_pPartD->setMaximum(max_value);
	m_pPartD->setButtonSymbols(QAbstractSpinBox::NoButtons);
	m_pLayout->addWidget(m_pPartD);

	setLayout(m_pLayout);

	bool q_assert = false;

	q_assert = QObject::connect(m_pPartA, SIGNAL(valueChanged(int)), this, SLOT(fieldChanged()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartB, SIGNAL(valueChanged(int)), this, SLOT(fieldChanged()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartC, SIGNAL(valueChanged(int)), this, SLOT(fieldChanged()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartD, SIGNAL(valueChanged(int)), this, SLOT(fieldChanged()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartA, SIGNAL(key_pressed(QKeyEvent *)), this, SLOT(keyPress(QKeyEvent *)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartB, SIGNAL(key_pressed(QKeyEvent *)), this, SLOT(keyPress(QKeyEvent *)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartC, SIGNAL(key_pressed(QKeyEvent *)), this, SLOT(keyPress(QKeyEvent *)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pPartD, SIGNAL(key_pressed(QKeyEvent *)), this, SLOT(keyPress(QKeyEvent *)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CIPTextBoxWidget::~CIPTextBoxWidget()
{
}

bool CIPTextBoxWidget::setIP(const QString & ip)
{
	bool ok = false;
	QList<int> address_list = toInt(ip, &ok);
	if (!ok)
		return false;

	m_pPartA->setValue(address_list[0]);
	m_pPartB->setValue(address_list[1]);
	m_pPartC->setValue(address_list[2]);
	m_pPartD->setValue(address_list[3]);

	return true;
}

QString CIPTextBoxWidget::getIP() const
{
	bool ok = false;
	QString ip_address = join(m_pPartA->value(), m_pPartB->value(), m_pPartC->value(), m_pPartD->value(), &ok);
	if(!ok)
		return QString();

	return ip_address;
}

void CIPTextBoxWidget::fieldChanged()
{
    QSpinBox * pSpinBox = qobject_cast<QSpinBox*>(QObject::sender());
    if (!pSpinBox)
	{
		LOGError("Incorrect sender");
		return;
	}

    bool jump_forward = pSpinBox->text().size() >= FIELD_SIZE;
	if (!jump_forward)
		return;

	jumpNext(true);
}

void CIPTextBoxWidget::jumpNext(bool highlight)
{
    CIPSpinBoxWidget * pCurrentSpinBox = getFocusedSpinBox();
    CIPSpinBoxWidget * pNewSpinBox = NULL;

    if (pCurrentSpinBox == m_pPartA)
        pNewSpinBox = m_pPartB;
	else
    if (pCurrentSpinBox == m_pPartB)
        pNewSpinBox = m_pPartC;
	else
    if (pCurrentSpinBox == m_pPartC)
        pNewSpinBox = m_pPartD;

    if (pNewSpinBox)
	{
        pNewSpinBox->setFocus(Qt::OtherFocusReason);
        if (highlight) pNewSpinBox->selectAll();
	}
}

void CIPTextBoxWidget::jumpPrevious(bool highlight)
{
    CIPSpinBoxWidget * pCurrentSpinBox = getFocusedSpinBox();
    CIPSpinBoxWidget * pNewSpinBox = NULL;

    if (pCurrentSpinBox == m_pPartB)
        pNewSpinBox = m_pPartA;
	else
    if (pCurrentSpinBox == m_pPartC)
        pNewSpinBox = m_pPartB;
	else
    if (pCurrentSpinBox == m_pPartD)
        pNewSpinBox = m_pPartC;

    if (pNewSpinBox)
	{
        pNewSpinBox->setFocus(Qt::OtherFocusReason);
        if (highlight) pNewSpinBox->selectAll();
	}
}

void CIPTextBoxWidget::keyPress(QKeyEvent * pEvent)
{
    if (!pEvent)
		return;

    if (pEvent->key() == Qt::Key_Down || pEvent->key() == Qt::Key_Right)
		jumpNext(false);
	else
    if (pEvent->key() == Qt::Key_Up || pEvent->key() == Qt::Key_Left)
		jumpPrevious(false);
	else
    if (pEvent->key() == Qt::Key_Backspace)
		jumpPrevious(false);
}

/* ============================================ */

CIPSpinBoxWidget::CIPSpinBoxWidget(QWidget *parent):
    QSpinBox(parent),
    m_cursor_position(0)
{
    lineEdit()->setCursorPosition(m_cursor_position);
}

CIPSpinBoxWidget * CIPTextBoxWidget::getFocusedSpinBox() const
{
    if (m_pPartA->hasFocus()) return m_pPartA;
	else
    if (m_pPartB->hasFocus()) return m_pPartB;
	else
    if (m_pPartC->hasFocus()) return m_pPartC;
	else
    if (m_pPartD->hasFocus()) return m_pPartD;

	return NULL;
}

void CIPSpinBoxWidget::keyPressEvent(QKeyEvent * pEvent)
{
    if (!pEvent)
		return;

    if (pEvent->key() != Qt::Key_Down && pEvent->key() != Qt::Key_Up)
        QSpinBox::keyPressEvent(pEvent);

	int current_position = lineEdit()->cursorPosition();
	if (current_position == m_cursor_position)
	{
        Q_EMIT key_pressed(pEvent);
	}

	m_cursor_position = current_position;
}

void CIPSpinBoxWidget::keyReleaseEvent(QKeyEvent * pEvent)
{
    if (!pEvent)
	{
		return;
	}

    if (pEvent->key() != Qt::Key_Down && pEvent->key() != Qt::Key_Up)
	{
        QSpinBox::keyPressEvent(pEvent);
	}

	int current_position = lineEdit()->cursorPosition();
	if (current_position == m_cursor_position)
	{
        Q_EMIT key_released(pEvent);
	}

	m_cursor_position = current_position;
}
