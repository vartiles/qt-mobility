/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef QABSTRACTCONTACTFILTER_H
#define QABSTRACTCONTACTFILTER_H

#include <QList>
#include "qtcontactsglobal.h"
#include "qcontactmanager.h"
#include "qcontactsortorder.h"
#include "qcontactfilter.h"

QTM_USE_NAMESPACE
class CntAbstractContactFilter
{
public:
    virtual ~CntAbstractContactFilter(){};

public:
    enum FilterSupport {
        /* The filter is not supported */
        NotSupported = 0,
        /* The filter is illegal. Filtering is not allowed and the operation
         * should fail immediately with an error.
         */
        IllegalFilter,
        /* The filter is supported natively */
        Supported,
        /* The filter is not fully natively supported, but for performance
         * reasons the contact filter implementation pretends supporting the
         * filter when it actually maps the filter to another, less strict
         * filter.
         * 
         * For example if the caller uses match flag QContactFilter::MatchExactly,
         * the filter actually gives the result as QContactFilter::MatchContains
         * (because of the limitations in the underlying database).
         * 
         * The result then needs to be filtered by the caller (for example by
         * using QContactManagerEngine::testFilter).
         */
        SupportedPreFilterOnly
    };
public:
    virtual QList<QContactLocalId> contacts(
            const QContactFilter &filter,
            const QList<QContactSortOrder> &sortOrders,
            bool &filterSupported,
            QContactManager::Error* error) = 0;
    virtual bool filterSupported(const QContactFilter& filter) = 0;
};

#endif /* QABSTRACTCONTACTFILTER_H */
