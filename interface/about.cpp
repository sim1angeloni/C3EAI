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
#include "about.h"
#include "programinfo.h"

CAboutDialog::CAboutDialog(QWidget *parent, Qt::WindowFlags f):
	QDialog(parent, f)
{
	FUNCTION_LOG

	m_ui.setupUi(this);

	setWindowTitle(QString("About %1").arg(PROG_NAME));
	m_ui.version_info->setText(m_ui.version_info->text().arg(PROG_VERSION).arg(PROG_ORGANIZATION).arg(QDateTime::fromTime_t(PROG_RELEASE_DATE).toString("MMMM, dd yyyy")));
	m_ui.copyright_info->setText(m_ui.copyright_info->text().arg(PROG_ORGANIZATION));

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.close_bt, SIGNAL(clicked()), this, SLOT(accept()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CAboutDialog::~CAboutDialog()
{
	FUNCTION_LOG
}
