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
#include "cnttransformanniversary.h"
#include "cntmodelextuids.h"

QList<CContactItemField *> CntTransformAnniversary::transformDetailL(const QContactDetail &detail)
{
    if(detail.definitionName() != QContactAnniversary::DefinitionName)
       User::Leave(KErrArgument);

    QList<CContactItemField *> fieldList;

	//cast to anniversary
	const QContactAnniversary &anniversary(static_cast<const QContactAnniversary&>(detail));

    //date field
	TDateTime dateTime(anniversary.originalDate().year(),
	    TMonth(anniversary.originalDate().month() - 1),
	    anniversary.originalDate().day() - 1, 0, 0, 0, 0);
    CContactItemField* dateField = CContactItemField::NewLC(KStorageTypeDateTime, KUidContactFieldAnniversary);
    dateField->DateTimeStorage()->SetTime(dateTime);
    dateField->SetMapping(KUidContactFieldVCardMapAnniversary);
    fieldList.append(dateField);
    CleanupStack::Pop(dateField);

    //event field
    transformToTextFieldL(anniversary, fieldList, anniversary.event(), KUidContactFieldAnniversaryEvent, KUidContactFieldVCardMapUnknown, false);
    
	return fieldList;
}

QContactDetail *CntTransformAnniversary::transformItemField(const CContactItemField& field, const QContact &contact)
{
    QContactAnniversary *anniversaryDetail = new QContactAnniversary(contact.detail<QContactAnniversary>());
    if (field.StorageType() == KStorageTypeDateTime) {
        CContactDateField* date = field.DateTimeStorage();
        TTime time(date->Time());
        QDate qDate(time.DateTime().Year(), time.DateTime().Month() + 1, time.DateTime().Day() + 1);
        anniversaryDetail->setOriginalDate(qDate);
    }
    else if (field.StorageType() == KStorageTypeText) {
        CContactTextField* event = field.TextStorage();
        QString eventString = QString::fromUtf16(event->Text().Ptr(), event->Text().Length());
        anniversaryDetail->setEvent(eventString);
    }

	return anniversaryDetail;
}

bool CntTransformAnniversary::supportsDetail(QString detailName) const
{
    bool ret = false;
    if (detailName == QContactAnniversary::DefinitionName) {
        ret = true;
    }
    return ret;
}

QList<TUid> CntTransformAnniversary::supportedFields() const
{
    return QList<TUid>()
        << KUidContactFieldAnniversary
        << KUidContactFieldAnniversaryEvent;
}

QList<TUid> CntTransformAnniversary::supportedSortingFieldTypes(QString /*detailFieldName*/) const
{
    // Sorting not supported
    return QList<TUid>();
}

/*!
 * Checks whether the subtype is supported
 *
 * \a subType The subtype to be checked
 * \return True if this subtype is supported
 */
bool CntTransformAnniversary::supportsSubType(const QString& subType) const
{
    if(QContactAnniversary::FieldSubType  == subType)
       return true;
    else
       return false;
}

/*!
 * Returns the filed id corresponding to a field
 *
 * \a fieldName The name of the supported field
 * \return fieldId for the fieldName, 0  if not supported
 */
quint32 CntTransformAnniversary::getIdForField(const QString& fieldName) const
{
    if (QContactAnniversary::FieldCalendarId  == fieldName)
        return 0;
    else if (QContactAnniversary::FieldOriginalDate == fieldName)
        return 0;
    else if (QContactAnniversary::FieldEvent == fieldName)
        return 0;
    else if (QContactAnniversary::SubTypeWedding == fieldName)
        return 0;
    else if (QContactAnniversary::SubTypeEngagement == fieldName)
        return 0;
    else if (QContactAnniversary::SubTypeHouse == fieldName)
        return 0;
    else if (QContactAnniversary::SubTypeEmployment == fieldName)
        return 0;
    else if (QContactAnniversary::SubTypeMemorial == fieldName)
        return 0;
    else
        return 0;
}

/*!
 * Modifies the detail definitions. The default detail definitions are
 * queried from QContactManagerEngine::schemaDefinitions and then modified
 * with this function in the transform leaf classes.
 *
 * \a definitions The detail definitions to modify.
 * \a contactType The contact type the definitions apply for.
 */
void CntTransformAnniversary::detailDefinitions(QMap<QString, QContactDetailDefinition> &definitions, const QString& contactType) const
{
    Q_UNUSED(contactType);

    if(definitions.contains(QContactAnniversary::DefinitionName)) {
        QContactDetailDefinition d = definitions.value(QContactAnniversary::DefinitionName);
        QMap<QString, QContactDetailFieldDefinition> fields = d.fields();

        // FieldCalendarId not supported in symbian back-end, remove
        fields.remove(QContactAnniversary::FieldCalendarId);

        // Sub-types not supported in symbian back-end, remove
        fields.remove(QContactAnniversary::FieldSubType);

        // Context not supported in symbian back-end, remove
        fields.remove(QContactAnniversary::FieldContext);

        d.setFields(fields);
        d.setUnique(true);

        // Replace original definitions
        definitions.insert(d.name(), d);
    }
}
