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
#include "qnmeapositioninfosource_p.h"
#include "qlocationutils_p.h"

#include <QIODevice>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QTimer>


QTM_BEGIN_NAMESPACE

QNmeaRealTimeReader::QNmeaRealTimeReader(QNmeaPositionInfoSourcePrivate *sourcePrivate)
        : QNmeaReader(sourcePrivate)
{
}

void QNmeaRealTimeReader::readAvailableData()
{
    QGeoPositionInfo update;
    bool hasFix = false;

    char buf[1024];
    qint64 size = m_proxy->m_device->readLine(buf, sizeof(buf));
    while (size > 0) {
        if (m_proxy->parsePosInfoFromNmeaData(buf, size, &update, &hasFix))
            m_proxy->notifyNewUpdate(&update, hasFix);
        memset(buf, 0, size);
        size = m_proxy->m_device->readLine(buf, sizeof(buf));
    }
}


//============================================================

QNmeaSimulatedReader::QNmeaSimulatedReader(QNmeaPositionInfoSourcePrivate *sourcePrivate)
        : QNmeaReader(sourcePrivate),
        m_currTimerId(-1),
        m_hasValidDateTime(false)
{
}

QNmeaSimulatedReader::~QNmeaSimulatedReader()
{
    if (m_currTimerId > 0)
        killTimer(m_currTimerId);
}

void QNmeaSimulatedReader::readAvailableData()
{
    if (m_currTimerId > 0)     // we are already reading
        return;

    if (!m_hasValidDateTime) {      // first update
        Q_ASSERT(m_proxy->m_device && (m_proxy->m_device->openMode() & QIODevice::ReadOnly));

        if (!setFirstDateTime()) {
            //m_proxy->notifyReachedEndOfFile();
            qWarning("QNmeaPositionInfoSource: cannot find NMEA sentence with valid date & time");
            return;
        }

        m_hasValidDateTime = true;
        simulatePendingUpdate();

    } else {
        // previously read to EOF, but now new data has arrived
        processNextSentence();
    }
}

bool QNmeaSimulatedReader::setFirstDateTime()
{
    // find the first update with valid date and time
    QGeoPositionInfo update;
    bool hasFix = false;
    while (m_proxy->m_device->bytesAvailable() > 0) {
        char buf[1024];
        qint64 size = m_proxy->m_device->readLine(buf, sizeof(buf));
        if (size <= 0)
            continue;
        bool ok = m_proxy->parsePosInfoFromNmeaData(buf, size, &update, &hasFix);
        if (ok && update.timestamp().isValid()) {
            QPendingGeoPositionInfo pending;
            pending.info = update;
            pending.hasFix = hasFix;
            m_pendingUpdates.enqueue(pending);
            return true;
        }
    }
    return false;
}

void QNmeaSimulatedReader::simulatePendingUpdate()
{
    if (m_pendingUpdates.size() > 0) {
        // will be dequeued in processNextSentence()
        QPendingGeoPositionInfo &pending = m_pendingUpdates.head();
        if (pending.info.coordinate().type() != QGeoCoordinate::InvalidCoordinate)
            m_proxy->notifyNewUpdate(&pending.info, pending.hasFix);
    }

    processNextSentence();
}

void QNmeaSimulatedReader::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    m_currTimerId = -1;
    simulatePendingUpdate();
}

void QNmeaSimulatedReader::processNextSentence()
{
    QGeoPositionInfo info;
    bool hasFix = false;
    int timeToNextUpdate = -1;
    QTime prevTime;
    if (m_pendingUpdates.size() > 0)
        prevTime = m_pendingUpdates.head().info.timestamp().time();

    // find the next update with a valid time (as long as the time is valid,
    // we can calculate when the update should be emitted)
    while (m_proxy->m_device && m_proxy->m_device->bytesAvailable() > 0) {
        char buf[1024];
        qint64 size = m_proxy->m_device->readLine(buf, sizeof(buf));
        if (size <= 0)
            continue;
        if (m_proxy->parsePosInfoFromNmeaData(buf, size, &info, &hasFix)) {
            QTime time = info.timestamp().time();
            if (time.isValid()) {
                if (!prevTime.isValid()) {
                    timeToNextUpdate = 0;
                    break;
                }
                timeToNextUpdate = prevTime.msecsTo(time);
                if (timeToNextUpdate >= 0)
                    break;
            }
        }
    }

    if (timeToNextUpdate < 0)
        return;

    m_pendingUpdates.dequeue();

    QPendingGeoPositionInfo pending;
    pending.info = info;
    pending.hasFix = hasFix;
    m_pendingUpdates.enqueue(pending);
    m_currTimerId = startTimer(timeToNextUpdate);
}


