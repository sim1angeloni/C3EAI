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
#include <QtGui/QMouseEvent>
#include <QtGui/QLineEdit>
#include "customSpinbox.h"

CCustomSpinboxWidget::CCustomSpinboxWidget(QWidget *parent):
	QDoubleSpinBox(parent)
{
	installEventFilter(this);

	m_previous_value = value();

	bool q_assert = false;

	q_assert = QObject::connect(this, SIGNAL(editingFinished()), this, SLOT(editing_finished()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CCustomSpinboxWidget::~CCustomSpinboxWidget()
{
}

bool CCustomSpinboxWidget::eventFilter(QObject * pObject, QEvent * pEvent)
{
    switch (pEvent->type())
	{
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonDblClick:
            {
                QMouseEvent *mouse = static_cast<QMouseEvent *>(pEvent);
                //Check what was clicked
                if (!lineEdit()->rect().contains(mouse->pos()))
                {
                    if (parentWidget()) parentWidget()->setFocus(Qt::OtherFocusReason);
                }
            }
            break;
	}

    return QDoubleSpinBox::eventFilter(pObject, pEvent);
}

void CCustomSpinboxWidget::editing_finished()
{
	if (m_previous_value != value())
	{
		m_previous_value = value();

		Q_EMIT sigValueChanged(value());
		Q_EMIT sigValueChanged(lineEdit()->text());
	}
}
