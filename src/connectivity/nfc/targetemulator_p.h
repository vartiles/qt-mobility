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

#ifndef TARGETEMULATOR_P_H
#define TARGETEMULATOR_P_H

#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QSettings)

class TagBase
{
public:
    TagBase();
    ~TagBase();

    virtual void load(QSettings *settings) = 0;

    virtual QByteArray processCommand(const QByteArray &command) = 0;

    virtual QByteArray uid() const = 0;

    quint64 lastAccessTime() const { return lastAccess; }

protected:
    mutable quint64 lastAccess;
};

class NfcTagType1 : public TagBase
{
public:
    NfcTagType1();
    ~NfcTagType1();

    void load(QSettings *settings);

    QByteArray processCommand(const QByteArray &command);

    QByteArray uid() const;

private:
    quint8 readData(quint8 block, quint8 byte);

    quint8 hr0;
    quint8 hr1;

    QByteArray memory;
};

class NfcTagType2 : public TagBase
{
public:
    NfcTagType2();
    ~NfcTagType2();

    void load(QSettings *settings);

    QByteArray processCommand(const QByteArray &command);

    QByteArray uid() const;

private:
    QByteArray memory;
    quint8 currentSector;
    bool expectPacket2;
};

#endif // TARGETEMULATOR_P_H