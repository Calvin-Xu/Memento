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

#ifndef DEFINITIONWIDGET_H
#define DEFINITIONWIDGET_H

#include <QWidget>

#include "../../dict/entry.h"
#include "../../anki/ankiclient.h"

#include <QWheelEvent>
#include <QMutex>

namespace Ui
{
    class DefinitionWidget;
}

class Definition;

class DefinitionWidget : public QWidget
{
    Q_OBJECT

public:
    DefinitionWidget(AnkiClient *client, QWidget *parent = 0);
    ~DefinitionWidget();

public Q_SLOTS:  
    /**
     * Sets the entries of this widget, deletes the old entry
     */
    void setEntries(const QList<Entry *> *entries);

Q_SIGNALS:
    void definitionHidden();

protected:
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    // Prevents these events from being sent to mpv when widget has focus
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE 
        { event->accept(); }
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE 
        { event->accept(); }
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE 
        { event->accept(); }
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE
        { event->accept(); }

private:
    Ui::DefinitionWidget *m_ui;
    AnkiClient *m_client;

    QList<Definition *> *m_definitions;
    unsigned int m_searchId;
    QMutex m_searchIdMutex;
    QMutex m_entryMutex;

    void clearEntries();
};

#endif // DEFINITIONWIDGET_H