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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHttp>

class QProgressBar;
class CHttpProtocol;

class CDownloader : public QObject
{
	Q_OBJECT

	public:
        CDownloader(QProgressBar * pBar, QObject *parent = 0);
        virtual ~CDownloader();

		void download(QString file_name, quint32 port);
        bool isUpToDate() { return m_lastversion; }

	Q_SIGNALS:
		void upToDate(bool, QString);
		void cantDownload();

    private:
		void CreateConnection(const QString &hostIP);

	private Q_SLOTS:
		void LookupDone(QHostInfo);

		void connected();
		void disconnected();
		void receivedError(QHttp::Error, QString err);
		void receivedData(QByteArray data);

    private:
        Q_DISABLE_COPY(CDownloader)

    private:
        CHttpProtocol * m_pConnection;
        bool m_lastversion;

        bool m_versionReceived;
        QProgressBar * m_pWdgBar;

        quint32 m_port;
        QString m_server_name;
        QString m_filepath_name;
};

#endif /* DOWNLOADER_H */
