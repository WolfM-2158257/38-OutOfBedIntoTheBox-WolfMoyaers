package com.nexus.weightedalarm.alarm

sealed interface AlarmAction {
    object SetTime: AlarmAction
}