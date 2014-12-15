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

#ifndef XMLRPC_H
#define XMLRPC_H

#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>
#include <QtXml/QDomDocument>
#include "http.h"

class CXmlRpcProtocol : public CHttpProtocol
{
	Q_OBJECT

	public:
        CXmlRpcProtocol(const QString &host, const quint16 port, QObject *parent = 0);
        virtual ~CXmlRpcProtocol();

		void execute(const QString &method, const QVariantList &params);
		void execute(const QString &method);

	Q_SIGNALS:
		void receivedXmlRpcError(QString);
		void receivedXmlRpcData(QString);

	private Q_SLOTS:
		void receivedError(QHttp::Error, QString err);
		void receivedData(QByteArray data);

    private:
        Q_DISABLE_COPY(CXmlRpcProtocol)

	private:
		QByteArray toXmlRpcRequest(const QString m, const QVariantList& l);
		void toXmlRpcValue(const int spaces, const QVariant & child, QByteArray & b);
		void toXmlRpcArray(const int spaces, const QVariantList & child, QByteArray & b);
		void toXmlRpcStruct(const int spaces, const QVariantMap & child, QByteArray & b);

		QVariant fromXmlRpcResponse(const QString d, QString &err);
		QVariant parseXmlRpcValue(const QDomElement & e, QString& err);
		QVariant parseXmlRpcStruct(const QDomElement &e, QString &err);
		QVariant parseXmlRpcArray(const QDomElement &e, QString &err);
};

#endif /* XMLRPC_H */
