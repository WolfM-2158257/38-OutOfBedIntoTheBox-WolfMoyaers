package com.nexus.weightedalarm.alarm

import java.time.LocalTime

sealed interface AlarmAction {
    object SetAlarmTime: AlarmAction
    object SetClockTime : AlarmAction
    object SetVolume : AlarmAction
    object SetBlanketWeight: AlarmAction
    object SetSnoozeTime: AlarmAction
}
