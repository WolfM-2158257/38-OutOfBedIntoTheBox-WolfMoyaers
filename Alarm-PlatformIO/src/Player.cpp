#include "Player.h"

Player::Player()
{

	delay(250);

	if (!SPIFFS.begin()) { // Initialize SPIFFS
		Serial.println(F("An Error has occurred while mounting SPIFFS"));
		return;
	}

	// Connect MAX98357 I2S Amplifier Module
	audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
	audio.setVolume(defaultVolume);

	// audio.connecttohost(currentChannel.c_str());
	audio.connecttoFS(SPIFFS, "/alarm.mp3");
	audio.setFileLoop(true);
}

void Player::loop()
{
	audio.loop();
}

bool Player::isPlaying()
{
    return audio.isRunning();
}

void Player::play()
{
    if (!isPlaying()){
        Serial.println("Audio is not running, playing alarm.");
		audio.pauseResume();
        // audio.connecttohost(currentChannel.c_str()); // can't use a radio, cuz we can't use wifi while using bluetooth...
    }
}

void Player::stop()
{
    if (isPlaying()){
        Serial.println("Audio is running, stopping it.");
		audio.pauseResume();
    }
}
void Player::setVolume(int volume)
{
	if (volume < 30){
		audio.setVolume(volume);
	}
	else{
		Serial.println("Volume is too high");
	}

}

void Player::setVolume(std::string volume){
	setVolume(stoi(volume));
}

int Player::getVolume()
{
    return audio.getVolume();
}

void Player::setChannel(String channel)
{
	currentChannel = channel;
	audio.connecttohost(channel.c_str());
}

// Audio status functions

void audio_info(const char *info)
{
	Serial.print("info        ");
	Serial.println(info);
}
void audio_id3data(const char *info)
{ // id3 metadata
	Serial.print("id3data     ");
	Serial.println(info);
}
void audio_eof_mp3(const char *info)
{ // end of file
	Serial.print("eof_mp3     ");
	Serial.println(info);
}
void audio_showstation(const char *info)
{
	Serial.print("station     ");
	Serial.println(info);
}
void audio_showstreaminfo(const char *info)
{
	Serial.print("streaminfo  ");
	Serial.println(info);
}
void audio_showstreamtitle(const char *info)
{
	Serial.print("streamtitle ");
	Serial.println(info);
}
void audio_bitrate(const char *info)
{
	Serial.print("bitrate     ");
	Serial.println(info);
}
void audio_commercial(const char *info)
{ // duration in sec
	Serial.print("commercial  ");
	Serial.println(info);
}
void audio_icyurl(const char *info)
{ // homepage
	Serial.print("icyurl      ");
	Serial.println(info);
}
void audio_lasthost(const char *info)
{ // stream URL played
	Serial.print("lasthost    ");
	Serial.println(info);
}
void audio_eof_speech(const char *info)
{
	Serial.print("eof_speech  ");
	Serial.println(info);
}