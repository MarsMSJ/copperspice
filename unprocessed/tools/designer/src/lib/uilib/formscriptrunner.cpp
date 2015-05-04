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

#include "formscriptrunner_p.h"
#include "formbuilderextra_p.h"
#include "ui4_p.h"

#include <QtScript/QScriptEngine>
#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

QT_BEGIN_NAMESPACE

namespace {
    enum { debugFormScriptRunner = 0 };
}

#ifdef QFORMINTERNAL_NAMESPACE
namespace QFormInternal {
#endif

class  QFormScriptRunner::QFormScriptRunnerPrivate {
public:
    QFormScriptRunnerPrivate() : m_options(DisableScripts) {}
    void clearErrors() { m_errors.clear(); }

    bool run(const QString &script, QWidget *widget, const WidgetList &children, QString *errorMessage);

    static void initializeEngine(QWidget *w, const WidgetList &children, QScriptEngine &scriptEngine);
    static QString engineError(QScriptEngine &scriptEngine);

    Options options() const { return m_options; }
    void setOptions(Options options) { m_options = options; }

    Errors errors() const { return m_errors; }
private:
    QScriptEngine m_scriptEngine;
    Options m_options;
    Errors m_errors;
};

bool QFormScriptRunner::QFormScriptRunnerPrivate::run(const QString &script, QWidget *widget, const WidgetList &children, QString *errorMessage) {
    bool rc = false;
    initializeEngine(widget, children, m_scriptEngine);

    do {
        m_scriptEngine.evaluate(script);
        if (m_scriptEngine.hasUncaughtException ()) {
            *errorMessage = QCoreApplication::tr("Exception at line %1: %2").arg(m_scriptEngine.uncaughtExceptionLineNumber()).arg(engineError(m_scriptEngine));
            break;
        }
        rc = true;
    } while (false);
    m_scriptEngine.popContext();

    if (!rc) {
        Error error;
        error.objectName = widget->objectName();
        error.script = script;
        error.errorMessage = *errorMessage;
        m_errors.push_back(error);
    }
    return rc;
}

void QFormScriptRunner::QFormScriptRunnerPrivate::initializeEngine(QWidget *w, const WidgetList &children, QScriptEngine &scriptEngine) {
    // Populate the script variables. This pushes a context which must be popped.
    QScriptContext *ctx = scriptEngine.pushContext();
    QScriptValue widgetObject =  scriptEngine.newQObject(w);
    QScriptValue childrenArray = scriptEngine.newArray (children.size());

    for(int i = 0; i < children.size(); i++) {
        childrenArray.setProperty(i, scriptEngine.newQObject(children[i]));
    }

    const QFormBuilderStrings &strings = QFormBuilderStrings::instance();
    ctx ->activationObject().setProperty(strings.scriptWidgetVariable, widgetObject);
    ctx ->activationObject().setProperty(strings.scriptChildWidgetsVariable, childrenArray);
}

QString QFormScriptRunner::QFormScriptRunnerPrivate::engineError(QScriptEngine &scriptEngine) {
    QScriptValue error = scriptEngine.evaluate(QLatin1String("Error"));
    if (error.isValid())
        return error.toString();
    return QCoreApplication::tr("Unknown error");
}
// -- QFormScriptRunner

QFormScriptRunner::QFormScriptRunner() : m_impl(new QFormScriptRunnerPrivate)
{
}

QFormScriptRunner::~QFormScriptRunner()
{
    delete m_impl;
}

bool QFormScriptRunner::run(const DomWidget *domWidget,
                            const QString &customWidgetScript,
                            QWidget *widget, const WidgetList &children,
                            QString *errorMessage)
{
    typedef QList<DomScript*> DomScripts;

    const Options scriptOptions =  m_impl->options();
    if (scriptOptions & DisableScripts)
        return true;
    // get list
    const DomScripts domScripts = domWidget->elementScript();
    // Concatenate snippets, starting with custom widget script
    QString  script = customWidgetScript;
    if (script.isEmpty() && domScripts.empty())
        return true;

    foreach (const DomScript *scriptSnippet, domScripts) {
        // Ensure new line
        if (!script.isEmpty() && !script.endsWith(QLatin1Char('\n')))
            script += QLatin1Char('\n');
        script += scriptSnippet->text();
    }

    if (script.isEmpty())
        return true;

    const bool rc =  m_impl->run(script, widget, children, errorMessage);

    if (debugFormScriptRunner) {
        qDebug() << "For " << widget << " with " << children.size() << " children, ran: " << script;
        if (!rc)
            qDebug() << *errorMessage;
    }

    if (!rc) {
        if (!(scriptOptions & DisableWarnings)) {
            const QString message = QCoreApplication::tr("An error occurred while running the script for %1: %2\nScript: %3").
                arg(widget->objectName()).arg(*errorMessage).arg(script);
            qWarning() <<  message;
        }
    }
    return rc;
}

QFormScriptRunner::Options QFormScriptRunner::options() const
{
    return  m_impl->options();
}

void QFormScriptRunner::setOptions(Options options)
{
     m_impl->setOptions(options);
}


QFormScriptRunner::Errors QFormScriptRunner::errors() const
{
    return m_impl->errors();
}

void QFormScriptRunner::clearErrors()
{
     m_impl->clearErrors();
}


#ifdef QFORMINTERNAL_NAMESPACE
} // namespace QFormInternal
#endif

QT_END_NAMESPACE
