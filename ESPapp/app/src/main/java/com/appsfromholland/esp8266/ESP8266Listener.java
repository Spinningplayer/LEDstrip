package com.appsfromholland.esp8266;

/**
 * Created by dkroeske on 15/05/2017.
 */

public interface ESP8266Listener {
    void onESP8266CounterAvailable(int counterValue);
    void onESP8266Error();
}
