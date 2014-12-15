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
#include "other.h"

#define MODE_ADD(list, type, name) { list.append(SGameMode(TR_MODE_##name, #name, TR_D_MODE_##name, type)); }
#define MOD_ADD(list, name, modes) { list.append(SModifier(TR_MOD_##name, #name, TR_D_MOD_##name, modes)); }

COtherWidget::COtherWidget(QWidget* parent, Qt::WindowFlags f):
	QWidget(parent, f)
{
	FUNCTION_LOG

    CSplashScreenDialog::logAction(CSplashScreenDialog::eActionsWidgetOther);
	m_ui.setupUi(this);

	/* BUILD LISTS */
	BuildGameRulesList();
	BuildModifiersList();

	/* POPULATE WIDGETS */
	PopulateGameRules();
	PopulateModifiers((EGameMode)0x0, true);

	bool q_assert = false;

	q_assert = QObject::connect(m_ui.gl_startgame, SIGNAL(clicked()), this, SLOT(Generic_Command_Clicked()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.gamerulesList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(Selected_GameRule(const QString &)), Qt::QueuedConnection);
	Q_ASSERT(q_assert);

	q_assert = QObject::connect(m_ui.startplaylist, SIGNAL(clicked()), this, SLOT(Change_PlayList()), Qt::QueuedConnection);
	Q_ASSERT(q_assert);
}

COtherWidget::~COtherWidget()
{
	FUNCTION_LOG
}

void COtherWidget::Generic_Command_Clicked()
{
	FUNCTION_LOG

    QPushButton * pButton = qobject_cast<QPushButton *>(QObject::sender());
    if (!pButton)
	{
		LOGError("Incorrect sender");
		return;
	}

    QString command_name = pButton->objectName();
	if (command_name.isEmpty())
	{
		LOGError("Empty command");
		return;
	}

	Q_EMIT command_used(command_name);
}

void COtherWidget::BuildGameRulesList()
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

void COtherWidget::BuildModifiersList()
{
	FUNCTION_LOG

	MOD_ADD(m_modifiers, Standard, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MOD_ADD(m_modifiers, Pro, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
	MOD_ADD(m_modifiers, Vanilla, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Spears);
	MOD_ADD(m_modifiers, BeginnersPlayground, eGameMode_Crash | eGameMode_CTR | eGameMode_TDM | eGameMode_DM | eGameMode_Ext | eGameMode_Assault | eGameMode_Hunter | eGameMode_Spears);
}

void COtherWidget::PopulateGameRules()
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

void COtherWidget::PopulateModifiers(const EGameMode mode, const bool reset)
{
	FUNCTION_LOG

	m_ui.modifiersList->clear();

	if (reset)
		return;

	for (int i = 0; i < m_modifiers.size(); ++i)
	{
		if (m_modifiers[i].m_gamemodes.testFlag(mode))
		{
			// The items will be deleted automatically using "clear()" (no memory leak)
            QListWidgetItem * pItem = new QListWidgetItem(m_modifiers[i].m_human_readable, m_ui.modifiersList);
            pItem->setToolTip(m_modifiers[i].m_description);
		}
	}
}

void COtherWidget::Selected_GameRule(const QString & mode)
{
	FUNCTION_LOG

	if (m_ui.gamerulesList->currentIndex() == 0)
	{
		PopulateModifiers((EGameMode)0x0, true);
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

	PopulateModifiers(game_mode);
}

void COtherWidget::Change_PlayList()
{
	FUNCTION_LOG

    m_ui.startplaylist->setEnabled(false);

    QListWidgetItem * pModItem = NULL;
    pModItem = m_ui.modifiersList->currentItem();
    if (!pModItem)
	{
		LOGError("Modifier not selected");
        m_ui.startplaylist->setEnabled(true);
		return;
	}

	if (m_ui.gamerulesList->currentIndex() == 0)
	{
		LOGError("Gamemode not selected");
        m_ui.startplaylist->setEnabled(true);
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
        m_ui.startplaylist->setEnabled(true);
		return;
	}

    QString selectedHumanReadableMod = pModItem->text();
	QString current_mod;
	for (int i = 0; i < m_modifiers.size(); ++i)
	{
		if (QString::compare(m_modifiers[i].m_human_readable, selectedHumanReadableMod) == 0)
		{
			current_mod = m_modifiers[i].m_name;
			break;
		}
	}

	if (current_mod.isNull() || current_mod.isEmpty())
	{
		LOGError("Unable to recover modifier from the list");
        m_ui.startplaylist->setEnabled(true);
		return;
	}

	QString command_name = m_ui.startplaylist->objectName();
	if (command_name.isEmpty())
	{
		LOGError("Empty command");
		return;
	}

	Q_EMIT command_used(QString("%1 %2__%3").arg(command_name).arg(current_gamemode).arg(current_mod));

    m_ui.startplaylist->setEnabled(true);
}
