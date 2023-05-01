package com.nexus.weightedalarm.alarm

sealed interface AlarmEvent {
    data class OnTimeChange(val hours: Int, val minutes: Int, val seconds: Int): AlarmEvent
}