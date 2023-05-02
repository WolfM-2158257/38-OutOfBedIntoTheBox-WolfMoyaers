package com.nexus.weightedalarm.alarm

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.marosseleng.compose.material3.datetimepickers.time.ui.TimePicker
import java.time.LocalTime

@Composable
fun AlarmScreen(
    viewModel: AlarmViewModel
) {
    val state by viewModel.state.collectAsState()

    UI(state, viewModel::onAction, viewModel::onEvent)
}

@Composable
fun UI(state: AlarmState, onAction: (AlarmAction) -> Unit, onEvent: (AlarmEvent) -> Unit) {
    val result = state.result
    Column(modifier = Modifier.padding(16.dp)){
        Alarm(state = state, onAction = onAction, onEvent = onEvent)
        Spacer(Modifier.weight(1.0f))
        Row{
            BluetoothActionButton(modifier = Modifier.weight(1.0f), result = state.result, text = "Update ESP Clock", action = {onAction(AlarmAction.SetClockTime)})
            BluetoothActionButton(modifier = Modifier.weight(1.0f), result = state.result, text = "Set Alarm", action = {onAction(AlarmAction.SetAlarmTime)})
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun Alarm(state: AlarmState, onAction: (AlarmAction) -> Unit, onEvent: (AlarmEvent) -> Unit){
    TimePicker(
        initialTime = LocalTime.now(),
        title = {
            Text(
                modifier = Modifier.fillMaxWidth(),
                text = "Alarm Time",
                fontSize = 32.sp,
                textAlign = TextAlign.Center
            )
        },
        onTimeChange = {
            onEvent(AlarmEvent.OnTimeChange(it.hour, it.minute, it.second))
        }
    )
}


@Composable
fun BluetoothActionButton(modifier: Modifier, result: EspResult, text: String, action: () -> Unit){
    Button(
        modifier = modifier,
        enabled = (result is EspResult.Success),
        onClick = action
    ) {
        when(result){
            is EspResult.Success -> Text(text = text)
            is EspResult.Loading -> CircularProgressIndicator()
            is EspResult.Error -> Text(text = "Something went wrong, trying again...")
        }
    }
}