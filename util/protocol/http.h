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

#ifndef HTTP_H
#define HTTP_H

#include <QtNetwork/QHttp>
#include <QtNetwork/QHttpResponseHeader>
#include <QtNetwork/QHttpRequestHeader>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkProxy>

class CHttpProtocol : public QObject
{
	Q_OBJECT

	public:
		enum HttpMethod { GET, POST };

        CHttpProtocol(const QString &host, const quint16 port, const QString &path, const QString &content_type, const HttpMethod method, QObject *parent = 0);
        virtual ~CHttpProtocol(void);

		void connect();
		void disconnect();
		void send(const QByteArray &content);
		void send();
		void get(const QString &content);

	Q_SIGNALS:
		void connected();
		void disconnected();
		void receivedHTTPError(QHttp::Error, QString);
		void receivedHTTPData(QByteArray);

	private Q_SLOTS:
		void timeout();
		void done(bool error);
        void proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * pAuthenticator);
		void readyRead(const QHttpResponseHeader & resp);
		void requestFinished(int id, bool error);
		void requestStarted(int id);
		void responseHeaderReceived(const QHttpResponseHeader & resp);
		void stateChanged(int state);

    private:
        Q_DISABLE_COPY(CHttpProtocol)

        void resetTimerTimeout();
        QHttpRequestHeader buildRequestHeader(const int content_size);

	private:
		typedef int iD;

    private:
        QString m_host;
        quint16 m_port;
        QString m_path;
        QString m_contentType;
        HttpMethod m_method;

        QHttp * m_pConnection;

        iD m_startconnID;
        iD m_endconnID;

        int m_timeoutCounter;
};

#endif /* HTTP_H */
