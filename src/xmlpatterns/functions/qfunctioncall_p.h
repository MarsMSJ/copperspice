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

#ifndef QFunctionCall_P_H
#define QFunctionCall_P_H

#include <qunlimitedcontainer_p.h>
#include <qfunctionsignature_p.h>
#include <qxpathhelper_p.h>

QT_BEGIN_NAMESPACE

namespace QPatternist {
class FunctionCall : public UnlimitedContainer
{
 public:
   typedef QExplicitlySharedDataPointer<FunctionCall> Ptr;

   virtual SequenceType::List expectedOperandTypes() const;
   virtual SequenceType::Ptr staticType() const;

   virtual void setSignature(const FunctionSignature::Ptr &sign);
   virtual FunctionSignature::Ptr signature() const;

   virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                     const SequenceType::Ptr &reqType);

   virtual Expression::Properties properties() const;

   virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;

   virtual ID id() const;

 private:
   FunctionSignature::Ptr m_signature;
};
}

QT_END_NAMESPACE

#endif
