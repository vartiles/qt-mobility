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

#ifndef QCONTACTABOOK_P_H
#define QCONTACTABOOK_P_H

#include <QObject>
#include <QMutex>

#include "qtcontacts.h"
#include "qcontactmaemo5debug_p.h"

#undef signals
#include <libosso-abook/osso-abook.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "qcontactidshash.h"

QTM_USE_NAMESPACE

/* Data shared with contact changes/added/removed callbacks */
struct cbSharedData;
/* Data shared with job callbacks */
struct jobSharedData;

class QContactABook : public QObject
{
  Q_OBJECT
  
public:  
  QContactABook(QObject* parent = 0);
  ~QContactABook();
  
  QList<QContactLocalId> contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const;
  //QList<QContactLocalId> contactIds(const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const;
  QContact* getQContact(const QContactLocalId& contactId, QContactManager::Error* error) const;
  bool removeContact(const QContactLocalId& contactId, QContactManager::Error* error);
  bool saveContact(QContact* contact, QContactManager::Error* error);

  const QString getDisplayName(const QContact& contact) const;
  
  QContactLocalId selfContactId(QContactManager::Error* errors) const;

Q_SIGNALS:
  void contactsAdded(const QList<QContactLocalId>& contactIds);
  void contactsChanged(const QList<QContactLocalId>& contactIds);
  void contactsRemoved(const QList<QContactLocalId>& contactIds);
  void jobSavingCompleted();
  void jobRemovingCompleted();
  
public:
  /* Members used by callbacks */
  void _contactsAdded(const QList<QContactLocalId>& contactIds ){ emit contactsAdded(contactIds); };
  void _contactsRemoved(const QList<QContactLocalId>& contactIds ){ emit contactsRemoved(contactIds); };
  void _contactsChanged(const QList<QContactLocalId>& contactIds ){ emit contactsChanged(contactIds); };
  void _jobSavingCompleted(){ emit jobSavingCompleted(); };
  void _jobRemovingCompleted(){ emit jobRemovingCompleted(); };
  
private:
  void initAddressBook();
  void initLocalIdHash();
  
  bool setDetailValues(const QVariantMap& data, QContactDetail* detail) const;
  
  OssoABookContact* getAContact(const QContactLocalId& contactId, QContactManager::Error* error) const;
  
  /* Filtering */
  bool contactActionsMatch(OssoABookContact *contact, QList<QContactActionDescriptor> descriptors) const;
  EBookQuery* convert(const QContactFilter& filter) const;
  
  /* Reading - eContact/abookContact to QContact methods */
  QContact* convert(EContact *eContact) const;
  
  QContactId getContactId(EContact *eContact) const;
  QList<QContactAddress*> getAddressDetail(EContact *eContact) const;
  QContactName* getNameDetail(EContact *eContact) const;
  QContactNickname* getNicknameDetail(EContact *eContact) const;
  QList<QContactEmailAddress*> getEmailDetail(EContact *eContact) const;
  QContactAvatar* getAvatarDetail(EContact *eContact) const;
  QContactBirthday* getBirthdayDetail(EContact *eContact) const;
  QContactGender* getGenderDetail(EContact *eContact) const;
  QContactGuid* getGuidDetail(EContact *eContact) const;
  QContactNote* getNoteDetail(EContact *eContact) const;
  void getOnlineAccountAndPresenceDetails(EContact *eContact, 
                                         QList<QContactOnlineAccount*>& onlineAccounts,
                                         QList<QContactPresence*>& presences) const;
  QContactOrganization* getOrganizationDetail(EContact *eContact) const;
  QList<QContactPhoneNumber*> getPhoneDetail(EContact *eContact) const;
  QList<QContactPresence*> getPresenceDetail(EContact *eContact) const;
  QContactTimestamp* getTimestampDetail(EContact *eContact) const; 
  QContactThumbnail* getThumbnailDetail(EContact *eContact) const;
  QList<QContactUrl*> getUrlDetail(EContact *eContact) const;
  
  /* Saving - QContact to abookContact */
  OssoABookContact* convert(const QContact *contact, QContactManager::Error* error) const;
  
  /* Save QDetails in OssoABookContact attributes */
  void setAddressDetail(const OssoABookContact* aContact, const QContactAddress& detail) const;
  void setAvatarDetail(const OssoABookContact* aContact, const QContactAvatar& detail) const;
  void setBirthdayDetail(const OssoABookContact* aContact, const QContactBirthday& detail) const;
  void setEmailDetail(const OssoABookContact* aContact, const QContactEmailAddress& detail) const;
  void setGenderDetail(const OssoABookContact* aContact, const QContactGender& detail) const;
  void setNameDetail(const OssoABookContact* aContact, const QContactName& detail) const;
  void setNicknameDetail(const OssoABookContact* aContact, const QContactNickname& detail) const;
  void setNoteDetail(const OssoABookContact* aContact, const QContactNote& detail) const;
  void setOnlineAccountDetail(const OssoABookContact* aContact, const QContactOnlineAccount& detail) const;
  void setOrganizationDetail(const OssoABookContact* aContact, const QContactOrganization& detail) const;
  void setPhoneDetail(const OssoABookContact* aContact, const QContactPhoneNumber& detail) const;
  void setPresenceDetail(const OssoABookContact* aContact, const QContactPresence& detail) const;
  void setThumbnailDetail(const OssoABookContact* aContact, const QContactThumbnail& detail) const;
  void setUrlDetail(const OssoABookContact* aContact, const QContactUrl& detail) const;
  
  /* Internal Vars */
  gulong m_contactAddedHandlerId;
  gulong m_contactChangedHandlerId;
  gulong m_contactRemovedHandlerId;
  
  OssoABookAggregator *m_abookAgregator;
  mutable QContactIDsHash m_localIds; //Converts QLocalId <=> eContactId
  
  QMutex m_saveContactMutex;
  QMutex m_delContactMutex;
  
  cbSharedData *m_cbSD;
  jobSharedData *m_deleteJobSD;
  jobSharedData *m_saveJobSD;
};

#endif
