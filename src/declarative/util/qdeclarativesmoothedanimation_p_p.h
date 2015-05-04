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

#ifndef QDECLARATIVESMOOTHEDANIMATION_P_P_H
#define QDECLARATIVESMOOTHEDANIMATION_P_P_H

#include <qdeclarativesmoothedanimation_p.h>
#include <qdeclarativeanimation_p.h>
#include <qdeclarativeanimation_p_p.h>
#include <qparallelanimationgroup.h>
#include <QTimer>

QT_BEGIN_NAMESPACE

class QSmoothedAnimation : public QAbstractAnimation
{
 public:
   QSmoothedAnimation(QObject *parent = 0);

   qreal to;
   qreal velocity;
   int userDuration;

   int maximumEasingTime;
   QDeclarativeSmoothedAnimation::ReversingMode reversingMode;

   qreal initialVelocity;
   qreal trackVelocity;

   QDeclarativeProperty target;

   int duration() const;
   void restart();
   void init();

 protected:
   virtual void updateCurrentTime(int);
   virtual void updateState(QAbstractAnimation::State, QAbstractAnimation::State);

 private:
   qreal easeFollow(qreal);
   qreal initialValue;

   bool invert;

   int finalDuration;

   // Parameters for use in updateCurrentTime()
   qreal a;  // Acceleration
   qreal d;  // Deceleration
   qreal tf; // Total time
   qreal tp; // Time at which peak velocity occurs
   qreal td; // Time at which decelleration begins
   qreal vp; // Velocity at tp
   qreal sp; // Displacement at tp
   qreal sd; // Displacement at td
   qreal vi; // "Normalized" initialvelocity
   qreal s;  // Total s

   int lastTime;

   bool recalc();
   void delayedStop();

   QTimer delayedStopTimer;
};

class QDeclarativeSmoothedAnimationPrivate : public QDeclarativePropertyAnimationPrivate
{
   Q_DECLARE_PUBLIC(QDeclarativeSmoothedAnimation)
 public:
   QDeclarativeSmoothedAnimationPrivate();
   void updateRunningAnimations();

   QParallelAnimationGroup *wrapperGroup;
   QSmoothedAnimation *anim;
   QHash<QDeclarativeProperty, QSmoothedAnimation *> activeAnimations;
};

QT_END_NAMESPACE

#endif // QDECLARATIVESMOOTHEDANIMATION_P_H
