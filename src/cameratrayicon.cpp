/***************************************************************************
 *   Copyright (C) 2009 by Cyp   *
 *      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "cameratrayicon.h"
#include <QTimer>
#include <QMenu>
#include <QApplication>
#include <sys/types.h>
#include <dirent.h>


CameraTrayIcon::CameraTrayIcon(QObject *parent)
 : QSystemTrayIcon(parent)
 , theApp(NULL)
 , lastState(-1)
{
    update();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    menu = new QMenu;  // new QMenu(this) doesn't work, since this is not a QWidget...
    menu->addAction("&Quit", this, SLOT(deleteLater()));
    setContextMenu(menu);
}


CameraTrayIcon::~CameraTrayIcon()
{
    menu->deleteLater();
    if(theApp != NULL)
        theApp->quit();
}

void CameraTrayIcon::quitOnDestroy(QApplication *app)
{
    theApp = app;
}

void CameraTrayIcon::update()
{
    int newState = countCameras();
    //newState = (lastState+1)%6;

    if(lastState != newState)
    {
        lastState = newState;
        char const *icon = NULL;
        QString message = NULL;
        switch(newState)
        {
            case 0:
                icon = ":/cameraoff.svg";
                message = "No camera attached";
                break;
            case 1:
                icon = ":/cameraon.svg";
                message = "Camera attached";
                break;
            case 2:
                icon = ":/cameraon2.svg";
                message = "2 cameras attached";
                break;
            case 3:
                icon = ":/cameraon3.svg";
                message = "3 cameras attached";
                break;
            default:
                icon = ":/cameraonp.svg";
                message = QString::number(newState)+" cameras attached";
                break;
        }
        setIcon(QIcon(icon));
        setToolTip(message);
        showMessage("Camera", message, Information, 2000);
    }
}

int CameraTrayIcon::countCameras()
{
    //char const *videoDir = "/tmp/traytest";
    char const *videoDir = "/dev/v4l";
    char const *prefix = "video";

    DIR *dir = opendir(videoDir);
    if(dir == NULL)
        return 0;

    int cameras = 0;
    while(true)
    {
        struct dirent *ent = readdir(dir);
        if(ent == NULL)
            break;

        if(std::memcmp(ent->d_name, prefix, strlen(prefix)) == 0)
            ++cameras;
    }
    closedir(dir);
    return cameras;
}
