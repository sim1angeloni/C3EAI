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
#include "changeMap.h"

#define MAP_ADD(list, name, modes) { list.append(SMap(TR_MAP_##name, #name, ":/maps_screen/" #name ".jpg", TR_D_MAP_##name, modes)); }
#define MODE_ADD(list, type, name) { list.append(SGameMode(TR_MODE_##name, #name, TR_D_MODE_##name, type)); }

CChangeMapWidget::CChangeMapWidget(QWidget* parent, Qt::WindowFlags f):
	QWidget(parent, f)
{
	FUNCTION_LOG

    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionsWidgetMaps);
	m_ui.setupUi(this);

	/* BUILD LISTS */
	BuildGameRulesList();
	BuildMapsList();

	/* POPULATE WIDGETS */
	PopulateGameRules();
	PopulateMaps((EGameMode)0x0, true);
	m_ui.mapDescription->clear();
	m_ui.mapScreenshot->clear();

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.mapSend_bt, SIGNAL(clicked()), this, SLOT(Change_Map()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.gamerulesList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(Selected_GameRule(const QString &)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.mapList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(Selected_Map(const QString &)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

CChangeMapWidget::~CChangeMapWidget()
{
	FUNCTION_LOG
}

void CChangeMapWidget::Change_Map()
{
	FUNCTION_LOG

    m_ui.mapSend_bt->setEnabled(false);

    QListWidgetItem * pMapItem = NULL;
    pMapItem = m_ui.mapList->currentItem();
    if (!pMapItem)
	{
		LOGError("Map not selected");
        m_ui.mapSend_bt->setEnabled(true);
		return;
	}

	if (m_ui.gamerulesList->currentIndex() == 0)
	{
		LOGError("Gamemode not selected");
        m_ui.mapSend_bt->setEnabled(true);
		return;
	}

	QString selectedHumanReadableMode = m_ui.gamerulesList->currentText();
	QString current_gamemode;
	for (int i = 0; i < m_modes.size(); ++i)
	{
		if (QString::compare(m_modes[i].m_human_readable, selectedHumanReadableMode) == 0)
		{
			current_gamemode = m_modes[i].m_name;
			break;
		}
	}

	if (current_gamemode.isNull() || current_gamemode.isEmpty())
	{
		LOGError("Unable to recover gamemode from the list");
        m_ui.mapSend_bt->setEnabled(true);
		return;
	}

    QString selectedHumanReadableMap = pMapItem->text();
	QString current_map;
	for (int i = 0; i < m_maps.size(); ++i)
	{
		if (QString::compare(m_maps[i].m_human_readable, selectedHumanReadableMap) == 0)
		{
			current_map = m_maps[i].m_name;
			break;
		}
	}

	if (current_map.isNull() || current_map.isEmpty())
	{
		LOGError("Unable to recover map from the list");
        m_ui.mapSend_bt->setEnabled(true);
		return;
	}

	Q_EMIT changed_map(current_gamemode, current_map);

    m_ui.mapSend_bt->setEnabled(true);
}

void CChangeMapWidget::BuildGameRulesList()
{
	FUNCTION_LOG

	MODE_ADD(m_modes, eGameMode_Crash, CrashSite);
	MODE_ADD(m_modes, eGameMode_CTR, CaptureTheRelay);
	MODE_ADD(m_modes, eGameMode_TDM, TeamDeathMatch);
	MODE_ADD(m_modes, eGameMode_DM, DeathMatch);
	MODE_ADD(m_modes, eGameMode_Ext, Extraction);
	MODE_ADD(m_modes, eGameMode_Assault, Assault);
	MODE_ADD(m_modes, eGameMode_Hunter, Hunter);
	MODE_ADD(m_modes, eGameMode_Spears, Spears);
}

void CChangeMapWidget::BuildMapsList()
{
	FUNCTION_LOG

	MAP_ADD(m_maps, c3mp_airport, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_bridge, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_canyon, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_cave, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_con_ed, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_dam, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_fields, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_museum, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_river, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_rooftop_gardens, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_swamp_boat, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MAP_ADD(m_maps, c3mp_tanker, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
}

void CChangeMapWidget::PopulateGameRules()
{
	FUNCTION_LOG

	m_ui.gamerulesList->clear();
	m_ui.gamerulesList->addItem(TR_DIALOG_SELECT_GAMEMODE);

	for (int i = 0; i < m_modes.size(); ++i)
	{
		int index = i + 1;

		m_ui.gamerulesList->addItem(m_modes[i].m_human_readable);
		m_ui.gamerulesList->setItemData(index, m_modes[i].m_description, Qt::ToolTipRole);
	}
}

void CChangeMapWidget::PopulateMaps(const EGameMode mode, const bool reset)
{
	FUNCTION_LOG

	m_ui.mapList->clear();
	m_ui.mapDescription->clear();
	m_ui.mapScreenshot->clear();

	if (reset)
		return;

	for (int i = 0; i < m_maps.size(); ++i)
	{
		if (m_maps[i].m_gamemodes.testFlag(mode))
		{
			m_ui.mapList->addItem(m_maps[i].m_human_readable);
		}
	}
}

void CChangeMapWidget::Selected_Map(const QString & map)
{
	FUNCTION_LOG

	for (int i = 0; i < m_maps.size(); ++i)
	{
		if (QString::compare(m_maps[i].m_human_readable, map) == 0)
		{
			m_ui.mapDescription->clear();
			m_ui.mapDescription->setText(m_maps[i].m_description);

			m_ui.mapScreenshot->clear();
			m_ui.mapScreenshot->setPixmap(QPixmap(m_maps[i].m_screenshot));
			break;
		}
	}
}

void CChangeMapWidget::Selected_GameRule(const QString & mode)
{
	FUNCTION_LOG

	if (m_ui.gamerulesList->currentIndex() == 0)
	{
		PopulateMaps((EGameMode)0x0, true);
		return;
	}

	EGameMode game_mode;
	bool game_mode_found = false;
	for (int i = 0; i < m_modes.size(); ++i)
	{
		if (QString::compare(m_modes[i].m_human_readable, mode) == 0)
		{
			game_mode = m_modes[i].m_type;
			game_mode_found = true;
			break;
		}
	}

	if (!game_mode_found)
	{
		LOGError(QString("Gamemode ID for Gamemode '%1' not found").arg(mode))
		return;
	}

	PopulateMaps(game_mode);
}
