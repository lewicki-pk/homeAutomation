Ficzery:
- komunikacja z node'ami temperaturowymi i wilgotnosciowymi
- komunikacja z OpenHAB
    - tworzenie plikow konfiguracyjnych
    - wysylanie update'ow przy uzyciu MQTT

Dependencies:
- [mosquitto](https://github.com/eclipse/mosquitto)
- [RF24 library](https://github.com/TMRh20/RF24)
- [OpenHAB](http://www.openhab.org/getting-started/downloads.html)


TODO:
- rewrite MQTTProxy to use boost::asio and listen to UDP packages (rename to MQTTProxyClient)
- write another application that receives RF24 messages and forwards them as TCP/IP (and the other way)
- rewrite application to use boost::asio
