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

#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtCore/QTime>
#include <QtGui/QWidget>
#include "ui_widget_server.h"

class CCryHttpProtocol;

class CServerWidget : public QWidget
{
	Q_OBJECT

    public:
        struct SServer
        {
            QString m_name;
            QString m_ip;
            QString m_version;
            QString m_level;
            QString m_gamemode;
            int m_players_current;
            int m_player_max;
            QTime m_time_remaining;

            SServer()
            {
                reset();
            }

            void reset()
            {
                m_name = "UNKNOWN";
                m_ip = "UNKNOWN";
                m_version = "UNKNOWN";
                m_level = "UNKNOWN";
                m_gamemode = "UNKNOWN";
                m_players_current = -1;
                m_player_max = -1;
                m_time_remaining = QTime(0, 0);
            }
        };

	public:
        CServerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~CServerWidget();

		bool StartConnection(const QString &host, const QString &password, const quint16 game_port, const quint16 http_port);

	Q_SIGNALS:
		void disconnected();
		void connected();

	private Q_SLOTS:
		void Received_UnknownCommand(QString command_name);
		void Received_ReservedSlotUnactive();

		void Change_Current_Widget();
        void Change_Server_Status(CServerWidget::SServer new_status);

		void Send_Command(QString command_name);
		void Send_Variable(QString variable_name, QString value);
		void Send_Kick(QString player_name);
		void Send_ReservedSlot_Add(QString player_name);
		void Send_ReservedSlot_Delete(QString player_name);
		void Send_Ban_Add(QString player_name);
		void Send_Ban_Delete(QString player_name);
		void Send_Refresh_Players();
		void Send_Refresh_Bans();
		void Send_Refresh_ReservedSlots();
		void Send_Map_Gamerules(QString gamerule_name, QString map_name);

		void Timer_timeout();

		void Server_Disconnect();
		void Server_NotConnected();
        void Server_ConnectedAuthCheck(bool yes);

    private:
        bool Send(QList<QString> commands, bool silent);
        bool Send(QString command, bool silent);
        void Timer_Enabled(bool enable);

        void Server_DoDisconnection();

    private:
        Q_DISABLE_COPY(CServerWidget)

	private:
        Ui::widget_server m_ui;

        QWidget * m_pWdgVariables;
        QWidget * m_pWdgPlayers;
        QWidget * m_pWdgChangeMap;
        QWidget * m_pWdgOther;

        bool m_connected;
        CCryHttpProtocol * m_pCryConn;

        SServer m_server;
};

#endif /* SERVER_H */
