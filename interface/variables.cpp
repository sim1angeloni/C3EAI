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
#include "variables.h"

CVariablesWidget::CVariablesWidget(QWidget* parent, Qt::WindowFlags f):
	QWidget(parent, f)
{
    FUNCTION_LOG

    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionsWidgetVariables);
	m_ui.setupUi(this);

	QObjectList list = this->children();

    bool q_assert = false;

    for (int i = 0; i < list.size(); ++i)
	{
        QGroupBox * pGroupBox = dynamic_cast<QGroupBox*>(list[i]);
        if (!pGroupBox)
            continue;

        QObjectList boxlist = pGroupBox->children();

        for (int k = 0; k < boxlist.size(); ++k)
        {
            CCustomSpinboxWidget * pSpin = dynamic_cast<CCustomSpinboxWidget*>(boxlist[k]);
            if(!pSpin)
                continue;

            q_assert = QObject::connect(pSpin, SIGNAL(sigValueChanged(const QString)), this, SLOT(Value_Changed(const QString)), Qt::QueuedConnection);
            Q_ASSERT(q_assert);
        }
	}
}

CVariablesWidget::~CVariablesWidget()
{
    FUNCTION_LOG
}

QList<QString> CVariablesWidget::get_list()
{
    FUNCTION_LOG

	QList<QString> ret;

	QObjectList list = this->children();

    for (int i = 0; i < list.size(); ++i)
	{
        QGroupBox * pGroupBox = dynamic_cast<QGroupBox*>(list[i]);
        if (!pGroupBox)
            continue;

        QObjectList boxlist = pGroupBox->children();

        for (int k = 0; k < boxlist.size(); ++k)
        {
            CCustomSpinboxWidget * pSpin = dynamic_cast<CCustomSpinboxWidget*>(boxlist[k]);
            if(!pSpin)
                continue;

            ret.append(pSpin->objectName());
        }
	}

	return ret;
}

void CVariablesWidget::set_variable(QString message)
{
    FUNCTION_LOG

	message = message.section("$3", 1, 1);

	QString variable_name = message.section("=", 0, 0).simplified();
	QString value_text = message.section("=", 1, 1).section("$6", 1, 1).section(" ", 0, 0).simplified();

	if (variable_name.isEmpty())
	{
		LOGError(QString("Unable to detect variable name for '%1'").arg(message))
		return;
	}

	bool ok = false;
	double value = value_text.toDouble(&ok);
	if (value_text.isEmpty() || !ok)
	{
		LOGError(QString("Unable to detect value for '%1'").arg(message))
		return;
	}

	QObjectList list = this->children();

    for (int i = 0; i < list.size(); ++i)
	{
        QGroupBox * pGroupBox = dynamic_cast<QGroupBox*>(list[i]);
        if (!pGroupBox)
            continue;

        QObjectList boxlist = pGroupBox->children();

        for (int k = 0; k < boxlist.size(); ++k)
        {
            CCustomSpinboxWidget * pSpin = dynamic_cast<CCustomSpinboxWidget*>(boxlist[k]);
            if(!pSpin)
                continue;

            if (QString::compare(pSpin->objectName().toLower(), variable_name.toLower()) == 0)
            {
                pSpin->setValue(value);
            }
        }
	}
}

void CVariablesWidget::Value_Changed(const QString value)
{
    FUNCTION_LOG

    CCustomSpinboxWidget * pSpin = qobject_cast<CCustomSpinboxWidget *>(QObject::sender());
    if (!pSpin)
	{
		LOGError("Incorrect sender");
		return;
	}

    QString variable_name = pSpin->objectName();
	if (variable_name.isEmpty())
	{
		LOGError("Empty variable");
		return;
	}

	Q_EMIT variable_changed(variable_name, value);
}
