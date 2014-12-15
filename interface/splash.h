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

#ifndef SPLASH_H
#define SPLASH_H

#include <QtCore/QObject>
#include <QtGui/QDialog>
#include "ui_window_splash.h"

class CUpdater;

class CSplashScreenDialog : public QDialog
{
	Q_OBJECT

	public:
		enum EActionLoading
		{
			eActionMainWindow,
			eActionProfiles,
			eActionsWidgetMaps,
			eActionsWidgetPlayers,
			eActionsWidgetServer,
			eActionsWidgetVariables,
			eActionsWidgetOther,
			eActionsUpdateCheckStart,
			eActionsUpdateCheckDone,

			eActionUnknown,
		};

        CSplashScreenDialog();
        virtual ~CSplashScreenDialog();

        Ui::Splash *GetUI() { return &m_ui; }
		
		void checkUpdates();
        void setAction(EActionLoading action);
        EActionLoading getCurrentAction() { return m_current_action; }

        static void logAction(EActionLoading action);

	Q_SIGNALS:
		void value_changed();
		void completed();

	private Q_SLOTS:
        void UpToDate(bool isUpToDate, QString url);
		void CannotCheckForUpdate();

    private:
        void BuildActionsMap();

    private:
        Q_DISABLE_COPY(CSplashScreenDialog)

	private:
        Ui::Splash m_ui;

        EActionLoading m_current_action;

        QMap<EActionLoading, QString> m_actions_map;
        int m_step;
        int m_module_diff;

        CUpdater * m_pUpdater;
};

#endif /*SPLASH_H */
