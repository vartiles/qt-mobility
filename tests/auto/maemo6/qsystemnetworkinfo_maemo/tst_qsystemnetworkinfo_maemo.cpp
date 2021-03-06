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

//TESTED_COMPONENT=src/systeminfo

#include <QtTest/QtTest>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include "qsysteminfo.h"

QTM_USE_NAMESPACE

const static int NORMAL_MODE =1;
const static int OFFLINE_MODE =0;

static QDBusInterface mceConnectionInterface("com.nokia.mce",
                                             "/com/nokia/mce/request",
                                             "com.nokia.mce.request",
                                             QDBusConnection::systemBus());

/**
 * Starts an event loop that runs until the given signal is received.
 * Optionally the event loop can return earlier on a timeout.
 *
 * \return \p true if the requested signal was received
 *         \p false on timeout
 */
static bool waitForSignal(QObject *obj, const char *signal, int timeout = 0)
{
    QEventLoop loop;
    QObject::connect(obj, signal, &loop, SLOT(quit()));
    QTimer timer;
    QSignalSpy timeoutSpy(&timer, SIGNAL(timeout()));
    if (timeout > 0) {
        QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.setSingleShot(true);
        timer.start(timeout);
    }
    loop.exec();
    return timeoutSpy.isEmpty();
}

static void log(QString message)
{
    qDebug() << QDateTime::currentDateTime().toTime_t() << " " << message;
}

static void setDeviceMode(int deviceMode)
{
    //"req_radio_states_change"
   QDBusMessage msg = mceConnectionInterface.call("req_radio_states_change", deviceMode,1);
//   qDebug() << Q_FUNC_INFO << msg.errorMessage();
}

class DropNetworkThread : public QThread
{
public:
    void run()
    {
        log("DropNetworkThread: Waiting 2s before dropping the cellular network");
        sleep(2);
        setDeviceMode(OFFLINE_MODE);
        log("DropNetworkThread: Cellular network dropped");
    }
};

class tst_QSystemNetworkInfo : public QObject
{
    Q_OBJECT

public:
    tst_QSystemNetworkInfo();
    virtual ~tst_QSystemNetworkInfo();

public slots:
    void init();
    void cleanup();

private slots:
    void networkSignalStrengthChanged();
    void networkStatusChanged();
    void mobileCountryCodeChanged();
    void mobileNetworkCodeChanged();
    void networkNameChanged();
    void networkModeChanged();

private:
    QSystemNetworkInfo *m_systemNetworkInfo;
    DropNetworkThread *m_dropNetworkThread;
};

tst_QSystemNetworkInfo::tst_QSystemNetworkInfo()
{
    m_dropNetworkThread = new DropNetworkThread();
    m_systemNetworkInfo = new QSystemNetworkInfo();
}

tst_QSystemNetworkInfo::~tst_QSystemNetworkInfo()
{
    delete m_dropNetworkThread, m_dropNetworkThread = 0;
    delete m_systemNetworkInfo, m_systemNetworkInfo = 0;
}

/*
 * Called before each test case
 */
void tst_QSystemNetworkInfo::init()
{
    log("init: setDeviceMode: normal");
    setDeviceMode(NORMAL_MODE);
    QTest::qWait(2000);
    log("init done");
}

/*
 * Called after each test case
 */
void tst_QSystemNetworkInfo::cleanup()
{
    QTest::qWait(2000);
    log("cleanup: setDeviceMode: normal");
    setDeviceMode(NORMAL_MODE);
    QTest::qWait(2000);
    log("cleanup done");
}

/*
 * The test cases
 */
void tst_QSystemNetworkInfo::networkSignalStrengthChanged()
{
    m_dropNetworkThread->start();

    log("Waiting 60s for network signal strength to change");
    QVERIFY(::waitForSignal(m_systemNetworkInfo, SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)), 60 * 1000));
    log("networkSignalStrengthChanged");
}

void tst_QSystemNetworkInfo::networkStatusChanged()
{
    m_dropNetworkThread->start();

    log("Waiting 60s for network status to change");
    QVERIFY(::waitForSignal(m_systemNetworkInfo, SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)), 60 * 1000));
    log("networkStatusChanged");
}

void tst_QSystemNetworkInfo::mobileCountryCodeChanged()
{
    m_dropNetworkThread->start();

    log("Waiting 60s for mobile country code to change");
    QVERIFY(::waitForSignal(m_systemNetworkInfo, SIGNAL(currentMobileCountryCodeChanged(QString)), 60 * 1000));
    log("currentMobileCountryCodeChanged");
}

void tst_QSystemNetworkInfo::mobileNetworkCodeChanged()
{
    m_dropNetworkThread->start();

    log("Waiting 60s for mobile network code to change");
    QVERIFY(::waitForSignal(m_systemNetworkInfo, SIGNAL(currentMobileNetworkCodeChanged(QString)), 60 * 1000));
    log("currentMobileNetworkCodeChanged");
}

void tst_QSystemNetworkInfo::networkNameChanged()
{
    m_dropNetworkThread->start();

    log("Waiting 60s for network name to change");
    QVERIFY(::waitForSignal(m_systemNetworkInfo, SIGNAL(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)), 60 * 1000));
    log("networkNameChanged");
}

void tst_QSystemNetworkInfo::networkModeChanged()
{
    m_dropNetworkThread->start();

    log("Waiting 60s for network mode to change");
    QVERIFY(::waitForSignal(m_systemNetworkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)), 60 * 1000));
    log("networkModeChanged");
}

QTEST_MAIN(tst_QSystemNetworkInfo)
#include "tst_qsystemnetworkinfo_maemo.moc"
