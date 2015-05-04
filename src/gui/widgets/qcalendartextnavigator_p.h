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

#ifndef QCALENDARTEXTNAVIGATOR_P_H
#define QCALENDARTEXTNAVIGATOR_P_H

#include <QtCore/qobject.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qbasictimer.h>

#ifndef QT_NO_CALENDARWIDGET

QT_BEGIN_NAMESPACE

class QLabel;
class QCalendarDateValidator;
class QFrame;

class QCalendarTextNavigator: public QObject
{
   CS_OBJECT(QCalendarTextNavigator)

 public:
   QCalendarTextNavigator(QObject *parent = 0)
      : QObject(parent), m_dateText(0), m_dateFrame(0), m_dateValidator(0), m_widget(0), m_editDelay(1500),
        m_date(QDate::currentDate()) { }

   QWidget *widget() const;
   void setWidget(QWidget *widget);

   int dateEditAcceptDelay() const;
   void setDateEditAcceptDelay(int delay);

   QDate date() const;
   void setDate(const QDate &date);

   bool eventFilter(QObject *o, QEvent *e);
   void timerEvent(QTimerEvent *e);

   GUI_CS_SIGNAL_1(Public, void dateChanged(const QDate &date))
   GUI_CS_SIGNAL_2(dateChanged, date)
   GUI_CS_SIGNAL_1(Public, void editingFinished())
   GUI_CS_SIGNAL_2(editingFinished)

 private:
   void applyDate();
   void updateDateLabel();
   void createDateLabel();
   void removeDateLabel();

   QLabel *m_dateText;
   QFrame *m_dateFrame;
   QBasicTimer m_acceptTimer;
   QCalendarDateValidator *m_dateValidator;
   QWidget *m_widget;
   int m_editDelay;

   QDate m_date;
};

QT_END_NAMESPACE

#endif // QT_NO_CALENDARWIDGET

#endif

