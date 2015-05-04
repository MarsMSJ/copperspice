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

#include <qhttpsocketengine_p.h>
#include <qtcpsocket.h>
#include <qhostaddress.h>
#include <qurl.h>
#include <qhttp.h>
#include <qelapsedtimer.h>
#include <qnetworkinterface.h>

#if !defined(QT_NO_NETWORKPROXY) && !defined(QT_NO_HTTP)
#include <qdebug.h>

QT_BEGIN_NAMESPACE

#define DEBUG

QHttpSocketEngine::QHttpSocketEngine(QObject *parent)
   : QAbstractSocketEngine(*new QHttpSocketEnginePrivate, parent)
{
}

QHttpSocketEngine::~QHttpSocketEngine()
{
}

bool QHttpSocketEngine::initialize(QAbstractSocket::SocketType type, QAbstractSocket::NetworkLayerProtocol protocol)
{
   Q_D(QHttpSocketEngine);
   if (type != QAbstractSocket::TcpSocket) {
      return false;
   }

   setProtocol(protocol);
   setSocketType(type);
   d->socket = new QTcpSocket(this);
#ifndef QT_NO_BEARERMANAGEMENT
   d->socket->setProperty("_q_networkSession", property("_q_networkSession"));
#endif

   // Explicitly disable proxying on the proxy socket itself to avoid
   // unwanted recursion.
   d->socket->setProxy(QNetworkProxy::NoProxy);

   // Intercept all the signals.
   connect(d->socket, SIGNAL(connected()),
           this, SLOT(slotSocketConnected()),
           Qt::DirectConnection);
   connect(d->socket, SIGNAL(disconnected()),
           this, SLOT(slotSocketDisconnected()),
           Qt::DirectConnection);
   connect(d->socket, SIGNAL(readyRead()),
           this, SLOT(slotSocketReadNotification()),
           Qt::DirectConnection);
   connect(d->socket, SIGNAL(bytesWritten(qint64)),
           this, SLOT(slotSocketBytesWritten()),
           Qt::DirectConnection);
   connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(slotSocketError(QAbstractSocket::SocketError)),
           Qt::DirectConnection);
   connect(d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
           this, SLOT(slotSocketStateChanged(QAbstractSocket::SocketState)),
           Qt::DirectConnection);

   return true;
}

bool QHttpSocketEngine::initialize(int, QAbstractSocket::SocketState)
{
   return false;
}

void QHttpSocketEngine::setProxy(const QNetworkProxy &proxy)
{
   Q_D(QHttpSocketEngine);
   d->proxy = proxy;
   QString user = proxy.user();
   if (!user.isEmpty()) {
      d->authenticator.setUser(user);
   }
   QString password = proxy.password();
   if (!password.isEmpty()) {
      d->authenticator.setPassword(password);
   }
}

int QHttpSocketEngine::socketDescriptor() const
{
   Q_D(const QHttpSocketEngine);
   return d->socket ? d->socket->socketDescriptor() : 0;
}

bool QHttpSocketEngine::isValid() const
{
   Q_D(const QHttpSocketEngine);
   return d->socket;
}

bool QHttpSocketEngine::connectInternal()
{
   Q_D(QHttpSocketEngine);

   d->credentialsSent = false;

   // If the handshake is done, enter ConnectedState state and return true.
   if (d->state == Connected) {
      qWarning("QHttpSocketEngine::connectToHost: called when already connected");
      setState(QAbstractSocket::ConnectedState);
      return true;
   }

   if (d->state == ConnectSent && d->socketState != QAbstractSocket::ConnectedState) {
      setState(QAbstractSocket::UnconnectedState);
   }

   // Handshake isn't done. If unconnected, start connecting.
   if (d->state == None && d->socket->state() == QAbstractSocket::UnconnectedState) {
      setState(QAbstractSocket::ConnectingState);
      //limit buffer in internal socket, data is buffered in the external socket under application control
      d->socket->setReadBufferSize(65536);
      d->socket->connectToHost(d->proxy.hostName(), d->proxy.port());
   }

   // If connected (might happen right away, at least for localhost services
   // on some BSD systems), there might already be bytes available.
   if (bytesAvailable()) {
      slotSocketReadNotification();
   }

   return d->socketState == QAbstractSocket::ConnectedState;
}

