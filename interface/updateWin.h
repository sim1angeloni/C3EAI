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

#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QtCore/QObject>
#include <QtGui/QDialog>
#include "ui_dialog_update.h"

class CUpdateDialog : public QDialog
{
	Q_OBJECT

	public:
        CUpdateDialog(QString url, QWidget *parent = 0, Qt::WindowFlags f = 0);
        virtual ~CUpdateDialog();

	private Q_SLOTS:
		void openLink(const QString & link);
		void release();

    private:
        Q_DISABLE_COPY(CUpdateDialog)

	private:
        Ui::dialog_update m_ui;
};

#endif /* UPDATEDIALOG_H */
