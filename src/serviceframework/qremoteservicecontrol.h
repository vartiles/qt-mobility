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

#ifndef QREMOTESERVICECONTROL_H
#define QREMOTESERVICECONTROL_H

#include "qmobilityglobal.h"
#include <QObject>
#include <QQueue>


QTM_BEGIN_NAMESPACE


class QRemoteServiceControlPrivate;
class Q_SERVICEFW_EXPORT QRemoteServiceControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool quitOnLastInstanceClosed READ quitOnLastInstanceClosed WRITE setQuitOnLastInstanceClosed)

public:
    QRemoteServiceControl(QObject* parent = 0);
    ~QRemoteServiceControl();

    void publishServices(const QString& ident );
    
    bool quitOnLastInstanceClosed() const;
    void setQuitOnLastInstanceClosed(const bool quit);

    typedef bool (*securityFilter)(const void *message);
    securityFilter setSecurityFilter(securityFilter filter);

Q_SIGNALS:
    void lastInstanceClosed();


private:
    QRemoteServiceControlPrivate* d;
};

struct QRemoteServiceControlLocalSocketCred {
    int fd;
    int pid;
    int uid;
    int gid;
};

QTM_END_NAMESPACE
#endif //QREMOTESERVICECONTROL_H