bool QHttpSocketEngine::connectToHost(const QHostAddress &address, quint16 port)
{
   Q_D(QHttpSocketEngine);

   setPeerAddress(address);
   setPeerPort(port);
   d->peerName.clear();

   return connectInternal();
}

bool QHttpSocketEngine::connectToHostByName(const QString &hostname, quint16 port)
{
   Q_D(QHttpSocketEngine);

   setPeerAddress(QHostAddress());
   setPeerPort(port);
   d->peerName = hostname;

   return connectInternal();
}

bool QHttpSocketEngine::bind(const QHostAddress &, quint16)
{
   return false;
}

bool QHttpSocketEngine::listen()
{
   return false;
}

int QHttpSocketEngine::accept()
{
   return 0;
}

void QHttpSocketEngine::close()
{
   Q_D(QHttpSocketEngine);
   if (d->socket) {
      d->socket->close();
      delete d->socket;
      d->socket = 0;
   }
}

qint64 QHttpSocketEngine::bytesAvailable() const
{
   Q_D(const QHttpSocketEngine);
   return d->readBuffer.size() + (d->socket ? d->socket->bytesAvailable() : 0);
}

qint64 QHttpSocketEngine::read(char *data, qint64 maxlen)
{
   Q_D(QHttpSocketEngine);
   qint64 bytesRead = d->socket->read(data, maxlen);

   if (d->socket->state() == QAbstractSocket::UnconnectedState
         && d->socket->bytesAvailable() == 0) {
      emitReadNotification();
   }

   if (bytesRead == -1) {
      // If nothing has been read so far, and the direct socket read
      // failed, return the socket's error. Otherwise, fall through and
      // return as much as we read so far.
      close();
      setError(QAbstractSocket::RemoteHostClosedError,
               QLatin1String("Remote host closed"));
      setState(QAbstractSocket::UnconnectedState);
      return -1;
   }
   return bytesRead;
}

qint64 QHttpSocketEngine::write(const char *data, qint64 len)
{
   Q_D(QHttpSocketEngine);
   return d->socket->write(data, len);
}

#ifndef QT_NO_UDPSOCKET
#ifndef QT_NO_NETWORKINTERFACE
bool QHttpSocketEngine::joinMulticastGroup(const QHostAddress &,
      const QNetworkInterface &)
{
   setError(QAbstractSocket::UnsupportedSocketOperationError,
            QLatin1String("Operation on socket is not supported"));
   return false;
}

bool QHttpSocketEngine::leaveMulticastGroup(const QHostAddress &,
      const QNetworkInterface &)
{
   setError(QAbstractSocket::UnsupportedSocketOperationError,
            QLatin1String("Operation on socket is not supported"));
   return false;
}

QNetworkInterface QHttpSocketEngine::multicastInterface() const
{
   return QNetworkInterface();
}

bool QHttpSocketEngine::setMulticastInterface(const QNetworkInterface &)
{
   setError(QAbstractSocket::UnsupportedSocketOperationError,
            QLatin1String("Operation on socket is not supported"));
   return false;
}
#endif // QT_NO_NETWORKINTERFACE

qint64 QHttpSocketEngine::readDatagram(char *, qint64, QHostAddress *,
                                       quint16 *)
{
   return 0;
}

qint64 QHttpSocketEngine::writeDatagram(const char *, qint64, const QHostAddress &,
                                        quint16)
{
   return 0;
}

bool QHttpSocketEngine::hasPendingDatagrams() const
{
   return false;
}

qint64 QHttpSocketEngine::pendingDatagramSize() const
{
   return 0;
}
#endif // QT_NO_UDPSOCKET

