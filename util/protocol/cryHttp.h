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

#ifndef CRYHTTP_H
#define CRYHTTP_H

#include <QtCore/QQueue>
#include <QtCore/QObject>

class CXmlRpcProtocol;

class CCryHttpProtocol : public QObject
{
	Q_OBJECT

    private:
        enum EHandShake
        {
            eHandShake_NotStarted	= 0,
            eHandShake_Started,
            eHandShake_SentChallenge,
            eHandShake_ReceivedAuthID,
            eHandShake_SentAuthCode,
            eHandShake_Finished
        };

	public:
        CCryHttpProtocol(QObject *parent = 0);
        virtual ~CCryHttpProtocol();

		bool connect(const QString &host, const QString &password, const quint16 game_port, const quint16 http_port);
		bool disconnect();
		bool send(const QString &cmd);
		bool queue_isempty();

	Q_SIGNALS:
		void not_connected();
		void authorization(bool);
		void status_received(QString);
		void ban_status_received(QString);
		void reservedslot_list_received(QString);
		void variable_value_received(QString);
		void reservedslot_unactive();
		void unknown_command(QString);

	private Q_SLOTS:
		void XMLRPC_connected();
		void XMLRPC_receivedMessage(QString msg);

		void Timer_timeout();

    private:
        void startHandshake();
        void sendChallenge();
        void receivedAuthResponse(QString response);
        void receivedAuthID(QString private_auth);
        void sendAuthCode(QString private_auth);

        void destroyConnection();

        void Timer_Enabled(bool enable);

    private:
        Q_DISABLE_COPY(CCryHttpProtocol)

    private:
        QString m_hostIP;
        QString m_http_password;
        quint16 m_http_port;
        quint16 m_game_port;

        CXmlRpcProtocol * m_pConnection;

        QQueue<QString> m_output_messages;

        EHandShake m_handShake;
};

#endif /* CRYHTTP_H */
