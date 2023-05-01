package com.nexus.weightedalarm

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothServerSocket
import android.bluetooth.BluetoothSocket
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.widget.TextView
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.core.app.ActivityCompat
import com.nexus.weightedalarm.ui.theme.WeightedAlarmTheme
import java.io.IOException
import java.util.UUID


// mac-address: 24:6F:28:96:68:86

class MainActivity : ComponentActivity() {
//
    val MODULE_MAC = "24:6F:28:96:68:86"
    val REQUEST_ENABLE_BT = 1
    val name = "WeightedAlarm"
    val uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.d("BLUETOOTHAPP", "initializing bluetooth")
        this.initBluetooth();
        Log.d("BLUETOOTHAPP", "initializing bluetooth done")

        setContent {
            WeightedAlarmTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Greeting("Android")
                }
            }
        }
    }

    private fun initBluetooth() {
        // get the bluetoothadapter
        val bluetoothManager: BluetoothManager = getSystemService(BluetoothManager::class.java)
        val bluetoothAdapter: BluetoothAdapter? = bluetoothManager.getAdapter()
        if (bluetoothAdapter == null) {
            Log.d("BLUETOOTHAPP", "no bluetooth adapter")
            return
            // Device doesn't support Bluetooth
        }

        // enable bluetooth

        // connect bluetooth device
        // this doesnt work?
        if (!bluetoothAdapter.isEnabled) {
            Log.d("BLUETOOTHAPP", "permission not enabled")
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT)
        }
        // check permission
        if (ActivityCompat.checkSelfPermission(
                this,
                Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            Log.d("BLUETOOTHAPP", "permission not granted")
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
//            return
        }
        val device: BluetoothDevice = bluetoothAdapter.getRemoteDevice(MODULE_MAC)
        val socket: BluetoothSocket = device.createRfcommSocketToServiceRecord(uuid)
        try{
            socket.connect()
            Log.d("BLUETOOTHAPP", "connected to device")
        }
        catch(e: Throwable){
            Log.d("BLUETOOTHAPP", "connection to device failed")
        }

        // blocking calls
        while(true){
            try{
                val inputStream = socket.inputStream
                val outputStream = socket.outputStream
                try {
                    outputStream.write("Yess".toByteArray())
                    Log.d("BLUETOOTHAPP", "written to outputstream")
                }catch(e: Throwable){
                    Log.d("BLUETOOTHAPP", "something went wrong writing" + e.message)
                }
            }
            catch(e: Throwable){
                Log.d("BLUETOOTHAPP", "something went wrong outputstream" + e.message)
            }
        }
    }
}

//    private inner class AcceptThread : Thread() {
//
//        private val mmServerSocket: BluetoothServerSocket? by lazy(LazyThreadSafetyMode.NONE) {
//            bluetoothAdapter?.listenUsingInsecureRfcommWithServiceRecord(NAME, MY_UUID)
//        }
//
//        override fun run() {
//            // Keep listening until exception occurs or a socket is returned.
//            var shouldLoop = true
//            while (shouldLoop) {
//                val socket: BluetoothSocket? = try {
//                    mmServerSocket?.accept()
//                } catch (e: IOException) {
//                    Log.e(TAG, "Socket's accept() method failed", e)
//                    shouldLoop = false
//                    null
//                }
//                socket?.also {
//                    manageMyConnectedSocket(it)
//                    mmServerSocket?.close()
//                    shouldLoop = false
//                }
//            }
//        }
//
//        // Closes the connect socket and causes the thread to finish.
//        fun cancel() {
//            try {
//                mmServerSocket?.close()
//            } catch (e: IOException) {
//                Log.e(TAG, "Could not close the connect socket", e)
//            }
//        }
//    }
//
//}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Log.d("GREETINGS", "GREEETINGS BROTHER")
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    WeightedAlarmTheme {
        Greeting("Android")
    }
}