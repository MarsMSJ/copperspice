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

#ifndef QDECLARATIVESTATEGROUP_P_H
#define QDECLARATIVESTATEGROUP_P_H

#include <qdeclarativestate_p.h>

QT_BEGIN_NAMESPACE

class QDeclarativeStateGroupPrivate;

class Q_DECLARATIVE_EXPORT QDeclarativeStateGroup : public QObject, public QDeclarativeParserStatus
{
   CS_OBJECT_MULTIPLE(QDeclarativeStateGroup, QObject)

   CS_INTERFACES(QDeclarativeParserStatus)
   Q_DECLARE_PRIVATE(QDeclarativeStateGroup)

   CS_PROPERTY_READ(state, state)
   CS_PROPERTY_WRITE(state, setState)
   CS_PROPERTY_NOTIFY(state, stateChanged)
   CS_PROPERTY_READ(states, statesProperty)
   CS_PROPERTY_DESIGNABLE(states, false)
   CS_PROPERTY_READ(transitions, transitionsProperty)
   CS_PROPERTY_DESIGNABLE(transitions, false)

 public:
   QDeclarativeStateGroup(QObject * = 0);
   virtual ~QDeclarativeStateGroup();

   QString state() const;
   void setState(const QString &);

   QDeclarativeListProperty<QDeclarativeState> statesProperty();
   QList<QDeclarativeState *> states() const;

   QDeclarativeListProperty<QDeclarativeTransition> transitionsProperty();

   QDeclarativeState *findState(const QString &name) const;

   virtual void classBegin();
   virtual void componentComplete();

   CS_SIGNAL_1(Public, void stateChanged(const QString &un_named_arg1))
   CS_SIGNAL_2(stateChanged, un_named_arg1)

 private:
   friend class QDeclarativeState;
   bool updateAutoState();
   void removeState(QDeclarativeState *state);
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeStateGroup)

#endif // QDECLARATIVESTATEGROUP_H
