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

#ifndef QMEDIASLIDESHOWSERVICE_P_H
#define QMEDIASLIDESHOWSERVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qmobilityglobal.h>
#include <qmediaservice.h>
#include <qmediaimageviewer.h>
#include <qvideorenderercontrol.h>
#include <qvideowidget.h>
#include <qvideowidgetcontrol.h>

#include <QtCore/qpointer.h>
#include <QtGui/qimage.h>

QT_BEGIN_NAMESPACE
class QAbstractVideoSurface;
class QNetworkAccessManager;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE

class QMediaImageViewerServicePrivate;

class QM_AUTOTEST_EXPORT QMediaImageViewerService : public QMediaService
{
    Q_OBJECT
public:
    explicit QMediaImageViewerService(QObject *parent = 0);
    ~QMediaImageViewerService();

    QMediaControl *requestControl(const char *name);
    void releaseControl(QMediaControl *);

    QNetworkAccessManager *networkManager() const;
    void setNetworkManager(QNetworkAccessManager *manager);

private:
    Q_DECLARE_PRIVATE(QMediaImageViewerService)
    friend class QMediaImageViewerControl;
    friend class QMediaImageViewerControlPrivate;
};

class QMediaImageViewerControlPrivate;

class QMediaImageViewerControl : public QMediaControl
{
    Q_OBJECT
public:
    explicit QMediaImageViewerControl(QMediaImageViewerService *parent);
    ~QMediaImageViewerControl();

    QMediaImageViewer::MediaStatus mediaStatus() const;

    void showMedia(const QMediaContent &media);

Q_SIGNALS:
    void mediaStatusChanged(QMediaImageViewer::MediaStatus status);

private:
    Q_DECLARE_PRIVATE(QMediaImageViewerControl)
    Q_PRIVATE_SLOT(d_func(), void _q_headFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_getFinished())
};

#define QMediaImageViewerControl_iid "com.nokia.Qt.QMediaImageViewerControl/1.0"
Q_MEDIA_DECLARE_CONTROL(QMediaImageViewerControl, QMediaImageViewerControl_iid)

class QMediaImageViewerRenderer : public QVideoRendererControl
{
    Q_OBJECT
public:
    QMediaImageViewerRenderer(QObject *parent = 0);
    ~QMediaImageViewerRenderer();

    QAbstractVideoSurface *surface() const;
    void setSurface(QAbstractVideoSurface *surface);

    void showImage(const QImage &image);

Q_SIGNALS:
    void surfaceChanged(QAbstractVideoSurface *surface);

private:
    QPointer<QAbstractVideoSurface> m_surface;
    QImage m_image;
};

QT_END_NAMESPACE

#endif
