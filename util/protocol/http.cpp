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
#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include "http.h"

static const int TIMER_TIMEOUT_CONNECTION = 5;
static const int TIMER_INTERVAL = 1000;
QTimer * m_timer_http = NULL;

CHttpProtocol::CHttpProtocol(const QString &host, const quint16 port, const QString &path, const QString &content_type, const HttpMethod method, QObject *parent):
	QObject(parent)
{
	FUNCTION_LOG

	m_host = host;
	m_port = port;
	m_path = path;
	m_contentType = content_type;
	m_method = method;

	m_startconnID = 0;
	m_endconnID = 0;

	m_pConnection = new QHttp();

	SAFE_DELETE(m_timer_http);
	m_timer_http = new QTimer();
	m_timer_http->setInterval(TIMER_INTERVAL);
    resetTimerTimeout();

	bool q_assert = false;

	q_assert = QObject::connect(m_timer_http, SIGNAL(timeout()), this, SLOT(timeout()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(done(bool)), this, SLOT(done(bool)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)), this, SLOT(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)));
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(readyRead(QHttpResponseHeader)), this, SLOT(readyRead(QHttpResponseHeader)));
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(requestStarted(int)), this, SLOT(requestStarted(int)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(responseHeaderReceived(QHttpResponseHeader)), this, SLOT(responseHeaderReceived(QHttpResponseHeader)));
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CHttpProtocol::~CHttpProtocol(void)
{
	FUNCTION_LOG

	SAFE_DELETE(m_timer_http);

    if(m_pConnection)
    {
        bool q_assert = QObject::disconnect(m_pConnection, 0, 0, 0);
        Q_ASSERT(q_assert);

        m_pConnection->abort();
    }

	SAFE_DELETE(m_pConnection);
}

void CHttpProtocol::connect()
{
	FUNCTION_LOG
	m_startconnID = m_pConnection->setHost(m_host, QHttp::ConnectionModeHttp, m_port);
}

void CHttpProtocol::disconnect()
{
	FUNCTION_LOG
	m_endconnID = m_pConnection->close();
}

void CHttpProtocol::send(const QByteArray &content)
{
	FUNCTION_LOG

	QHttpRequestHeader header = buildRequestHeader(content.size());
	m_pConnection->request(header, content);
}

void CHttpProtocol::send()
{
	FUNCTION_LOG

	QHttpRequestHeader header = buildRequestHeader(0);
	m_pConnection->request(header);
}

void CHttpProtocol::get(const QString &content)
{
	FUNCTION_LOG
	m_pConnection->get(QUrl::toPercentEncoding(content));
}

void CHttpProtocol::done(bool error)
{
	FUNCTION_LOG

	if (error)
	{
		Q_EMIT receivedHTTPError(m_pConnection->error(), m_pConnection->errorString());
		return;
	}
}

void CHttpProtocol::timeout()
{
	FUNCTION_LOG

	m_timeoutCounter--;
	if (m_timeoutCounter == 0)
	{
		m_timer_http->stop();
		Q_EMIT disconnected();
		return;
	}
}

void CHttpProtocol::resetTimerTimeout()
{
    m_timeoutCounter = TIMER_TIMEOUT_CONNECTION;
}

void CHttpProtocol::proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * pAuthenticator)
{
    Q_UNUSED(proxy)
    Q_UNUSED(pAuthenticator)

	FUNCTION_LOG
}

void CHttpProtocol::readyRead(const QHttpResponseHeader & resp)
{
    Q_UNUSED(resp)

	FUNCTION_LOG
}

void CHttpProtocol::requestFinished(int id, bool error)
{
	FUNCTION_LOG

	if (error)
	{
		Q_EMIT receivedHTTPError(m_pConnection->error(), m_pConnection->errorString());
		return;
	}

	if (id == m_startconnID)
	{
		Q_EMIT connected();
		return;
	}

	if (id == m_endconnID)
	{
		Q_EMIT disconnected();
		return;
	}

	Q_EMIT receivedHTTPData(m_pConnection->readAll());
}

void CHttpProtocol::requestStarted(int id)
{
    Q_UNUSED(id)

	FUNCTION_LOG
}

void CHttpProtocol::responseHeaderReceived(const QHttpResponseHeader & resp)
{
    Q_UNUSED(resp)

	FUNCTION_LOG
}

void CHttpProtocol::stateChanged(int state)
{
	FUNCTION_LOG

	switch (state)
	{
		case QHttp::Connecting:
		{
            resetTimerTimeout();
			m_timer_http->start();
			break;
		}
		case QHttp::Connected:
		{
			m_timer_http->stop();
            resetTimerTimeout();
			break;
		}
		case QHttp::Closing:
		{
			Q_EMIT disconnected();
			break;
		}
	}
}

QHttpRequestHeader CHttpProtocol::buildRequestHeader(const int content_size)
{
	FUNCTION_LOG

	QString method;
	if (m_method == POST)
		method = "POST";
	else
		method = "GET";

	QString path("%1 %2/%3");
	path = path.arg(m_path);
    path = path.arg("HTTP");
    path = path.arg("1.0");

	QString host("%1:%2");
	host = host.arg(m_host);
	host = host.arg(QString::number(m_port));

	QHttpRequestHeader header(method, path);
	header.setValue("Host", host);

	if (content_size != 0)
	{
		header.setContentType(m_contentType);
		header.setContentLength(content_size);
	}

	return header;
}
