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
#include <QtCore/QDateTime>
#include "xmlRpc.h"

CXmlRpcProtocol::CXmlRpcProtocol(const QString &host, const quint16 port, QObject *parent):
	CHttpProtocol(host, port, "/RPC2", "text/xml", CHttpProtocol::POST)
{
    Q_UNUSED(parent)

	FUNCTION_LOG

	bool q_assert = false;

	q_assert = QObject::connect(this, SIGNAL(receivedHTTPData(QByteArray)), this, SLOT(receivedData(QByteArray)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(this, SIGNAL(receivedHTTPError(QHttp::Error, QString)), this, SLOT(receivedError(QHttp::Error, QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CXmlRpcProtocol::~CXmlRpcProtocol()
{
	FUNCTION_LOG
}

void CXmlRpcProtocol::execute(const QString &method, const QVariantList &params)
{
	FUNCTION_LOG
	send(toXmlRpcRequest(method, params));
}

void CXmlRpcProtocol::execute(const QString &method)
{
	FUNCTION_LOG
	send(toXmlRpcRequest(method, QVariantList()));
}

void CXmlRpcProtocol::receivedError(QHttp::Error, QString err)
{
	FUNCTION_LOG
	Q_EMIT receivedXmlRpcError(QString("HTTP: %1").arg(err));
}

void CXmlRpcProtocol::receivedData(QByteArray data)
{
	FUNCTION_LOG

	QString err;
	QVariant value = fromXmlRpcResponse(QString(data), err);

	if (err.isEmpty() && value.isValid())
		Q_EMIT receivedXmlRpcData(value.toString());
	else
	if (err.isEmpty() && !value.isValid())
		Q_EMIT receivedXmlRpcError(QString("XML/RPC: %1").arg("Invalid response"));
	else
		Q_EMIT receivedXmlRpcError(QString("XML/RPC: %1").arg(err.simplified()));
}

QByteArray CXmlRpcProtocol::toXmlRpcRequest(const QString m, const QVariantList& l)
{
	FUNCTION_LOG

    QByteArray r("<?xml version=\"1.0\" encoding=\"UTF-8\"?><methodCall>");

    r.append("<methodName>");
    r.append(m);
    r.append("</methodName>");

	if (l.isEmpty())
	{
        r.append("<params/>");
	}
	else
	{
        r.append("<params>");

        for (int i = 0; i < l.size(); i++)
		{
            r.append("<param>");
            toXmlRpcValue(6, l.at(i), r);
            r.append("</param>");
		}

        r.append("</params>");
	}

    r.append("</methodCall>");

	return r;
}

void CXmlRpcProtocol::toXmlRpcValue(const int spaces, const QVariant & child, QByteArray & b)
{
	FUNCTION_LOG

	switch(child.type())
	{
		case QVariant::Int:
            b.append("<value><int>" + QString::number(child.toInt()) + "</int></value>");
			break;
		case QVariant::Bool:
            b.append(QString("<value><boolean>") + (child.toBool() ? "1" : "0") + "</boolean></value>");
			break;
		case QVariant::String:
            b.append("<value><string>" + child.toString() + "</string></value>");
			break;
		case QVariant::Double:
            b.append("<value><double>" + QString::number(child.toDouble()) + "</double></value>");
			break;
		case QVariant::DateTime:
            b.append("<value><dateTime.iso8601>" + child.toDateTime().toString("yyyyMMddTHH:mm:ss") + "</dateTime.iso8601></value>");
			break;
		case QVariant::ByteArray:
            b.append("<value><base64>" + child.toByteArray().toBase64() + "</base64></value>");
			break;
		case QVariant::List:
            b.append("<array><data>");
            toXmlRpcArray(spaces + 2, child.toList(), b);
            b.append("</data></array>");
			break;
		case QVariant::Map:
            b.append("<value><struct>");
            toXmlRpcStruct(spaces + 2, child.toMap(), b);
            b.append("</struct></value>");
			break;
		default:
			LOGFatalErrorANDExit(QString("Unknown return xmlrpc type=").arg(child.typeName()))
	}
}

void CXmlRpcProtocol::toXmlRpcArray(const int spaces, const QVariantList & child, QByteArray & b)
{
	FUNCTION_LOG

	QListIterator< QVariant > i(child);

    while(i.hasNext())
        toXmlRpcValue(spaces + 2, i.next(), b);
}

void CXmlRpcProtocol::toXmlRpcStruct(const int spaces, const QVariantMap & child, QByteArray & b)
{
	FUNCTION_LOG

	QMapIterator< QString, QVariant > i(child);

    while(i.hasNext())
	{
		i.next();
        b.append("<member>");
        b.append("<name>" + i.key() + "</name>");
        toXmlRpcValue(spaces + 2, i.value(), b);
        b.append("</member>");
	}
}

QVariant CXmlRpcProtocol::fromXmlRpcResponse(const QString d, QString &err)
{
	FUNCTION_LOG

	QVariant res = QVariant::Invalid;

	QDomDocument doc;
    if (doc.setContent(d, &err))
	{
		QDomElement methodResponse = doc.firstChildElement("methodResponse");
        if (methodResponse.isNull())
		{
			err = "Element 'methodResponse' is absent in response";
		}
		else
		{
			QDomElement result = methodResponse.firstChildElement();
            if (result.isNull())
			{
				err = "Element 'params' is absent in response";
				return QVariant::Invalid;
			}
            else if (result.tagName() == "params")
			{
				QDomElement param = result.firstChild().firstChild().toElement();
                res = parseXmlRpcValue(param, err);
			}
			else
			{
				err = result.text();
			}
		}
	}

	return res;
}

QVariant CXmlRpcProtocol::parseXmlRpcValue(const QDomElement & e, QString& err)
{
	FUNCTION_LOG

	QVariant v = QVariant::Invalid;

	QString tagName = e.tagName();
    if(tagName != "value")
	{
		err = "first param tag is not value";
		return v;
	}

	QDomElement t = e.firstChild().toElement();
	QString type = t.tagName();

    if(type == "int" || type == "i4")
	{
		bool ok;
        v = t.firstChild().toText().data().toInt(&ok);
        if(!ok)
			err = "Can't convert int text '" + t.firstChild().toText().data() + "' to number";
	}
    else
    if(type == "boolean")
		v = t.firstChild().toText().data() == "1" ? true : false;
    else
    if(type == "string")
		v = t.firstChild().toText().data();
    else
    if(type == "double")
	{
		bool ok;
        v = t.firstChild().toText().data().toDouble(&ok);
        if(!ok)
		{
			err = "Can't convert int text '" + t.firstChild().toText().data() + "' to number";
		}
	}
    else
    if(type == "dateTime.iso8601")
        v = QDateTime::fromString(t.firstChild().toText().data(), "yyyyMMddTHH:mm:ss");
    else
    if(type == "base64")
        v = QByteArray::fromBase64(t.firstChild().toText().data().toLatin1());
    else
    if (type == "array")
        v = parseXmlRpcArray(t.firstChild().toElement(), err);
    else
    if (type == "struct")
        v = parseXmlRpcStruct(t.firstChild().toElement() , err);
    else
    if (type.length() == 0)
		v = e.toElement().firstChild().toText().data();
    else
		err = "unknown type: '" + type + "'";

	return v;
}


QVariant CXmlRpcProtocol::parseXmlRpcStruct(const QDomElement &e, QString &err)
{
	FUNCTION_LOG

	QVariantMap r;
	QDomElement t = e;

    while(!t.isNull())
    {
        if(t.tagName() != "member")
        {
			err = "no member tag in struct, tag " + e.tagName();
			return r;
		}

		QDomElement s = t.firstChild().toElement();

        if(s.tagName() != "name")
        {
			err = "no name tag in member struct, tag " + s.tagName();
			return r;
		}

		// set map value
        r[ s.firstChild().toText().data() ] = parseXmlRpcValue(s.nextSibling().toElement(), err);

        if(!err.isEmpty())
			return r;

		t = t.nextSibling().toElement();
	}

	return r;
}


QVariant CXmlRpcProtocol::parseXmlRpcArray(const QDomElement &e, QString &err)
{
	FUNCTION_LOG

	QVariantList r;
    if(e.tagName() != "data")
    {
		err = "no data tag in array, tag " + e.tagName();
		return r;
	}

	QDomElement t = e.firstChild().toElement();

    while(!t.isNull())
    {
        r.append(parseXmlRpcValue(t,err));
        if(!err.isEmpty())
			return r;

		t = t.nextSibling().toElement();
	}

	return r;
}
