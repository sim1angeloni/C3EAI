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
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include "updateWin.h"
#include "programinfo.h"

CUpdateDialog::CUpdateDialog(QString url, QWidget *parent, Qt::WindowFlags f):
	QDialog(parent, f)
{
    FUNCTION_LOG

	m_ui.setupUi(this);

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.text, SIGNAL(linkActivated(const QString&)), this, SLOT(openLink(const QString &)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.ok_bt, SIGNAL(clicked()), this, SLOT(accept()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(this, SIGNAL(accepted()), this, SLOT(release()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(this, SIGNAL(rejected()), this, SLOT(release()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	m_ui.text->setText(m_ui.text->text().arg(PROG_NAME).arg(url));
}

CUpdateDialog::~CUpdateDialog()
{
    FUNCTION_LOG
}

void CUpdateDialog::openLink(const QString & link)
{
    FUNCTION_LOG
	QDesktopServices::openUrl(link);
}

void CUpdateDialog::release()
{
	delete this;
}
