/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QVariant>
#include <QVariantList>
#include "qnfctagtestcommon.h"
#include <qnearfieldtagtype3.h>
#include "qnfctestcommon.h"

QTM_USE_NAMESPACE
class tst_qnearfieldtagtype3 : public QObject
{
    Q_OBJECT

public:
    tst_qnearfieldtagtype3();
    void testCommandSet();

private Q_SLOTS:
    void initTestCase();
    
    void testSmoke_data();
    void testSmoke();

    void testNdefAccess();

    void testRawCommand_data();
    void testRawCommand();
    
    void testWaitRawCommand_data();
    void testWaitRawCommand();
    
    void testMixRawCommandAndNdefAccess_data();
    void testMixRawCommandAndNdefAccess();
    
    void testWaitMixRawCommandAndNdefAccess_data();
    void testWaitMixRawCommandAndNdefAccess();
    
    void testWaitInSlot_data();
    void testWaitInSlot();

    void testDeleteTargetBeforeAsyncRequestComplete_data();
    void testDeleteTargetBeforeAsyncRequestComplete();

    void testRemoveTargetBeforeAsyncRequestComplete_data();
    void testRemoveTargetBeforeAsyncRequestComplete();

    void testCancelNdefOperation();
    void cleanupTestCase(){}
private:
    QNfcTagTestCommon<QtMobility::QNearFieldTagType3> tester;
    QMap<QString, QPair<QVariant, QVariant> > dataPool;
};


tst_qnearfieldtagtype3::tst_qnearfieldtagtype3()
{
}

void tst_qnearfieldtagtype3::initTestCase()
{
    tester.touchTarget();
    QByteArray uid = tester.target->uid();
    QVERIFY(!uid.isEmpty());

    tester.removeTarget();

    // prepare data
    {
        QString name = "RID";
        QByteArray command;
        command.append(char(0x78));     // RID
        command.append(char(0x00));     // Address (unused)
        command.append(char(0x00));     // Data (unused)
        command.append(uid.left(4));  // 4 bytes of UID
        // Hardware will append CRC bytes. The CRC value appended 
        // to the command will be ignored.
        command.append(char(0x00)); // CRC1
        command.append(char(0x00)); // CRC2
        
        dataPool.insert(name, qMakePair(QVariant(command), QVariant()));
    }

    {
        QString name = "READ ALL";
        QByteArray command;
        command.append(char(0x00)); // RALL
        command.append(char(0x00));
        command.append(char(0x00));
        command.append(uid.left(4)); // UID
        // Hardware will append CRC bytes. The CRC value appended 
        // to the command will be ignored.
        command.append(char(0x00)); // CRC1
        command.append(char(0x00)); // CRC2

        dataPool.insert(name, qMakePair(QVariant(command), QVariant()));
    }

    {
        QString name = "INVALID";
        QByteArray command;
        command.append(1);
        command.append(1);
        command.append(1);
        command.append(1);
        command.append(1);
        command.append(1);
        command.append(1);
        command.append(1);
        command.append(1);

        QVariantList cmdList;
        cmdList.append(command);
        cmdList.append(command);
        QVariantList rspList;
        rspList.append(QVariant());
        rspList.append(QVariant());

        dataPool.insert(name, qMakePair(QVariant(cmdList), QVariant(rspList)));
    }   
}

void tst_qnearfieldtagtype3::testSmoke_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}

void tst_qnearfieldtagtype3::testSmoke()
{
    tester.touchTarget();

    testCommandSet();

    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testSmoke(dsp, cmd, rsp);
    tester.removeTarget();
}

