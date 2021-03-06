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
#include "qmessageaccountid.h"
#include "qmfhelpers_p.h"

#include <qmailid.h>

QTM_BEGIN_NAMESPACE

class QMessageAccountIdPrivate
{
public:
    QMailAccountId _id;
};

QMessageAccountId::QMessageAccountId()
    : d_ptr(0)
{
}

QMessageAccountId::QMessageAccountId(const QMessageAccountId& other)
    : d_ptr(0)
{
    this->operator=(other);
}

QMessageAccountId::QMessageAccountId(const QString& id)
    : d_ptr(0)
{
    QMailAccountId aid(QmfHelpers::stripIdentifierPrefix(id).toULongLong());
    if (aid.isValid()) {
        d_ptr = new QMessageAccountIdPrivate;
        d_ptr->_id = aid;
    }
}

QMessageAccountId::~QMessageAccountId()
{
    delete d_ptr;
}

bool QMessageAccountId::operator==(const QMessageAccountId& other) const
{
    if (isValid()) {
        return (other.isValid() ? (d_ptr->_id == other.d_ptr->_id) : false);
    } else {
        return !other.isValid();
    }
}

QMessageAccountId& QMessageAccountId::operator=(const QMessageAccountId& other)
{
    if (&other != this) {
        if (other.isValid()) {
            if (!d_ptr) {
                d_ptr = new QMessageAccountIdPrivate;
            }
            d_ptr->_id = other.d_ptr->_id;
        } else {
            delete d_ptr;
        }
    }

    return *this;
}

bool QMessageAccountId::operator<(const QMessageAccountId& other) const
{
    if (isValid() && other.isValid())
        return (d_ptr->_id < other.d_ptr->_id);
    
    if (isValid()) {
        return false; // other is invalid, valid > invalid
    } else if (other.isValid()) {
        return true; // invalid < valid
    }
    return false; // both invalid
}

QString QMessageAccountId::toString() const
{
    QString result = (isValid() ? QString::number(d_ptr->_id.toULongLong()) : QString());
    return QmfHelpers::prefixIdentifier(result);
}

bool QMessageAccountId::isValid() const
{
    return (d_ptr && d_ptr->_id.isValid());
}

uint qHash(const QMessageAccountId &id)
{
    return qHash(QmfHelpers::convert(id));
}

QTM_END_NAMESPACE
