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

#ifndef ICONNECTIONCONTACTINFO_H
#define ICONNECTIONCONTACTINFO_H

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

#include "maemo/interfaces/itypes.h"
#include "maemo/interfaces/iabstract.h"
#include "maemo/interfaces/iconnection.h"
#include "maemo/dbusproxy.h"

namespace DBus
{
    namespace Interfaces
    {
        class IConnectionContactInfo : public DBus::IAbstract
        {
            Q_OBJECT

        public:
            static inline const char *staticInterfaceName()
            {
                return "org.freedesktop.Telepathy.Connection.Interface.ContactInfo";
            }

            IConnectionContactInfo(
                const QString& busName,
                const QString& objectPath,
                QObject* parent = 0
            );
            IConnectionContactInfo(
                const QDBusConnection& connection,
                const QString& busName,
                const QString& objectPath,
                QObject* parent = 0
            );
            IConnectionContactInfo(DBus::DBusProxy *proxy);
            explicit IConnectionContactInfo(const DBus::Interfaces::IConnection& mainInterface);
            IConnectionContactInfo(const DBus::Interfaces::IConnection& mainInterface, QObject* parent);

            Q_PROPERTY(uint ContactInfoFlags READ ContactInfoFlags)
            inline uint ContactInfoFlags() const
            {
                return qvariant_cast<uint>(internalPropGet("ContactInfoFlags"));
            }

            Q_PROPERTY(DBus::Interfaces::FieldSpecs SupportedFields READ SupportedFields)
            inline DBus::Interfaces::FieldSpecs SupportedFields() const
            {
                return qvariant_cast<DBus::Interfaces::FieldSpecs>(internalPropGet("SupportedFields"));
            }

        public Q_SLOTS:
            inline QDBusPendingReply<DBus::Interfaces::ContactInfoMap> GetContactInfo(const DBus::Interfaces::UIntList& contacts)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<DBus::Interfaces::ContactInfoMap>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(contacts);
                return asyncCallWithArgumentList(QLatin1String("GetContactInfo"), argumentList);
            }
            inline QDBusPendingReply<> RefreshContactInfo(const DBus::Interfaces::UIntList& contacts)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(contacts);
                return asyncCallWithArgumentList(QLatin1String("RefreshContactInfo"), argumentList);
            }
            inline QDBusPendingReply<DBus::Interfaces::ContactInfoFieldList> RequestContactInfo(uint contact)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<DBus::Interfaces::ContactInfoFieldList>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(contact);
                return asyncCallWithArgumentList(QLatin1String("RequestContactInfo"), argumentList);
            }
            inline QDBusPendingReply<> SetContactInfo(const DBus::Interfaces::ContactInfoFieldList& contactInfo)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(contactInfo);
                return asyncCallWithArgumentList(QLatin1String("SetContactInfo"), argumentList);
            }

        Q_SIGNALS:
            void ContactInfoChanged(uint contact, const DBus::Interfaces::ContactInfoFieldList& contactInfo);

        protected:
            virtual void invalidate(DBus::DBusProxy *, const QString &, const QString &);
        };
    }//Interfaces
}//DBus
Q_DECLARE_METATYPE(DBus::Interfaces::IConnectionContactInfo*)

#endif //ICONNECTIONCONTACTINFO_H