package com.nexus.weightedalarm.alarm

import java.time.LocalTime

sealed interface AlarmEvent {
    data class OnTimeChange(val time: LocalTime): AlarmEvent
    data class OnVolumeChange(val volume: String) : AlarmEvent
    data class OnBlanketWeightChange(val blanketWeight: String) : AlarmEvent
    data class OnSnoozeTimeChange(val snoozeTime: String) : AlarmEvent
}