void tst_qnearfieldtagtype3::testCommandSet()
{
    QList<quint16> list1;
    list1.append(1);
    list1.append(2);

    QList<quint16> list2;
    list2.append(3);
    list2.append(280);

    QMap<quint16, QList<quint16> > map;
    map.insert(1,list1);
    map.insert(2,list2);

    QByteArray data;
    for(int i = 0; i < 4; ++i)
    {
        data.append((char)i);
    }

    QSignalSpy okSpy(tester.target, SIGNAL(requestCompleted(const QNearFieldTarget::RequestId&)));
    QSignalSpy errSpy(tester.target, SIGNAL(error(QNearFieldTarget::Error, const QNearFieldTarget::RequestId&)));
    
    int okCount = 0;
    int errCount = 0;

    QNearFieldTarget::RequestId id1 = tester.target->check(map);
    QVERIFY(id1.isValid());
    QVERIFY(!tester.target->waitForRequestCompleted(id1));
    ++errCount;

    QNearFieldTarget::RequestId id2 = tester.target->update(map, data);
    QVERIFY(id2.isValid());
    QVERIFY(!tester.target->waitForRequestCompleted(id2));
    ++errCount;

    QTRY_COMPARE(okSpy.count(), okCount);
    QTRY_COMPARE(errSpy.count(), errCount);
}    

void tst_qnearfieldtagtype3::testNdefAccess()
{
    tester.testNdefAccess();
}

void tst_qnearfieldtagtype3::testRawCommand_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}
void tst_qnearfieldtagtype3::testRawCommand()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testRawCommand(dsp, cmd, rsp); 
}

void tst_qnearfieldtagtype3::testWaitRawCommand_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}

void tst_qnearfieldtagtype3::testWaitRawCommand()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testWaitRawCommand(dsp, cmd, rsp);
}

void tst_qnearfieldtagtype3::testMixRawCommandAndNdefAccess_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}
void tst_qnearfieldtagtype3::testMixRawCommandAndNdefAccess()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testMixRawCommandAndNdefAccess(dsp, cmd, rsp);
}

void tst_qnearfieldtagtype3::testWaitMixRawCommandAndNdefAccess_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}

void tst_qnearfieldtagtype3::testWaitMixRawCommandAndNdefAccess()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testWaitMixRawCommandAndNdefAccess(dsp, cmd, rsp);
}

void tst_qnearfieldtagtype3::testWaitInSlot_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}
void tst_qnearfieldtagtype3::testWaitInSlot()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testWaitInSlot(dsp, cmd, rsp);
}

void tst_qnearfieldtagtype3::testDeleteTargetBeforeAsyncRequestComplete_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}
void tst_qnearfieldtagtype3::testDeleteTargetBeforeAsyncRequestComplete()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testDeleteTargetBeforeAsyncRequestComplete(dsp, cmd, rsp);
}

void tst_qnearfieldtagtype3::testRemoveTargetBeforeAsyncRequestComplete_data()
{
    QTest::addColumn<QStringList>("dsp");
    QTest::addColumn<QVariantList>("cmd");
    QTest::addColumn<QVariantList>("rsp");
    
    QStringList dsp;
    QVariantList cmd;
    QVariantList rsp;
    
    dsp<<"RID"<<"READ ALL"<<"INVALID";
    cmd.append(dataPool["RID"].first);
    cmd.append(dataPool["READ ALL"].first);
    cmd.append(dataPool["INVALID"].first);
    rsp.append(dataPool["RID"].second);
    rsp.append(dataPool["READ ALL"].second);
    rsp.append(dataPool["INVALID"].second);
    QTest::newRow("data 1")<<dsp<<cmd<<rsp;
}
void tst_qnearfieldtagtype3::testRemoveTargetBeforeAsyncRequestComplete()
{
    QFETCH(QStringList, dsp);
    QFETCH(QVariantList, cmd);
    QFETCH(QVariantList, rsp);
    tester.testRemoveTargetBeforeAsyncRequestComplete(dsp, cmd, rsp);
}

void tst_qnearfieldtagtype3::testCancelNdefOperation()
{
    tester.testCancelNdefOperation();
}
QTEST_MAIN(tst_qnearfieldtagtype3);

#include "tst_qnearfieldtagtype3.moc"