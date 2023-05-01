package com.nexus.weightedalarm.alarm

import java.time.LocalTime

data class AlarmState(
    val result: EspResult = EspResult.Loading,
    val alarmTime: AlarmTime = AlarmTime()
)

sealed interface EspResult{
    data class Error(val message: String): EspResult
    object Loading: EspResult
    object Success: EspResult
}

data class AlarmTime(
    val hours: Int = LocalTime.now().hour,
    val minutes: Int = LocalTime.now().minute,
    val seconds: Int = 0
)
