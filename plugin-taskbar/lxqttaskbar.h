/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *   Maciej Płaza <plaza.maciej@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef LXQTTASKBAR_H
#define LXQTTASKBAR_H

#include "lxqttaskbarconfiguration.h"
#include <QtGui/QFrame>
#include <QtGui/QBoxLayout>
#include <QtCore/QHash>
#include <X11/Xlib.h>
#include "../panel/ilxqtpanel.h"

class LxQtTaskButton;
class ILxQtPanelPlugin;

namespace LxQt {
class GridLayout;
}

class LxQtTaskBar : public QFrame
{
    Q_OBJECT
public:
    explicit LxQtTaskBar(ILxQtPanelPlugin *plugin, QWidget* parent = 0);
    virtual ~LxQtTaskBar();

    virtual void x11EventFilter(XEvent* event);
    virtual void settingsChanged();

    void realign();

public slots:
    void activeWindowChanged();
    void refreshIconGeometry();

private:
    void refreshTaskList();
    void refreshButtonVisibility();
    QHash<Window, LxQtTaskButton*> mButtonsHash;
    LxQt::GridLayout *mLayout;
    LxQtTaskButton* buttonByWindow(Window window) const;
    bool windowOnActiveDesktop(Window window) const;
    Window mRootWindow;
    Qt::ToolButtonStyle mButtonStyle;
    int mButtonWidth;
    void setButtonStyle(Qt::ToolButtonStyle buttonStyle);
    bool mShowOnlyCurrentDesktopTasks;

    void handlePropertyNotify(XPropertyEvent* event);
    void wheelEvent(QWheelEvent* event);
    ILxQtPanelPlugin *mPlugin;
    LxQtTaskButton *mPlaceHolder;
};

#endif // LXQTTASKBAR_H
