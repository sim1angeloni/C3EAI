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

#ifndef PLAYERS_H
#define PLAYERS_H

#include <QtCore/QObject>
#include <QtGui/QWidget>
#include "ui_widget_player.h"
#include "server.h"

class CPlayersModel;
class CReservedSlotsModel;
class CBansModel;

class CPlayersWidget : public QWidget
{
	Q_OBJECT

    public:
        enum EPlayerSection
        {
            ePlayerName,
            ePlayerID,
            ePlayerIP,
            ePlayerPing,
            ePlayerState,
            ePlayerProfile,

            ePlayerNONE
        };

	public:
        CPlayersWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~CPlayersWidget();

	public Q_SLOTS:
		void received_status_playerlist(QString list);
		void received_banslist(QString list);
		void received_reservedslotslist(QString list);

	Q_SIGNALS:
        void update_server_status(CServerWidget::SServer);

		void add_reservedslot(QString);
		void del_reservedslot(QString);
		void kick_player(QString);
		void ban_player(QString);
		void unban_player(QString);
		void refresh_playerlist();
		void refresh_banlist();
		void refresh_reservedslotlist();

	private Q_SLOTS:
		void Add_Ban();
		void Remove_Ban();
		void Assign_Ban();
		void Add_ReservedSlot();
		void Remove_ReservedSlot();
		void Assign_ReservedSlot();
		void Kick();

    private:
        void BuildBansList();
        void BuildPlayersList();
        void BuildReservedSlotsList();

    private:
        Q_DISABLE_COPY(CPlayersWidget)

	private:
        Ui::widget_player m_ui;

        CPlayersModel * m_pPlayersModel;
        CReservedSlotsModel * m_pReservedSlotsModel;
        CBansModel * m_pBansModel;
};

#endif /* PLAYERS_H */
