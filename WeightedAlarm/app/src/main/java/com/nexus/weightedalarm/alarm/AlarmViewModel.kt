package com.nexus.weightedalarm.alarm

import android.Manifest
import android.app.Application
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.pm.PackageManager
import android.util.Log
import androidx.core.app.ActivityCompat
import androidx.core.text.isDigitsOnly
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import java.time.LocalTime
import java.util.UUID

class AlarmViewModel(application: Application) : AndroidViewModel(application) {
    private val UUID_ESP = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB") // uuid which corresponds to the esp uuid
    private val MODULE_MAC = "24:6F:28:96:68:86" // mac-address of the bluetooth module (esp)

    private lateinit var socket: BluetoothSocket

    private var _state: MutableStateFlow<AlarmState> = MutableStateFlow(AlarmState())
    val state: StateFlow<AlarmState>
        get() = _state.asStateFlow()

    init {
        viewModelScope.launch(Dispatchers.IO) {
            connectTillConnected()
        }
    }

    private suspend fun connectTillConnected() {
        while (_state.value.result !is EspResult.Success) {
            connectDevice();
            if (_state.value.result is EspResult.Error) {
                delay(1000)
            }
        }
    }

    private suspend fun connectDevice() {
        val bluetoothManager: BluetoothManager? =
            getApplication<Application>().applicationContext.getSystemService(BluetoothManager::class.java)
        val bluetoothAdapter: BluetoothAdapter? = bluetoothManager?.adapter
        if (bluetoothAdapter == null) {
            Log.d("ALARM_BT", "no bluetooth adapter")
            setResult(result = EspResult.Error("No bluetooth adapter."))
            return;
        }
        val device: BluetoothDevice = bluetoothAdapter.getRemoteDevice(MODULE_MAC)
        if (ActivityCompat.checkSelfPermission(
                getApplication<Application>().applicationContext,
                Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            Log.d("ALARM_BT", "no permission")
            setResult(result = EspResult.Error("No permission"))
            return;
        }
        socket = device.createRfcommSocketToServiceRecord(UUID_ESP)
        try {
            socket.connect()
        } catch (e: Throwable) {
            Log.d("ALARM_BT", "Couldn't connect")
            setResult(result = EspResult.Error("Couldn't connect to socket"))
            return;
        }
        setResult(EspResult.Success)
    }

    private fun setResult(result: EspResult) {
        _state.update {
            it.copy(
                result = result
            )
        }
    }

    public fun onEvent(event: AlarmEvent) {
        when (event) {
            is AlarmEvent.OnTimeChange -> setTime(event.time)
            is AlarmEvent.OnVolumeChange -> setVolume(event.volume)
            is AlarmEvent.OnBlanketWeightChange -> setBlanketWeight(event.blanketWeight)
            is AlarmEvent.OnSnoozeTimeChange -> setSnoozeTime(event.snoozeTime)
        }
    }

    private fun setSnoozeTime(snoozeTime: String) {
        _state.update {
            it.copy(
                snoozeTime = snoozeTime
            )
        }
    }

    private fun setBlanketWeight(blanketWeight: String) {
        _state.update {
            it.copy(
                blanketWeight = blanketWeight
            )
        }
    }

    private fun setVolume(volume: String) {
        _state.update {
            it.copy(
                volume = volume
            )
        }
    }

    private fun setTime(time: LocalTime) {
        _state.update {
            it.copy(
                alarmTime = time
            )
        }
    }

    public fun onAction(action: AlarmAction) {
        when (action) {
            is AlarmAction.SetAlarmTime -> updateAlarmTime()
            is AlarmAction.SetClockTime -> updateClockTime()
            is AlarmAction.SetVolume -> updateVolume()
            is AlarmAction.SetBlanketWeight -> updateBlanketWeight()
            is AlarmAction.SetSnoozeTime -> updateSnoozeTime()
        }
    }

    private fun updateVolume(){
        val volume = _state.value.volume
        if (volume.isDigitsOnly()){
            sendCommand("V$volume")
        }
    }

    private fun updateSnoozeTime() {
        val snoozeTime = _state.value.snoozeTime
        if (snoozeTime.isDigitsOnly()){
            sendCommand("S${snoozeTime.toFloat().times(60)}")
        }
    }

    private fun updateBlanketWeight() {
        val blanketWeight = _state.value.blanketWeight
        if (blanketWeight.isDigitsOnly()){
            sendCommand("B$blanketWeight")
        }
    }

    private fun updateAlarmTime() {
        // first update the clock time
        val time = _state.value.alarmTime

        val hours = time.hour.toString().padStart(2, '0')
        val minutes = time.minute.toString().padStart(2, '0')
        val seconds = time.second.toString().padStart(2, '0')

        val timeStr = "T$hours:$minutes:$seconds"

        Log.d("ALARM_BT", "time: $timeStr")
        sendCommand(timeStr)
    }

    private fun updateClockTime() {
        val time = LocalTime.now()
        val hours = time.hour.toString().padStart(2, '0')
        val minutes = time.minute.toString().padStart(2, '0')
        val seconds = time.second.toString().padStart(2, '0')

        val timeStr = "C$hours:$minutes:$seconds"
        Log.d("ALARM_BT", "clock time: $timeStr")
        sendCommand(timeStr)
    }

    private fun sendCommand(command: String){
        viewModelScope.launch(Dispatchers.IO) {
            try {
                socket.outputStream.write(command.toByteArray())
            } catch (e: Throwable) {
                setResult(EspResult.Error("Not connected"))
                connectTillConnected()
                Log.d("ALARM_BT", "Something went wrong sending time...")
            }
        }
    }

}