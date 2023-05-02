package com.nexus.weightedalarm.alarm

sealed interface AlarmAction {
    object SetAlarmTime: AlarmAction
    object SetClockTime : AlarmAction
}