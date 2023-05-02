package com.nexus.weightedalarm.alarm

import java.time.LocalTime

data class AlarmState(
    val result: EspResult = EspResult.Loading,
    val alarmTime: LocalTime = LocalTime.now(),
    val volume: String = "5",
    val blanketWeight: String = "500",
    val snoozeTime: String = "3"
) {
}

sealed interface EspResult{
    data class Error(val message: String): EspResult
    object Loading: EspResult
    object Success: EspResult
}
