set(CORE_PUBLIC_INCLUDES
    ${CORE_PUBLIC_INCLUDES}
    QAbstractEventDispatcher
    QAbstractItemModel
    QAbstractListModel
    QAbstractTableModel
    QBasicTimer
    QArgument
    QChildEvent
    QCoreApplication
    QCustomEvent
    QDynamicPropertyChangeEvent
    QEvent
    QEventLoop
    QGenericArgument
    QGenericReturnArgument
    QMetaClassInfo
    QMetaEnum
    QMetaMethod
    QMetaObject
    QMetaObjectAccessor
    QMetaObjectExtraData
    QMetaProperty
    QMetaType
    QMetaTypeId
    QMetaTypeId2
    QMimeData
    QModelIndex
    QModelIndexList
    QObject
    QObjectCleanupHandler
    QPersistentModelIndex
    QPointer
    QObjectData
    QObjectUserData
    QObjectList
    QReturnArgument
    QSharedMemory
    QSignalMapper
    QSocketNotifier
    QSystemSemaphore
    QTimer
    QTimerEvent
    QTranslator
    QVariant
    QVariantComparisonHelper
    QVariantList
    QVariantHash
    QVariantMap
    QtCleanUpFunction
    QtMsgHandler
)

set(CORE_INCLUDES
    ${CORE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracteventdispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractitemmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractlistmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracttablemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qargument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qbasictimer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qchildevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcustomevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qdynamicpropertychangeevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventloop.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgenericargument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qgenericreturnargument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmath.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaclassinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaenum.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetamethod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobjectaccessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobjectextradata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaproperty.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatypeid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatypeid2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmimedata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmodelindex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmodelindexlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtmsghandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectuserdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectcleanuphandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpersistentmodelindex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpointer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qreturnargument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsharedmemory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsignalmapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsocketnotifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemsemaphore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtcleanupfunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtimer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtimerevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtranslator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariant.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariantcomparisonhelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariantlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvarianthash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariantmap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_callevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_internal_1.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_internal_2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csobject_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csobject_macro.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csregister1.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csregister2.h
)

set(CORE_PRIVATE_INCLUDES
    ${CORE_PRIVATE_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractitemmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcorecmdlineargs_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_unix_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_mac_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtranslator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracteventdispatcher_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariant_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreglobaldata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsharedmemory_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemsemaphore_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_win_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_unix_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwineventnotifier_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qfunctions_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcrashhandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemerror_p.h
)
set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstracteventdispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qabstractitemmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qbasictimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreglobaldata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventloop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetaobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmetatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qmimedata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qobjectcleanuphandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qpointer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsharedmemory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsignalmapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsocketnotifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemsemaphore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qtranslator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qvariant.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_callevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_classinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_enum.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_method.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csmeta_prop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csobject_debug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/csobject_private.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Win32")
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qwineventnotifier_p.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsharedmemory_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemsemaphore_win.cpp
    )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "(Linux|OpenBSD|FreeBSD|NetBSD)")
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcrashhandler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsharedmemory_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemsemaphore_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_glib.cpp
    )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcore_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qcoreapplication_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qeventdispatcher_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsharedmemory_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qsystemsemaphore_unix.cpp
    )
endif()