#pragma once

#include "Arduino.h"
#include "Audio.h"
#include <SPIFFS.h>


class Player{
    private:
        // Define I2S connections for MAX98357 I2S Amplifier module
        const int I2S_LRC = 12;
        const int I2S_BCLK = 13;
        const int I2S_DOUT = 15;

        Audio audio{};
        String currentChannel = "0n-80s.radionetz.de:8000/0n-70s.mp3";
        static const int defaultVolume = 5;

    public:
        Player();
        void loop();
        bool isPlaying();
        void play();
        void stop();
        void setVolume(int volume);
        void setVolume(std::string volume);
        int getVolume();

        /**
         * @brief 
         * Sets the current radio channel
         * possible channels:
            * "http://vis.media-ice.musicradio.com/CapitalMP3"
            * "mediaserv30.live-nect MAX98357 I2S Amplifier Modu
            * "www.surfmusic.de/m3u/100-5-das-hitradio,4529.m3u"
            * "stream.1a-webradio.de/deutsch/mp3-128/vtuner-1a"
            * "www.antenne.de/webradio/antenne.m3u"
        */
        void setChannel(String channel);
};