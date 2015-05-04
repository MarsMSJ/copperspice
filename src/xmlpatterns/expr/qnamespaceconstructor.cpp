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

#include "qcommonsequencetypes_p.h"

#include "qnamespaceconstructor_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

NamespaceConstructor::NamespaceConstructor(const QXmlName nb) : m_binding(nb)
{
   Q_ASSERT(!m_binding.isNull());
}

void NamespaceConstructor::evaluateToSequenceReceiver(const DynamicContext::Ptr &context) const
{
   context->outputReceiver()->namespaceBinding(m_binding);
}

SequenceType::Ptr NamespaceConstructor::staticType() const
{
   return CommonSequenceTypes::ExactlyOneAttribute;
}

SequenceType::List NamespaceConstructor::expectedOperandTypes() const
{
   SequenceType::List result;
   result.append(CommonSequenceTypes::ExactlyOneString);
   return result;
}

Expression::Properties NamespaceConstructor::properties() const
{
   return DisableElimination | IsNodeConstructor;
}

ExpressionVisitorResult::Ptr NamespaceConstructor::accept(const ExpressionVisitor::Ptr &visitor) const
{
   return visitor->visit(this);
}

Expression::ID NamespaceConstructor::id() const
{
   return IDNamespaceConstructor;
}

QT_END_NAMESPACE
