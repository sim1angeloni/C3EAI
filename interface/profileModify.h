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

#ifndef PROFILEMODIFY_H
#define PROFILEMODIFY_H

#include <QtCore/QObject>
#include <QtGui/QDialog>
#include "ui_dialog_profilemodify.h"
#include "util/profiles/profilesDB.h"

class CProfileModifyFrontend : public QDialog
{
	Q_OBJECT

	public:
        CProfileModifyFrontend(QWidget *parent = 0, Qt::WindowFlags f = 0);
        virtual ~CProfileModifyFrontend();

		void setProfile(CProfilesDatabase::SProfile profile);
		CProfilesDatabase::SProfile getProfile();

	private Q_SLOTS:
		void Buttons_Clicked(QAbstractButton * button);
		void DisplayPassword();

    private:
        Q_DISABLE_COPY(CProfileModifyFrontend)

    private:
        void TryModify(QAbstractButton * pButton, bool & passed);

	private:
        Ui::dialog_profilemodify m_ui;
};

#endif /* PROFILEMODIFY_H */
