/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef QORGANIZERITEMMANAGERENGINE_H
#define QORGANIZERITEMMANAGERENGINE_H

#include <QSharedData>
#include <QMap>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QObject>

#include "qorganizeritem.h"
#include "qorganizeritemdetaildefinition.h"
#include "qorganizeritemmanager.h"
#include "qorganizeritemabstractrequest.h"
#include "qorganizeritemrequests.h"
#include "qorganizeritemfetchhint.h"

QTM_BEGIN_NAMESPACE

class QOrganizerItemFilter;
class QOrganizerItemSortOrder;

class Q_CONTACTS_EXPORT QOrganizerItemManagerEngine : public QObject
{
    Q_OBJECT

public:
    QOrganizerItemManagerEngine() {}

    /* URI reporting */
    virtual QString managerName() const = 0;                       // e.g. "Symbian"
    virtual QMap<QString, QString> managerParameters() const;  // e.g. "filename=private.db"
    virtual int managerVersion() const = 0;

    /* Default and only implementation of this */
    QString managerUri() const;

    /* Filtering */
    virtual QList<QOrganizerItemLocalId> contactIds(const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, QOrganizerItemManager::Error* error) const;
    virtual QList<QOrganizerItem> contacts(const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerItemManager::Error* error) const;
    virtual QOrganizerItem contact(const QOrganizerItemLocalId& contactId, const QOrganizerItemFetchHint& fetchHint, QOrganizerItemManager::Error* error) const;

    virtual bool saveContact(QOrganizerItem* contact, QOrganizerItemManager::Error* error);
    virtual bool removeContact(const QOrganizerItemLocalId& contactId, QOrganizerItemManager::Error* error);
    virtual bool saveRelationship(QOrganizerItemRelationship* relationship, QOrganizerItemManager::Error* error);
    virtual bool removeRelationship(const QOrganizerItemRelationship& relationship, QOrganizerItemManager::Error* error);

    virtual bool saveContacts(QList<QOrganizerItem>* contacts, QMap<int, QOrganizerItemManager::Error>* errorMap, QOrganizerItemManager::Error* error);
    virtual bool removeContacts(const QList<QOrganizerItemLocalId>& contactIds, QMap<int, QOrganizerItemManager::Error>* errorMap, QOrganizerItemManager::Error* error);

    /* Return a pruned or modified contact which is valid and can be saved in the backend */
    virtual QOrganizerItem compatibleContact(const QOrganizerItem& original, QOrganizerItemManager::Error* error) const;

    /* Synthesize the display label of a contact */
    virtual QString synthesizedDisplayLabel(const QOrganizerItem& contact, QOrganizerItemManager::Error* error) const;

    /* "Self" contact id (MyCard) */
    virtual bool setSelfContactId(const QOrganizerItemLocalId& contactId, QOrganizerItemManager::Error* error);
    virtual QOrganizerItemLocalId selfContactId(QOrganizerItemManager::Error* error) const;

    /* Relationships between contacts */
    virtual QList<QOrganizerItemRelationship> relationships(const QString& relationshipType, const QOrganizerItemId& participantId, QOrganizerItemRelationship::Role role, QOrganizerItemManager::Error* error) const;
    virtual bool saveRelationships(QList<QOrganizerItemRelationship>* relationships, QMap<int, QOrganizerItemManager::Error>* errorMap, QOrganizerItemManager::Error* error);
    virtual bool removeRelationships(const QList<QOrganizerItemRelationship>& relationships, QMap<int, QOrganizerItemManager::Error>* errorMap, QOrganizerItemManager::Error* error);

    /* Validation for saving */
    virtual bool validateContact(const QOrganizerItem& contact, QOrganizerItemManager::Error* error) const;
    virtual bool validateDefinition(const QOrganizerItemDetailDefinition& def, QOrganizerItemManager::Error* error) const;

    /* Definitions - Accessors and Mutators */
    virtual QMap<QString, QOrganizerItemDetailDefinition> detailDefinitions(const QString& contactType, QOrganizerItemManager::Error* error) const;
    virtual QOrganizerItemDetailDefinition detailDefinition(const QString& definitionId, const QString& contactType, QOrganizerItemManager::Error* error) const;
    virtual bool saveDetailDefinition(const QOrganizerItemDetailDefinition& def, const QString& contactType, QOrganizerItemManager::Error* error);
    virtual bool removeDetailDefinition(const QString& definitionId, const QString& contactType, QOrganizerItemManager::Error* error);

    /* Asynchronous Request Support */
    virtual void requestDestroyed(QOrganizerItemAbstractRequest* req);
    virtual bool startRequest(QOrganizerItemAbstractRequest* req);
    virtual bool cancelRequest(QOrganizerItemAbstractRequest* req);
    virtual bool waitForRequestFinished(QOrganizerItemAbstractRequest* req, int msecs);

