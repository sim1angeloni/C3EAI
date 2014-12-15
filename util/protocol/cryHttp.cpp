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
#include <QtCore/QCryptographicHash>
#include <QtCore/QTimer>
#include "cryHttp.h"
#include "xmlRpc.h"

/* Disable queue for output messages */
//#define DISABLE_QUEUE

static const int TIMER_INTERVAL = 100;
QTimer * m_timer_message = NULL;

CCryHttpProtocol::CCryHttpProtocol(QObject *parent):
	QObject(parent)
{
	FUNCTION_LOG

	m_output_messages.clear();
	m_pConnection = NULL;
	m_handShake = eHandShake_NotStarted;

	#ifndef DISABLE_QUEUE
		Timer_Enabled(true);
	#endif
}

CCryHttpProtocol::~CCryHttpProtocol()
{
	FUNCTION_LOG

	#ifndef DISABLE_QUEUE
		Timer_Enabled(false);
	#endif

	destroyConnection();
}

void CCryHttpProtocol::destroyConnection()
{
	FUNCTION_LOG

	m_handShake = eHandShake_NotStarted;

    if(m_pConnection)
    {
        bool q_assert = QObject::disconnect(m_pConnection, 0, 0, 0);
        Q_ASSERT(q_assert);
    }

	SAFE_DELETE(m_pConnection);
}

bool CCryHttpProtocol::connect(const QString &host, const QString &password, const quint16 game_port, const quint16 http_port)
{
	FUNCTION_LOG

	destroyConnection();

	if (host.isEmpty())
	{
		LOGError("Host address is empty");
		return false;
	}

	if (password.isEmpty())
	{
		LOGError("password is empty");
		return false;
	}

	if (game_port == 0 || http_port == 0)
	{
		LOGError(QString("Ports are null, game_port=%1, http_port=%2").arg(game_port).arg(http_port));
		return false;
	}

	m_hostIP = host;
	m_http_password = password;
	m_http_port = http_port;
	m_game_port = game_port;

	m_pConnection = new CXmlRpcProtocol(host, http_port);

	bool q_assert = false;

	q_assert = QObject::connect(m_pConnection, SIGNAL(connected()), this, SLOT(XMLRPC_connected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(disconnected()), this, SIGNAL(not_connected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(receivedXmlRpcData(QString)), this, SLOT(XMLRPC_receivedMessage(QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	startHandshake();

	return true;
}

bool CCryHttpProtocol::disconnect()
{
	FUNCTION_LOG

	destroyConnection();
	return true;
}

void CCryHttpProtocol::XMLRPC_receivedMessage(QString msg)
{
	FUNCTION_LOG

	if (m_handShake == eHandShake_SentChallenge)
		receivedAuthID(msg);
	else
	if (m_handShake == eHandShake_SentAuthCode)
		receivedAuthResponse(msg);
	else
	if (msg.contains("Server Status", Qt::CaseSensitive))
		Q_EMIT status_received(msg);
	else
	if (msg.contains("[Warning] Unknown command", Qt::CaseSensitive))
		Q_EMIT unknown_command(msg.remove("[Warning] Unknown command:", Qt::CaseSensitive).simplified());
	else
	if (msg.contains("[Reserved slots] System not active", Qt::CaseSensitive))
		Q_EMIT reservedslot_unactive();
	else
    if (msg.contains("|              Name              | Profile  |Time Remaining|", Qt::CaseSensitive))	//FIXME find another way to identify the commands, regexp?
		Q_EMIT ban_status_received(msg);
	else
	if (msg.contains("|              Name              | Profile  |", Qt::CaseSensitive))
		Q_EMIT reservedslot_list_received(msg);
	else
	if (msg.contains(QRegExp("\\s{4}\\$3\\w*.{1}=")))
		Q_EMIT variable_value_received(msg.simplified());
	else
		LOGInfo(QString("The following message has not been handled:\n%1").arg(msg));
}

void CCryHttpProtocol::startHandshake()
{
	FUNCTION_LOG

	if (!m_pConnection)
	{
		LOGError("XML/RPC connection not started");
		return;
	}

	if (m_handShake == eHandShake_NotStarted)
	{
		m_handShake = eHandShake_Started;
		m_pConnection->connect();
	}
}

void CCryHttpProtocol::XMLRPC_connected()
{
	FUNCTION_LOG

	if (m_handShake == eHandShake_Started)
		sendChallenge();
}

void CCryHttpProtocol::sendChallenge()
{
	FUNCTION_LOG

	if (!m_pConnection)
	{
		LOGError("XML/RPC connection not started");
		return;
	}

	m_handShake = eHandShake_SentChallenge;
	m_pConnection->execute("challenge");
}

void CCryHttpProtocol::receivedAuthID(QString private_auth)
{
	FUNCTION_LOG

	m_handShake = eHandShake_ReceivedAuthID;
	sendAuthCode(private_auth);
}

void CCryHttpProtocol::receivedAuthResponse(QString response)
{
	FUNCTION_LOG

	m_handShake = eHandShake_Finished;

	if (QString::compare(response, "authorized") == 0)
		Q_EMIT authorization(true);
	else
		Q_EMIT authorization(false);
}

void CCryHttpProtocol::sendAuthCode(QString private_auth)
{
	FUNCTION_LOG

	if (!m_pConnection)
	{
		LOGError("XML/RPC connection not started");
		return;
	}

	QString key("%1:%2");
	key = key.arg(private_auth);
	key = key.arg(m_http_password);

	QString hash(QCryptographicHash::hash(key.toAscii(), QCryptographicHash::Md5).toHex());

	m_handShake = eHandShake_SentAuthCode;
	m_pConnection->execute(QString("authenticate %1").arg(hash));
}

bool CCryHttpProtocol::queue_isempty()
{
	#ifdef DISABLE_QUEUE
		return true;
	#else
		return m_output_messages.isEmpty();
	#endif
}

bool CCryHttpProtocol::send(const QString &cmd)
{
	FUNCTION_LOG

	if (!m_pConnection)
	{
		LOGError("XML/RPC connection not started");
		return false;
	}

	if (cmd.isEmpty())
	{
		LOGError("Command to send is empty");
		return false;
	}

	if (m_handShake != eHandShake_Finished)
	{
		LOGError("HandShake not completed, unable to continue");
		return false;
	}

	#ifdef DISABLE_QUEUE
		m_connection->execute(cmd);
	#else
		m_output_messages.enqueue(cmd);
	#endif

	return true;
}

void CCryHttpProtocol::Timer_Enabled(bool enable)
{
	FUNCTION_LOG

	if (enable)
	{
		SAFE_DELETE(m_timer_message);
		m_timer_message = new QTimer();
		m_timer_message->setInterval(TIMER_INTERVAL);

		bool q_assert = false;

		q_assert = QObject::connect(m_timer_message, SIGNAL(timeout()), this, SLOT(Timer_timeout()), Qt::QueuedConnection);
		Q_ASSERT(q_assert);

		m_output_messages.clear();
		m_timer_message->start();
	}
	else
	{
		if (m_timer_message)
			m_timer_message->stop();

		m_output_messages.clear();
		SAFE_DELETE(m_timer_message);
	}
}

void CCryHttpProtocol::Timer_timeout()
{
	if (!m_pConnection)
		return;

	if (m_output_messages.isEmpty())
		return;

	QString cmd = m_output_messages.dequeue();
	if (cmd.isEmpty())
		return;

	m_pConnection->execute(cmd);
}
