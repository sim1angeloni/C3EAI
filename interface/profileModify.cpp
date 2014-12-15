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
#include <QtNetwork/QHostAddress>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include "profileModify.h"

CProfileModifyFrontend::CProfileModifyFrontend(QWidget *parent, Qt::WindowFlags f):
	QDialog(parent, f)
{
    FUNCTION_LOG

	m_ui.setupUi(this);
	m_ui.buttonBox->button(QDialogButtonBox::Save)->setIcon(QIcon(":/save.png"));
	m_ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/delete.png"));

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(Buttons_Clicked(QAbstractButton *)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.displayPassword, SIGNAL(stateChanged(int)), this, SLOT(DisplayPassword()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CProfileModifyFrontend::~CProfileModifyFrontend()
{
    FUNCTION_LOG
}

void CProfileModifyFrontend::DisplayPassword()
{
    FUNCTION_LOG

	if (m_ui.displayPassword->checkState() == Qt::Checked)
		m_ui.serverPassword->setEchoMode(QLineEdit::Normal);
	else
		m_ui.serverPassword->setEchoMode(QLineEdit::Password);
}

void CProfileModifyFrontend::setProfile(CProfilesDatabase::SProfile profile)
{
    FUNCTION_LOG

	/* THE FIELDS MUST HAVE BEEN CHECKED BEFORE TO CALL THIS FUNCTION */

	m_ui.profileName->setText(profile.m_name);
	m_ui.serverIP->setIP(profile.m_host);
	m_ui.serverGamePort->setValue(profile.m_game_port);
	m_ui.serverHttpPort->setValue(profile.m_http_port);
	m_ui.serverPassword->setText(profile.m_password);
}

CProfilesDatabase::SProfile CProfileModifyFrontend::getProfile()
{
    FUNCTION_LOG

	/* THE FIELDS MUST HAVE BEEN CHECKED BEFORE TO CALL THIS FUNCTION */

	QHostAddress addr;
	addr.setAddress(m_ui.serverIP->getIP());

	return CProfilesDatabase::SProfile(m_ui.profileName->text(),
        addr.toString(),
        m_ui.serverPassword->text(),
        (quint16)m_ui.serverGamePort->text().toUShort(),
        (quint16)m_ui.serverHttpPort->text().toUShort());
}

void CProfileModifyFrontend::Buttons_Clicked(QAbstractButton * button)
{
    FUNCTION_LOG

	if (!button)
	{
		LOGError("Button does not exist");
		return;
	}

	QDialogButtonBox::StandardButton bt_type = m_ui.buttonBox->standardButton(button);

	switch (bt_type)
	{
		case QDialogButtonBox::Save:
			{
				bool passed;
				TryModify(button, passed);

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

void CProfileModifyFrontend::TryModify(QAbstractButton * pButton, bool & passed)
{
    FUNCTION_LOG

	passed = false;

    pButton->setEnabled(false);

	/* PROFILE NAME */
    if (isFieldEmpty(m_ui.profileName->text(), "Profile-name", TR_FIELD_PROFILE_NAME, this)) { pButton->setEnabled(true); return; }
    if (!isFieldText(m_ui.profileName->text(), "Profile-name", TR_FIELD_PROFILE_NAME, this)) { pButton->setEnabled(true); return; }

	/* IP-ADDRESS */
    if (isFieldEmpty(m_ui.serverIP->getIP(), "IP-address", TR_FIELD_IP_ADDRESS, this)) { pButton->setEnabled(true); return; }
    QHostAddress addr;
    if (!addr.setAddress(m_ui.serverIP->getIP()))
    {
        LOGError(QString("Field '%1' not correct").arg("IP-address"));
        QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_IPADDRESS);
        pButton->setEnabled(true);
        return;
    }

	/* GAME PORT */
    if (isFieldEmpty(m_ui.serverGamePort->text(), "Game-port", TR_FIELD_GAME_PORT, this)) { pButton->setEnabled(true); return; }
	quint32 gamePort = m_ui.serverGamePort->text().toUInt();				//TESTED WITH UINT32 IN ORDER TO VERIFY THE OVERFLOW
    if (isFieldPortZero(gamePort, "Game-port", TR_FIELD_GAME_PORT, this)) { pButton->setEnabled(true); return; }
    if (isFieldPortOverflow(gamePort, "Game-port", TR_FIELD_GAME_PORT, this)) { pButton->setEnabled(true); return; }

	/* HTTP PORT */
    if (isFieldEmpty(m_ui.serverHttpPort->text(), "HTTP-port", TR_FIELD_HTTP_PORT, this)) { pButton->setEnabled(true); return; }
	quint32 httpPort = m_ui.serverHttpPort->text().toUInt();				//TESTED WITH UINT32 IN ORDER TO VERIFY THE OVERFLOW
    if (isFieldPortZero(httpPort, "HTTP-port", TR_FIELD_HTTP_PORT, this)) { pButton->setEnabled(true); return; }
    if (isFieldPortOverflow(httpPort, "HTTP-port", TR_FIELD_HTTP_PORT, this)) { pButton->setEnabled(true); return; }

	/* PASSWORD */
    if (isFieldEmpty(m_ui.serverPassword->text(), "Password", TR_FIELD_PASSWORD, this)) { pButton->setEnabled(true); return; }
    if (!isFieldText(m_ui.serverPassword->text(), "Password", TR_FIELD_PASSWORD, this)) { pButton->setEnabled(true); return; }

    pButton->setEnabled(true);

	passed = true;
}
