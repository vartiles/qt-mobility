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


#include "qdeclarativedeviceinfo_p.h"
#include "qsystemdeviceinfo.h"
#include <QMetaType>
#include <QDebug>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QSystemDeviceInfo, deviceInfo)

/*!
    \qmlclass DeviceInfo QDeclarativeDeviceInfo
    \brief The DeviceInfo element allows you to access information anbout the device and
        receive notifications from the device.

    \inherits QSystemDeviceInfo

    \ingroup qml-systeminfo
    \since Mobility 1.1

    This element is part of the \bold{QtMobility.systeminfo 1.1} module.
    It is a convience class to make QML usage easier.

    Note: To use notification signals, you need to set the monitor* properties to true.


    The following table lists the \bold{incompatible} changes made to the start* functions between QtMobility 1.1 and 1.2,
    where they became Q_PROPERTY:

    \table
    \header
        \o QtMobility 1.1 \o QtMobility 1.2 \o Notes
    \row
        \o slot void startBatteryLevelChanged();
        \o void startBatteryLevelChanged(bool on);
        \o Became Q_PROPERTY monitorBatteryLevelChanges in QtMobility 1.2
    \row
        \o slot void startBatteryStatusChanged();
        \o void startBatteryStatusChanged(bool on);
        \o Became Q_PROPERTY monitorBatteryStatusChanges in QtMobility 1.2
    \row
        \o slot void startPowerStateChanged();
        \o void startPowerStateChanged(bool on);
        \o Became Q_PROPERTY monitorPowerStateChanges in QtMobility 1.2
    \row
        \o slot void startCurrentProfileChanged();
        \o void startCurrentProfileChanged(bool on);
        \o Became Q_PROPERTY monitorCurrentProfileChanges in QtMobility 1.2
    \row
        \o slot void startBluetoothStateChanged();
        \o void startBluetoothStateChanged(bool on);
        \o Became Q_PROPERTY monitorBluetoothStateChanges in QtMobility 1.2
    \endtable

    Example new usage:

QtMobility 1.1:
\code
      deviceinfo.startBatteryLevelChanged();
\endcode

QtMobility 1.2:
\code
      monitorBatteryLevelChanges: true
\endcode

\sa QSystemDeviceInfo
*/

/*!
    \qmlsignal DeviceInfo::batteryLevelChanged(int)
    \since Mobility 1.1

    This handler is called when battery level has changed.
*/
/*!
    \qmlsignal DeviceInfo::batteryStatusChanged(DeviceInfo::BatteryStatus)
    \since Mobility 1.1

    This handler is called when battery status has changed.

*/
/*!
    \qmlsignal DeviceInfo::powerStateChanged(DeviceINfo::PowerState)
    \since Mobility 1.1

    This handler is called when the power state has changed.
*/
/*!
    \qmlsignal DeviceInfo::currentProfileChanged(DeviceInfo::Profile)
    \since Mobility 1.1

    This handler is called when current device profile has changed.

*/
/*!
    \qmlsignal DeviceInfo::bluetoothStateChanged(bool)
    \since Mobility 1.1

    This handler is called when bluetooth power state has changed.

*/

/*!
    \qmlsignal DeviceInfo::wirelessKeyboardConnected(bool)
    \since Mobility 1.2

  This signal is emitted whenever a wireless keyboard is connected
*/

/*!
    \qmlsignal DeviceInfo::keyboardFlipped(bool)
    \since Mobility 1.2

      This signal is emitted whenever a phone flips open.
*/

/*!
    \qmlsignal DeviceInfo::deviceLocked(bool)
    \since Mobility 1.2

    This signal is emitted whenever the device lock state changes
*/

/*!
    \qmlsignal DeviceInfo::lockStatusChanged(DeviceInfo::LockTypeFlags)
    \since Mobility 1.2

    This signal is emitted whenever the lock state changes
*/

/*!
    \qmlsignal DeviceInfo::thermalStateChanged(DeviceInfo::ThermalState)
    \since Mobility 1.2

    This handler is called when thermal state has changed.
*/

QDeclarativeDeviceInfo::QDeclarativeDeviceInfo(QObject *parent) :
    QSystemDeviceInfo(parent)
{
}

/*!
   This function starts the batteryLevelChanged notification
   \since Mobility 1.1

*/