qint64 QHttpSocketEngine::bytesToWrite() const
{
   Q_D(const QHttpSocketEngine);
   if (d->socket) {
      return d->socket->bytesToWrite();
   } else {
      return 0;
   }
}

int QHttpSocketEngine::option(SocketOption option) const
{
   Q_D(const QHttpSocketEngine);
   if (d->socket) {
      // convert the enum and call the real socket
      if (option == QAbstractSocketEngine::LowDelayOption) {
         return d->socket->socketOption(QAbstractSocket::LowDelayOption).toInt();
      }
      if (option == QAbstractSocketEngine::KeepAliveOption) {
         return d->socket->socketOption(QAbstractSocket::KeepAliveOption).toInt();
      }
   }
   return -1;
}

bool QHttpSocketEngine::setOption(SocketOption option, int value)
{
   Q_D(QHttpSocketEngine);
   if (d->socket) {
      // convert the enum and call the real socket
      if (option == QAbstractSocketEngine::LowDelayOption) {
         d->socket->setSocketOption(QAbstractSocket::LowDelayOption, value);
      }
      if (option == QAbstractSocketEngine::KeepAliveOption) {
         d->socket->setSocketOption(QAbstractSocket::KeepAliveOption, value);
      }
      return true;
   }
   return false;
}

/*
   Returns the difference between msecs and elapsed. If msecs is -1,
   however, -1 is returned.
*/
static int qt_timeout_value(int msecs, int elapsed)
{
   if (msecs == -1) {
      return -1;
   }

   int timeout = msecs - elapsed;
   return timeout < 0 ? 0 : timeout;
}

bool QHttpSocketEngine::waitForRead(int msecs, bool *timedOut)
{
   Q_D(const QHttpSocketEngine);

   if (!d->socket || d->socket->state() == QAbstractSocket::UnconnectedState) {
      return false;
   }

   QElapsedTimer stopWatch;
   stopWatch.start();

   // Wait for more data if nothing is available.
   if (!d->socket->bytesAvailable()) {
      if (!d->socket->waitForReadyRead(qt_timeout_value(msecs, stopWatch.elapsed()))) {
         if (d->socket->state() == QAbstractSocket::UnconnectedState) {
            return true;
         }
         setError(d->socket->error(), d->socket->errorString());
         if (timedOut && d->socket->error() == QAbstractSocket::SocketTimeoutError) {
            *timedOut = true;
         }
         return false;
      }
   }

   // If we're not connected yet, wait until we are, or until an error
   // occurs.
   while (d->state != Connected && d->socket->waitForReadyRead(qt_timeout_value(msecs, stopWatch.elapsed()))) {
      // Loop while the protocol handshake is taking place.
   }

   // Report any error that may occur.
   if (d->state != Connected) {
      setError(d->socket->error(), d->socket->errorString());
      if (timedOut && d->socket->error() == QAbstractSocket::SocketTimeoutError) {
         *timedOut = true;
      }
      return false;
   }
   return true;
}

bool QHttpSocketEngine::waitForWrite(int msecs, bool *timedOut)
{
   Q_D(const QHttpSocketEngine);

   // If we're connected, just forward the call.
   if (d->state == Connected) {
      if (d->socket->bytesToWrite()) {
         if (!d->socket->waitForBytesWritten(msecs)) {
            if (d->socket->error() == QAbstractSocket::SocketTimeoutError && timedOut) {
               *timedOut = true;
            }
            return false;
         }
      }
      return true;
   }

   QElapsedTimer stopWatch;
   stopWatch.start();

   // If we're not connected yet, wait until we are, and until bytes have
   // been received (i.e., the socket has connected, we have sent the
   // greeting, and then received the response).
   while (d->state != Connected && d->socket->waitForReadyRead(qt_timeout_value(msecs, stopWatch.elapsed()))) {
      // Loop while the protocol handshake is taking place.
   }

   // Report any error that may occur.
   if (d->state != Connected) {
      //        setError(d->socket->error(), d->socket->errorString());
      if (timedOut && d->socket->error() == QAbstractSocket::SocketTimeoutError) {
         *timedOut = true;
      }
   }

   return true;
}

