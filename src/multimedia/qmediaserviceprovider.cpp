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

#include <QtCore/qdebug.h>
#include <QtCore/qmap.h>

#include "qmediaservice.h"
#include "qmediaserviceprovider.h"
#include "qmediaserviceproviderplugin.h"
#include "qmediapluginloader_p.h"
#include "qmediaplayer.h"

QT_BEGIN_NAMESPACE

class QMediaServiceProviderHintPrivate : public QSharedData
{
public:
    QMediaServiceProviderHintPrivate(QMediaServiceProviderHint::Type type)
        :type(type), features(0)
    {
    }

    QMediaServiceProviderHintPrivate(const QMediaServiceProviderHintPrivate &other)
        :QSharedData(other),
        type(other.type),
        device(other.device),
        mimeType(other.mimeType),
        codecs(other.codecs),
        features(other.features)
    {
    }

    ~QMediaServiceProviderHintPrivate()
    {
    }

    QMediaServiceProviderHint::Type type;
    QByteArray device;
    QString mimeType;
    QStringList codecs;
    QMediaServiceProviderHint::Features features;
};

/*!
    \class QMediaServiceProviderHint

    \brief The QMediaServiceProviderHint class describes what is required of a QMediaService.

    \inmodule QtMultimediaKit
    \ingroup multimedia-serv
    \since 1.0

    The QMediaServiceProvider class uses hints to select an appropriate media service.
*/

/*!
    \enum QMediaServiceProviderHint::Feature

    Enumerates features a media service may provide.

    \value LowLatencyPlayback
            The service is expected to play simple audio formats,
            but playback should start without significant delay.
            Such playback service can be used for beeps, ringtones, etc.

    \value RecordingSupport
            The service provides audio or video recording functions.

    \value StreamPlayback
            The service is capable of playing QIODevice based streams.

    \value VideoSurface
            The service is capable of renderering to a QAbstractVideoSurface
            output.
*/

/*!
    \enum QMediaServiceProviderHint::Type

    Enumerates the possible types of media service provider hint.

    \value Null               En empty hint, use the default service.
    \value ContentType        Select media service most suitable for certain content type.
    \value Device             Select media service which supports certain device.
    \value SupportedFeatures  Select media service supporting the set of optional features.
*/


/*!
    Constructs an empty media service provider hint.
*/
QMediaServiceProviderHint::QMediaServiceProviderHint()
    :d(new QMediaServiceProviderHintPrivate(Null))
{
}

/*!
    Constructs a ContentType media service provider hint.

    This type of hint describes a service that is able to play content of a specific MIME \a type
    encoded with one or more of the listed \a codecs.
    \since 1.0
*/
QMediaServiceProviderHint::QMediaServiceProviderHint(const QString &type, const QStringList& codecs)
    :d(new QMediaServiceProviderHintPrivate(ContentType))
{
    d->mimeType = type;
    d->codecs = codecs;
}

/*!
  Constructs a Device media service provider hint.

  This type of hint describes a media service that utilizes a specific \a device.
  \since 1.0
*/
QMediaServiceProviderHint::QMediaServiceProviderHint(const QByteArray &device)
    :d(new QMediaServiceProviderHintPrivate(Device))
{
    d->device = device;
}

/*!
    Constructs a SupportedFeatures media service provider hint.

    This type of hint describes a service which supports a specific set of \a features.
    \since 1.0
*/
QMediaServiceProviderHint::QMediaServiceProviderHint(QMediaServiceProviderHint::Features features)
    :d(new QMediaServiceProviderHintPrivate(SupportedFeatures))
{
    d->features = features;
}

/*!
    Constructs a copy of the media service provider hint \a other.
    \since 1.0
*/
QMediaServiceProviderHint::QMediaServiceProviderHint(const QMediaServiceProviderHint &other)
    :d(other.d)
{
}

/*!
    Destroys a media service provider hint.
*/
QMediaServiceProviderHint::~QMediaServiceProviderHint()
{
}

/*!
    Assigns the value \a other to a media service provider hint.
    \since 1.0
*/
QMediaServiceProviderHint& QMediaServiceProviderHint::operator=(const QMediaServiceProviderHint &other)
{
    d = other.d;
    return *this;
}

