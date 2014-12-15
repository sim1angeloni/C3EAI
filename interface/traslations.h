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

#ifndef TRASLATIONS_H
#define TRASLATIONS_H

/* DIALOG WIN TITLES */
#define TR_DIALOG_INFO_TITLE						QApplication::translate("General", "Information", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_WARNING_TITLE						QApplication::translate("General", "Warning", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_TITLE						QApplication::translate("General", "Error", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_QUESTION_TITLE					QApplication::translate("General", "Confirmation request", 0, QApplication::UnicodeUTF8)

/* DIALOG WIN QUESTION BODIES */
#define TR_DIALOG_QUESTION_DELETE_PROFILE			QApplication::translate("General", "Do you really want to delete '%1' profile?", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_QUESTION_SAVE_PROFILE				QApplication::translate("General", "Do you want to save '%1' profile?", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_QUESTION_LOAD_MAP					QApplication::translate("General", "Do you want to load map '%1' with game mode '%2'?", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_QUESTION_RUN_PLAYLIST				QApplication::translate("General", "Do you want to run playlist '%1 - %2'?", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_QUESTION_START_GAME				QApplication::translate("General", "Do you want to send start-game command?", 0, QApplication::UnicodeUTF8)

/* DIALOG WIN ERROR BODIES */
#define TR_DIALOG_ERROR_PROFILE_NOT_SELECTED		QApplication::translate("General", "No profile selected", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_PROFILE_NAME_IN_USE			QApplication::translate("General", "Profile name '%1' already in use", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_IPADDRESS					QApplication::translate("General", "Wrong IP address", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_PORTOVERFLOW				QApplication::translate("General", "%1 value too big", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_PORTZERO					QApplication::translate("General", "%1 is 0 or not a number", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_EMPTYFIELD					QApplication::translate("General", "%1 field is empty", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_AZ09FIELD					QApplication::translate("General", "%1 field is not correct. Characters allowed: [A-z][0-9][_]", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_CANTCONNECT_1				QApplication::translate("General", "Cannot start the connection to server", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_CANTCONNECT_2				QApplication::translate("General", "Cannot connect to server", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_AUTHFAILED					QApplication::translate("General", "Authorization failed", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_NOTCONNECTED				QApplication::translate("General", "Not connected to server", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_CANTSEND					QApplication::translate("General", "Cannot send command '%1' to server", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_UNKNOWNCMD					QApplication::translate("General", "Unknown command '%1'", 0, QApplication::UnicodeUTF8)
#define TR_DIALOG_ERROR_RESERVEDSLOT_UNACTIVE		QApplication::translate("General", "Reserved slot system is currently unactive", 0, QApplication::UnicodeUTF8)

/* SPECIAL FIELDS */
#define TR_FIELD_PROFILE_NAME						QApplication::translate("General", "Profile-name", 0, QApplication::UnicodeUTF8)
#define TR_FIELD_IP_ADDRESS							QApplication::translate("General", "IP-address", 0, QApplication::UnicodeUTF8)
#define TR_FIELD_GAME_PORT							QApplication::translate("General", "Game-port", 0, QApplication::UnicodeUTF8)
#define TR_FIELD_HTTP_PORT							QApplication::translate("General", "HTTP-port", 0, QApplication::UnicodeUTF8)
#define TR_FIELD_PASSWORD							QApplication::translate("General", "Password", 0, QApplication::UnicodeUTF8)

/* SPLASH SCREEN */
#define TR_SPLASH_UPDATE_CHECKING					QApplication::translate("Splash", "Checking for updates...", 0, QApplication::UnicodeUTF8)
#define TR_SPLASH_UPDATE_YES						QApplication::translate("Splash", "No updates available", 0, QApplication::UnicodeUTF8)
#define TR_SPLASH_UPDATE_UNABLE						QApplication::translate("Splash", "Unable to check for updates", 0, QApplication::UnicodeUTF8)
#define TR_SPLASH_UPDATE_DOWNLOAD					QApplication::translate("Splash", "Download now", 0, QApplication::UnicodeUTF8)
#define TR_SPLASH_UPDATE_NO							QApplication::translate("Splash", "There is 1 update available", 0, QApplication::UnicodeUTF8)
#define TR_SPLASH_CONTINUE							QApplication::translate("Splash", "Continue", 0, QApplication::UnicodeUTF8)

/* MAPS LIST */
#define TR_MAP_c3mp_airport							QApplication::translate("Maps", "Airport", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_bridge							QApplication::translate("Maps", "Brooklyn Bridge", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_canyon							QApplication::translate("Maps", "Financial District", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_cave							QApplication::translate("Maps", "Central Cavern", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_con_ed							QApplication::translate("Maps", "East River", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_dam								QApplication::translate("Maps", "Hydro dam", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_fields							QApplication::translate("Maps", "Penn Depot", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_museum							QApplication::translate("Maps", "Museum", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_river							QApplication::translate("Maps", "Hell's Kitchen", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_rooftop_gardens					QApplication::translate("Maps", "Skyline", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_swamp_boat						QApplication::translate("Maps", "Chinatown", 0, QApplication::UnicodeUTF8)
#define TR_MAP_c3mp_tanker							QApplication::translate("Maps", "Williamsburg", 0, QApplication::UnicodeUTF8)

