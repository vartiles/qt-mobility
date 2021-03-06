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

#ifndef QTM_NETWORKCONFIGURATION_H
#define QTM_NETWORKCONFIGURATION_H

#include "qmobilityglobal.h"

#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtNetwork/qnetworkconfiguration.h>

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QNetworkConfigurationPrivate;
class Q_BEARER_EXPORT QNetworkConfiguration : public QT_PREPEND_NAMESPACE(QNetworkConfiguration)
{
public:
    QNetworkConfiguration();
    QNetworkConfiguration(const QNetworkConfiguration& other);
    QNetworkConfiguration &operator=(const QNetworkConfiguration& other);
    QNetworkConfiguration(const QT_PREPEND_NAMESPACE(QNetworkConfiguration) &); //internal
    ~QNetworkConfiguration();

    bool operator==(const QNetworkConfiguration& cp) const;
    inline bool operator!=(const QNetworkConfiguration& cp) const
    { return !operator==(cp); }

    enum Type {
        InternetAccessPoint = 0,
        ServiceNetwork,
        UserChoice,
        Invalid
    };

    enum Purpose {
        UnknownPurpose = 0,
        PublicPurpose,
        PrivatePurpose,
        ServiceSpecificPurpose
    };

    enum StateFlag {
        Undefined        = 0x0000001,
        Defined          = 0x0000002,
        Discovered       = 0x0000006,
        Active           = 0x000000e
    };

    Q_DECLARE_FLAGS(StateFlags, StateFlag)

    StateFlags state() const;
    Type type() const;
    Purpose purpose() const;
    QString bearerName() const;
    QString identifier() const;
    bool isRoamingAvailable() const;
    QList<QNetworkConfiguration> children() const;

    QString name() const;
    bool isValid() const;

private:
    //'d' is removed, but the base class has the same 'd' so class size remains the same.
    //QExplicitlySharedDataPointer<QNetworkConfigurationPrivate> d;
};

QTM_END_NAMESPACE

QT_END_HEADER

#endif //QTM_NETWORKCONFIGURATION_H