bool QHttpSocketEngine::waitForReadOrWrite(bool *readyToRead, bool *readyToWrite,
      bool checkRead, bool checkWrite,
      int msecs, bool *timedOut)
{
   Q_UNUSED(checkRead);

   if (!checkWrite) {
      // Not interested in writing? Then we wait for read notifications.
      bool canRead = waitForRead(msecs, timedOut);
      if (readyToRead) {
         *readyToRead = canRead;
      }
      return canRead;
   }

   // Interested in writing? Then we wait for write notifications.
   bool canWrite = waitForWrite(msecs, timedOut);
   if (readyToWrite) {
      *readyToWrite = canWrite;
   }
   return canWrite;
}

bool QHttpSocketEngine::isReadNotificationEnabled() const
{
   Q_D(const QHttpSocketEngine);
   return d->readNotificationEnabled;
}

void QHttpSocketEngine::setReadNotificationEnabled(bool enable)
{
   Q_D(QHttpSocketEngine);
   if (d->readNotificationEnabled == enable) {
      return;
   }

   d->readNotificationEnabled = enable;
   if (enable) {
      // Enabling read notification can trigger a notification.
      if (bytesAvailable()) {
         slotSocketReadNotification();
      }
   }
}

bool QHttpSocketEngine::isWriteNotificationEnabled() const
{
   Q_D(const QHttpSocketEngine);
   return d->writeNotificationEnabled;
}

void QHttpSocketEngine::setWriteNotificationEnabled(bool enable)
{
   Q_D(QHttpSocketEngine);
   d->writeNotificationEnabled = enable;
   if (enable && d->state == Connected && d->socket->state() == QAbstractSocket::ConnectedState) {
      QMetaObject::invokeMethod(this, "writeNotification", Qt::QueuedConnection);
   }
}

bool QHttpSocketEngine::isExceptionNotificationEnabled() const
{
   Q_D(const QHttpSocketEngine);
   return d->exceptNotificationEnabled;
}

void QHttpSocketEngine::setExceptionNotificationEnabled(bool enable)
{
   Q_D(QHttpSocketEngine);
   d->exceptNotificationEnabled = enable;
}

void QHttpSocketEngine::slotSocketConnected()
{
   Q_D(QHttpSocketEngine);

   // Send the greeting.
   const char method[] = "CONNECT";
   QByteArray peerAddress = d->peerName.isEmpty() ?
                            d->peerAddress.toString().toLatin1() :
                            QUrl::toAce(d->peerName);
   QByteArray path = peerAddress + ':' + QByteArray::number(d->peerPort);
   QByteArray data = method;
   data += " ";
   data += path;
   data += " HTTP/1.1\r\n";
   data += "Proxy-Connection: keep-alive\r\n"
           "User-Agent: ";
   QVariant v = property("_q_user-agent");
   if (v.isValid()) {
      data += v.toByteArray();
   } else {
      data += "Mozilla/5.0";
   }
   data += "\r\n"
           "Host: " + peerAddress + "\r\n";
   QAuthenticatorPrivate *priv = QAuthenticatorPrivate::getPrivate(d->authenticator);
   //qDebug() << "slotSocketConnected: priv=" << priv << (priv ? (int)priv->method : -1);
   if (priv && priv->method != QAuthenticatorPrivate::None) {
      d->credentialsSent = true;
      data += "Proxy-Authorization: " + priv->calculateResponse(method, path);
      data += "\r\n";
   }
   data += "\r\n";
   //     qDebug() << ">>>>>>>> sending request" << this;
   //     qDebug() << data;
   //     qDebug() << ">>>>>>>";
   d->socket->write(data);
   d->state = ConnectSent;
}

