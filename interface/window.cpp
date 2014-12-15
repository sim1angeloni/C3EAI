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
#include <QtNetwork/QHostAddress>
#include "window.h"
#include "about.h"
#include "programinfo.h"
#include "profileModify.h"
#include "profileSave.h"
#include "util/profiles/profilesModel.h"

CMainWindow::CMainWindow()
{
	FUNCTION_LOG

	bool q_assert = false;

	if (!gEnv->bDebug)
	{
		q_assert = QObject::connect(gEnv->pSplashScreen, SIGNAL(completed()), this, SLOT(Program_Loaded()), Qt::QueuedConnection);
		Q_ASSERT(q_assert);
	}

    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionMainWindow);
    m_ui.setupUi(this);

	setVisible(false);

	m_pProfilesModel = NULL;

	/* BUILD LISTS */
    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionProfiles);
	BuildBProfilesList();

	/* SET WINDOWS TITLE AND TABS */
	if (gEnv->bDebug)
	{
		setWindowTitle(QString("%1 %2 (DEBUG)").arg(PROG_NAME).arg(PROG_VERSION));
		EnableDisableTabs(true);
	}
	else
	{
		setWindowTitle(QString("%1 %2").arg(PROG_NAME).arg(PROG_VERSION));
		EnableDisableTabs(false);
	}

    q_assert = QObject::connect(m_ui.actionExit, SIGNAL(triggered()), this, SLOT(Program_Close()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.actionInformation, SIGNAL(triggered()), this, SLOT(Program_Info()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.connect_bt, SIGNAL(clicked()), this, SLOT(Program_Connect()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.server, SIGNAL(connected()), this, SLOT(Program_Connected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.server, SIGNAL(disconnected()), this, SLOT(Program_Disconnected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.displayPassword, SIGNAL(stateChanged(int)), this, SLOT(Programm_DisplayPassword()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.profileList, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Profile_DoubleClicked(const QModelIndex &)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.loadProfile_bt, SIGNAL(clicked()), this, SLOT(Profile_Load()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.saveProfile_bt, SIGNAL(clicked()), this, SLOT(Profile_Save()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.deleteProfile_bt, SIGNAL(clicked()), this, SLOT(Profile_Remove()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.modifyProfile_bt, SIGNAL(clicked()), this, SLOT(Profile_Modify()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_ui.actionSave_current_profile, SIGNAL(triggered(bool)), this, SLOT(Profile_Save()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CMainWindow::~CMainWindow()
{
	FUNCTION_LOG
}

void CMainWindow::Program_Loaded()
{
	SAFE_DELETE(gEnv->pSplashScreen);
	setVisible(true);
}

void CMainWindow::Program_Close()
{
	FUNCTION_LOG
	close();
}

void CMainWindow::Program_Info()
{
	FUNCTION_LOG

	CAboutDialog about_win(this);
	about_win.exec();
}

void CMainWindow::Program_Connect()
{
	FUNCTION_LOG

    m_ui.connect_bt->setEnabled(false);

	/* IP-ADDRESS */
    if (isFieldEmpty(m_ui.serverIP->getIP(), "IP-address", TR_FIELD_IP_ADDRESS, this)) { m_ui.connect_bt->setEnabled(true); return; }
    QHostAddress addr;
    if (!addr.setAddress(m_ui.serverIP->getIP()))
    {
        LOGError(QString("Field '%1' not correct").arg("IP-address"));
        QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_IPADDRESS);
        m_ui.connect_bt->setEnabled(true);
        return;
    }

	/* GAME PORT */
    quint32 gamePort = m_ui.serverGamePort->value();					//TESTED WITH UINT32 IN ORDER TO VERIFY THE OVERFLOW
    if (isFieldPortZero(gamePort, "Game-port", TR_FIELD_GAME_PORT, this)) { m_ui.connect_bt->setEnabled(true); return; }
    if (isFieldPortOverflow(gamePort, "Game-port", TR_FIELD_GAME_PORT, this)) { m_ui.connect_bt->setEnabled(true); return; }

	/* HTTP PORT */
    quint32 httpPort = m_ui.serverHttpPort->value();					//TESTED WITH UINT32 IN ORDER TO VERIFY THE OVERFLOW
    if (isFieldPortZero(httpPort, "HTTP-port", TR_FIELD_HTTP_PORT, this)) { m_ui.connect_bt->setEnabled(true); return; }
    if (isFieldPortOverflow(httpPort, "HTTP-port", TR_FIELD_HTTP_PORT, this)) { m_ui.connect_bt->setEnabled(true); return; }

	/* PASSWORD */
    QString password = m_ui.serverPassword->text();
    if (isFieldEmpty(password, "Password", TR_FIELD_PASSWORD, this)) { m_ui.connect_bt->setEnabled(true); return; }
    if (!isFieldText(password, "Password", TR_FIELD_PASSWORD, this)) { m_ui.connect_bt->setEnabled(true); return; }

    if (!m_ui.server->StartConnection(addr.toString(), password, (quint16)gamePort, (quint16)httpPort))
        m_ui.connect_bt->setEnabled(true);
}

void CMainWindow::Program_Connected()
{
	FUNCTION_LOG
	EnableDisableTabs(true);
}

void CMainWindow::Program_Disconnected()
{
	FUNCTION_LOG

    m_ui.connect_bt->setEnabled(true);
	EnableDisableTabs(false);
}

void CMainWindow::EnableDisableTabs(bool isConnected)
{
	FUNCTION_LOG

	if (isConnected)
	{
        m_ui.tabWidget->setTabEnabled(1,true);
        m_ui.tabWidget->setCurrentIndex(0);
	}
	else
	{
        m_ui.tabWidget->setCurrentIndex(0);
        m_ui.tabWidget->setTabEnabled(1,false);
	}
}

void CMainWindow::BuildBProfilesList()
{
	FUNCTION_LOG

	SAFE_DELETE(m_pProfilesModel);

	m_pProfilesModel = new CProfilesModel(m_profilesDB);

    m_ui.profileList->setModel(m_pProfilesModel);
    m_ui.profileList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ui.profileList->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ui.profileList->horizontalHeader()->setStretchLastSection(true);
    m_ui.profileList->horizontalHeader()->hide();
}

void CMainWindow::Programm_DisplayPassword()
{
	FUNCTION_LOG

    if (m_ui.displayPassword->checkState() == Qt::Checked)
        m_ui.serverPassword->setEchoMode(QLineEdit::Normal);
	else
        m_ui.serverPassword->setEchoMode(QLineEdit::Password);
}

void CMainWindow::Profile_Save()
{
	FUNCTION_LOG

	if (!m_pProfilesModel)
	{
		LOGError("Profiles list model does not exist");
		return;
	}

    m_ui.saveProfile_bt->setEnabled(false);

	/* IP-ADDRESS */
    if (isFieldEmpty(m_ui.serverIP->getIP(), "IP-address", TR_FIELD_IP_ADDRESS, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }
    QHostAddress addr;
    if (!addr.setAddress(m_ui.serverIP->getIP()))
    {
        LOGError(QString("Field '%1' not correct").arg("IP-address"));
        QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_IPADDRESS);
        m_ui.saveProfile_bt->setEnabled(true);
        return;
    }

	/* GAME PORT */
    quint32 gamePort = m_ui.serverGamePort->value();					//TESTED WITH UINT32 IN ORDER TO VERIFY THE OVERFLOW
    if (isFieldPortZero(gamePort, "Game-port", TR_FIELD_GAME_PORT, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }
    if (isFieldPortOverflow(gamePort, "Game-port", TR_FIELD_GAME_PORT, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }

	/* HTTP PORT */
    quint32 httpPort = m_ui.serverHttpPort->value();					//TESTED WITH UINT32 IN ORDER TO VERIFY THE OVERFLOW
    if (isFieldPortZero(httpPort, "HTTP-port", TR_FIELD_HTTP_PORT, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }
    if (isFieldPortOverflow(httpPort, "HTTP-port", TR_FIELD_HTTP_PORT, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }

	/* PASSWORD */
    QString password = m_ui.serverPassword->text();
    if (isFieldEmpty(password, "Password", TR_FIELD_PASSWORD, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }
    if (!isFieldText(password, "Password", TR_FIELD_PASSWORD, this)) { m_ui.saveProfile_bt->setEnabled(true); return; }

	CProfileSaveFrontend save_win;
	if (save_win.exec() == QDialog::Accepted)
	{
		QString profile_name = save_win.getProfileName().simplified();

		if (m_profilesDB.profileNameIsFree(profile_name))
		{
			m_profilesDB.addProfile(profile_name, addr.toString(), password, (quint16)gamePort, (quint16)httpPort);
			m_pProfilesModel->reload();
		}
		else
		{
			LOGError(QString("Profile name '%1' was not free").arg(profile_name));
			QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_PROFILE_NAME_IN_USE.arg(profile_name));
		}
	}

    m_ui.saveProfile_bt->setEnabled(true);
}

void CMainWindow::Profile_Modify()
{
	FUNCTION_LOG

	if (!m_pProfilesModel)
	{
		LOGError("Profiles list model does not exist");
		return;
	}

    m_ui.modifyProfile_bt->setEnabled(false);

    CProfilesDatabase::SProfile profile = m_pProfilesModel->getProfile(m_ui.profileList->currentIndex());

	if (profile.isNull())
	{
		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_PROFILE_NOT_SELECTED);
	}
	else
	{
		CProfileModifyFrontend modify_win;
		modify_win.setProfile(profile);

		if (modify_win.exec() == QDialog::Accepted)
		{
			m_profilesDB.updateProfile(profile.m_name, modify_win.getProfile());

			m_pProfilesModel->reload();
		}
	}

    m_ui.modifyProfile_bt->setEnabled(true);
}

void CMainWindow::Profile_Remove()
{
	FUNCTION_LOG

	if (!m_pProfilesModel)
	{
		LOGError("Profiles list model does not exist");
		return;
	}

    m_ui.deleteProfile_bt->setEnabled(false);

    CProfilesDatabase::SProfile profile = m_pProfilesModel->getProfile(m_ui.profileList->currentIndex());

	if (profile.isNull())
	{
		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_PROFILE_NOT_SELECTED);
	}
	else
	{
		m_profilesDB.removeProfile(profile);
		m_pProfilesModel->reload();
	}

    m_ui.deleteProfile_bt->setEnabled(true);
}

void CMainWindow::Profile_DoubleClicked(const QModelIndex &index)
{
	FUNCTION_LOG

	if (!m_pProfilesModel)
	{
		LOGError("Profiles list model does not exist");
		return;
	}

	Profile_Load(index);
}

void CMainWindow::Profile_Load()
{
	FUNCTION_LOG

	if (!m_pProfilesModel)
	{
		LOGError("Profiles list model does not exist");
		return;
	}

    Profile_Load(m_ui.profileList->currentIndex());
}

void CMainWindow::Profile_Load(const QModelIndex &index)
{
	CProfilesDatabase::SProfile profile = m_pProfilesModel->getProfile(index);

    m_ui.loadProfile_bt->setEnabled(false);

	if (profile.isNull())
	{
		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_PROFILE_NOT_SELECTED);
	}
	else
	{
        m_ui.serverIP->setIP(profile.m_host);
        m_ui.serverPassword->setText(profile.m_password);
        m_ui.serverGamePort->setValue(profile.m_game_port);
        m_ui.serverHttpPort->setValue(profile.m_http_port);
	}

    m_ui.loadProfile_bt->setEnabled(true);
}
