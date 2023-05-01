package com.nexus.weightedalarm.alarm

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewmodel.viewModelFactory
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
        Button(
            modifier = Modifier.fillMaxWidth(),
            enabled = (result is EspResult.Success),
            onClick = {onAction(AlarmAction.SetTime)}
        ) {
            when(result){
                is EspResult.Success -> Text(text = "Set Alarm")
                is EspResult.Loading -> CircularProgressIndicator()
                is EspResult.Error -> Text(text = "Something went wrong, trying again...")
            }
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
