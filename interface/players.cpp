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
#include "players.h"
#include "util/players/playersModel.h"
#include "util/players/reservedSlotsModel.h"
#include "util/players/bansModel.h"

CPlayersWidget::CPlayersWidget(QWidget* parent, Qt::WindowFlags f):
	QWidget(parent, f)
{
	FUNCTION_LOG

	/* GENERATE MAIN UI */
    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionsWidgetPlayers);
	m_ui.setupUi(this);

	/* TABLE MODELS */
	m_pPlayersModel = NULL;
	m_pReservedSlotsModel = NULL;
	m_pBansModel = NULL;

	/* BUILD LISTS */
	BuildBansList();
	BuildPlayersList();
	BuildReservedSlotsList();

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.refreshPlayer_bt, SIGNAL(clicked()), this, SIGNAL(refresh_playerlist()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.refreshBan_bt, SIGNAL(clicked()), this, SIGNAL(refresh_banlist()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.refreshReservedSlot_bt, SIGNAL(clicked()), this, SIGNAL(refresh_reservedslotlist()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.addBan_bt, SIGNAL(clicked()), this, SLOT(Add_Ban()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.removeBan_bt, SIGNAL(clicked()), this, SLOT(Remove_Ban()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.assignBan_bt, SIGNAL(clicked()), this, SLOT(Assign_Ban()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.addReservedSlot_bt, SIGNAL(clicked()), this, SLOT(Add_ReservedSlot()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.removeReservedSlot_bt, SIGNAL(clicked()), this, SLOT(Remove_ReservedSlot()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.assignReservedSlot_bt, SIGNAL(clicked()), this, SLOT(Assign_ReservedSlot()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.kickPlayer_bt, SIGNAL(clicked()), this, SLOT(Kick()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CPlayersWidget::~CPlayersWidget()
{
	FUNCTION_LOG
}

void CPlayersWidget::received_status_playerlist(QString list)
{
	FUNCTION_LOG

	QList<QString> status_parts = list.split('\n', QString::SkipEmptyParts);

	if (status_parts.isEmpty())
	{
		LOGError("Received empty status");
		return;
	}

    CServerWidget::SServer current_server;

	bool server_section = true;
	while(!status_parts.isEmpty() && server_section)
	{
		QString part = status_parts.takeFirst();

		if (part.contains("Server Status", Qt::CaseSensitive))
			server_section = true;
		else
		if (part.contains("Connection Status", Qt::CaseSensitive))
			server_section = false;
		else
		if (part.contains("name", Qt::CaseSensitive))
			current_server.m_name = part.remove("name:").simplified();
		else
		if (part.contains("version", Qt::CaseSensitive))
			current_server.m_version = part.remove("version:").simplified();
		else
		if (part.contains("ip", Qt::CaseSensitive))
			current_server.m_ip = part.remove("ip:").simplified();
		else
		if (part.contains("level", Qt::CaseSensitive))
			current_server.m_level = part.remove("level:").simplified();
		else
		if (part.contains("gamerules", Qt::CaseSensitive))
			current_server.m_level = part.remove("gamerules:").simplified();
		else
		if (part.contains("players", Qt::CaseSensitive))
		{
			QString players = part.remove("gamerules:").simplified();
			QList<QString> players_count = players.split('/', QString::SkipEmptyParts);

			if (players_count.size() != 2)
				continue;

			current_server.m_players_current = players_count.at(0).toInt();
			current_server.m_player_max = players_count.at(1).toInt();
		}
		else
		if (part.contains("time remaining", Qt::CaseSensitive))
		{
			QString time = part.remove("time remaining:").simplified();
			QList<QString> time_count = time.split(':', QString::SkipEmptyParts);

			if (time_count.size() != 2)
				continue;

			current_server.m_time_remaining = QTime(time_count.at(0).toInt(), time_count.at(1).toInt());
		}
	}

	Q_EMIT update_server_status(current_server);

	if (!m_pPlayersModel)
	{
		LOGError("Players list model does not exist");
		return;
	}

	m_pPlayersModel->resetPlayers();

	if (status_parts.isEmpty())
	{
		LOGInfo("Server is empty");
		return;
	}

	for (int p = 0; p < status_parts.size(); ++p)
	{
		QList<QString> player_parts = status_parts[p].split(' ', QString::SkipEmptyParts);
		if (player_parts.isEmpty())
			continue;

		CPlayersModel::SPlayer player;
		EPlayerSection nextElement = ePlayerNONE;
		while(!player_parts.isEmpty())
		{
			QString currentPart = player_parts.takeFirst().simplified();

			switch(nextElement)
			{
				case ePlayerName:
				{
					player.m_name = currentPart;
					nextElement = ePlayerNONE;
					break;
				}
				case ePlayerID:
				{
					player.m_id = currentPart.toInt();
					nextElement = ePlayerNONE;
					break;
				}
				case ePlayerIP:
				{
					player.m_ip = currentPart;
					nextElement = ePlayerNONE;
					break;
				}
				case ePlayerPing:
				{
					player.m_ping = currentPart.toInt();
					nextElement = ePlayerNONE;
					break;
				}
				case ePlayerState:
				{
					player.m_state = currentPart.toInt();
					nextElement = ePlayerNONE;
					break;
				}
				case ePlayerProfile:
				{
					player.m_profile = currentPart.toInt();
					nextElement = ePlayerNONE;
					break;
				}
				default:
				{
					break;
				}
			}

			if (nextElement != ePlayerNONE)
			{
				nextElement = ePlayerNONE;
				continue;
			}

			if (currentPart.contains("name", Qt::CaseSensitive))
				nextElement = ePlayerName;
			else
			if (currentPart.contains("id", Qt::CaseSensitive))
				nextElement = ePlayerID;
			else
			if (currentPart.contains("ip", Qt::CaseSensitive))
				nextElement = ePlayerIP;
			else
			if (currentPart.contains("ping", Qt::CaseSensitive))
				nextElement = ePlayerPing;
			else
			if (currentPart.contains("state", Qt::CaseSensitive))
				nextElement = ePlayerState;
			else
			if (currentPart.contains("profile", Qt::CaseSensitive))
				nextElement = ePlayerProfile;
		}

		m_pPlayersModel->addPlayer(player);
	}
}

void CPlayersWidget::received_banslist(QString list)
{
	FUNCTION_LOG

	if (!m_pBansModel)
	{
		LOGError("Bans slots list model does not exist");
		return;
	}

	m_pBansModel->resetBans();

	QList<QString> list_parts = list.split('\n', QString::SkipEmptyParts);

	if (list_parts.isEmpty())
	{
		LOGError("Received wrong bans list");
		return;
	}

	while(!list_parts.isEmpty())
	{
		QString part = list_parts.takeFirst();

		if (!part.contains("+--------------------------------+----------+--------------+", Qt::CaseSensitive) &&
			!part.contains("|              Name              | Profile  |Time Remaining|", Qt::CaseSensitive))
		{
			CBansModel::SBan ban;

			QString user = part.simplified();
			QList<QString> user_parts = user.split("|", QString::SkipEmptyParts);

			if (user_parts.size() != 3)
				continue;

			ban.m_name = user_parts.at(0).simplified();
			ban.m_profileid = user_parts.at(1).simplified().toInt();
			ban.m_time_remaining = user_parts.at(2).simplified();

			m_pBansModel->addBan(ban);
		}
	}
}

void CPlayersWidget::received_reservedslotslist(QString list)
{
	FUNCTION_LOG

	if (!m_pReservedSlotsModel)
	{
		LOGError("Reserved slots list model does not exist");
		return;
	}

	m_pReservedSlotsModel->resetSlots();

	QList<QString> list_parts = list.split('\n', QString::SkipEmptyParts);

	if (list_parts.isEmpty())
	{
		LOGError("Received wrong reserved slots list");
		return;
	}

	while(!list_parts.isEmpty())
	{
		QString part = list_parts.takeFirst();

		if (!part.contains("+--------------------------------+----------+", Qt::CaseSensitive) &&
			!part.contains("|              Name              | Profile  |", Qt::CaseSensitive))
		{
			CReservedSlotsModel::SSlot slot;

			QString user = part.simplified();
			QList<QString> user_parts = user.split("|", QString::SkipEmptyParts);

			if (user_parts.size() != 2)
				continue;

			slot.m_name = user_parts.at(0).simplified();
			slot.m_profileid = user_parts.at(1).simplified().toInt();

			m_pReservedSlotsModel->addSlot(slot);
		}
	}
}

void CPlayersWidget::Add_Ban()
{
	FUNCTION_LOG

    m_ui.addBan_bt->setEnabled(false);

	QString playerName = m_ui.argBan->text().simplified();

	if (playerName.isEmpty())
	{
		LOGError("Player name not specified");
		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_EMPTYFIELD.arg("Player name"));
        m_ui.addBan_bt->setEnabled(true);
		return;
	}

	Q_EMIT ban_player(playerName);
	Q_EMIT refresh_banlist();
	Q_EMIT refresh_playerlist();

    m_ui.addBan_bt->setEnabled(true);
}

void CPlayersWidget::Remove_Ban()
{
	FUNCTION_LOG

	if (!m_pBansModel)
	{
		LOGError("Bans list model does not exist");
		return;
	}

    m_ui.removeBan_bt->setEnabled(false);

	CBansModel::SBan ban = m_pBansModel->getBan(m_ui.banList->currentIndex());

	if (!ban.isNull())
	{
		Q_EMIT unban_player(ban.m_name);
		Q_EMIT refresh_banlist();
	}

    m_ui.removeBan_bt->setEnabled(true);
}

void CPlayersWidget::Assign_Ban()
{
	FUNCTION_LOG

	if (!m_pPlayersModel)
	{
		LOGError("Players list model does not exist");
		return;
	}

    m_ui.assignBan_bt->setEnabled(false);

	CPlayersModel::SPlayer player = m_pPlayersModel->getPlayer(m_ui.playerList->currentIndex());

	if (!player.isNull())
	{
		Q_EMIT ban_player(player.m_name);
		Q_EMIT refresh_banlist();
		Q_EMIT refresh_playerlist();
	}

    m_ui.assignBan_bt->setEnabled(true);
}

void CPlayersWidget::Add_ReservedSlot()
{
	FUNCTION_LOG

    m_ui.addReservedSlot_bt->setEnabled(false);

	QString playerName = m_ui.argReservedSlot->text().simplified();

	if (playerName.isEmpty())
	{
		LOGError("Player name not specified");
		QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_DIALOG_ERROR_EMPTYFIELD.arg("Player name"));
        m_ui.addReservedSlot_bt->setEnabled(true);
		return;
	}

	Q_EMIT add_reservedslot(playerName);
	Q_EMIT refresh_reservedslotlist();

    m_ui.addReservedSlot_bt->setEnabled(true);
}

void CPlayersWidget::Remove_ReservedSlot()
{
	FUNCTION_LOG

	if (!m_pReservedSlotsModel)
	{
		LOGError("Reserved slots list model does not exist");
		return;
	}

    m_ui.removeReservedSlot_bt->setEnabled(false);

	CReservedSlotsModel::SSlot slot = m_pReservedSlotsModel->getSlot(m_ui.reservedSlotList->currentIndex());

	if (!slot.isNull())
	{
		Q_EMIT del_reservedslot(slot.m_name);
		Q_EMIT refresh_reservedslotlist();
	}

    m_ui.removeReservedSlot_bt->setEnabled(true);
}

void CPlayersWidget::Assign_ReservedSlot()
{
	FUNCTION_LOG

	if (!m_pPlayersModel)
	{
		LOGError("Players list model does not exist");
		return;
	}

    m_ui.assignReservedSlot_bt->setEnabled(false);

	CPlayersModel::SPlayer player = m_pPlayersModel->getPlayer(m_ui.playerList->currentIndex());

	if (!player.isNull())
	{
		Q_EMIT add_reservedslot(player.m_name);
		Q_EMIT refresh_reservedslotlist();
	}

    m_ui.assignReservedSlot_bt->setEnabled(true);
}

void CPlayersWidget::Kick()
{
	FUNCTION_LOG

	if (!m_pPlayersModel)
	{
		LOGError("Players list model does not exist");
		return;
	}

    m_ui.kickPlayer_bt->setEnabled(false);

	CPlayersModel::SPlayer player = m_pPlayersModel->getPlayer(m_ui.playerList->currentIndex());

	if (!player.isNull())
	{
		Q_EMIT kick_player(player.m_name);
		Q_EMIT refresh_playerlist();
	}

    m_ui.kickPlayer_bt->setEnabled(true);
}

void CPlayersWidget::BuildPlayersList()
{
	FUNCTION_LOG

	SAFE_DELETE(m_pPlayersModel);

	m_pPlayersModel = new CPlayersModel();

	m_ui.playerList->setModel(m_pPlayersModel);
	m_ui.playerList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ui.playerList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.playerList->setColumnWidth(0, 130);
	m_ui.playerList->setColumnWidth(1, 170);
	m_ui.playerList->setColumnWidth(2, 80);
	m_ui.playerList->setColumnHidden(3,true);
	m_ui.playerList->setColumnHidden(4,true);
	m_ui.playerList->setColumnWidth(5, 100);
}

void CPlayersWidget::BuildReservedSlotsList()
{
	FUNCTION_LOG

	SAFE_DELETE(m_pReservedSlotsModel);

	m_pReservedSlotsModel = new CReservedSlotsModel();

	m_ui.reservedSlotList->setModel(m_pReservedSlotsModel);
	m_ui.reservedSlotList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ui.reservedSlotList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.reservedSlotList->setColumnWidth(0, 130);
	m_ui.reservedSlotList->horizontalHeader()->setStretchLastSection(true);
}

void CPlayersWidget::BuildBansList()
{
	FUNCTION_LOG

	SAFE_DELETE(m_pBansModel);

	m_pBansModel = new CBansModel();

	m_ui.banList->setModel(m_pBansModel);
	m_ui.banList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ui.banList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.banList->setColumnWidth(0, 130);
	m_ui.banList->setColumnWidth(1, 100);
	m_ui.banList->setColumnWidth(2, 150);
}
