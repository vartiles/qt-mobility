/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOROUTESEGMENT_H
#define QGEOROUTESEGMENT_H

#include "qmobilityglobal.h"

#include <QExplicitlySharedDataPointer>
#include <QList>

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QGeoCoordinate;
class QGeoManeuver;
class QGeoRouteSegmentPrivate;

class Q_LOCATION_EXPORT QGeoRouteSegment
{

public:
    QGeoRouteSegment();
    QGeoRouteSegment(const QGeoRouteSegment &other);
    ~QGeoRouteSegment();

    QGeoRouteSegment& operator= (const QGeoRouteSegment &other);

    bool operator ==(const QGeoRouteSegment &other) const;
    bool operator !=(const QGeoRouteSegment &other) const;

    bool isValid() const;

    void setNextRouteSegment(const QGeoRouteSegment &routeSegment);
    QGeoRouteSegment nextRouteSegment() const;

    void setTravelTime(int secs);
    int travelTime() const;

    void setDistance(qreal distance);
    qreal distance() const;

    void setPath(const QList<QGeoCoordinate> &path);
    QList<QGeoCoordinate> path() const;

    void setManeuver(const QGeoManeuver &maneuver);
    QGeoManeuver maneuver() const;

protected:
    QGeoRouteSegment(QExplicitlySharedDataPointer<QGeoRouteSegmentPrivate> &d_ptr);

private:
    QExplicitlySharedDataPointer<QGeoRouteSegmentPrivate> d_ptr;
};

QTM_END_NAMESPACE

QT_END_HEADER

#endif
