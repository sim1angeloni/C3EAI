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
#include <QtGui/QMessageBox>
#include "splash.h"
#include "util/protocol/updater.h"
#include "updateWin.h"
#include "programinfo.h"

static const int PROGRESS_VALUE_MIN	= 0;
static const int PROGRESS_VALUE_MAX	= 100;

CSplashScreenDialog::CSplashScreenDialog():
	QDialog(0, Qt::SplashScreen | Qt::WindowStaysOnTopHint),
	m_current_action(eActionUnknown),
	m_pUpdater(NULL)
{
    FUNCTION_LOG

	m_ui.setupUi(this);

	setWindowTitle(QString("%1 %2").arg(PROG_NAME).arg(PROG_VERSION));

	BuildActionsMap();

	m_step = (int)(PROGRESS_VALUE_MAX / m_actions_map.size());
	m_module_diff = PROGRESS_VALUE_MAX % m_actions_map.size();

	m_ui.progressBar->setMinimum(PROGRESS_VALUE_MIN);
	m_ui.progressBar->setMaximum(PROGRESS_VALUE_MAX);
	m_ui.progressBar->setValue(PROGRESS_VALUE_MIN);
}

CSplashScreenDialog::~CSplashScreenDialog()
{
    FUNCTION_LOG

	SAFE_DELETE(m_pUpdater);
}

void CSplashScreenDialog::setAction(EActionLoading action)
{
    FUNCTION_LOG

	m_current_action = action;
	Q_EMIT value_changed();

	m_ui.currentAction->setText(m_actions_map.value(action, QString()));

	int new_value = m_ui.progressBar->value() + m_step;
	m_ui.progressBar->setValue(new_value);

	if (new_value + m_module_diff >= PROGRESS_VALUE_MAX)
		Q_EMIT completed();
}

void CSplashScreenDialog::logAction(EActionLoading action)
{
    if (!gEnv->bDebug && gEnv->pSplashScreen)
    {
        gEnv->pSplashScreen->setAction(action);
    }
}

void CSplashScreenDialog::checkUpdates()
{
    FUNCTION_LOG

	m_pUpdater = new CUpdater();

    bool q_assert = false;

	q_assert = QObject::connect(m_pUpdater, SIGNAL(upToDate(bool, QString)), this, SLOT(UpToDate(bool, QString)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_pUpdater, SIGNAL(cantCheckForUpdate()), this, SLOT(CannotCheckForUpdate()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	setAction(eActionsUpdateCheckStart);
	m_pUpdater->checkForUpdate();
}

void CSplashScreenDialog::BuildActionsMap()
{
    FUNCTION_LOG

	m_actions_map.insert(eActionMainWindow, QApplication::translate("LoadingActions", "eActionMainWindow", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionProfiles, QApplication::translate("LoadingActions", "eActionProfiles", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsWidgetMaps, QApplication::translate("LoadingActions", "eActionsWidgetMaps", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsWidgetPlayers, QApplication::translate("LoadingActions", "eActionsWidgetPlayers", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsWidgetServer, QApplication::translate("LoadingActions", "eActionsWidgetServer", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsWidgetVariables, QApplication::translate("LoadingActions", "eActionsWidgetVariables", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsWidgetOther, QApplication::translate("LoadingActions", "eActionsWidgetOther", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsUpdateCheckStart, QApplication::translate("LoadingActions", "eActionsUpdateCheckStart", 0, QApplication::UnicodeUTF8));
	m_actions_map.insert(eActionsUpdateCheckDone, QApplication::translate("LoadingActions", "eActionsUpdateCheckDone", 0, QApplication::UnicodeUTF8));
}

void CSplashScreenDialog::CannotCheckForUpdate()
{
    FUNCTION_LOG

	QMessageBox::critical(this, TR_DIALOG_ERROR_TITLE, TR_SPLASH_UPDATE_UNABLE);
	setAction(eActionsUpdateCheckDone);
}

void CSplashScreenDialog::UpToDate(bool isUpToDate, QString url)
{
    FUNCTION_LOG

    if (!isUpToDate)
	{
        CUpdateDialog * pUpdateInfo = new CUpdateDialog(url);
        pUpdateInfo->show();
	}

	setAction(eActionsUpdateCheckDone);
}
