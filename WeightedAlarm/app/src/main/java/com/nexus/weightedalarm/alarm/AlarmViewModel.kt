package com.nexus.weightedalarm.alarm

import android.Manifest
import android.annotation.SuppressLint
import android.app.Application
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.pm.PackageManager
import android.util.Log
import androidx.core.app.ActivityCompat
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.util.UUID

class AlarmViewModel(application: Application) : AndroidViewModel(application) {
    private val UUID_ESP = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    private val MODULE_MAC = "24:6F:28:96:68:86"

    private lateinit var socket: BluetoothSocket

    private var _state: MutableStateFlow<AlarmState> = MutableStateFlow(AlarmState())
    val state: StateFlow<AlarmState>
        get() = _state.asStateFlow()

    init {
        viewModelScope.launch {
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
            is AlarmEvent.OnTimeChange -> setTime(event.hours, event.minutes, event.seconds)
        }
    }

    private fun setTime(hours: Int, minutes: Int, seconds: Int) {
        _state.update {
            it.copy(
                alarmTime = AlarmTime(hours, minutes, seconds)
            )
        }
    }

    public fun onAction(action: AlarmAction) {
        when (action) {
            AlarmAction.SetTime -> uploadTime()
        }
    }

    private fun uploadTime() {
        val time = _state.value.alarmTime

        val hours = time.hours.toString().padStart(2, '0')
        val minutes = time.minutes.toString().padStart(2, '0')
        val seconds = time.seconds.toString().padStart(2, '0')

        val timeStr = "T$hours:$minutes:$seconds"

        Log.d("ALARM_BT", "time: $timeStr")
        viewModelScope.launch(Dispatchers.IO) {
            try {
                socket.outputStream.write(timeStr.toByteArray())
            } catch (e: Throwable) {
                setResult(EspResult.Error("Not connected"))
                connectTillConnected()
                Log.d("ALARM_BT", "Something went wrong sending time...")
            }
        }
    }
}