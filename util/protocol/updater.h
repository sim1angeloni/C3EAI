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

#ifndef UPDATER_H
#define UPDATER_H

#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHttp>

class QProgressBar;
class CHttpProtocol;

class CUpdater : public QObject
{
	Q_OBJECT

	public:
        CUpdater(QProgressBar * pBar = 0, QObject *parent = 0);
        virtual ~CUpdater();

		void checkForUpdate();
        bool isUpToDate() { return m_lastversion; }

	Q_SIGNALS:
		void upToDate(bool, QString);
		void cantCheckForUpdate();

    private:
        void CreateConnection(const QString &hostIP);

    private Q_SLOTS:
        void LookupDone(QHostInfo);

        void connected();
        void disconnected();
        void receivedError(QHttp::Error, QString err);
        void receivedData(QByteArray data);

    private:
        Q_DISABLE_COPY(CUpdater)

	private:
        CHttpProtocol * m_pConnection;
        bool m_lastversion;

        bool m_versionReceived;
        QProgressBar * m_pWtgBar;
};

#endif /* UPDATER_H */