void QHttpSocketEngine::slotSocketDisconnected()
{
}

void QHttpSocketEngine::slotSocketReadNotification()
{
   Q_D(QHttpSocketEngine);
   if (d->state != Connected && d->socket->bytesAvailable() == 0) {
      return;
   }

   if (d->state == Connected) {
      // Forward as a read notification.
      if (d->readNotificationEnabled) {
         emitReadNotification();
      }
      return;
   }

readResponseContent:
   if (d->state == ReadResponseContent) {
      char dummybuffer[4096];
      while (d->pendingResponseData) {
         int read = d->socket->read(dummybuffer, qMin(sizeof(dummybuffer), (size_t)d->pendingResponseData));
         if (read >= 0) {
            dummybuffer[read] = 0;
         }

         if (read == 0) {
            return;
         }
         if (read == -1) {
            d->socket->disconnectFromHost();
            emitWriteNotification();
            return;
         }
         d->pendingResponseData -= read;
      }
      if (d->pendingResponseData > 0) {
         return;
      }
      d->state = SendAuthentication;
      slotSocketConnected();
      return;
   }

   // Still in handshake mode. Wait until we've got a full response.
   bool done = false;
   do {
      d->readBuffer += d->socket->readLine();
   } while (!(done = d->readBuffer.endsWith("\r\n\r\n")) && d->socket->canReadLine());

   if (!done) {
      // Wait for more.
      return;
   }

   if (!d->readBuffer.startsWith("HTTP/1.")) {
      // protocol error, this isn't HTTP
      d->readBuffer.clear();
      d->socket->close();
      setState(QAbstractSocket::UnconnectedState);
      setError(QAbstractSocket::ProxyProtocolError, tr("Did not receive HTTP response from proxy"));
      emitConnectionNotification();
      return;
   }

   QHttpResponseHeader responseHeader(QString::fromLatin1(d->readBuffer));
   d->readBuffer.clear(); // we parsed the proxy protocol response. from now on direct socket reading will be done

   int statusCode = responseHeader.statusCode();
   QAuthenticatorPrivate *priv = 0;
   if (statusCode == 200) {
      d->state = Connected;
      setLocalAddress(d->socket->localAddress());
      setLocalPort(d->socket->localPort());
      setState(QAbstractSocket::ConnectedState);
      d->authenticator.detach();
      priv = QAuthenticatorPrivate::getPrivate(d->authenticator);
      priv->hasFailed = false;
   } else if (statusCode == 407) {
      if (d->credentialsSent) {
         //407 response again means the provided username/password were invalid.
         d->authenticator =
            QAuthenticator(); //this is needed otherwise parseHttpResponse won't set the state, and then signal isn't emitted.
         d->authenticator.detach();
         priv = QAuthenticatorPrivate::getPrivate(d->authenticator);
         priv->hasFailed = true;
      } else if (d->authenticator.isNull()) {
         d->authenticator.detach();
      }
      priv = QAuthenticatorPrivate::getPrivate(d->authenticator);

      priv->parseHttpResponse(responseHeader, true);

      if (priv->phase == QAuthenticatorPrivate::Invalid) {
         // problem parsing the reply
         d->socket->close();
         setState(QAbstractSocket::UnconnectedState);
         setError(QAbstractSocket::ProxyProtocolError, tr("Error parsing authentication request from proxy"));
         emitConnectionNotification();
         return;
      }

      bool willClose;
      QString proxyConnectionHeader = responseHeader.value(QLatin1String("Proxy-Connection"));
      // Although most proxies use the unofficial Proxy-Connection header, the Connection header
      // from http spec is also allowed.
      if (proxyConnectionHeader.isEmpty()) {
         proxyConnectionHeader = responseHeader.value(QLatin1String("Connection"));
      }
      proxyConnectionHeader = proxyConnectionHeader.toLower();
      if (proxyConnectionHeader == QLatin1String("close")) {
         willClose = true;
      } else if (proxyConnectionHeader == QLatin1String("keep-alive")) {
         willClose = false;
      } else {
         // no Proxy-Connection header, so use the default
         // HTTP 1.1's default behaviour is to keep persistent connections
         // HTTP 1.0 or earlier, so we expect the server to close
         willClose = (responseHeader.majorVersion() * 0x100 + responseHeader.minorVersion()) <= 0x0100;
      }

      if (willClose) {
         // the server will disconnect, so let's avoid receiving an error
         // especially since the signal below may trigger a new event loop
         d->socket->disconnectFromHost();
         d->socket->readAll();
      }

      if (priv->phase == QAuthenticatorPrivate::Done) {
         emit proxyAuthenticationRequired(d->proxy, &d->authenticator);
      }
      // priv->phase will get reset to QAuthenticatorPrivate::Start if the authenticator got modified in the signal above.
      if (priv->phase == QAuthenticatorPrivate::Done) {
         setError(QAbstractSocket::ProxyAuthenticationRequiredError, tr("Authentication required"));
         d->socket->disconnectFromHost();
      } else {
         // close the connection if it isn't already and reconnect using the chosen authentication method
         d->state = SendAuthentication;
         if (willClose) {
            d->socket->connectToHost(d->proxy.hostName(), d->proxy.port());
         } else {
            bool ok;
            int contentLength = responseHeader.value(QLatin1String("Content-Length")).toInt(&ok);
            if (ok && contentLength > 0) {
               d->state = ReadResponseContent;
               d->pendingResponseData = contentLength;
               goto readResponseContent;
            } else {
               d->state = SendAuthentication;
               slotSocketConnected();
            }
         }
         return;
      }
   } else {
      d->socket->close();
      setState(QAbstractSocket::UnconnectedState);
      if (statusCode == 403 || statusCode == 405) {
         // 403 Forbidden
         // 405 Method Not Allowed
         setError(QAbstractSocket::SocketAccessError, tr("Proxy denied connection"));
      } else if (statusCode == 404) {
         // 404 Not Found: host lookup error
         setError(QAbstractSocket::HostNotFoundError, QAbstractSocket::tr("Host not found"));
      } else if (statusCode == 503) {
         // 503 Service Unavailable: Connection Refused
         setError(QAbstractSocket::ConnectionRefusedError, QAbstractSocket::tr("Connection refused"));
      } else {
         // Some other reply
         //qWarning("UNEXPECTED RESPONSE: [%s]", responseHeader.toString().toLatin1().data());
         setError(QAbstractSocket::ProxyProtocolError, tr("Error communicating with HTTP proxy"));
      }
   }

   // The handshake is done; notify that we're connected (or failed to connect)
   emitConnectionNotification();
}

