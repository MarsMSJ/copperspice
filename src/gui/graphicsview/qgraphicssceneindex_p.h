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

#ifndef QGRAPHICSSCENEINDEX_P_H
#define QGRAPHICSSCENEINDEX_P_H

#include <qgraphicsscene_p.h>
#include <qgraphicsscene.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qobject.h>
#include <QtGui/qtransform.h>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

#if !defined(QT_NO_GRAPHICSVIEW)

class QGraphicsSceneIndexIntersector;
class QGraphicsSceneIndexPointIntersector;
class QGraphicsSceneIndexRectIntersector;
class QGraphicsSceneIndexPathIntersector;
class QGraphicsSceneIndexPrivate;
class QPointF;
class QRectF;

template<typename T>
class QList;

class QGraphicsSceneIndex : public QObject
{
   CS_OBJECT(QGraphicsSceneIndex)

 public:
   QGraphicsSceneIndex(QGraphicsScene *scene = 0);
   virtual ~QGraphicsSceneIndex();

   QGraphicsScene *scene() const;

   virtual QList<QGraphicsItem *> items(Qt::SortOrder order = Qt::DescendingOrder) const  = 0;
   virtual QList<QGraphicsItem *> items(const QPointF &pos, Qt::ItemSelectionMode mode,
                                        Qt::SortOrder order, const QTransform &deviceTransform = QTransform()) const;
   virtual QList<QGraphicsItem *> items(const QRectF &rect, Qt::ItemSelectionMode mode,
                                        Qt::SortOrder order, const QTransform &deviceTransform = QTransform()) const;
   virtual QList<QGraphicsItem *> items(const QPolygonF &polygon, Qt::ItemSelectionMode mode,
                                        Qt::SortOrder order, const QTransform &deviceTransform = QTransform()) const;
   virtual QList<QGraphicsItem *> items(const QPainterPath &path, Qt::ItemSelectionMode mode,
                                        Qt::SortOrder order, const QTransform &deviceTransform = QTransform()) const;
   virtual QList<QGraphicsItem *> estimateItems(const QPointF &point, Qt::SortOrder order) const;
   virtual QList<QGraphicsItem *> estimateItems(const QRectF &rect, Qt::SortOrder order) const = 0;
   virtual QList<QGraphicsItem *> estimateTopLevelItems(const QRectF &, Qt::SortOrder order) const;

 protected :
   GUI_CS_SLOT_1(Protected, virtual void updateSceneRect(const QRectF &rect))
   GUI_CS_SLOT_2(updateSceneRect)

   virtual void clear();
   virtual void addItem(QGraphicsItem *item) = 0;
   virtual void removeItem(QGraphicsItem *item) = 0;
   virtual void deleteItem(QGraphicsItem *item);

   virtual void itemChange(const QGraphicsItem *item, QGraphicsItem::GraphicsItemChange, const void *const value);
   virtual void prepareBoundingRectChange(const QGraphicsItem *item);

   QGraphicsSceneIndex(QGraphicsSceneIndexPrivate &dd, QGraphicsScene *scene);
   QScopedPointer<QGraphicsSceneIndexPrivate> d_ptr;

   friend class QGraphicsScene;
   friend class QGraphicsScenePrivate;
   friend class QGraphicsItem;
   friend class QGraphicsItemPrivate;
   friend class QGraphicsSceneBspTreeIndex;

 private:
   Q_DISABLE_COPY(QGraphicsSceneIndex)
   Q_DECLARE_PRIVATE(QGraphicsSceneIndex)

};

class QGraphicsSceneIndexPrivate
{
   Q_DECLARE_PUBLIC(QGraphicsSceneIndex)

 public:
   QGraphicsSceneIndexPrivate(QGraphicsScene *scene);
   virtual ~QGraphicsSceneIndexPrivate();

   void init();
   static bool itemCollidesWithPath(const QGraphicsItem *item, const QPainterPath &path, Qt::ItemSelectionMode mode);

   void recursive_items_helper(QGraphicsItem *item, QRectF exposeRect,
                               QGraphicsSceneIndexIntersector *intersector, QList<QGraphicsItem *> *items,
                               const QTransform &viewTransform,
                               Qt::ItemSelectionMode mode, qreal parentOpacity = 1.0) const;

   inline void items_helper(const QRectF &rect, QGraphicsSceneIndexIntersector *intersector,
                            QList<QGraphicsItem *> *items, const QTransform &viewTransform,
                            Qt::ItemSelectionMode mode, Qt::SortOrder order) const;

   QGraphicsScene *scene;
   QGraphicsSceneIndexPointIntersector *pointIntersector;
   QGraphicsSceneIndexRectIntersector *rectIntersector;
   QGraphicsSceneIndexPathIntersector *pathIntersector;

 protected:
   QGraphicsSceneIndex *q_ptr;

};

inline void QGraphicsSceneIndexPrivate::items_helper(const QRectF &rect, QGraphicsSceneIndexIntersector *intersector,
      QList<QGraphicsItem *> *items, const QTransform &viewTransform,
      Qt::ItemSelectionMode mode, Qt::SortOrder order) const
{
   Q_Q(const QGraphicsSceneIndex);
   const QList<QGraphicsItem *> tli = q->estimateTopLevelItems(rect, Qt::AscendingOrder);
   for (int i = 0; i < tli.size(); ++i) {
      recursive_items_helper(tli.at(i), rect, intersector, items, viewTransform, mode);
   }
   if (order == Qt::DescendingOrder) {
      const int n = items->size();
      for (int i = 0; i < n / 2; ++i) {
         items->swap(i, n - i - 1);
      }
   }
}

class QGraphicsSceneIndexIntersector
{
 public:
   QGraphicsSceneIndexIntersector() { }
   virtual ~QGraphicsSceneIndexIntersector() { }
   virtual bool intersect(const QGraphicsItem *item, const QRectF &exposeRect, Qt::ItemSelectionMode mode,
                          const QTransform &deviceTransform) const = 0;
};

#endif // QT_NO_GRAPHICSVIEW

QT_END_NAMESPACE

#endif // QGRAPHICSSCENEINDEX_H
