package com.nexus.weightedalarm.alarm

import androidx.compose.foundation.gestures.Orientation
import androidx.compose.foundation.gestures.scrollable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.layout.wrapContentHeight
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.rounded.Send
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.unit.dp
import com.marosseleng.compose.material3.datetimepickers.time.ui.TimePicker

import java.time.LocalTime

@Composable
fun AlarmScreen(
    viewModel: AlarmViewModel
) {
    val state by viewModel.state.collectAsState()

    UI(state, viewModel::onAction, viewModel::onEvent)
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun UI(state: AlarmState, onAction: (AlarmAction) -> Unit, onEvent: (AlarmEvent) -> Unit) {
    val result = state.result
    when(result){
        EspResult.Success -> {
            Scaffold(
                bottomBar = {
                    Row() {
                        BluetoothActionButton(
                            modifier = Modifier.weight(1.0f),
                            text = "Sync ESP Clock",
                            isPrimary = false,
                            action = { onAction(AlarmAction.SetClockTime) })
                        BluetoothActionButton(
                            modifier = Modifier.weight(1.0f),
                            text = "Set Alarm",
                            action = { onAction(AlarmAction.SetAlarmTime) })
                    }
                }
            ) {
                Alarm(modifier = Modifier.padding(it), state = state,  onAction = onAction, onEvent = onEvent)
            }
        }
        else -> {
            Column(
                modifier = Modifier.fillMaxSize(),
                horizontalAlignment = Alignment.CenterHorizontally,
                verticalArrangement = Arrangement.Center
            ){
                CircularProgressIndicator(modifier = Modifier.size(64.dp))
                Text(text = "Connecting...")
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun Alarm(modifier: Modifier = Modifier, state: AlarmState, onAction: (AlarmAction) -> Unit, onEvent: (AlarmEvent) -> Unit){
    Column(
        modifier = modifier
            .padding(8.dp)
            .wrapContentHeight()
            .scrollable(state = rememberScrollState(), orientation = Orientation.Vertical),
    ) {
        TimePicker(
            title = {},
            initialTime = LocalTime.now(),
            onTimeChange = {
                onEvent(AlarmEvent.OnTimeChange(it))
            }
        )
        Spacer(modifier = Modifier.height(8.dp))
        ParameterContainer(
            text = "Volume (%)",
            value = state.volume,
            onValueChange = { onEvent(AlarmEvent.OnVolumeChange(it)) },
            uploadAction = { onAction(AlarmAction.SetVolume) }
        )
        ParameterContainer(
            text = "Snooze time (minutes)",
            value = state.snoozeTime,
            onValueChange = { onEvent(AlarmEvent.OnSnoozeTimeChange(it)) },
            uploadAction = { onAction(AlarmAction.SetSnoozeTime) }
        )
        ParameterContainer(
            text = "Blanket weight (grams)",
            value = state.blanketWeight,
            onValueChange = { onEvent(AlarmEvent.OnBlanketWeightChange(it)) },
            uploadAction = { onAction(AlarmAction.SetBlanketWeight) }
        )
    }
}

@Composable
fun BluetoothActionButton(modifier: Modifier = Modifier, text: String, isPrimary: Boolean = true, action: () -> Unit){
    Button(
        modifier = modifier,
        onClick = action,
        shape = RectangleShape,
        colors = ButtonDefaults.buttonColors(
            containerColor = if (isPrimary) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.secondary,
            contentColor = if (isPrimary) MaterialTheme.colorScheme.onPrimary else MaterialTheme.colorScheme.onSecondary
        )
    ) {
        Text(text = text)
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ParameterContainer(text: String, value: String, onValueChange: (String) -> Unit , uploadAction: () -> Unit){
    Spacer(modifier = Modifier.height(8.dp))
    Row(
        verticalAlignment = Alignment.CenterVertically
    ){
        OutlinedTextField(
            modifier = Modifier.weight(1.0f),
            label = {
                Text(text = text)
            },
            value = value,
            onValueChange = { onValueChange(it) },
            maxLines = 1,
            keyboardOptions = KeyboardOptions.Default.copy(keyboardType = KeyboardType.Number),
        )
        Spacer(modifier = Modifier.width(4.dp))
        IconButton(
            modifier = Modifier.padding(top = 4.dp),
            onClick = uploadAction,
        ){
            Icon(
                imageVector = Icons.Rounded.Send,
                contentDescription = "Upload"
            )
        }
    }
}