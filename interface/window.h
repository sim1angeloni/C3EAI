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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QObject>
#include <QtGui/QMainWindow>
#include "ui_window_main.h"
#include "util/profiles/profilesDB.h"

class CProfilesModel;

class CMainWindow : public QMainWindow
{
	Q_OBJECT
	public:
        CMainWindow();
        virtual ~CMainWindow();

        Ui::MainWindow *GetUI() { return &m_ui; }

    private Q_SLOTS:
        void Program_Close();
        void Program_Info();
        void Program_Connect();
        void Program_Connected();
        void Program_Disconnected();
        void Program_Loaded();
        void Programm_DisplayPassword();

        void Profile_Save();
        void Profile_Modify();
        void Profile_Remove();
        void Profile_DoubleClicked(const QModelIndex &index);
        void Profile_Load();

    private:
        void EnableDisableTabs(bool isConnected);

        void BuildBProfilesList();
        void Profile_Load(const QModelIndex &index);

    private:
        Q_DISABLE_COPY(CMainWindow)
		
	private:
        Ui::MainWindow m_ui;

        CProfilesModel * m_pProfilesModel;
        CProfilesDatabase m_profilesDB;
};

#endif /* MAINWINDOW_H */
