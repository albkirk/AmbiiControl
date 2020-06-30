void deepsleep_loop() {
    if (config.DEEPSLEEP && millis() > ONTime_Offset + (ulong(config.ONTime) + Extend_time)*1000) {
        mqtt_publish(mqtt_pathtele(), "Status", "DeepSleep");
        //mqtt_unsubscribe(mqtt_pathconf(), "+");       // already included in mqtt_disconnect() function
        mqtt_disconnect();
        telnet_println("Going to sleep until next event... zzZz :) ");
        delay(100);
        telnet_println("Total time ON: " + String(millis()) + " msec");
        GoingToSleep(config.SLEEPTime, curUTCTime());
    }

}
bool Batt_OK_check() {                      // If LOW Batt, it will DeepSleep forever!
    Batt_Level = getVoltage();              // Check Battery Level
    if (Batt_Level < Batt_L_Thrs) {
          mqtt_publish(mqtt_pathtele(), "Status", "LOW Battery");
          mqtt_publish(mqtt_pathtele(), "BattLevel", String(Batt_Level));
          mqtt_disconnect();
          telnet_println("Going to sleep forever. Please, recharge the battery ! ! ! ");
          delay(100);
          GoingToSleep(0, curUnixTime());   // Sleep forever
          return false;                     // Actually, it will never return !!
    }
    return true;
}

void status_report() {
    if (BattPowered && Batt_OK_check()) {
         mqtt_publish(mqtt_pathtele(), "Status", "Battery");
         //mqtt_publish(mqtt_pathtele(), "BattLevel", String(Batt_Level));

    }
    else mqtt_publish(mqtt_pathtele(), "Status", "Mains");
}