/*!
    Identifies if \a other is of equal value to a media service provider hint.

    Returns true if the hints are equal, and false if they are not.
    \since 1.0
*/
bool QMediaServiceProviderHint::operator == (const QMediaServiceProviderHint &other) const
{
    return (d == other.d) ||
           (d->type == other.d->type &&
            d->device == other.d->device &&
            d->mimeType == other.d->mimeType &&
            d->codecs == other.d->codecs &&
            d->features == other.d->features);
}

/*!
    Identifies if \a other is not of equal value to a media service provider hint.

    Returns true if the hints are not equal, and false if they are.
    \since 1.0
*/
bool QMediaServiceProviderHint::operator != (const QMediaServiceProviderHint &other) const
{
    return !(*this == other);
}

/*!
    Returns true if a media service provider is null.
    \since 1.0
*/
bool QMediaServiceProviderHint::isNull() const
{
    return d->type == Null;
}

/*!
    Returns the type of a media service provider hint.
    \since 1.0
*/
QMediaServiceProviderHint::Type QMediaServiceProviderHint::type() const
{
    return d->type;
}

/*!
    Returns the mime type of the media a service is expected to be able play.
    \since 1.0
*/
QString QMediaServiceProviderHint::mimeType() const
{
    return d->mimeType;
}

/*!
    Returns a list of codes a media service is expected to be able to decode.
    \since 1.0
*/
QStringList QMediaServiceProviderHint::codecs() const
{
    return d->codecs;
}

/*!
    Returns the name of a device a media service is expected to utilize.
    \since 1.0
*/
QByteArray QMediaServiceProviderHint::device() const
{
    return d->device;
}

/*!
    Returns a set of features a media service is expected to provide.
    \since 1.0
*/
QMediaServiceProviderHint::Features QMediaServiceProviderHint::features() const
{
    return d->features;
}


Q_GLOBAL_STATIC_WITH_ARGS(QMediaPluginLoader, loader,
        (QMediaServiceProviderFactoryInterface_iid, QLatin1String("mediaservice"), Qt::CaseInsensitive))


class QPluginServiceProvider : public QMediaServiceProvider
{
    QMap<QMediaService*, QMediaServiceProviderPlugin*> pluginMap;

public:
    QMediaService* requestService(const QByteArray &type, const QMediaServiceProviderHint &hint)
    {
        QString key(type);

        QList<QMediaServiceProviderPlugin *>plugins;
        foreach (QObject *obj, loader()->instances(key)) {
            QMediaServiceProviderPlugin *plugin =
                qobject_cast<QMediaServiceProviderPlugin*>(obj);
            if (plugin)
                plugins << plugin;
        }

        if (!plugins.isEmpty()) {
            QMediaServiceProviderPlugin *plugin = 0;

            switch (hint.type()) {
            case QMediaServiceProviderHint::Null:
                plugin = plugins[0];
                //special case for media player, if low latency was not asked,
                //prefer services not offering it, since they are likely to support
                //more formats
                if (type == QByteArray(Q_MEDIASERVICE_MEDIAPLAYER)) {
                    foreach (QMediaServiceProviderPlugin *currentPlugin, plugins) {
                        QMediaServiceFeaturesInterface *iface =
                                qobject_cast<QMediaServiceFeaturesInterface*>(currentPlugin);

                        if (!iface || !(iface->supportedFeatures(type) &
                                        QMediaServiceProviderHint::LowLatencyPlayback)) {
                            plugin = currentPlugin;
                            break;
                        }

                    }
                }
                break;
            case QMediaServiceProviderHint::SupportedFeatures:
                plugin = plugins[0];
                foreach (QMediaServiceProviderPlugin *currentPlugin, plugins) {
                    QMediaServiceFeaturesInterface *iface =
                            qobject_cast<QMediaServiceFeaturesInterface*>(currentPlugin);

                    if (iface) {
                        if ((iface->supportedFeatures(type) & hint.features()) == hint.features()) {
                            plugin = currentPlugin;
                            break;
                        }
                    }
                }
                break;
            case QMediaServiceProviderHint::Device: {
                    foreach (QMediaServiceProviderPlugin *currentPlugin, plugins) {
                        QMediaServiceSupportedDevicesInterface *iface =
                                qobject_cast<QMediaServiceSupportedDevicesInterface*>(currentPlugin);

                        if (!iface) {
                            // the plugin may support the device,
                            // but this choice still can be overridden
                            plugin = currentPlugin;
                        } else {
                            if (iface->devices(type).contains(hint.device())) {
                                plugin = currentPlugin;
                                break;
                            }
                        }
                    }
                }
                break;
            case QMediaServiceProviderHint::ContentType: {
                    QtMultimediaKit::SupportEstimate estimate = QtMultimediaKit::NotSupported;
                    foreach (QMediaServiceProviderPlugin *currentPlugin, plugins) {
                        QtMultimediaKit::SupportEstimate currentEstimate = QtMultimediaKit::MaybeSupported;
                        QMediaServiceSupportedFormatsInterface *iface =
                                qobject_cast<QMediaServiceSupportedFormatsInterface*>(currentPlugin);

                        if (iface)
                            currentEstimate = iface->hasSupport(hint.mimeType(), hint.codecs());

                        if (currentEstimate > estimate) {
                            estimate = currentEstimate;
                            plugin = currentPlugin;

                            if (currentEstimate == QtMultimediaKit::PreferredService)
                                break;
                        }
                    }
                }
                break;
            }

            if (plugin != 0) {
                QMediaService *service = plugin->create(key);
                if (service != 0)
                    pluginMap.insert(service, plugin);

                return service;
            }
        }

        qWarning() << "defaultServiceProvider::requestService(): no service found for -" << key;
        return 0;
    }

