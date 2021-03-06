/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
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

#include "lxqtkbindicator.h"

#include <X11/XKBlib.h>

#include <QtGui/QLabel>
#include <QtGui/QX11Info>
#include <QTimer>

Q_EXPORT_PLUGIN2(panelkbindicator, LxQtKbIndicatorLibrary)

LxQtKbIndicator::LxQtKbIndicator(const ILxQtPanelPluginStartupInfo &startupInfo):
    QObject(),
    ILxQtPanelPlugin(startupInfo),
    mContent(new QLabel())
{
    connect(this, SIGNAL(indicatorsChanged(uint,uint)), this, SLOT(setIndicators(uint,uint)));
    mContent->setAlignment(Qt::AlignCenter);

    int code;
    int major = XkbMajorVersion;
    int minor = XkbMinorVersion;
    int XkbErrorBase;

    mDisplay = QX11Info::display();

    if (XkbLibraryVersion(&major, &minor))
        if (XkbQueryExtension(mDisplay, &code, &mXkbEventBase, &XkbErrorBase, &major, &minor))
            if (XkbUseExtension(mDisplay, &major, &minor))
                XkbSelectEvents(mDisplay, XkbUseCoreKbd, XkbIndicatorStateNotifyMask, XkbIndicatorStateNotifyMask);

    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

LxQtKbIndicator::~LxQtKbIndicator()
{
    delete mContent;
}

void LxQtKbIndicator::delayedInit()
{
    settingsChanged();
    realign();
}

QWidget *LxQtKbIndicator::widget()
{
    return mContent;
}

void LxQtKbIndicator::settingsChanged()
{
    mBit = settings()->value("bit", 0).toInt();
    mContent->setText(settings()->value("text", QString("C")).toString());
    mContent->setEnabled(getLockStatus(mBit));
}

QDialog *LxQtKbIndicator::configureDialog()
{
    return new LxQtKbIndicatorConfiguration(settings());
}

void LxQtKbIndicator::realign()
{
    if (panel()->isHorizontal())
        mContent->setMinimumSize(0, panel()->iconSize());
    else
        mContent->setMinimumSize(panel()->iconSize(), 0);
}

void LxQtKbIndicator::setIndicators(unsigned int changed, unsigned int state)
{
    if (changed & (1 << mBit))
        mContent->setEnabled(state & (1 << mBit));
}

bool LxQtKbIndicator::getLockStatus(int bit)
{
    bool state = false;
    if (mDisplay)
    {
        unsigned n;
        XkbGetIndicatorState(mDisplay, XkbUseCoreKbd, &n);
        state = (n & (1 << bit));
    }
    return state;
}

void LxQtKbIndicator::x11EventFilter(XEvent* event)
{
    XkbEvent* xkbEvent = reinterpret_cast<XkbEvent*>(event);

    if (xkbEvent->type == mXkbEventBase + XkbEventCode)
        if (xkbEvent->any.xkb_type == XkbIndicatorStateNotify)
            emit indicatorsChanged(xkbEvent->indicators.changed, xkbEvent->indicators.state);
}
