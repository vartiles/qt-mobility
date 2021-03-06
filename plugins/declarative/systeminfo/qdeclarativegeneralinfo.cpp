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


#include "qdeclarativegeneralinfo_p.h"
#include "qsystemgeneralinfo.h"
#include <QMetaType>
#include <QDebug>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QSystemInfo, generalInfo)


/*!
    \qmlclass GeneralInfo QDeclarativeGeneralInfo
    \brief The GeneralInfo element allows you access to general system information and to receive notifications from the device.

    \inherits QSystemInfo

    \ingroup qml-systeminfo
    \since Mobility 1.1

    This element is part of the \bold{QtMobility.systeminfo 1.1} module.
    It is a convience class to make QML usage easier.

    Note: To use notification signals, you need to use the start* slots.


    \qml

        Component.onCompleted {
            generalInfo.startCurrentLanguageChanged()
        }

    \endqml

\sa QSystemInfo
*/


/*!
    \qmlsignal GeneralInfo::currentLanguageChanged(string language)
    \since Mobility 1.1

    This handler is called when current system language has changed.
    Note: To receive this notification, you must first call \a startCurrentLanguageChanged.
*/
QDeclarativeGeneralInfo::QDeclarativeGeneralInfo(QObject *parent) :
    QSystemInfo(parent)
{
}

/*!
    \qmlmethod void GeneralInfo::startCurrentLanguageChanged()
    \since Mobility 1.1

    Start the connection for the currentLanguageChanged signal.
*/
void QDeclarativeGeneralInfo::startCurrentLanguageChanged()
{
    connect(generalInfo(),SIGNAL(currentLanguageChanged(QString)),
            this,SLOT(declarativeCurrentLanguageChanged(QString)),Qt::UniqueConnection);
}


void QDeclarativeGeneralInfo::declarativeCurrentLanguageChanged(const QString &language)
{
    Q_EMIT currentLanguageChanged(language);
}

/*!
  \qmlproperty string GeneralInfo::osVersion
  \since Mobility 1.1

   Returns the version of the Operating System.
*/
QString QDeclarativeGeneralInfo::osVersion()
{
    return generalInfo()->version(QSystemInfo::Os);
}

/*!
  \qmlproperty string GeneralInfo::qtCoreVersion
  \since Mobility 1.1

   Returns the version of the installed Qt Core library.
*/
QString QDeclarativeGeneralInfo::qtCoreVersion()
{
    return generalInfo()->version(QSystemInfo::QtCore);
}

/*!
  \qmlproperty string GeneralInfo::firmwareVersion
  \since Mobility 1.1

   Returns the version of the firmware as a whole.
*/
QString QDeclarativeGeneralInfo::firmwareVersion()
{
    return generalInfo()->version(QSystemInfo::Firmware);
}

/*!
  \qmlproperty string GeneralInfo::qtMobilityVersion
  \since Mobility 1.1

   Returns the version of the installed Qt Mobility library.
*/
QString QDeclarativeGeneralInfo::qtMobilityVersion()
{
    return generalInfo()->version(QSystemInfo::QtMobility);
}

/*!
    \qmlproperty string GeneralInfo::currentLanguage
    \since Mobility 1.1
    Returns the current language in two letter ISO 639-1 format.
 */

/*!
    \qmlproperty list GeneralInfo::availableLanguages
    \since Mobility 1.1
    Returns a QStringList of available Qt language translations in two letter ISO 639-1 format.
    If the Qt translations cannot be found, returns the current system language.
*/

/*!
    \qmlproperty string GeneralInfo::currentCountryCode
    \since Mobility 1.1
    Returns the two letter ISO 3166-1 for the current country code.
*/

