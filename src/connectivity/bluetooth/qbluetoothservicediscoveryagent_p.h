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

#ifndef QBLUETOOTHSERVICEDISCOVERYAGENT_P_H
#define QBLUETOOTHSERVICEDISCOVERYAGENT_P_H

#include "qbluetoothaddress.h"
#include "qbluetoothdeviceinfo.h"
#include "qbluetoothserviceinfo.h"
#include "qbluetoothservicediscoveryagent.h"

#include <QStack>

#ifdef QTM_SYMBIAN_BLUETOOTH
#include <btsdp.h>
#endif

#ifdef QTM_BLUEZ_BLUETOOTH
class OrgBluezManagerInterface;
class OrgBluezAdapterInterface;
class OrgBluezDeviceInterface;
class QDBusPendingCallWatcher;
class QXmlStreamReader;
#endif

QT_BEGIN_HEADER

QTM_BEGIN_NAMESPACE

class QBluetoothDeviceDiscoveryAgent;

class QBluetoothServiceDiscoveryAgentPrivate
#ifdef QTM_SYMBIAN_BLUETOOTH
: public MSdpAgentNotifier, public MSdpAttributeValueVisitor
#endif
{
    Q_DECLARE_PUBLIC(QBluetoothServiceDiscoveryAgent)

public:
    enum DiscoveryState {
        Inactive,
        DeviceDiscovery,
        ServiceDiscovery,
    };

    QBluetoothServiceDiscoveryAgentPrivate(const QBluetoothAddress &address);
    ~QBluetoothServiceDiscoveryAgentPrivate();

    void startDeviceDiscovery();
    void stopDeviceDiscovery();
    void startServiceDiscovery();
    void stopServiceDiscovery();

    void setDiscoveryState(DiscoveryState s) { state = s; }
    DiscoveryState discoveryState() { return state; }

    void setDiscoveryMode(QBluetoothServiceDiscoveryAgent::DiscoveryMode m) { mode = m; }
    QBluetoothServiceDiscoveryAgent::DiscoveryMode DiscoveryMode() { return mode; }

    // private slots
    void _q_deviceDiscoveryFinished();
    void _q_deviceDiscovered(const QBluetoothDeviceInfo &info);
    void _q_serviceDiscoveryFinished();
#ifdef QTM_BLUEZ_BLUETOOTH
    void _q_discoveredServices(QDBusPendingCallWatcher *watcher);
    void _q_createdDevice(QDBusPendingCallWatcher *watcher);
#endif

#ifdef QTM_SYMBIAN_BLUETOOTH
    /* MSdpAgentNotifier virtual functions */
    void NextRecordRequestComplete(TInt aError, TSdpServRecordHandle aHandle, TInt aTotalRecordsCount);
    void AttributeRequestResult(TSdpServRecordHandle aHandle, TSdpAttributeID aAttrID, CSdpAttrValue *aAttrValue);
    void AttributeRequestComplete(TSdpServRecordHandle, TInt aError);

    /* MSdpAttributeValueVisitor virtual functions */
    void VisitAttributeValueL(CSdpAttrValue &aValue, TSdpElementType aType);
    void StartListL(CSdpAttrValueList &);
    void EndListL();
#endif

private:
    void start(const QBluetoothAddress &address);
    void stop();

#ifdef QTM_SYMBIAN_BLUETOOTH
    void startL(const QBluetoothAddress &address);
    void initL(const QBluetoothAddress &address);
#elif defined(QTM_BLUEZ_BLUETOOTH)
    QVariant readAttributeValue(QXmlStreamReader &xml);
#endif

public:
    QBluetoothServiceDiscoveryAgent::Error error;
    QString errorString;

    QList<QBluetoothServiceInfo> discoveredServices;
    QList<QBluetoothDeviceInfo> discoveredDevices;

private:
    DiscoveryState state;
    QBluetoothAddress deviceAddress;
    QList<QBluetoothUuid> uuidFilter;

    QBluetoothDeviceDiscoveryAgent *deviceDiscoveryAgent;

    QBluetoothServiceDiscoveryAgent::DiscoveryMode mode;

    bool singleDevice;

#ifdef QTM_SYMBIAN_BLUETOOTH
    CSdpAgent *m_sdpAgent;
    CSdpSearchPattern *m_filter;
    CSdpAttrIdMatchList *m_attributes;
    QBluetoothServiceInfo m_serviceInfo;
    TSdpAttributeID m_currentAttributeId;

    QStack<QVariant> m_stack;
#elif defined(QTM_BLUEZ_BLUETOOTH)
    OrgBluezManagerInterface *manager;
    OrgBluezAdapterInterface *adapter;
    OrgBluezDeviceInterface *device;
#endif

protected:
    QBluetoothServiceDiscoveryAgent *q_ptr;
};

QTM_END_NAMESPACE

QT_END_HEADER

#endif
