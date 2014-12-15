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
#include "updater.h"
#include "http.h"
#include "programinfo.h"

static const QString MASTER_SERVER_HOST("www.om-studios.com");
static const int MASTER_SERVER_PORT = 80;
static const QString MASTER_SERVER_FILE = QString("repository/%1/%1_%2.xml").arg(PROG_NAME_SHORT).arg(PROG_VERSION_UL);

CUpdater::CUpdater(QProgressBar * pBar, QObject * parent):
	QObject(parent)
{
	FUNCTION_LOG

	m_pConnection = NULL;
	m_lastversion = false;
	m_versionReceived = false;

    if (pBar) m_pWtgBar = pBar;
    else     m_pWtgBar = NULL;

	if (m_pWtgBar)
		m_pWtgBar->setValue(0);
}

CUpdater::~CUpdater()
{
	FUNCTION_LOG

	if (m_pConnection) m_pConnection->disconnect();
	SAFE_DELETE(m_pConnection);
}

void CUpdater::checkForUpdate()
{
	FUNCTION_LOG

	if (m_pWtgBar)
		m_pWtgBar->setValue(20);

    QHostInfo::lookupHost(MASTER_SERVER_HOST, this, SLOT(LookupDone(QHostInfo)));
}

void CUpdater::LookupDone(QHostInfo host)
{
	FUNCTION_LOG

    if (host.error() != QHostInfo::NoError)
    {
    	LOGError("Unable to find server");

    	if (m_pWtgBar)
    		m_pWtgBar->setValue(100);

    	Q_EMIT cantCheckForUpdate();
    	return;
    }

    QList<QHostAddress> addresses = host.addresses();
    if (addresses.isEmpty())
    {
    	LOGError("Unable to find server ip address");

    	if (m_pWtgBar)
    		m_pWtgBar->setValue(100);

    	Q_EMIT cantCheckForUpdate();
    	return;
    }

	if (m_pWtgBar)
		m_pWtgBar->setValue(40);

    CreateConnection(addresses.at(0).toString());
}

void CUpdater::CreateConnection(const QString &hostIP)
{
	FUNCTION_LOG

	SAFE_DELETE(m_pConnection);

	QString path = QString("/%2").arg(MASTER_SERVER_FILE);
	m_pConnection = new CHttpProtocol(hostIP, MASTER_SERVER_PORT, path, "application/xml", CHttpProtocol::GET);

	bool q_assert = false;

	q_assert = QObject::connect(m_pConnection, SIGNAL(connected()), this, SLOT(connected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(receivedHTTPData(QByteArray)), this, SLOT(receivedData(QByteArray)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pConnection, SIGNAL(receivedHTTPError(QHttp::Error, QString)), this, SLOT(receivedError(QHttp::Error, QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	if (m_pWtgBar)
		m_pWtgBar->setValue(60);

	m_pConnection->connect();
}

void CUpdater::connected()
{
	FUNCTION_LOG

	QString path = QString("/%2").arg(MASTER_SERVER_FILE);
	m_pConnection->get(path);
}

void CUpdater::disconnected()
{
    //FIXME do something
	FUNCTION_LOG
}

void CUpdater::receivedError(QHttp::Error, QString err)
{
    //FIXME do something
	FUNCTION_LOG
}

void CUpdater::receivedData(QByteArray data)
{
	FUNCTION_LOG

	if (m_versionReceived)
		return;

	if (m_pWtgBar)
	{
		m_pWtgBar->setValue(80);
	}

	QString url;
	m_lastversion = false;
	m_versionReceived = true;

	QString err;

	QDomDocument doc;
    bool ret = doc.setContent(data, &err);
    if (ret)
	{
		QDomElement methodResponse = doc.firstChildElement("c3EAIUpdater");
        if (methodResponse.isNull())
		{
			err = "Element 'c3EAIUpdater' is absent in response";
		}
		else
		{
			QDomElement result = methodResponse.firstChildElement("version");
            if (result.isNull())
			{
				err = "Element 'version' is absent in response";
			}
			else
			{
				if (QString::compare(result.attribute("value"), PROG_VERSION) == 0)
					m_lastversion = true;
				else
					url = result.attribute("location");
			}
		}
	}

	if (m_pWtgBar)
		m_pWtgBar->setValue(100);

	if (!ret || !err.isEmpty())
	{
		LOGError(QString("Received wrong string from server. %1").arg(err.simplified()));
		Q_EMIT cantCheckForUpdate();
		return;
	}

	Q_EMIT upToDate(m_lastversion, url);
}
