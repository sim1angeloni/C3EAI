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
#include <QtGui/QProgressBar>
#include <QtXml/QDomDocument>
#include "downloader.h"
#include "http.h"

CDownloader::CDownloader(QProgressBar * pBar, QObject *parent):
	QObject(parent)
{
	FUNCTION_LOG

	m_pConnection = NULL;
	m_lastversion = false;
	m_versionReceived = false;

    if (pBar) m_pWdgBar = pBar;
    else     m_pWdgBar = NULL;

	if (m_pWdgBar)
		m_pWdgBar->setValue(0);
}

CDownloader::~CDownloader()
{
	FUNCTION_LOG

	m_pConnection->disconnect();
	SAFE_DELETE(m_pConnection);
}

void CDownloader::download(QString file_name, quint32 port)
{
	FUNCTION_LOG

	file_name = file_name.simplified();
    file_name = file_name.remove("http://", Qt::CaseSensitive);
	m_server_name = file_name.section("/", 0, 0);
	m_filepath_name = file_name.section("/", 1);
	m_port = port;

	if (m_pWdgBar)
		m_pWdgBar->setValue(20);

	QHostInfo::lookupHost(m_server_name, this, SLOT(LookupDone(QHostInfo)));
}

void CDownloader::LookupDone(QHostInfo host)
{
	FUNCTION_LOG

    if (host.error() != QHostInfo::NoError)
    {
    	LOGError("Unable to find server");

    	if (m_pWdgBar)
    		m_pWdgBar->setValue(100);

    	Q_EMIT cantDownload();
    	return;
    }

    QList<QHostAddress> addresses = host.addresses();
    if (addresses.isEmpty())
    {
    	LOGError("Unable to find server ip address");

    	if (m_pWdgBar)
    		m_pWdgBar->setValue(100);

    	Q_EMIT cantDownload();
    	return;
    }

	if (m_pWdgBar)
		m_pWdgBar->setValue(40);

    CreateConnection(addresses.at(0).toString());
}

void CDownloader::CreateConnection(const QString &hostIP)
{
	FUNCTION_LOG

	SAFE_DELETE(m_pConnection);

	QString path = QString("/%2").arg(m_filepath_name);
	m_pConnection = new CHttpProtocol(hostIP, m_port, path, "application/zip", CHttpProtocol::GET);

	bool q_assert = false;

	q_assert = QObject::connect(m_pConnection, SIGNAL(connected()), this, SLOT(connected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(receivedHTTPData(QByteArray)), this, SLOT(receivedData(QByteArray)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(receivedHTTPError(QHttp::Error, QString)), this, SLOT(receivedError(QHttp::Error, QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	if (m_pWdgBar)
		m_pWdgBar->setValue(60);

	m_pConnection->connect();
}

void CDownloader::connected()
{
	FUNCTION_LOG

	QString path = QString("/%2").arg(m_filepath_name);
	m_pConnection->get(path);
}

void CDownloader::disconnected()
{
    //TODO do something
	FUNCTION_LOG
}

void CDownloader::receivedError(QHttp::Error, QString err)
{
    //TODO do something
	FUNCTION_LOG
}

void CDownloader::receivedData(QByteArray data)
{
    //TODO do something
	FUNCTION_LOG
}