/* MAPS DESCRIPTIONS */
#define TR_D_MAP_c3mp_airport						QApplication::translate("MapsDescriptions", "Airport", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_bridge						QApplication::translate("MapsDescriptions", "Brooklyn Bridge", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_canyon						QApplication::translate("MapsDescriptions", "Financial District", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_cave							QApplication::translate("MapsDescriptions", "Central Cavern", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_con_ed						QApplication::translate("MapsDescriptions", "East River", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_dam							QApplication::translate("MapsDescriptions", "Hydro dam", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_fields						QApplication::translate("MapsDescriptions", "Penn Depot", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_museum						QApplication::translate("MapsDescriptions", "Museum", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_river							QApplication::translate("MapsDescriptions", "Hell's Kitchen", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_rooftop_gardens				QApplication::translate("MapsDescriptions", "Skyline", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_swamp_boat					QApplication::translate("MapsDescriptions", "Chinatown", 0, QApplication::UnicodeUTF8)
#define TR_D_MAP_c3mp_tanker						QApplication::translate("MapsDescriptions", "Williamsburg", 0, QApplication::UnicodeUTF8)

/* GAME MODES LIST*/
#define TR_DIALOG_SELECT_GAMEMODE					QApplication::translate("General", "<Choose game mode>", 0, QApplication::UnicodeUTF8)
#define TR_MODE_CrashSite							QApplication::translate("GameRules", "Crash Site", 0, QApplication::UnicodeUTF8)
#define TR_MODE_CaptureTheRelay						QApplication::translate("GameRules", "Capture the Relay", 0, QApplication::UnicodeUTF8)
#define TR_MODE_TeamDeathMatch						QApplication::translate("GameRules", "Team Deathmatch", 0, QApplication::UnicodeUTF8)
#define TR_MODE_DeathMatch							QApplication::translate("GameRules", "Deathmatch", 0, QApplication::UnicodeUTF8)
#define TR_MODE_Extraction							QApplication::translate("GameRules", "Extraction", 0, QApplication::UnicodeUTF8)
#define TR_MODE_Assault								QApplication::translate("GameRules", "Assault", 0, QApplication::UnicodeUTF8)
#define TR_MODE_Hunter								QApplication::translate("GameRules", "Hunter", 0, QApplication::UnicodeUTF8)
#define TR_MODE_Spears								QApplication::translate("GameRules", "Spears", 0, QApplication::UnicodeUTF8)

/* GAME MODES DESCRIPTIONS */
#define TR_D_MODE_CrashSite							QApplication::translate("GameRulesDescriptions", "Crash Site", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_CaptureTheRelay					QApplication::translate("GameRulesDescriptions", "Capture the Relay", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_TeamDeathMatch					QApplication::translate("GameRulesDescriptions", "Team Deathmatch", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_DeathMatch						QApplication::translate("GameRulesDescriptions", "Deathmatch", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_Extraction						QApplication::translate("GameRulesDescriptions", "Extraction", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_Assault							QApplication::translate("GameRulesDescriptions", "Assault", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_Hunter							QApplication::translate("GameRulesDescriptions", "Hunter", 0, QApplication::UnicodeUTF8)
#define TR_D_MODE_Spears							QApplication::translate("GameRulesDescriptions", "Spears", 0, QApplication::UnicodeUTF8)

/* MODIFIERS LIST*/
#define TR_MOD_Standard								QApplication::translate("Modifiers", "Standard", 0, QApplication::UnicodeUTF8)
#define TR_MOD_Pro									QApplication::translate("Modifiers", "Pro", 0, QApplication::UnicodeUTF8)
#define TR_MOD_Vanilla								QApplication::translate("Modifiers", "Vanilla", 0, QApplication::UnicodeUTF8)
#define TR_MOD_BeginnersPlayground					QApplication::translate("Modifiers", "Beginners Playground", 0, QApplication::UnicodeUTF8)

/* MODIFIERS LIST*/
#define TR_D_MOD_Standard							QApplication::translate("ModifiersDescriptions", "Standard", 0, QApplication::UnicodeUTF8)
#define TR_D_MOD_Pro								QApplication::translate("ModifiersDescriptions", "Pro", 0, QApplication::UnicodeUTF8)
#define TR_D_MOD_Vanilla							QApplication::translate("ModifiersDescriptions", "Vanilla", 0, QApplication::UnicodeUTF8)
#define TR_D_MOD_BeginnersPlayground				QApplication::translate("ModifiersDescriptions", "Beginners Playground", 0, QApplication::UnicodeUTF8)

/* PLAYERS LIST TITLES */
#define TR_PLAYER_NICK								QApplication::translate("Players", "Nickname", 0, QApplication::UnicodeUTF8)
#define TR_PLAYER_ID								QApplication::translate("Players", "Server ID", 0, QApplication::UnicodeUTF8)
#define TR_PLAYER_IP_PORT							QApplication::translate("Players", "IP:PORT", 0, QApplication::UnicodeUTF8)
#define TR_PLAYER_PING								QApplication::translate("Players", "Ping", 0, QApplication::UnicodeUTF8)
#define TR_PLAYER_STATUS							QApplication::translate("Players", "Status", 0, QApplication::UnicodeUTF8)
#define TR_PLAYER_PROFILE							QApplication::translate("Players", "Profile", 0, QApplication::UnicodeUTF8)
#define TR_PLAYER_TIMEREMAINING						QApplication::translate("Players", "Time remaining", 0, QApplication::UnicodeUTF8)

#endif /* TRASLATIONS_H */