void QDeclarativeDeviceInfo::startBatteryLevelChanged(bool on)
{
    monitoringBatteryLevel = on;
    if(on) {
        connect(deviceInfo(),SIGNAL(batteryLevelChanged(int)),
                this,SIGNAL(batteryLevelChanged(int)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(batteryLevelChanged(int)),
                   this,SIGNAL(batteryLevelChanged(int)));
    }
}

/*!
    \qmlproperty bool DeviceInfo::monitorBatteryLevelChanges
    \since Mobility 1.2
    Use the monitorBatteryLevelChanges signal.

  */
bool QDeclarativeDeviceInfo::monitorBatteryLevelChanges()
{
    return monitoringBatteryLevel;
}

/*!
   This function starts the batteryStatusChanged notification
   \since Mobility 1.1

*/
void QDeclarativeDeviceInfo::startBatteryStatusChanged(bool on)
{
    monitoringBatteryStatus = on;
    if(on) {
    connect(deviceInfo(),SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)),
            this,SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)),
                this,SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)));
    }
}

/*!
    \qmlproperty bool DeviceInfo::monitorBatteryStatusChanges
    \since Mobility 1.2
    Use the monitorBatteryStatusChanges signal.
  */
bool QDeclarativeDeviceInfo::monitorBatteryStatusChanges()
{
    return monitoringBatteryStatus;
}


/*!
   This function starts the powerStateChanged notification
   \since Mobility 1.1

*/
void QDeclarativeDeviceInfo::startPowerStateChanged(bool on)
{
    monitoringPowerState = on;
    if(on) {
    connect(deviceInfo(),SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)),
            this,SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)),
                this,SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)));
    }
}

/*!
    \qmlproperty bool DeviceInfo::monitorPowerStateChanges
    \since Mobility 1.2
    Use the monitorPowerStateChanges signal.
  */
bool QDeclarativeDeviceInfo::monitorPowerStateChanges()
{
    return monitoringPowerState;
}


/*!
   This function is needed to start currentProfileChanged notification
   \since Mobility 1.1

*/
void QDeclarativeDeviceInfo::startCurrentProfileChanged(bool on)
{
    monitoringCurrentProfile = on;
    if(on) {
    connect(deviceInfo(),SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
            this,SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
                this,SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)));
    }
}

/*!
    \qmlproperty bool DeviceInfo::monitorCurrentProfileChanges
    \since Mobility 1.2
    Use the monitorCurrentProfileChanges signal.
  */
bool QDeclarativeDeviceInfo::monitorCurrentProfileChanges()
{
    return monitoringCurrentProfile;
}


