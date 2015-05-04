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

/********************************************************
**  This file is part of the KDE project.
********************************************************/

#include <gst/gst.h>
#include "message.h"

QT_BEGIN_NAMESPACE

static int wuchi = qRegisterMetaType<Phonon::Gstreamer::Message>();

namespace Phonon
{
namespace Gstreamer
{

/*!
    \class gstreamer::Message
    \internal
*/
Message::Message():
        m_message(0),
        m_source(0)
{}

Message::Message(GstMessage* message, MediaObject *source):
        m_message(message),
        m_source(source)
{
    Q_ASSERT(m_message);
    gst_message_ref(m_message);
}

Message::Message(const Message &other)
{
    m_message = other.m_message;
    gst_message_ref(m_message);
    m_source = other.m_source;
}

Message::~Message()
{
    gst_message_unref(m_message);
}

GstMessage* Message::rawMessage() const
{
    return m_message;
}

MediaObject *Message::source() const
{
    return m_source;
}

}   // ns gstreamer
}   // ns phonon

QT_END_NAMESPACE