//============================================================


QNmeaPositionInfoSourcePrivate::QNmeaPositionInfoSourcePrivate(QNmeaPositionInfoSource *parent)
        : QObject(parent),
        m_invokedStart(false),
        m_source(parent),
        m_nmeaReader(0),
        m_updateTimer(0),
        m_requestTimer(0),
        m_noUpdateLastInterval(false),
        m_updateTimeoutSent(false),
        m_connectedReadyRead(false)
{
}

QNmeaPositionInfoSourcePrivate::~QNmeaPositionInfoSourcePrivate()
{
    delete m_nmeaReader;
    delete m_updateTimer;
}

bool QNmeaPositionInfoSourcePrivate::openSourceDevice()
{
    if (!m_device) {
        qWarning("QNmeaPositionInfoSource: no QIODevice data source, call setDevice() first");
        return false;
    }

    if (!m_device->isOpen() && !m_device->open(QIODevice::ReadOnly)) {
        qWarning("QNmeaPositionInfoSource: cannot open QIODevice data source");
        return false;
    }

    connect(m_device, SIGNAL(aboutToClose()), SLOT(sourceDataClosed()));
    connect(m_device, SIGNAL(readChannelFinished()), SLOT(sourceDataClosed()));
    connect(m_device, SIGNAL(destroyed()), SLOT(sourceDataClosed()));

    return true;
}

void QNmeaPositionInfoSourcePrivate::sourceDataClosed()
{
    if (m_nmeaReader && m_device && m_device->bytesAvailable())
        m_nmeaReader->readAvailableData();
}

void QNmeaPositionInfoSourcePrivate::readyRead()
{
    if (m_nmeaReader)
        m_nmeaReader->readAvailableData();
}

bool QNmeaPositionInfoSourcePrivate::initialize()
{
    if (m_nmeaReader)
        return true;

    if (!openSourceDevice())
        return false;

    if (m_updateMode == QNmeaPositionInfoSource::RealTimeMode)
        m_nmeaReader = new QNmeaRealTimeReader(this);
    else
        m_nmeaReader = new QNmeaSimulatedReader(this);

    return true;
}

void QNmeaPositionInfoSourcePrivate::prepareSourceDevice()
{
    // some data may already be available
    if (m_updateMode == QNmeaPositionInfoSource::SimulationMode) {
        if (m_nmeaReader && m_device->bytesAvailable())
            m_nmeaReader->readAvailableData();
    }

    if (!m_connectedReadyRead) {
        connect(m_device, SIGNAL(readyRead()), SLOT(readyRead()));
        m_connectedReadyRead = true;
    }
}

bool QNmeaPositionInfoSourcePrivate::parsePosInfoFromNmeaData(const char *data, int size,
        QGeoPositionInfo *posInfo, bool *hasFix)
{
    return m_source->parsePosInfoFromNmeaData(data, size, posInfo, hasFix);
}

void QNmeaPositionInfoSourcePrivate::startUpdates()
{
    if (m_invokedStart)
        return;

    m_invokedStart = true;
    m_pendingUpdate = QGeoPositionInfo();
    m_noUpdateLastInterval = false;

    bool initialized = initialize();
    if (!initialized)
        return;

    if (m_updateMode == QNmeaPositionInfoSource::RealTimeMode) {
        // skip over any buffered data - we only want the newest data
        if (m_device->bytesAvailable()) {
            if (m_device->isSequential())
                m_device->readAll();
            else
                m_device->seek(m_device->bytesAvailable());
        }
    }

    if (m_updateTimer)
        m_updateTimer->stop();

    if (m_source->updateInterval() > 0) {
        if (!m_updateTimer)
            m_updateTimer = new QBasicTimer;
        m_updateTimer->start(m_source->updateInterval(), this);
    }

    if (initialized)
        prepareSourceDevice();
}

void QNmeaPositionInfoSourcePrivate::stopUpdates()
{
    m_invokedStart = false;
    if (m_updateTimer)
        m_updateTimer->stop();
    m_pendingUpdate = QGeoPositionInfo();
    m_noUpdateLastInterval = false;
}

