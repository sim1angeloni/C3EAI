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

#ifndef ABOUT_H
#define ABOUT_H

#include <QtGui/QDialog>
#include "ui_dialog_about.h"

class CAboutDialog : public QDialog
{
	public:
        CAboutDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
        virtual ~CAboutDialog();

        Ui::About *GetUI() { return &m_ui; }

    private:
        Q_DISABLE_COPY(CAboutDialog)

	private:
        Ui::About m_ui;
};

#endif /* ABOUT_H */