    void releaseService(QMediaService *service)
    {
        if (service != 0) {
            QMediaServiceProviderPlugin *plugin = pluginMap.take(service);

            if (plugin != 0)
                plugin->release(service);
        }
    }

    QtMultimediaKit::SupportEstimate hasSupport(const QByteArray &serviceType,
                                     const QString &mimeType,
                                     const QStringList& codecs,
                                     int flags) const
    {
        QList<QObject*> instances = loader()->instances(serviceType);

        if (instances.isEmpty())
            return QtMultimediaKit::NotSupported;

        bool allServicesProvideInterface = true;
        QtMultimediaKit::SupportEstimate supportEstimate = QtMultimediaKit::NotSupported;

        foreach(QObject *obj, instances) {
            QMediaServiceSupportedFormatsInterface *iface =
                    qobject_cast<QMediaServiceSupportedFormatsInterface*>(obj);


            if (flags) {
                QMediaServiceFeaturesInterface *iface =
                        qobject_cast<QMediaServiceFeaturesInterface*>(obj);

                if (iface) {
                    QMediaServiceProviderHint::Features features = iface->supportedFeatures(serviceType);

                    //if low latency playback was asked, skip services known
                    //not to provide low latency playback
                    if ((flags & QMediaPlayer::LowLatency) &&
                        !(features & QMediaServiceProviderHint::LowLatencyPlayback))
                            continue;

                    //the same for QIODevice based streams support
                    if ((flags & QMediaPlayer::StreamPlayback) &&
                        !(features & QMediaServiceProviderHint::StreamPlayback))
                            continue;
                }
            }

            if (iface)
                supportEstimate = qMax(supportEstimate, iface->hasSupport(mimeType, codecs));
            else
                allServicesProvideInterface = false;
        }

        //don't return PreferredService
        supportEstimate = qMin(supportEstimate, QtMultimediaKit::ProbablySupported);

        //Return NotSupported only if no services are available of serviceType
        //or all the services returned NotSupported, otherwise return at least MaybeSupported
        if (!allServicesProvideInterface)
            supportEstimate = qMax(QtMultimediaKit::MaybeSupported, supportEstimate);

        return supportEstimate;
    }

    QStringList supportedMimeTypes(const QByteArray &serviceType, int flags) const
    {
        QList<QObject*> instances = loader()->instances(serviceType);

        QStringList supportedTypes;

        foreach(QObject *obj, instances) {
            QMediaServiceSupportedFormatsInterface *iface =
                    qobject_cast<QMediaServiceSupportedFormatsInterface*>(obj);


            if (flags) {
                QMediaServiceFeaturesInterface *iface =
                        qobject_cast<QMediaServiceFeaturesInterface*>(obj);

                if (iface) {
                    QMediaServiceProviderHint::Features features = iface->supportedFeatures(serviceType);

                    // If low latency playback was asked for, skip MIME types from services known
                    // not to provide low latency playback
                    if ((flags & QMediaPlayer::LowLatency) &&
                        !(features & QMediaServiceProviderHint::LowLatencyPlayback))
                        continue;

                    //the same for QIODevice based streams support
                    if ((flags & QMediaPlayer::StreamPlayback) &&
                        !(features & QMediaServiceProviderHint::StreamPlayback))
                            continue;

                    //the same for QAbstractVideoSurface support
                    if ((flags & QMediaPlayer::VideoSurface) &&
                        !(features & QMediaServiceProviderHint::VideoSurface))
                            continue;
                }
            }

            if (iface) {
                supportedTypes << iface->supportedMimeTypes();
            }
        }

        // Multiple services may support the same MIME type
        supportedTypes.removeDuplicates();

        return supportedTypes;
    }

