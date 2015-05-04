/***********************************************************************
*
* Copyright (c) 2012-2015 Barbara Geller
* Copyright (c) 2012-2015 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or 
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see 
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QWAYLANDEGLINTEGRATION_H
#define QWAYLANDEGLINTEGRATION_H

#include "gl_integration/qwaylandglintegration.h"

#include "qwaylandeglinclude.h"

class QWaylandWindow;
class QWidget;

class QWaylandEglIntegration : public QWaylandGLIntegration
{
public:
    QWaylandEglIntegration(struct wl_display *waylandDisplay);
    ~QWaylandEglIntegration();

    void initialize();

    QWaylandWindow *createEglWindow(QWidget *window);

    EGLDisplay eglDisplay() const;
    struct wl_egl_display *nativeDisplay() const;
private:
    struct wl_display *mWaylandDisplay;

    EGLDisplay mEglDisplay;
};

#endif // QWAYLANDEGLINTEGRATION_H
