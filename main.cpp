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
#include <QtNetwork/QHttp>
#include <QtCore/QTranslator>
#include "programinfo.h"
#include "util/logger/logCommon.h"

/* LIBRARIES INCLUDE FOR gEnv */
#include "util/logger/logSystem.h"
#include "util/settings/settings.h"
#include "util/generic/ipUtils.h"
SSystemGlobalEnvironment *gEnv = NULL;

/* WINDOWS */
#include "interface/window.h"
#include "interface/splash.h"
#include "interface/server.h"

/**
 * This takes care to handle the logs coming from Qt.
 * @parar type Message type (debug, critical, warning, ...).
 * @paran msg Message body.
 */
void myMessageDebugOutput(QtMsgType type, const char *msg)
{
	LOGQt(QString("Type:'%1', Message:'%2'").arg((int)type).arg(msg));
}

/**
 * Create global environment and attach it to gEnv global pointer.
 */
void MakeGEnv()
{
	gEnv = new SSystemGlobalEnvironment();
	gEnv->pLog = new CLogSystem();
	gEnv->pSettings = new CSettingsFile();
	gEnv->pUtils->pIPStringUtils = new CIPUtils();

	if (!gEnv->bDebug)
	{
		gEnv->pSplashScreen = new CSplashScreenDialog();
	}

	gEnv->pLog->setLogTypes(gEnv->pSettings->getValueString(SETTING_LOGTYPES));
	gEnv->pLog->setLogOutputs(gEnv->pSettings->getValueString(SETTING_LOGOUTPUT));
}

/**
 * Main program.
 * You should know what it does :)
 */
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROG_NAME_SHORT);
    QCoreApplication::setOrganizationName(PROG_ORGANIZATION);

    qRegisterMetaType<QHttp::Error> ("QHttp::Error");
    qRegisterMetaType<CServerWidget::SServer> ("CServerWidget::SServer");

    qInstallMsgHandler(myMessageDebugOutput);

    QTranslator translator;
	translator.load(QString(":/translations.qm"));
	app.installTranslator(&translator);

    MakeGEnv();

	if (!gEnv->bDebug)
		gEnv->pSplashScreen->show();

    CMainWindow mainWin;

	if (!gEnv->bDebug)
		gEnv->pSplashScreen->checkUpdates();

	if (gEnv->bDebug)
		mainWin.show();

	return app.exec();
}