void QHttpSocketEngine::slotSocketBytesWritten()
{
   Q_D(QHttpSocketEngine);
   if (d->state == Connected && d->writeNotificationEnabled) {
      emitWriteNotification();
   }
}

void QHttpSocketEngine::slotSocketError(QAbstractSocket::SocketError error)
{
   Q_D(QHttpSocketEngine);
   d->readBuffer.clear();

   if (d->state != Connected) {
      // we are in proxy handshaking stages
      if (error == QAbstractSocket::HostNotFoundError) {
         setError(QAbstractSocket::ProxyNotFoundError, tr("Proxy server not found"));
      } else if (error == QAbstractSocket::ConnectionRefusedError) {
         setError(QAbstractSocket::ProxyConnectionRefusedError, tr("Proxy connection refused"));
      } else if (error == QAbstractSocket::SocketTimeoutError) {
         setError(QAbstractSocket::ProxyConnectionTimeoutError, tr("Proxy server connection timed out"));
      } else if (error == QAbstractSocket::RemoteHostClosedError) {
         setError(QAbstractSocket::ProxyConnectionClosedError, tr("Proxy connection closed prematurely"));
      } else {
         setError(error, d->socket->errorString());
      }
      emitConnectionNotification();
      return;
   }

   // We're connected
   if (error == QAbstractSocket::SocketTimeoutError) {
      return;   // ignore this error
   }

   d->state = None;
   setError(error, d->socket->errorString());
   if (error != QAbstractSocket::RemoteHostClosedError) {
      qDebug() << "QHttpSocketEngine::slotSocketError: got weird error =" << error;
   }
   //read notification needs to always be emitted, otherwise the higher layer doesn't get the disconnected signal
   emitReadNotification();
}