    QList<QByteArray> devices(const QByteArray &serviceType) const
    {
        QList<QByteArray> res;

        foreach(QObject *obj, loader()->instances(serviceType)) {
            QMediaServiceSupportedDevicesInterface *iface =
                    qobject_cast<QMediaServiceSupportedDevicesInterface*>(obj);

            if (iface) {
                res.append(iface->devices(serviceType));
            }
        }

        return res;
    }

    QString deviceDescription(const QByteArray &serviceType, const QByteArray &device)
    {
        foreach(QObject *obj, loader()->instances(serviceType)) {
            QMediaServiceSupportedDevicesInterface *iface =
                    qobject_cast<QMediaServiceSupportedDevicesInterface*>(obj);

            if (iface) {
                if (iface->devices(serviceType).contains(device))
                    return iface->deviceDescription(serviceType, device);
            }
        }

        return QString();
    }
};

Q_GLOBAL_STATIC(QPluginServiceProvider, pluginProvider);

/*!
    \class QMediaServiceProvider

    \brief The QMediaServiceProvider class provides an abstract allocator for media services.
    \inmodule QtMultimediaKit
    \ingroup multimedia-serv
    \since 1.0
*/

/*!
    \fn QMediaServiceProvider::requestService(const QByteArray &type, const QMediaServiceProviderHint &hint)

    Requests an instance of a \a type service which best matches the given \a
    hint.

    Returns a pointer to the requested service, or a null pointer if there is
    no suitable service.

    The returned service must be released with releaseService when it is
    finished with.
    \since 1.0
*/

/*!
    \fn QMediaServiceProvider::releaseService(QMediaService *service)

    Releases a media \a service requested with requestService().
    \since 1.0
*/

/*!
    \fn QtMultimediaKit::SupportEstimate QMediaServiceProvider::hasSupport(const QByteArray &serviceType, const QString &mimeType, const QStringList& codecs, int flags) const

    Returns how confident a media service provider is that is can provide a \a
    serviceType service that is able to play media of a specific \a mimeType
    that is encoded using the listed \a codecs while adhering to constraints
    identified in \a flags.
    \since 1.0
*/
QtMultimediaKit::SupportEstimate QMediaServiceProvider::hasSupport(const QByteArray &serviceType,
                                                        const QString &mimeType,
                                                        const QStringList& codecs,
                                                        int flags) const
{
    Q_UNUSED(serviceType);
    Q_UNUSED(mimeType);
    Q_UNUSED(codecs);
    Q_UNUSED(flags);

    return QtMultimediaKit::MaybeSupported;
}

/*!
    \fn QStringList QMediaServiceProvider::supportedMimeTypes(const QByteArray &serviceType, int flags) const

    Returns a list of MIME types supported by the service provider for the
    specified \a serviceType.

    The resultant list is restricted to MIME types which can be supported given
    the constraints in \a flags.
    \since 1.0
*/
QStringList QMediaServiceProvider::supportedMimeTypes(const QByteArray &serviceType, int flags) const
{
    Q_UNUSED(serviceType);
    Q_UNUSED(flags);

    return QStringList();
}

/*!
  Returns the list of devices related to \a service type.
  \since 1.0
*/
QList<QByteArray> QMediaServiceProvider::devices(const QByteArray &service) const
{
    Q_UNUSED(service);
    return QList<QByteArray>();
}

/*!
    Returns the description of \a device related to \a serviceType, suitable for use by
    an application for display.
    \since 1.0
*/
QString QMediaServiceProvider::deviceDescription(const QByteArray &serviceType, const QByteArray &device)
{
    Q_UNUSED(serviceType);
    Q_UNUSED(device);
    return QString();
}


