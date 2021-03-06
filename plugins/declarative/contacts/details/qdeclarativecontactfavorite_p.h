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


#ifndef QDECLARATIVECONTACTFAVORITE_H
#define QDECLARATIVECONTACTFAVORITE_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontactfavorite.h"

class QDeclarativeContactFavorite : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(bool favorite READ isFavorite WRITE setFavorite NOTIFY fieldsChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY fieldsChanged)
    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "index")
public:
    enum FieldType {
        Favorite = 0,
        Index
    };
    QDeclarativeContactFavorite(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactFavorite());
        connect(this, SIGNAL(fieldsChanged()), SIGNAL(valueChanged()));
    }
    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::Favorite;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case Favorite:
            return QContactFavorite::FieldFavorite;
        case Index:
            return QContactFavorite::FieldIndex;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
    }
    void setFavorite(bool v)
    {
        if (!readOnly() && v != isFavorite()) {
            detail().setValue(QContactFavorite::FieldFavorite, v);
            emit fieldsChanged();
        }
    }
    bool isFavorite() const {return detail().variantValue(QContactFavorite::FieldFavorite).toBool();}
    void setIndex(int v)
    {
        if (!readOnly() && v != index()) {
            detail().setValue(QContactFavorite::FieldIndex, v);
            emit fieldsChanged();
        }
    }
    int index() const {return detail().variantValue(QContactFavorite::FieldIndex).toInt();}
signals:
    void fieldsChanged();
};

QML_DECLARE_TYPE(QDeclarativeContactFavorite)
#endif
