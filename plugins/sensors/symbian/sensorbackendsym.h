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

#ifndef SENSORBACKENDSYM_H
#define SENSORBACKENDSYM_H

// QT Mobility Sensor API headers
#include <qsensorbackend.h>

// Standard Symbian Headers
#include <e32std.h>
#include <e32cmn.h>

// Sensor client headers
// Common Sensor Headers
#include <sensrvchannel.h>
#include <sensrvchannelinfo.h>
#include <sensrvtypes.h>
#include <sensrvchannelfinder.h>
#include <sensrvdatalistener.h>
#include <sensrvpropertylistener.h>

// Internal Headers
#include "sensorbackenddatasym.h"

QTM_USE_NAMESPACE

class CSensorBackendSym : public CBase, public QSensorBackend, public MSensrvDataListener, public MSensrvPropertyListener
    {
    public:
        // From QSensorBackend

        /**
         *  start is used to start listening to the sensor
         */
        virtual void start();

        /*
         * stop is used to stop listening to the sensor
         */
        virtual void stop();

        /*
         * Backend Destructor
         */
        ~CSensorBackendSym();

        // From MSensrvDataListener

        /*
         * DataReceived is called by the Sensor Server when ever data is available in the
         * sensor buffer
         */
        // implemnented by sub-classes
        //void  DataReceived(CSensrvChannel &aChannel, TInt aCount, TInt aDataLost);

        /**
         * DataError is called to indicate an error, fatal errors are unrecoverable
         */
        void  DataError(CSensrvChannel &aChannel, TSensrvErrorSeverity aError);

        /*
         * GetDataListenerInterfaceL is used to get a pointer to the sensor server backend
         * It is not required for QT Mobility Sensors API
         */
        void  GetDataListenerInterfaceL(TUid aInterfaceUid, TAny *&aInterface);

        // From MSensrvProeprtyListener

        /**
         * Notification about the changed value of a property.
         */
        void  PropertyChanged (CSensrvChannel &aChannel, const TSensrvProperty &aChangedProperty);

        /**
         *  Property listening failed.
         */
        void  PropertyError (CSensrvChannel &aChannel, TSensrvErrorSeverity aError);

        /**
         * Set a listener for the indication, if the setting of the property succeeded.
         */
        void  SetPropertySuccessIndicationChanged (TSetPropertySuccessIndicator aIndication);

        /*
         * Returns a pointer to a specified interface extension - to allow future extension of this class without breaking binary compatibility.
         */
        void  GetPropertyListenerInterfaceL (TUid aInterfaceUid, TAny *&aInterface);

        /*
         * To enable/disable data/property listening
         */
        void SetListening(TBool aDataListening, TBool aPropertyListening);

    protected:

        /*
         * Default C++ constructor
         */
        CSensorBackendSym(QSensor *sensor);

        /**
         * Deriving class implements this if it requires property change notification
         */
        virtual void HandlePropertyChange(CSensrvChannel &aChannel, const TSensrvProperty &aChangedProperty);

        /*
         * InitializeL is used to create and init the sensor server objects
         */
        void InitializeL();

        /*
         * SetProperty is used to set property to the channel
         */
        TInt SetProperty(TSensrvPropertyId aPropertyId, TSensrvPropertyType aPropertyType, TSensrvArrayIndex aArrayIndex, TReal aValue);

        /*
         * SetMeasurementRange is used to check measurement range type and set the measurement range
         */
        TInt SetMeasurementRange();

        /*
         * SetDataRate is used to calculate appropriate data rate for given interval and set that interval to the channel
         */
        TInt SetDataRate();

        /*
         * SetProperties is used to set properties on the channel before start data listening
         */
        void SetProperties();

        /*
         * GetPropertiesL used to get the properties from sensor server and sets as metadata for Qt.
         */
        void GetPropertiesL();

        /*
         * GetDescription used to get description of sensor from symbian and set
         * as description in Qt
         */
        void GetDescription();

        /*
         * GetDataRate used to get available datarates from symbian and set
         * as availableDataRates in Qt
         */
        void GetDataRate();

        /*
         * GetMeasurementrangeAndAccuracy used to get measurement ranges and accuracy from
         * symbian and set as outputRanges in Qt
         */
        virtual void GetMeasurementrangeAndAccuracy();

        /*
         * Close is used to release all the sensor server objects
         * May change when error handling is supported by mobility apis
         */
        TInt Close();

        /*
         * FindSensorL is used to find if a specific sensor is available on the
         * device, if FindSensorL leaves then the sensor is not supported
         */
        void FindSensorL();

        /*
         * OpenSensorChannelL is used to open the channel for sensor as indicated by
         * iPrivateData.iChannelInfo
         */
        void OpenSensorChannelL();

        /*
         * CloseSensorChannel is used to close the sensor channel and release the
         * resources
         */
        void CloseSensorChannelL();

        /*
         * Used to start listening to the sensor
         */
        void StartListeningL();

        /*
         * Used to stop listening to the sensor
         */
        void StopListeningL();

        /*
         * ProcessReading is used to process one sensor reading
         * It is implemented the the sensor concrete class and handles sensor specific
         * reading data and provides conversion and utility code
         */
        virtual void ProcessReading() = 0;

        /*
         * ProcessData is called by the sub-clases to handle the conditional fetching logic
         * It either processes all arriving readings or just the last one. It calls
         * ProcessReading to do the actual processing
         */
        template <typename T>
        void ProcessData(CSensrvChannel &aChannel, TInt aCount, T &iData)
            {
            int loopMax = aCount;
            if (!m_processAllReadings)
                {
                for (int i = 0; i < aCount; i++)
                    {
                    TPckg<T> pkg( iData );
                    TInt ret = aChannel.GetData( pkg );
                    if (ret != KErrNone)
                        return;
                    }
                loopMax = 1;
                }

            for (int i = 0; i < loopMax; i++)
                {
                if (m_processAllReadings)
                    {
                    TPckg<T> pkg( iData );
                    TInt ret = aChannel.GetData( pkg );
                    if (ret != KErrNone)
                        return;
                    }
                ProcessReading();
                }
            }

    private:
        TSensrvPropertyType propertyType(TSensrvPropertyId, TInt&);

    protected:
        TSensorBackendDataSym iBackendData;
        int m_maximumReadingCount;
        bool m_processAllReadings;
        int m_desiredReadingCount;
        int m_bufferingPeriod;
    };

#endif //SENSORBACKENDSYM_H