void QHttpSocketEngine::slotSocketStateChanged(QAbstractSocket::SocketState state)
{
   Q_UNUSED(state);
}

void QHttpSocketEngine::emitPendingReadNotification()
{
   Q_D(QHttpSocketEngine);
   d->readNotificationPending = false;
   if (d->readNotificationEnabled) {
      emit readNotification();
   }
}

void QHttpSocketEngine::emitPendingWriteNotification()
{
   Q_D(QHttpSocketEngine);
   d->writeNotificationPending = false;
   if (d->writeNotificationEnabled) {
      emit writeNotification();
   }
}

void QHttpSocketEngine::emitPendingConnectionNotification()
{
   Q_D(QHttpSocketEngine);
   d->connectionNotificationPending = false;
   emit connectionNotification();
}

void QHttpSocketEngine::emitReadNotification()
{
   Q_D(QHttpSocketEngine);
   d->readNotificationActivated = true;
   // if there is a connection notification pending we have to emit the readNotification
   // incase there is connection error. This is only needed for Windows, but it does not
   // hurt in other cases.
   if ((d->readNotificationEnabled && !d->readNotificationPending) || d->connectionNotificationPending) {
      d->readNotificationPending = true;
      QMetaObject::invokeMethod(this, "emitPendingReadNotification", Qt::QueuedConnection);
   }
}

void QHttpSocketEngine::emitWriteNotification()
{
   Q_D(QHttpSocketEngine);
   d->writeNotificationActivated = true;
   if (d->writeNotificationEnabled && !d->writeNotificationPending) {
      d->writeNotificationPending = true;
      QMetaObject::invokeMethod(this, "emitPendingWriteNotification", Qt::QueuedConnection);
   }
}

void QHttpSocketEngine::emitConnectionNotification()
{
   Q_D(QHttpSocketEngine);
   if (!d->connectionNotificationPending) {
      d->connectionNotificationPending = true;
      QMetaObject::invokeMethod(this, "emitPendingConnectionNotification", Qt::QueuedConnection);
   }
}

QHttpSocketEnginePrivate::QHttpSocketEnginePrivate()
   : readNotificationEnabled(false)
   , writeNotificationEnabled(false)
   , exceptNotificationEnabled(false)
   , readNotificationActivated(false)
   , writeNotificationActivated(false)
   , readNotificationPending(false)
   , writeNotificationPending(false)
   , connectionNotificationPending(false)
   , credentialsSent(false)
   , pendingResponseData(0)
{
   socket = 0;
   state = QHttpSocketEngine::None;
}

QHttpSocketEnginePrivate::~QHttpSocketEnginePrivate()
{
}

QAbstractSocketEngine *QHttpSocketEngineHandler::createSocketEngine(QAbstractSocket::SocketType socketType,
      const QNetworkProxy &proxy,
      QObject *parent)
{
   if (socketType != QAbstractSocket::TcpSocket) {
      return 0;
   }

   // proxy type must have been resolved by now
   if (proxy.type() != QNetworkProxy::HttpProxy) {
      return 0;
   }

   // we only accept active sockets
   if (!qobject_cast<QAbstractSocket *>(parent)) {
      return 0;
   }

   QHttpSocketEngine *engine = new QHttpSocketEngine(parent);
   engine->setProxy(proxy);
   return engine;
}

QAbstractSocketEngine *QHttpSocketEngineHandler::createSocketEngine(int, QObject *)
{
   return 0;
}

QT_END_NAMESPACE

#endif