void QNmeaPositionInfoSourcePrivate::requestUpdate(int msec)
{
    if (m_requestTimer && m_requestTimer->isActive())
        return;

    if (msec <= 0 || msec < m_source->minimumUpdateInterval()) {
        emit m_source->updateTimeout();
        return;
    }

    if (!m_requestTimer) {
        m_requestTimer = new QTimer(this);
        connect(m_requestTimer, SIGNAL(timeout()), SLOT(updateRequestTimeout()));
    }

    bool initialized = initialize();
    if (!initialized) {
        emit m_source->updateTimeout();
        return;
    }

    m_requestTimer->start(msec);

    if (initialized)
        prepareSourceDevice();
}

void QNmeaPositionInfoSourcePrivate::updateRequestTimeout()
{
    m_requestTimer->stop();
    emit m_source->updateTimeout();
}

void QNmeaPositionInfoSourcePrivate::notifyNewUpdate(QGeoPositionInfo *update, bool hasFix)
{
    // include <QDebug> before uncommenting
    //qDebug() << "QNmeaPositionInfoSourcePrivate::notifyNewUpdate()" << update->timestamp() << hasFix << m_invokedStart << (m_requestTimer && m_requestTimer->isActive());

    QDate date = update->timestamp().date();
    if (date.isValid()) {
        m_currentDate = date;
    } else {
        // some sentence have time but no date
        QTime time = update->timestamp().time();
        if (time.isValid() && m_currentDate.isValid())
            update->setTimestamp(QDateTime(m_currentDate, time, Qt::UTC));
    }

    if (hasFix && update->isValid()) {
        if (m_requestTimer && m_requestTimer->isActive()) {
            m_requestTimer->stop();
            emitUpdated(*update);
        } else if (m_invokedStart) {
            if (m_updateTimer && m_updateTimer->isActive()) {
                // for periodic updates, only want the most recent update
                m_pendingUpdate = *update;
                if (m_noUpdateLastInterval) {
                    emitPendingUpdate();
                    m_noUpdateLastInterval = false;
                }
            } else {
                emitUpdated(*update);
            }
        }
        m_lastUpdate = *update;
    }
}

void QNmeaPositionInfoSourcePrivate::timerEvent(QTimerEvent *)
{
    emitPendingUpdate();
}

void QNmeaPositionInfoSourcePrivate::emitPendingUpdate()
{
    if (m_pendingUpdate.isValid()) {
        m_updateTimeoutSent = false;
        m_noUpdateLastInterval = false;
        emitUpdated(m_pendingUpdate);
        m_pendingUpdate = QGeoPositionInfo();
    } else {
        if (m_noUpdateLastInterval && !m_updateTimeoutSent) {
            m_updateTimeoutSent = true;
            m_pendingUpdate = QGeoPositionInfo();
            emit m_source->updateTimeout();
        }
        m_noUpdateLastInterval = true;
    }
}

void QNmeaPositionInfoSourcePrivate::emitUpdated(const QGeoPositionInfo &update)
{
    m_lastUpdate = update;
    emit m_source->positionUpdated(update);
}

//=========================================================

/*!
    \class QNmeaPositionInfoSource
    \brief The QNmeaPositionInfoSource class provides positional information using a NMEA data source.

    \inmodule QtLocation
    \since 1.0

    \ingroup location

    NMEA is a commonly used protocol for the specification of one's global
    position at a certain point in time. The QNmeaPositionInfoSource class reads NMEA
    data and uses it to provide positional data in the form of
    QGeoPositionInfo objects.

    A QNmeaPositionInfoSource instance operates in either \l {RealTimeMode} or
    \l {SimulationMode}. These modes allow NMEA data to be read from either a
    live source of positional data, or replayed for simulation purposes from
    previously recorded NMEA data.

    The source of NMEA data is set with setDevice().

    Use startUpdates() to start receiving regular position updates and stopUpdates() to stop these
    updates.  If you only require updates occasionally, you can call requestUpdate() to request a
    single update.

    In both cases the position information is received via the positionUpdated() signal and the
    last known position can be accessed with lastKnownPosition().
*/


/*!
    \enum QNmeaPositionInfoSource::UpdateMode
    Defines the available update modes.

    \value RealTimeMode Positional data is read and distributed from the data source as it becomes available. Use this mode if you are using a live source of positional data (for example, a GPS hardware device).
    \value SimulationMode The data and time information in the NMEA source data is used to provide positional updates at the rate at which the data was originally recorded. Use this mode if the data source contains previously recorded NMEA data and you want to replay the data for simulation purposes.
*/


