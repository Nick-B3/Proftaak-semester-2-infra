# Proftaak-semester-2-infra
Het proftaak project van Fontys ICT infrastructuur semester 2. 

In dit project wordt er aan de hand van een zelfbedacht protocol data verstuurd over een websocket naar de verbonden IoT devices. 
## Installatie handleiding

### Go Websocket server
Om de server te gebruiken als lokale development server:
1. Download de github als zip
2. Unzip het bestand.
3. Ga naar go-websocket-server/
4. Voer het commando uit: go run .

Onze websocket stream stuurt data vanaf:
>https://socket.fhict.be/ws

### Websocket client
Om de websocket te gebruiken op een Arduino WiFi Shield 101 and MKR1000 bord, moet je een speciale Wifi101 bibliotheek gebruiken.
1. Navigeer naar [Wifi101](https://github.com/khoih-prog/WiFi101).
2. Klik op code, en download ZIP-bestand.
3. Pak het ZIP-bestand uit en hernoem het naar Wifi101.
4. Kopieer de gehele map naar de Arduino libraries' map (Documents/Arduino/libraries/).
5. Verander in de Websocket INI de LED pin en aantal LED
6. Verander credentials-example.h -> credentials.h
7. Pas Wi-Fi naam en wachtwoord aan om met Wi-Fi te verbinden

De deserialized JSON-string is aan te roepen door gebruik te maken van de gedefinieerde variabelen, bijvoorbeeld Type_Light_RGB_Green of Intensity_Light. 
## Credits
[Gin Web Framework](https://github.com/gin-gonic/gin)
<br>
[Websockets 2 Library for ESP32/Arduino boards](https://github.com/khoih-prog/WebSockets2_Generic)

## Protocol
De volgende data komt in JSON-string format binnen op de websocket stream.
```JSON
{
    "Location":"right/left",
    "Duration":"0-100(ms)",
    "Type":{
        "Light":{
            "ONOFF":"true/false",
            "RGB":{
                "Red":"0-255", 
                "Green":"0-255",
                "Blue":"0-255"
            }, 
            "HEX":"000000-FFFFFF"
        },
        "Sound":{
            "Frequence":"0-160000(Hz)"
        },
        "Movement":"true/false"
    },
    "Intensity":{
        "LightIntensity":"0-100",
        "SoundIntensity":"0-100"
    }
}
```
