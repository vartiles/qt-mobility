/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeomappolygonobject.h"
#include "qgeomappolygonobject_p.h"

#include "qgeocoordinate.h"

QTM_BEGIN_NAMESPACE

/*!
*/
QGeoMapPolygonObject::QGeoMapPolygonObject(QGeoMapObject *parent)
        : QGeoMapObject(new QGeoMapPolygonObjectPrivate(this, parent)) {}

/*!
*/
QGeoMapPolygonObject::~QGeoMapPolygonObject()
{
}

void QGeoMapPolygonObject::setPath(const QList<QGeoCoordinate> &path)
{
    Q_D(QGeoMapPolygonObject);
    if (d->path != path) {
        d->path = path;
        objectUpdate();
        emit pathChanged(emit d->path);
    }
}

QList<QGeoCoordinate> QGeoMapPolygonObject::path() const
{
    Q_D(const QGeoMapPolygonObject);
    return d->path;
}

void QGeoMapPolygonObject::setPen(const QPen &pen)
{
    Q_D(QGeoMapPolygonObject);
    if (d->pen != pen) {
        d->pen = pen;
        objectUpdate();
        emit penChanged(emit d->pen);
    }
}

QPen QGeoMapPolygonObject::pen() const
{
    Q_D(const QGeoMapPolygonObject);
    return d->pen;
}


void QGeoMapPolygonObject::setBrush(const QBrush &brush)
{
    Q_D(QGeoMapPolygonObject);
    if (d->brush != brush) {
        d->brush = brush;
        objectUpdate();
        emit brushChanged(d->brush);
    }
}

QBrush QGeoMapPolygonObject::brush() const
{
    Q_D(const QGeoMapPolygonObject);
    return d->brush;
}

/*******************************************************************************
*******************************************************************************/

QGeoMapPolygonObjectPrivate::QGeoMapPolygonObjectPrivate(QGeoMapObject *impl, QGeoMapObject *parent)
        : QGeoMapObjectPrivate(impl, parent, QGeoMapObject::PolygonType) {}

QGeoMapPolygonObjectPrivate::~QGeoMapPolygonObjectPrivate() {}

#include "moc_qgeomappolygonobject.cpp"

QTM_END_NAMESPACE
