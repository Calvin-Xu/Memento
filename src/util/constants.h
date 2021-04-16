////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2020 Ripose
//
// This file is part of Memento.
//
// Memento is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2 of the License.
//
// Memento is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Memento.  If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define VERSION                         (QString("v0.4.4-alpha"))
#define GITHUB_API_LINK                 (QString("https://api.github.com/repos/ripose-jp/memento/releases"))
#define GITHUB_RELEASES                 (QString("https://github.com/ripose-jp/memento/releases"))

#define SETTINGS_DICTIONARIES           "dictionaries"

#define SETTINGS_SEARCH                 "search"
#define SETTINGS_SEARCH_LIMIT           "limit"
#define SETTINGS_SEARCH_METHOD          "method"
#define SETTINGS_SEARCH_DELAY           "delay"
#define SETTINGS_SEARCH_MODIFIER        "modifier"
#define SETTINGS_SEARCH_HIDE_BAR        "hide-bar"
#define SETTINGS_SEARCH_REPLACE_LINES   "replace-lines"
#define SETTINGS_SERACH_REPLACE_WITH    "replace-with"

#define MODIFIER_ALT                    "Alt"
#define MODIFIER_CTRL                   "Control"
#define MODIFIER_SHIFT                  "Shift"
#define MODIFIER_SUPER                  "Super"

#define SEARCH_METHOD_HOVER             "Hover"
#define SEARCH_METHOD_MODIFIER          "Modifier"

#define DEFAULT_LIMIT                   10
#define DEFAULT_METHOD                  SEARCH_METHOD_HOVER
#define DEFAULT_DELAY                   250
#define DEFAULT_MODIFIER                MODIFIER_SHIFT
#define DEFAULT_HIDE_BAR                true
#define DEFAULT_REPLACE_LINES           false
#define DEFAULT_REPLACE_WITH            ""

enum class Theme
{
    System       = 0,
    Light        = 1,
    Dark         = 2
};

#define SETTINGS_INTERFACE                                  "interface"

#define SETTINGS_INTERFACE_THEME                            "theme"
#define SETTINGS_INTERFACE_THEME_DEFAULT                    Theme::System

#define SETTINGS_INTERFACE_STYLESHEETS                      "stylesheets-enabled"
#define SETTINGS_INTERFACE_STYLESHEETS_DEFAULT              false

#define SETTINGS_INTERFACE_SUB_OFFSET                       "sub-offset-percent"
#define SETTINGS_INTERFACE_SUB_OFFSET_DEFAULT               0.0

#define SETTINGS_INTERFACE_SUBTITLE_LIST_STYLE              "sublist-style"
#define SETTINGS_INTERFACE_SUBTITLE_LIST_STYLE_DEFAULT      \
    "QListWidget {\n"\
    "    background: black;\n"\
    "    color: white;\n"\
    "    font-family: \"Noto Sans\", \"Noto Sans CJK JP\", sans-serif;\n"\
    "    font-size: 14pt;\n"\
    "}"

#define SETTINGS_INTERFACE_PLAYER_SPLITTER_STYLE            "player-splitter-style"
#define SETTINGS_INTERFACE_PLAYER_SPLITTER_STYLE_DEFAULT    \
    "QSplitter::handle {\n"\
    "    background: black;\n"\
    "}"

#define SETTINGS_INTERFACE_DEFINITION_STYLE                 "definition-style"
#define SETTINGS_INTERFACE_DEFINITION_STYLE_DEFAULT         \
    "QLabel {\n"\
    "    font-family: \"Noto Sans\", \"Noto Sans CJK JP\", sans-serif;\n"\
    "    font-size: 11pt;\n"\
    "}"

#endif // CONSTANTS_H