    /* Capabilities reporting */
    virtual bool hasFeature(QOrganizerItemManager::ManagerFeature feature, const QString& contactType) const;
    virtual bool isRelationshipTypeSupported(const QString& relationshipType, const QString& contactType) const;
    virtual bool isFilterSupported(const QOrganizerItemFilter& filter) const;
    virtual QList<QVariant::Type> supportedDataTypes() const;
    virtual QStringList supportedContactTypes() const;

    /* Reports the built-in definitions from the schema */
    static QMap<QString, QMap<QString, QOrganizerItemDetailDefinition> > schemaDefinitions();

Q_SIGNALS:
    void dataChanged();
    void contactsAdded(const QList<QOrganizerItemLocalId>& contactIds);
    void contactsChanged(const QList<QOrganizerItemLocalId>& contactIds);
    void contactsRemoved(const QList<QOrganizerItemLocalId>& contactIds);
    void relationshipsAdded(const QList<QOrganizerItemLocalId>& affectedContactIds);
    void relationshipsRemoved(const QList<QOrganizerItemLocalId>& affectedContactIds);
    void selfContactIdChanged(const QOrganizerItemLocalId& oldId, const QOrganizerItemLocalId& newId);

public:
    // Async update functions
    static void updateRequestState(QOrganizerItemAbstractRequest* req, QOrganizerItemAbstractRequest::State state);

    static void updateContactLocalIdFetchRequest(QOrganizerItemLocalIdFetchRequest* req, const QList<QOrganizerItemLocalId>& result, QOrganizerItemManager::Error error, QOrganizerItemAbstractRequest::State);
    static void updateContactFetchRequest(QOrganizerItemFetchRequest* req, const QList<QOrganizerItem>& result, QOrganizerItemManager::Error error, QOrganizerItemAbstractRequest::State);
    static void updateContactRemoveRequest(QOrganizerItemRemoveRequest* req, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateContactSaveRequest(QOrganizerItemSaveRequest* req, const QList<QOrganizerItem>& result, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateDefinitionSaveRequest(QOrganizerItemDetailDefinitionSaveRequest* req, const QList<QOrganizerItemDetailDefinition>& result, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateDefinitionRemoveRequest(QOrganizerItemDetailDefinitionRemoveRequest* req, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateDefinitionFetchRequest(QOrganizerItemDetailDefinitionFetchRequest* req, const QMap<QString, QOrganizerItemDetailDefinition>& result, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateRelationshipSaveRequest(QOrganizerItemRelationshipSaveRequest* req, const QList<QOrganizerItemRelationship>& result, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateRelationshipRemoveRequest(QOrganizerItemRelationshipRemoveRequest* req, QOrganizerItemManager::Error error, const QMap<int, QOrganizerItemManager::Error>& errorMap, QOrganizerItemAbstractRequest::State);
    static void updateRelationshipFetchRequest(QOrganizerItemRelationshipFetchRequest* req, const QList<QOrganizerItemRelationship>& result, QOrganizerItemManager::Error error, QOrganizerItemAbstractRequest::State);

    // Other protected area update functions
    static void setDetailAccessConstraints(QOrganizerItemDetail* detail, QOrganizerItemDetail::AccessConstraints constraints);
    static void setContactDisplayLabel(QOrganizerItem* contact, const QString& displayLabel);
    static void setContactRelationships(QOrganizerItem* contact, const QList<QOrganizerItemRelationship>& relationships);

    /* Helper functions */
    static int compareContact(const QOrganizerItem& a, const QOrganizerItem& b, const QList<QOrganizerItemSortOrder>& sortOrders);
    static void addSorted(QList<QOrganizerItem>* sorted, const QOrganizerItem& toAdd, const QList<QOrganizerItemSortOrder>& sortOrders);
    static int compareVariant(const QVariant& first, const QVariant& second, Qt::CaseSensitivity sensitivity);
    static bool testFilter(const QOrganizerItemFilter& filter, const QOrganizerItem& contact);
    static bool validateActionFilter(const QOrganizerItemFilter& filter);
    static QList<QOrganizerItemLocalId> sortContacts(const QList<QOrganizerItem>& contacts, const QList<QOrganizerItemSortOrder>& sortOrders);

    static QOrganizerItemFilter canonicalizedFilter(const QOrganizerItemFilter& filter);

private:
    /* QOrganizerItemChangeSet is a utility class used to emit the appropriate signals */
    friend class QOrganizerItemChangeSet;
};

QTM_END_NAMESPACE

#endif