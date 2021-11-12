# Proftaak-semester-2-infra
Het proftaak project van Fontys ICT infrastructuur semester 2. In dit project maken wij een dirigent server deze kan meerdere IOT devices aansturen.  
Om mee te doen met ons project ga naar onze website die in onze README te vinden is. Dit legt stap voor stap uit hoe je jouw device aan onze omgeving kan koppelen.
## Credits
[Gin Web Framework](https://github.com/gin-gonic/gin)

## Protocol JSON 
```
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
        "Light":"0-100",
        "Sound":"0-100"
    }
}
```