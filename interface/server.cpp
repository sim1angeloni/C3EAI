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
#include <QtCore/QTimer>
#include "server.h"
#include "variables.h"
#include "players.h"
#include "changeMap.h"
#include "other.h"
#include "util/protocol/cryHttp.h"

static const QRgb ACTIVE_COLOR(0xeca733);

static const int TIMER_INTERVAL = 10000;
QTimer * m_timer_status = NULL;

CServerWidget::CServerWidget(QWidget* parent, Qt::WindowFlags f):
	QWidget(parent, f)
{
    FUNCTION_LOG

    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionsWidgetServer);
	m_ui.setupUi(this);

	m_connected = false;
	m_pCryConn = NULL;

	m_pWdgVariables = new CVariablesWidget();
	m_pWdgPlayers = new CPlayersWidget();
	m_pWdgChangeMap = new CChangeMapWidget();
	m_pWdgOther = new COtherWidget();

	/* SET ACTIVE COLOR FOR CURRENT WIDGET */
	QPalette current_palette = m_ui.variables_bt->palette();
    current_palette.setColor(QPalette::Button, QColor(ACTIVE_COLOR));
	m_ui.variables_bt->setPalette(current_palette);

	/* SET CURRENT WIDGET */
	m_ui.widget_area->setWidget(m_pWdgVariables);

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.variables_bt, SIGNAL(clicked()), this, SLOT(Change_Current_Widget()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.players_bt, SIGNAL(clicked()), this, SLOT(Change_Current_Widget()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.changemap_bt, SIGNAL(clicked()), this, SLOT(Change_Current_Widget()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.other_bt, SIGNAL(clicked()), this, SLOT(Change_Current_Widget()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.disconnect_bt, SIGNAL(clicked()), this, SLOT(Server_Disconnect()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgVariables, SIGNAL(variable_changed(QString, QString)), this, SLOT(Send_Variable(QString, QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgOther, SIGNAL(command_used(QString)), this, SLOT(Send_Command(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(kick_player(QString)), this, SLOT(Send_Kick(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(add_reservedslot(QString)), this, SLOT(Send_ReservedSlot_Add(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(del_reservedslot(QString)), this, SLOT(Send_ReservedSlot_Delete(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(ban_player(QString)), this, SLOT(Send_Ban_Add(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(unban_player(QString)), this, SLOT(Send_Ban_Delete(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(refresh_playerlist()), this, SLOT(Send_Refresh_Players()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(refresh_banlist()), this, SLOT(Send_Refresh_Bans()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(refresh_reservedslotlist()), this, SLOT(Send_Refresh_ReservedSlots()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

    q_assert = QObject::connect(m_pWdgPlayers, SIGNAL(update_server_status(CServerWidget::SServer)), this, SLOT(Change_Server_Status(CServerWidget::SServer)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pWdgChangeMap, SIGNAL(changed_map(QString, QString)), this, SLOT(Send_Map_Gamerules(QString, QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CServerWidget::~CServerWidget()
{
    FUNCTION_LOG

	SAFE_DELETE(m_pWdgVariables);
	SAFE_DELETE(m_pWdgPlayers);
	SAFE_DELETE(m_pWdgChangeMap);
	SAFE_DELETE(m_pWdgOther);
}

void CServerWidget::Change_Current_Widget()
{
    FUNCTION_LOG

    QPushButton * pButton = qobject_cast<QPushButton *>(QObject::sender());
    if (!pButton)
	{
		LOGError("Incorrect sender");
		return;
	}

	/* RESET ACTIVE COLOR */
	m_ui.variables_bt->setPalette(this->palette());
	m_ui.players_bt->setPalette(this->palette());
	m_ui.changemap_bt->setPalette(this->palette());
	m_ui.other_bt->setPalette(this->palette());

    QWidget * pUseWidget = NULL;
    if (pButton == m_ui.variables_bt)
        pUseWidget = m_pWdgVariables;
	else
    if (pButton == m_ui.players_bt)
        pUseWidget = m_pWdgPlayers;
	else
    if (pButton == m_ui.changemap_bt)
        pUseWidget = m_pWdgChangeMap;
	else
    if (pButton == m_ui.other_bt)
        pUseWidget = m_pWdgOther;
	else
	{
		LOGError("Wrong sender");
		return;
	}

    if (!pUseWidget)
	{
		LOGError("Sender was NULL");
		return;
	}

	/* SET ACTIVE COLOR FOR CURRENT WIDGET */
    QPalette current_palette = pButton->palette();
    current_palette.setColor(QPalette::Button, QColor(ACTIVE_COLOR));
    pButton->setPalette(current_palette);

	/* SET CURRENT WIDGET */
	m_ui.widget_area->takeWidget();
    m_ui.widget_area->setWidget(pUseWidget);
}

void CServerWidget::Change_Server_Status(CServerWidget::SServer new_status)
{
    FUNCTION_LOG

	m_server = new_status;
}

bool CServerWidget::StartConnection(const QString &host, const QString &password, const quint16 game_port, const quint16 http_port)
{
    FUNCTION_LOG

    bool q_assert = false;

    if(m_pCryConn)
    {
        q_assert = QObject::disconnect(m_pCryConn, 0, 0, 0);
        Q_ASSERT(q_assert);
    }

	SAFE_DELETE(m_pCryConn);

	m_pCryConn = new CCryHttpProtocol();

	q_assert = QObject::connect(m_pCryConn, SIGNAL(not_connected()), this, SLOT(Server_NotConnected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(authorization(bool)), this, SLOT(Server_ConnectedAuthCheck(bool)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(unknown_command(QString)), this, SLOT(Received_UnknownCommand(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(reservedslot_unactive()), this, SLOT(Received_ReservedSlotUnactive()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(status_received(QString)), m_pWdgPlayers, SLOT(received_status_playerlist(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(reservedslot_list_received(QString)), m_pWdgPlayers, SLOT(received_reservedslotslist(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(ban_status_received(QString)), m_pWdgPlayers, SLOT(received_banslist(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pCryConn, SIGNAL(variable_value_received(QString)), m_pWdgVariables, SLOT(set_variable(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	if (!m_pCryConn->connect(host, password, game_port, http_port))
	{
        if(m_pCryConn)
        {
            q_assert = QObject::disconnect(m_pCryConn, 0, 0, 0);
            Q_ASSERT(q_assert);
        }

		SAFE_DELETE(m_pCryConn);

		LOGError(QString("Cannot start CryHTTP connection with '%1:%2'").arg(host).arg(http_port));
		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_CANTCONNECT_1);
		return false;
	}

	return true;
}

void CServerWidget::Received_UnknownCommand(QString command_name)
{
    FUNCTION_LOG

	LOGInfo(QString("Unknown command '%1'").arg(command_name));
	QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_UNKNOWNCMD.arg(command_name));
}

void CServerWidget::Received_ReservedSlotUnactive()
{
    FUNCTION_LOG

	LOGInfo("Reserved slot system unactive");
	QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_RESERVEDSLOT_UNACTIVE);
}

void CServerWidget::Send_Command(QString command_name)
{
    FUNCTION_LOG
    Send(command_name, false);
}

void CServerWidget::Send_Variable(QString variable_name, QString value)
{
    FUNCTION_LOG
    Send(QString("%1 %2").arg(variable_name).arg(value), false);
}

void CServerWidget::Send_Kick(QString player_name)
{
    FUNCTION_LOG
    Send(QString("kick %1").arg(player_name), false);
}

void CServerWidget::Send_ReservedSlot_Add(QString player_name)
{
    FUNCTION_LOG
    Send(QString("net_add_reserved_slot %1").arg(player_name), false);
}

void CServerWidget::Send_ReservedSlot_Delete(QString player_name)
{
    FUNCTION_LOG
    Send(QString("net_remove_reserved_slot %1").arg(player_name), false);
}

void CServerWidget::Send_Ban_Add(QString player_name)
{
    FUNCTION_LOG
    Send(QString("ban %1").arg(player_name), false);
}

void CServerWidget::Send_Ban_Delete(QString player_name)
{
    FUNCTION_LOG
    Send(QString("ban_remove %1").arg(player_name), false);
}

void CServerWidget::Send_Map_Gamerules(QString gamerule_name, QString map_name)
{
    FUNCTION_LOG
    Send(QString("gl_gamerules %1").arg(gamerule_name), false);
    Send(QString("gl_map %1").arg(map_name), false);
}

void CServerWidget::Send_Refresh_Players()
{
    FUNCTION_LOG
    Send("status", false);
}

void CServerWidget::Send_Refresh_Bans()
{
    FUNCTION_LOG
    Send("ban_status", false);
}

void CServerWidget::Send_Refresh_ReservedSlots()
{
    FUNCTION_LOG
    Send("net_list_reserved_slot", false);
}

void CServerWidget::Timer_timeout()
{
	if (!m_pCryConn)
	{
		LOGError("CryHTTP connection not started");
		return;
	}

	if (!m_pCryConn->queue_isempty())
	{
		LOGError("CryHTTP queue not empty");
		return;
	}

    if (!Send("status", true))
        return;

    if (!Send("ban_status", true))
        return;

    if (!Send("net_list_reserved_slot", true))
        return;

    Send(((CVariablesWidget*)m_pWdgVariables)->get_list(), true);
}

void CServerWidget::Timer_Enabled(bool enable)
{
    FUNCTION_LOG

	if (enable)
	{
		SAFE_DELETE(m_timer_status);
		m_timer_status = new QTimer();
		m_timer_status->setInterval(TIMER_INTERVAL);

		bool q_assert = false;

		q_assert = QObject::connect(m_timer_status, SIGNAL(timeout()), this, SLOT(Timer_timeout()), Qt::QueuedConnection);
		Q_ASSERT(q_assert);

		m_timer_status->start();
	}
    else
	{
        if (m_timer_status)
			m_timer_status->stop();

		SAFE_DELETE(m_timer_status);
	}
}

void CServerWidget::Server_DoDisconnection()
{
    FUNCTION_LOG

	Timer_Enabled(false);

    if(m_pCryConn)
    {
        bool q_assert = QObject::disconnect(m_pCryConn, 0, 0, 0);
        Q_ASSERT(q_assert);
    }

	SAFE_DELETE(m_pCryConn);

	m_connected = false;
}

void CServerWidget::Server_Disconnect()
{
    FUNCTION_LOG

	Server_DoDisconnection();

	Q_EMIT disconnected();
}

void CServerWidget::Server_NotConnected()
{
    FUNCTION_LOG

	Server_DoDisconnection();

	QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_CANTCONNECT_2);

	Q_EMIT disconnected();
}

void CServerWidget::Server_ConnectedAuthCheck(bool yes)
{
    FUNCTION_LOG

	if (yes)
	{
		Timer_Enabled(true);
		Timer_timeout();

		m_connected = true;

		Q_EMIT connected();
	}
	else
	{
		Server_DoDisconnection();

		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_AUTHFAILED);

		Q_EMIT disconnected();
	}
}

bool CServerWidget::Send(QList<QString> commands, bool silent)
{
    FUNCTION_LOG

	for (int i = 0; i < commands.size(); ++i)
	{
		bool passed = Send(commands.at(i), silent);
		if (!passed)
			return false;
	}

	return true;
}

bool CServerWidget::Send(QString command, bool silent)
{
    FUNCTION_LOG

	if (!m_pCryConn)
	{
		LOGError("CryHTTP connection not started");
		if (!silent)
			QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_NOTCONNECTED);

		return false;
	}

	if (!m_pCryConn->send(command))
	{
		LOGError(QString("Unable to send command '%1'").arg(command));
		if (!silent)
			QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_CANTSEND.arg(command));

		return false;
	}

	return true;
}