#ifdef QTM_BUILD_UNITTESTS

static QMediaServiceProvider *qt_defaultMediaServiceProvider = 0;

/*!
    Sets a media service \a provider as the default.

    \internal
    \since 1.0
*/
void QMediaServiceProvider::setDefaultServiceProvider(QMediaServiceProvider *provider)
{
    qt_defaultMediaServiceProvider = provider;
}

#endif

/*!
    Returns a default provider of media services.
*/
QMediaServiceProvider *QMediaServiceProvider::defaultServiceProvider()
{
#ifdef QTM_BUILD_UNITTESTS
    return qt_defaultMediaServiceProvider != 0
            ? qt_defaultMediaServiceProvider
            : static_cast<QMediaServiceProvider *>(pluginProvider());
#else
    return pluginProvider();
#endif
}

/*!
    \class QMediaServiceProviderPlugin

    \brief The QMediaServiceProviderPlugin class interface provides an interface for QMediaService
    plug-ins.
    \inmodule QtMultimediaKit
    \since 1.0

    A media service provider plug-in may implement one or more of
    QMediaServiceSupportedFormatsInterface,
    QMediaServiceSupportedDevicesInterface, and QMediaServiceFeaturesInterface
    to identify the features it supports.
*/

/*!
    \fn QMediaServiceProviderPlugin::keys() const

    Returns a list of keys for media services a plug-in can create.
    \since 1.0
*/

/*!
    \fn QMediaServiceProviderPlugin::create(const QString &key)

    Constructs a new instance of the QMediaService identified by \a key.

    The QMediaService returned must be destroyed with release().
    \since 1.0
*/

/*!
    \fn QMediaServiceProviderPlugin::release(QMediaService *service)

    Destroys a media \a service constructed with create().
    \since 1.0
*/


/*!
    \class QMediaServiceSupportedFormatsInterface
    \brief The QMediaServiceSupportedFormatsInterface class interface
    identifies if a media service plug-in supports a media format.
    \inmodule QtMultimediaKit
    \since 1.0

    A QMediaServiceProviderPlugin may implement this interface.
*/

/*!
    \fn QMediaServiceSupportedFormatsInterface::~QMediaServiceSupportedFormatsInterface()

    Destroys a media service supported formats interface.
*/

/*!
    \fn QMediaServiceSupportedFormatsInterface::hasSupport(const QString &mimeType, const QStringList& codecs) const

    Returns the level of support a media service plug-in has for a \a mimeType
    and set of \a codecs.
    \since 1.0
*/

/*!
    \fn QMediaServiceSupportedFormatsInterface::supportedMimeTypes() const

    Returns a list of MIME types supported by the media service plug-in.
    \since 1.0
*/

/*!
    \class QMediaServiceSupportedDevicesInterface
    \brief The QMediaServiceSupportedDevicesInterface class interface
    identifies the devices supported by a media service plug-in.
    \inmodule QtMultimediaKit
    \since 1.0

    A QMediaServiceProviderPlugin may implement this interface.
*/

/*!
    \fn QMediaServiceSupportedDevicesInterface::~QMediaServiceSupportedDevicesInterface()

    Destroys a media service supported devices interface.
*/

/*!
    \fn QMediaServiceSupportedDevicesInterface::devices(const QByteArray &service) const

    Returns a list of devices supported by a plug-in \a service.
    \since 1.0
*/

/*!
    \fn QMediaServiceSupportedDevicesInterface::deviceDescription(const QByteArray &service, const QByteArray &device)

    Returns a description of a \a device supported by a plug-in \a service.
    \since 1.0
*/

/*!
    \class QMediaServiceFeaturesInterface
    \brief The QMediaServiceFeaturesInterface class interface identifies
    features supported by a media service plug-in.
    \inmodule QtMultimediaKit
    \since 1.0

    A QMediaServiceProviderPlugin may implement this interface.
*/

/*!
    \fn QMediaServiceFeaturesInterface::~QMediaServiceFeaturesInterface()

    Destroys a media service features interface.
*/
/*!
    \fn QMediaServiceFeaturesInterface::supportedFeatures(const QByteArray &service) const

    Returns a set of features supported by a plug-in \a service.
    \since 1.0
*/

#include "moc_qmediaserviceprovider.cpp"
#include "moc_qmediaserviceproviderplugin.cpp"
QT_END_NAMESPACE

