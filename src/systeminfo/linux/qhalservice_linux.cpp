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

#if !defined(QT_NO_HAL)

#include "qhalservice_linux_p.h"

QTM_BEGIN_NAMESPACE

#define HAL_DBUS_SERVICE "org.freedesktop.Hal"

#define HAL_DBUS_MANAGER_PATH "/org/freedesktop/Hal/Manager"
#define HAL_DBUS_MANAGER_INTERFACE "org.freedesktop.Hal.Manager"

#define HAL_DEVICE_INTERFACE "org.freedesktop.Hal.Device"

#define HAL_DEVICES_LAPTOPPANEL_INTERFACE "org.freedesktop.Hal.Device.LaptopPanel"
#define HAL_DEVICE_KILLSWITCH_INTERFACE "org.freedesktop.Hal.Device.KillSwitch"

class QHalInterfacePrivate
{
public:
    QDBusInterface *connectionInterface;
    bool valid;
};

QHalInterface::QHalInterface(QObject *parent)
    : QObject(parent)
{
    d = new QHalInterfacePrivate();
    d->connectionInterface = new QDBusInterface(QLatin1String(HAL_DBUS_SERVICE), QLatin1String(HAL_DBUS_MANAGER_PATH),
                                                QLatin1String(HAL_DBUS_MANAGER_INTERFACE), QDBusConnection::systemBus());
    if (!d->connectionInterface->isValid())
        d->valid = false;
    else
        d->valid = true;
}

QHalInterface::~QHalInterface()
{
    delete d->connectionInterface;
    delete d;
}

bool QHalInterface::isValid()
{
    return d->valid;
}

QStringList QHalInterface::findDeviceByCapability(const QString &cap)
{
    QDBusReply<QStringList> reply = d->connectionInterface->call(QLatin1String("FindDeviceByCapability"), cap);
    if (reply.isValid())
        return reply.value();
    return QStringList();
}

QStringList QHalInterface::getAllDevices()
{
    QDBusReply<QStringList> reply = d->connectionInterface->call(QLatin1String("GetAllDevices"));
    if (reply.isValid())
        return reply.value();
    return QStringList();
}

class QHalDeviceInterfacePrivate
{
public:
    QDBusInterface *connectionInterface;
    QString path;
    bool valid;
};

QHalDeviceInterface::QHalDeviceInterface(const QString &devicePathName, QObject *parent )
    : QObject(parent)
{
    d = new QHalDeviceInterfacePrivate();
    d->path = devicePathName;
    d->connectionInterface = new QDBusInterface(QLatin1String(HAL_DBUS_SERVICE), d->path,
                                                QLatin1String(HAL_DEVICE_INTERFACE), QDBusConnection::systemBus());
    if (!d->connectionInterface->isValid())
        d->valid = false;
    else
        d->valid = true;
}

QHalDeviceInterface::~QHalDeviceInterface()
{
    delete d->connectionInterface;
    delete d;
}

bool QHalDeviceInterface::isValid()
{
    return d->valid;
}

bool QHalDeviceInterface::setConnections()
{
    if (!isValid())
        return false;

    if (QDBusConnection::systemBus().connect(QLatin1String(HAL_DBUS_SERVICE), d->path,
                                             QLatin1String(HAL_DEVICE_INTERFACE), QLatin1String("PropertyModified"),
                                             this, SIGNAL(propertyModified(int, QVariantList)))) {
        return true;
    }

    return false;
}

bool QHalDeviceInterface::getPropertyBool(const QString &prop)
{
    QDBusReply<bool> reply = d->connectionInterface->call(QLatin1String("GetPropertyBoolean"), prop);
    if (reply.isValid())
        return reply.value();
    return false;
}

QString QHalDeviceInterface::getPropertyString(const QString &prop)
{
    QDBusReply<QString> reply = d->connectionInterface->call(QLatin1String("GetPropertyString"), prop);
    if (reply.isValid())
        return reply.value();
    return QString();
}

qint32 QHalDeviceInterface::getPropertyInt(const QString &prop)
{
    QDBusReply<qint32> reply = d->connectionInterface->call(QLatin1String("GetPropertyInteger"), prop);
    if (reply.isValid())
        return reply.value();
    return 0;
}

class QHalDeviceLaptopPanelInterfacePrivate
{
public:
    QDBusInterface *connectionInterface;
    QString path;
    bool valid;
};

QHalDeviceLaptopPanelInterface::QHalDeviceLaptopPanelInterface(const QString &devicePathName, QObject *parent )
    : QObject(parent)
{
    d = new QHalDeviceLaptopPanelInterfacePrivate();
    d->path = devicePathName;
    d->connectionInterface = new QDBusInterface(QLatin1String(HAL_DBUS_SERVICE), d->path,
                                                QLatin1String(HAL_DEVICES_LAPTOPPANEL_INTERFACE), QDBusConnection::systemBus());
    if (!d->connectionInterface->isValid())
        d->valid = false;
    else
        d->valid = true;
}

QHalDeviceLaptopPanelInterface::~QHalDeviceLaptopPanelInterface()
{
    delete d->connectionInterface;
    delete d;
}

quint32 QHalDeviceLaptopPanelInterface::getBrightness()
{
    QDBusReply<qint32> reply = d->connectionInterface->call(QLatin1String("GetBrightness"));
    if (reply.isValid())
        return reply.value();
    return -1;
}

#include "moc_qhalservice_linux_p.cpp"

QTM_END_NAMESPACE

#endif // QT_NO_HAL
