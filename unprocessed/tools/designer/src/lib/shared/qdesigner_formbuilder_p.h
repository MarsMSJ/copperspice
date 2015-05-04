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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDESIGNER_FORMBUILDER_H
#define QDESIGNER_FORMBUILDER_H

#include "shared_global_p.h"
#include "deviceprofile_p.h"

#include <QtDesigner/private/formscriptrunner_p.h>
#include <QtDesigner/formbuilder.h>

#include <QtCore/QMap>
#include <QtCore/QSet>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;

class QPixmap;
class QtResourceSet;

namespace qdesigner_internal {

class DesignerPixmapCache;
class DesignerIconCache;

/* Form builder used for previewing forms and widget box.
 * It applies the system settings to its toplevel window. */

class QDESIGNER_SHARED_EXPORT QDesignerFormBuilder: public QFormBuilder
{
public:
    enum Mode {
        DisableScripts,
        EnableScripts
    };

    QDesignerFormBuilder(QDesignerFormEditorInterface *core,
                         Mode mode,
                         const DeviceProfile &deviceProfile = DeviceProfile());

    QWidget *createWidgetFromContents(const QString &contents, QWidget *parentWidget = 0);

    virtual QWidget *createWidget(DomWidget *ui_widget, QWidget *parentWidget = 0)
    { return QFormBuilder::create(ui_widget, parentWidget); }

    inline QDesignerFormEditorInterface *core() const
    { return m_core; }

    QString systemStyle() const;

    typedef QFormScriptRunner::Errors ScriptErrors;
    // Create a preview widget (for integrations) or return 0. The widget has to be embedded into a main window.
    // Experimental, depending on script support.
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName /* ="" */,
                                  const QString &appStyleSheet  /* ="" */,
                                  const DeviceProfile &deviceProfile,
                                  ScriptErrors *scriptErrors, QString *errorMessage);
    // Convenience that pops up message boxes in case of failures.
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName = QString());
    //  Create a preview widget (for integrations) or return 0. The widget has to be embedded into a main window.
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName, const QString &appStyleSheet, QString *errorMessage);
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName, const QString &appStyleSheet, const DeviceProfile &deviceProfile, QString *errorMessage);
    // Convenience that pops up message boxes in case of failures.
    static QWidget *createPreview(const QDesignerFormWindowInterface *fw, const QString &styleName, const QString &appStyleSheet);

    // Create a preview image
    static QPixmap createPreviewPixmap(const QDesignerFormWindowInterface *fw, const QString &styleName = QString(), const QString &appStyleSheet = QString());

protected:
    using QFormBuilder::createDom;
    using QFormBuilder::create;

    virtual QWidget *create(DomUI *ui, QWidget *parentWidget);
    virtual DomWidget *createDom(QWidget *widget, DomWidget *ui_parentWidget, bool recursive = true);
    virtual QWidget *create(DomWidget *ui_widget, QWidget *parentWidget);
    virtual QLayout *create(DomLayout *ui_layout, QLayout *layout, QWidget *parentWidget);
    virtual void createResources(DomResources *resources);

    virtual QWidget *createWidget(const QString &widgetName, QWidget *parentWidget, const QString &name);
    virtual bool addItem(DomWidget *ui_widget, QWidget *widget, QWidget *parentWidget);
    virtual bool addItem(DomLayoutItem *ui_item, QLayoutItem *item, QLayout *layout);

    virtual QIcon nameToIcon(const QString &filePath, const QString &qrcPath);
    virtual QPixmap nameToPixmap(const QString &filePath, const QString &qrcPath);

    virtual void applyProperties(QObject *o, const QList<DomProperty*> &properties);

    virtual void loadExtraInfo(DomWidget *ui_widget, QWidget *widget, QWidget *parentWidget);

    QtResourceSet *internalResourceSet() const { return m_tempResourceSet; }

    DeviceProfile deviceProfile() const { return m_deviceProfile; }

private:
    QDesignerFormEditorInterface *m_core;
    const Mode m_mode;

    typedef QSet<QWidget *> WidgetSet;
    WidgetSet m_customWidgetsWithScript;

    const DeviceProfile m_deviceProfile;

    DesignerPixmapCache *m_pixmapCache;
    DesignerIconCache *m_iconCache;
    bool m_ignoreCreateResources;
    QtResourceSet *m_tempResourceSet;
    bool m_mainWidget;
};

// Form builder for a new form widget (preview). To allow for promoted
// widgets in the template, it implements the handling of custom widgets
// (adding of them to the widget database).

class QDESIGNER_SHARED_EXPORT NewFormWidgetFormBuilder: public QDesignerFormBuilder {
public:
    NewFormWidgetFormBuilder(QDesignerFormEditorInterface *core,
                             Mode mode,
                             const DeviceProfile &deviceProfile = DeviceProfile());

protected:
    virtual void createCustomWidgets(DomCustomWidgets *);
};


} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_FORMBUILDER_H