/*!
    Constructs a QNmeaPositionInfoSource instance with the given \a parent
    and \a updateMode.
    \since 1.0
*/
QNmeaPositionInfoSource::QNmeaPositionInfoSource(UpdateMode updateMode, QObject *parent)
        : QGeoPositionInfoSource(parent),
        d(new QNmeaPositionInfoSourcePrivate(this))
{
    d->m_updateMode = updateMode;
    d->m_device = 0;
}

/*!
    Destroys the position source.
*/
QNmeaPositionInfoSource::~QNmeaPositionInfoSource()
{
    delete d;
}

/*!
    Parses an NMEA sentence string into a QGeoPositionInfo.

    The default implementation will parse standard NMEA sentences.
    This method should be reimplemented in a subclass whenever the need to deal with non-standard
    NMEA sentences arises.

    The parser reads \a size bytes from \a data and uses that information to setup \a posInfo and
    \a hasFix.  If \a hasFix is set to false then \a posInfo may contain only the time or the date
    and the time.

    Returns true if the sentence was succsesfully parsed, otherwise returns false and should not
    modifiy \a posInfo or \a hasFix.
    \since 1.0
*/
bool QNmeaPositionInfoSource::parsePosInfoFromNmeaData(const char *data, int size,
        QGeoPositionInfo *posInfo, bool *hasFix)
{
    return QLocationUtils::getPosInfoFromNmea(data, size, posInfo, hasFix);
}

/*!
    Returns the update mode.
    \since 1.0
*/
QNmeaPositionInfoSource::UpdateMode QNmeaPositionInfoSource::updateMode() const
{
    return d->m_updateMode;
}

/*!
    Sets the NMEA data source to \a device. If the device is not open, it
    will be opened in QIODevice::ReadOnly mode.

    The source device can only be set once and must be set before calling
    startUpdates() or requestUpdate().

    \bold {Note:} The \a device must emit QIODevice::readyRead() for the
    source to be notified when data is available for reading.
    QNmeaPositionInfoSource does not assume the ownership of the device,
    and hence does not deallocate it upon destruction.
    \since 1.0
*/
void QNmeaPositionInfoSource::setDevice(QIODevice *device)
{
    if (device != d->m_device) {
        if (!d->m_device)
            d->m_device = device;
        else
            qWarning("QNmeaPositionInfoSource: source device has already been set");
    }
}

/*!
    Returns the NMEA data source.
    \since 1.0
*/
QIODevice *QNmeaPositionInfoSource::device() const
{
    return d->m_device;
}

/*!
    \reimp
    \since 1.0
*/
void QNmeaPositionInfoSource::setUpdateInterval(int msec)
{
    int interval = msec;
    if (interval != 0)
        interval = qMax(msec, minimumUpdateInterval());
    QGeoPositionInfoSource::setUpdateInterval(interval);
    if (d->m_invokedStart) {
        d->stopUpdates();
        d->startUpdates();
    }
}

/*!
    \reimp
    \since 1.0
*/
void QNmeaPositionInfoSource::startUpdates()
{
    d->startUpdates();
}

/*!
    \reimp
    \since 1.0
*/
void QNmeaPositionInfoSource::stopUpdates()
{
    d->stopUpdates();
}

/*!
    \reimp
    \since 1.0
*/
void QNmeaPositionInfoSource::requestUpdate(int msec)
{
    d->requestUpdate(msec == 0 ? 60000 * 5 : msec);
}

/*!
    \reimp
    \since 1.0
*/
QGeoPositionInfo QNmeaPositionInfoSource::lastKnownPosition(bool) const
{
    // the bool value does not matter since we only use satellite positioning
    return d->m_lastUpdate;
}

/*!
    \reimp
    \since 1.0
*/
QGeoPositionInfoSource::PositioningMethods QNmeaPositionInfoSource::supportedPositioningMethods() const
{
    return SatellitePositioningMethods;
}

/*!
    \reimp
    \since 1.0
*/
int QNmeaPositionInfoSource::minimumUpdateInterval() const
{
    return 100;
}

#include "moc_qnmeapositioninfosource.cpp"
#include "moc_qnmeapositioninfosource_p.cpp"

QTM_END_NAMESPACE
