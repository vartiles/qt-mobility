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

#include "simulatorvideorenderercontrol.h"
#include <qabstractvideosurface.h>
#include <qvideoframe.h>
#include <qvideosurfaceformat.h>
#include <QFile>
#include <QColor>
#include <QPainter>

QSimulatorVideoRendererControl::QSimulatorVideoRendererControl(QObject *parent) :
    QVideoRendererControl(parent)
        , mSurface(0)
        , mRunning(false)
{
}

QSimulatorVideoRendererControl::~QSimulatorVideoRendererControl()
{
}

QAbstractVideoSurface *QSimulatorVideoRendererControl::surface() const
{
    return mSurface;
}

void QSimulatorVideoRendererControl::setSurface(QAbstractVideoSurface *surface)
{
    mSurface = surface;
}

void QSimulatorVideoRendererControl::setImagePath(const QString &imagePath)
{
    if (QFile::exists(imagePath)) {
        mImage = QImage(imagePath);
    } else {
        mImage = QImage(800, 600, QImage::Format_RGB32);
        mImage.fill(qRgb(200, 50, 50));
        QPainter painter(&mImage);
        painter.drawText(0, 0, 800, 600, Qt::AlignCenter, imagePath);
    }
    if (mRunning)
        start();
}

void QSimulatorVideoRendererControl::start()
{
    if (!mSurface)
        return;
    stop();
    QVideoFrame::PixelFormat pixelFormat = QVideoFrame::pixelFormatFromImageFormat(mImage.format());
    if (pixelFormat == QVideoFrame::Format_Invalid) {
        mImage = mImage.convertToFormat(QImage::Format_RGB32);
        pixelFormat = QVideoFrame::Format_RGB32;
    }
    QVideoSurfaceFormat format(mImage.size(), pixelFormat);
    mSurface->start(format);
    mSurface->present(mImage);
    mRunning = true;
}

void QSimulatorVideoRendererControl::stop()
{
    if (!mSurface)
        return;

    if (mSurface->isActive())
        mSurface->stop();
    mRunning = false;
}

const QImage * QSimulatorVideoRendererControl::image() const
{
    return &mImage;
}