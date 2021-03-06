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

class IMyInterface 
{
    
public:
    //successful part
    Q_SIGNAL void signalFct1();
    Q_SIGNAL void signalFct2();
    Q_SIGNAL void signalFct3(long arg1, double arg2, QObject arg3);
    Q_SIGNAL void signalFct4(bool arg1, QString arg2, int arg3);

    Q_SIGNAL void signalFct5(const long& arg1, const double& arg2, const QObject& arg3);
    Q_SIGNAL void signalFct6(const bool& arg1, const QString& arg2, const int& arg3);
    Q_SIGNAL void signalFct7(const long* arg1, const double* arg2, const QObject* arg3);
    Q_SIGNAL void signalFct8(const bool* arg1, const QString* arg2, const int* arg3);
signals:
    void signalFct9(long& arg1, double& arg2, QObject& arg3);
    void signalFct10(bool& arg1, QString& arg2, int& arg3);
    void signalFct11(long* arg1, double* arg2, QObject* arg3);
    void signalFct12(bool* arg1, QString* arg2, int* arg3);

    void signalFct13(long arg1, double arg2, QObject arg3);
    void signalFct14(bool arg1, QString arg2, int arg3);
    void signalFct15(long arg1, double arg2, QObject arg3);
    void signalFct16(bool arg1, QString arg2, int arg3);

    //this is implemented in the base class
    void implementedInBase(bool arg1, QString arg2, int arg3);

    //unsuccessful  part
    Q_SIGNAL int signalFct17(bool arg1, QString arg2, int arg3); //return type
    Q_SIGNAL void diffnames1(bool arg1, QString arg2, int arg3); //name
    Q_SIGNAL void overloadedDiffFct(bool arg1, QString arg2, int arg3);//first parameter

signals;
    void noSingnal();//signal
};
