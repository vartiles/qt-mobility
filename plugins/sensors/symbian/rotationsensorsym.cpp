/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the Qt Mobility Components.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Solutions Commercial License Agreement provided
 ** with the Software or, alternatively, in accordance with the terms
 ** contained in a written agreement between you and Nokia.
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
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** Please note Third Party Software included with Qt Solutions may impose
 ** additional restrictions and it is the user's responsibility to ensure
 ** that they have met the licensing requirements of the GPL, LGPL, or Qt
 ** Solutions Commercial license and the relevant license of the Third
 ** Party Software they are using.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at qt-sales@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

// Internal Headers
#include "rotationsensorsym.h"

/**
 * set the id of the proximity sensor
 */
char const * const CRotationSensorSym::id("sym.rotation");

/**
 * Factory function, this is used to create the rotation sensor object
 * @return CRotationSensorSym if successful, leaves on failure
 */
CRotationSensorSym* CRotationSensorSym::NewL(QSensor *sensor)
    {
    CRotationSensorSym* self = new (ELeave) CRotationSensorSym(sensor);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

/**
 * Destructor
 * Closes the backend resources
 */
CRotationSensorSym::~CRotationSensorSym()
    {
    // Release the backend resources
    Close();
    }

/**
 * Default constructor
 */
CRotationSensorSym::CRotationSensorSym(QSensor *sensor):CSensorBackendSym(sensor)
        {
        setReading<QRotationReading>(&iReading);    
        iBackendData.iSensorType = KSensrvChannelTypeIdRotationData;
        }

/*
 * RecvData is used to retrieve the sensor reading from sensor server
 * It is implemented here to handle rotation sensor specific
 * reading data and provides conversion and utility code
 */ 
void CRotationSensorSym::RecvData(CSensrvChannel &aChannel)
    {
    TPckg<TSensrvRotationData> rotationpkg( iData );
    TInt ret = aChannel.GetData( rotationpkg );
    if(KErrNone != ret)
        {
        // If there is no reading available, return without setting
        return;
        }
    // Get a lock on the reading data
    iBackendData.iReadingLock.Wait();
    // To Do verify with ds and ramsay
    iReading.setX(iData.iDeviceRotationAboutXAxis);
    iReading.setY(iData.iDeviceRotationAboutYAxis-180);
    if(iData.iDeviceRotationAboutZAxis == TSensrvRotationData::KSensrvRotationUndefined)
        {
        sensor()->setProperty("hasZ", QVariant(FALSE));
        }
    else
        {
        sensor()->setProperty("hasZ", QVariant(TRUE));
        iReading.setZ(iData.iDeviceRotationAboutZAxis-180);
        }
    // Set the timestamp
    iReading.setTimestamp(iData.iTimeStamp.Int64());
    // Release the lock
    iBackendData.iReadingLock.Signal();
    }


/**
 * Second phase constructor
 * Initialize the backend resources
 */
void CRotationSensorSym::ConstructL()
    {
    //Initialize the backend resources
    InitializeL();    
    }
