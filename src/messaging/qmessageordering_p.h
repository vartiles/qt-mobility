/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef QMESSAGEORDERINGPRIVATE_H
#define QMESSAGEORDERINGPRIVATE_H
#include "qmessageordering.h"
#if defined(Q_OS_WIN)
#include "qmessagestore.h"
#include "qmessagefilter.h"
#include <qpair.h>
#include "winhelpers_p.h"
#endif
#ifdef Q_OS_SYMBIAN
#include <qpair.h>
#endif


QTM_BEGIN_NAMESPACE

class QMessageOrderingPrivate
{
    Q_DECLARE_PUBLIC(QMessageOrdering)

public:
    QMessageOrderingPrivate(QMessageOrdering *ordering);

    QMessageOrdering *q_ptr;
    bool _valid;
#if defined(Q_OS_WIN)
    enum Field { Type = 0, Sender, Recipients, Subject, TimeStamp, ReceptionTimeStamp, Read, HasAttachments, Incoming, Removed, Priority, Size };
    QList<QPair<Field, Qt::SortOrder> > _fieldOrderList;
    static bool lessThan(const QMessageOrdering &ordering, const QMessage &left, const QMessage &right);
    static void sortTable(QMessageStore::ErrorCode *lastError, const QMessageOrdering &ordering, LPMAPITABLE);
    static QMessageOrdering from(QMessageOrderingPrivate::Field field, Qt::SortOrder order);

    static bool isFilterType(const QMessageOrdering &ordering);
    static QList<QMessageFilter> normalize(const QList<QMessageFilter> &filters, const QMessageOrdering &ordering);
#endif
#ifdef Q_OS_SYMBIAN
    enum Field { Type = 0, Sender, Recipients, Subject, TimeStamp, ReceptionTimeStamp, Read, HasAttachments, Incoming, Removed, Priority, Size };
    QList<QPair<Field, Qt::SortOrder> > _fieldOrderList;

    static QMessageOrdering from(QMessageOrderingPrivate::Field field, Qt::SortOrder order);
    static QMessageOrderingPrivate* implementation(const QMessageOrdering &ordering);
    static bool lessThan(const QMessageOrdering &ordering, const QMessage &message1, const QMessage &message2);
#endif
};
QTM_END_NAMESPACE
#endif