/*!
   This function starts the bluetoothStateChanged notification
   \since Mobility 1.1

*/
void QDeclarativeDeviceInfo::startBluetoothStateChanged(bool on)
{
    monitorBluetoothState = on;
    if(on) {
    connect(deviceInfo(),SIGNAL(bluetoothStateChanged(bool)),
            this,SIGNAL(bluetoothStateChanged(bool)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(bluetoothStateChanged(bool)),
                this,SIGNAL(bluetoothStateChanged(bool)));
    }
}

/*!
    \qmlproperty bool DeviceInfo::monitorBluetoothStateChanges
    \since Mobility 1.2
    Use the bluetoothStateChanges signal.
  */
bool QDeclarativeDeviceInfo::monitorBluetoothStateChanges()
{
   return monitorBluetoothState;
}

/*!
   This function starts the thermalStateChanged notification
   \since Mobility 1.2

*/
void QDeclarativeDeviceInfo::startThermalStateChanged(bool on)
{
    monitorThermalState = on;
    if (on) {
    connect(deviceInfo(), SIGNAL(thermalStateChanged(QSystemDeviceInfo::ThermalState)),
            this, SIGNAL(thermalStateChanged(QSystemDeviceInfo::ThermalState)), Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(), SIGNAL(thermalStateChanged(QSystemDeviceInfo::ThermalState)),
                this, SIGNAL(thermalStateChanged(QSystemDeviceInfo::ThermalState)));
    }
}

/*!
    \qmlproperty bool DeviceInfo::monitorThermalStateChanges
    \since Mobility 1.2
    Use the thermalStateChanges signal.
  */
bool QDeclarativeDeviceInfo::monitorThermalStateChanges()
{
   return monitorThermalState;
}

/*!

  \qmlproperty bool DeviceInfo::monitorWirelessKeyboardConnects()
  \since Mobility 1.2

  Start the connection for the wirelessKeyboardConnected signal.
  */
bool QDeclarativeDeviceInfo::monitorWirelessKeyboardConnects()
{
 return monitoringWirelessKeyboard;
}


/*!
   This function starts the startBatteryStatusChanged notification.
   \since Mobility 1.1

*/
void QDeclarativeDeviceInfo::startWirelessKeyboardConnected(bool on)
{
    monitoringWirelessKeyboard = on;
    if(on) {
        connect(deviceInfo(),SIGNAL(wirelessKeyboardConnected(bool)),
                this,SIGNAL(wirelessKeyboardConnected(bool)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(wirelessKeyboardConnected(bool)),
                this,SIGNAL(wirelessKeyboardConnected(bool)));
    }
}


/*!

  \qmlproperty bool DeviceInfo::monitorKeyboardFlips()
  \since Mobility 1.2

  Start the connection for the keyboardFlipped signal.
  */
bool QDeclarativeDeviceInfo::monitorKeyboardFlips()
{
    return monitoringKeyboardFlipped;
}

/*!
   This function starts the startKeyboardFlipped notification.
   \since Mobility 1.2

*/
void QDeclarativeDeviceInfo::startKeyboardFlipped(bool on)
{
    monitoringKeyboardFlipped = on;
    if(on) {
        connect(deviceInfo(),SIGNAL(keyboardFlipped(bool)),
                this,SIGNAL(keyboardFlipped(bool)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(keyboardFlipped(bool)),
                this,SIGNAL(keyboardFlipped(bool)));
    }
}


/*!

  \qmlproperty bool Device::monitorDeviceLocks()
  \since Mobility 1.2

  Start the connection for the deviceLocked signal.
  */
bool QDeclarativeDeviceInfo::monitorDeviceLocks()
{
    return monitoringDeviceLocked;
}

/*!
   This function starts the startDeviceLocked notification.
   \since Mobility 1.2

*/
void QDeclarativeDeviceInfo::startDeviceLocked(bool on)
{
    monitoringDeviceLocked = on;
    if(on) {
        connect(deviceInfo(),SIGNAL(deviceLocked(bool)),
                this,SIGNAL(deviceLocked(bool)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(deviceLocked(bool)),
                this,SIGNAL(deviceLocked(bool)));
    }
}

/*!

  \qmlproperty bool DeviceInfo::monitorLockStatusChanges()
  \since Mobility 1.2

  Start the connection for the lockStatusChanged signal.
  */

bool QDeclarativeDeviceInfo::monitorLockStatusChanges()
{
    return monitoringLockStatusChanged;
}

/*!
   This function starts the startLockStatusChanged notification.
   \since Mobility 1.2

*/
void QDeclarativeDeviceInfo::startLockStatusChanged(bool on)
{
    monitoringLockStatusChanged = on;
    if(on) {
        connect(deviceInfo(),SIGNAL(lockStatusChanged(QSystemDeviceInfo::LockTypeFlags)),
                this,SIGNAL(lockStatusChanged(QSystemDeviceInfo::LockTypeFlags)),Qt::UniqueConnection);
    } else {
        disconnect(deviceInfo(),SIGNAL(lockStatusChanged(QSystemDeviceInfo::LockTypeFlags)),
                this,SIGNAL(lockStatusChanged(QSystemDeviceInfo::LockTypeFlags)));
    }
}

/*!
  \qmlproperty int DeviceInfo::messageRingtoneVolume()
  \since Mobility 1.2

    Returns the active profile's message ringtone volume. From 0 to 100.
  */
int QDeclarativeDeviceInfo::messageRingtoneVolume()
{
    return deviceInfo()->activeProfileDetails().messageRingtoneVolume();
}

/*!
  \qmlproperty int DeviceInfo::voiceRingtoneVolume()
  \since Mobility 1.2

    Returns the active profile's voice ringtone volume. From 0 to 100.

  */
int QDeclarativeDeviceInfo::voiceRingtoneVolume()
{
    return deviceInfo()->activeProfileDetails().voiceRingtoneVolume();
}

/*!
  \qmlproperty bool DeviceInfo::vibrationActive()
  \since Mobility 1.2

    Returns the whether the active profile's vibration is active.

  */
bool QDeclarativeDeviceInfo::vibrationActive()
{
    return deviceInfo()->activeProfileDetails().vibrationActive();
}

/*!
  \qmlproperty bool DeviceInfo::primaryKeypadLightOn()
  \since Mobility 1.2

  Returns the whether he primary keypad  or keyboard light is on.
*/
/*!
  Returns true if the primary key pad light is on, otherwise false;
  \since Mobility 1.2
*/
bool QDeclarativeDeviceInfo::primaryKeypadLightOn()
{
    return deviceInfo()->keypadLightOn(QSystemDeviceInfo::PrimaryKeypad);
}

/*!
  \qmlproperty bool DeviceInfo::secondaryKeypadLightOn()
  \since Mobility 1.2

    Returns the whether he secondary keypad or keyboard light is on.
  */
/*!
  Returns true if the key pad light is on, otherwise false;
  \since Mobility 1.2
*/
bool QDeclarativeDeviceInfo::secondaryKeypadLightOn()
{
    return deviceInfo()->keypadLightOn(QSystemDeviceInfo::SecondaryKeypad);

}



/*!
  \qmlproperty QString DeviceInfo::imei
  \since Mobility 1.2

     Returns the International Mobile Equipment Identity (IMEI), or a null QString in the case of none.
  */

/*!
  \qmlproperty QString DeviceInfo::imsi
  \since Mobility 1.2
  Returns the International Mobile Subscriber Identity (IMSI), or a null QString in the case of none
  */

/*!
  \qmlproperty QString DeviceInfo::manufacturer
  \since Mobility 1.2
  Returns the name of the manufacturer of this device. In the case of desktops, the name of the vendor
    of the motherboard.
  */
/*!
  \qmlproperty QString DeviceInfo::model
  \since Mobility 1.2
  Returns the model information of the device. In the case of desktops where no
    model information is present, the CPU architect, such as i686, and machine type, such as Server,
    Desktop or Laptop.
  */
/*!
  \qmlproperty QString DeviceInfo::productName
  \since Mobility 1.2
  Returns the product name of the device. In the case where no product information is available, an empty string will be returned.

  */
/*!
  \qmlproperty int DeviceInfo::batteryLevel
  \since Mobility 1.1
  Returns the battery charge level as percentage 1 - 100 scale.
  */
/*!
  \qmlproperty InputMethodFlags DeviceInfo::inputMethodType
  \since Mobility 1.2
  Returns the QSystemDeviceInfo::InputMethodFlags InputMethodType that the system uses.
  */
/*!
  \qmlproperty BatteryStatus DeviceInfo::batteryStatus
  \since Mobility 1.1
  Returns the battery charge status.
  */
/*!
  \qmlproperty bool DeviceInfo::isDeviceLocked
  \since Mobility 1.2

  Returns true if the device is locked, otherwise false.
  */
/*!
  \qmlproperty SimStatus DeviceInfo::simStatus
  \since Mobility 1.2
  Returns the QSystemDeviceInfo::simStatus status of SIM card.
  */
/*!
  \qmlproperty Profile DeviceInfo::currentProfile
  \since Mobility 1.2
  Gets the current QSystemDeviceInfo::currentProfile device profile.
  */
/*!
  \qmlproperty PowerState DeviceInfo::currentPowerState
  \since Mobility 1.2

  Gets the current QSystemDeviceInfo::currentPowerState state.
  */
/*!
  \qmlproperty bool DeviceInfo::currentBluetoothPowerState
  \since Mobility 1.2
  Gets the current bluetooth power state.
  */
/*!
  \qmlproperty KeyboardTypeFlags DeviceInfo::keyboardTypes
  \since Mobility 1.2
  Returns the type of keyboards found.
  */
/*!
  \qmlproperty bool DeviceInfo::isWirelessKeyboardConnected
  \since Mobility 1.2
  Returns true if a wireless keyboard is connected, otherwise false;
  */
/*!
  \qmlproperty bool DeviceInfo::isKeyboardFlippedOpen
  \since Mobility 1.2
  Returns true if the flip keyboard is open, otherwise false;
  */

/*!
  \qmlproperty LockTypeFlags DeviceInfo::lockStatus
  \since Mobility 1.2
  Returns the QSystemDeviceInfo::LockTypeFlags type of lock state the device might be in.
  The LockType must be currently active not just enabled.
  */

/*!
    \qmlproperty ThermalState DeviceInfo::currentThermalState
    Returns the current thermal state of the device.

    \since Mobility 1.2
    \sa QSystemDeviceInfo::ThermalState
*/


/*!
  \qmlproperty string DeviceInfo::uniqueID
  \since Mobility 1.2

  Returns a unique identifier for the machine.

  Depending on security enforcement on platform, this may return a non unique number, or 0.

  */
