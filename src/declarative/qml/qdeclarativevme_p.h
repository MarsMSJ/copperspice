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

#ifndef QDECLARATIVEVME_P_H
#define QDECLARATIVEVME_P_H

#include "qdeclarativeerror.h"
#include "qbitfield_p.h"
#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtCore/QVarLengthArray>

QT_BEGIN_NAMESPACE

class QObject;
class QDeclarativeInstruction;
class QDeclarativeCompiledData;
class QDeclarativeCompiledData;
class QDeclarativeContextData;

class QDeclarativeVMEObjectStack;
class QDeclarativeVME
{
 public:
   QDeclarativeVME();

   QObject *run(QDeclarativeContextData *, QDeclarativeCompiledData *,
                int start = -1, int count = -1,
                const QBitField & = QBitField());
   void runDeferred(QObject *);

   bool isError() const;
   QList<QDeclarativeError> errors() const;

 private:
   QObject *run(QDeclarativeVMEObjectStack &,
                QDeclarativeContextData *, QDeclarativeCompiledData *,
                int start, int count,
                const QBitField &);
   QList<QDeclarativeError> vmeErrors;
};

QT_END_NAMESPACE

#endif // QDECLARATIVEVME_P_H
