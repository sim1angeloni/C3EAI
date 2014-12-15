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
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include "profileSave.h"

CProfileSaveFrontend::CProfileSaveFrontend(QWidget *parent, Qt::WindowFlags f):
	QDialog(parent, f)
{
    FUNCTION_LOG

	m_ui.setupUi(this);
	m_ui.buttonBox->button(QDialogButtonBox::Save)->setIcon(QIcon(":/save.png"));
	m_ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/delete.png"));

    bool q_assert = QObject::connect(m_ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(Buttons_Clicked(QAbstractButton *)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CProfileSaveFrontend::~CProfileSaveFrontend()
{
    FUNCTION_LOG
}

QString CProfileSaveFrontend::getProfileName()
{
    FUNCTION_LOG

	/* THE FIELD MUST HAVE BEEN CHECKED BEFORE TO CALL THIS FUNCTION */

	return m_ui.profileName->text();
}

void CProfileSaveFrontend::Buttons_Clicked(QAbstractButton * pButton)
{
    FUNCTION_LOG

    if (!pButton)
	{
		LOGError("Button does not exist");
		return;
	}

    QDialogButtonBox::StandardButton bt_type = m_ui.buttonBox->standardButton(pButton);

	switch (bt_type)
	{
		case QDialogButtonBox::Save:
			{
				bool passed;
                TrySave(pButton, passed);

				if (passed)
					accept();

				break;
			}
		case QDialogButtonBox::Cancel:
			{
				reject();
				break;
			}
		default:
			{
				LOGError("Button incorrect");
				return;
			}
	}
}

void CProfileSaveFrontend::TrySave(QAbstractButton * pButton, bool & passed)
{
    FUNCTION_LOG

	passed = false;

    pButton->setEnabled(false);

	/* PROFILE NAME */
    if (isFieldEmpty(m_ui.profileName->text(), "Profile-name", TR_FIELD_PROFILE_NAME, this)) { pButton->setEnabled(true); return; }
    if (!isFieldText(m_ui.profileName->text(), "Profile-name", TR_FIELD_PROFILE_NAME, this)) { pButton->setEnabled(true); return; }

    pButton->setEnabled(true);

	passed = true;
}
