# Ioniq-BSO-Remote-Monitor
### Monitor the charge of your Ioniq PHEV remotely from your smartphone

![Screenshot_IONIQ_BSO](https://user-images.githubusercontent.com/50306926/59957631-8d6b3a80-949a-11e9-94d0-610644a23630.jpg)

![yt](https://user-images.githubusercontent.com/50306926/59966678-19737580-9520-11e9-961e-71a477c88894.jpg) Visita [Mi lista de Youtube](https://www.youtube.com/playlist?list=PLTzjC-Oy2s-5wAB1-0_I-gAGDHgAlO9_A) para ver algunos videos de ejemplo

---------------------------------------------------------------

# VERSIÓN EN ESPAÑOL
**As soon as I finish writing in Spanish, I will try to write it in English. Meanwhile, Google Translate will help you.**

- [¿Para que sirve?](#para-que-sirve)
- [¿Como funciona?](#como-funciona)
- [Funcionalidades](#funcionalidades)
- [¿Qué necesito?](#qué-necesito)
- [Preguntas previas sobre el proyecto](#preguntas-previas-sobre-el-proyecto)
  - [¿Como sé que el OBD va a funcionar?](#como-sé-que-el-obd-va-a-funcionar)
  - [¿Pero voy a poder usar el OBD en otros programas?](#Pero-voy-a-poder-usar-el-OBD-en-otros-programas)
  - [WTF!!! ¿Puedo montar Torque Pro o Hybrid Assitant en el multimedia de mi Ioniq?](#Puedo-montar-Torque-Pro-o-Hybrid-Assistant-en-mi-multimedia)
  - [¿Pero porqué no utilizar un OBD WiFi directamente?](#Pero-porqué-no-utilizar-un-OBD-WiFi-directamente)
  - [¿Pero porqué no usar un OBD Bluetooth directamente?](#Pero-porqué-no-usar-un-OBD-Bluetooth-directamente)
  - [¿Porqué Lolin NodeMCU y no otra placa de la familia ESP?](#porqué-Lolin-NodeMCU-y-no-otra-placa-de-la-familia-ESP)
  - [Instalación del software ESP-LINK](#instalación-del-software-esp-link)

- [Instalación del software ESP-LINK](#Instalación-del-software-ESP-LINK)
- [Modificación del OBD](#modificación-del-OBD)
- [Configuración del servicio MQTT](#Configuración-del-servicio-MQTT)
- [Instalación de Ioniq BSO Remote Monitor en ESP8266](#Instalación-de-Ioniq-BSO-Remote-Monitor-en-ESP8266)
  -[Configuración de `Config.h`](#Configuración-de-`Config.h`)
    - [Configuración de WiFi y OBD](#Configuración-de-WiFi-y-OBD) 
    - [Configuración del broker MQTT](#Configuración-del-broker-MQTT])
    - [Configuración de Telegram](#Configuración-de-Telegram])
    - [Configuración de DDNS](#Configuración-de-DDNS)
    - [Otras configuraciones](#Otras-configuraciones)
    
  - [Librerías](#librerías)  
  - [Compilado e instalación](#Compilado-e-instalación)
  - [Visualización de datos en tu smartphone o navegador](#Visualización-de-datos-en-tu-smartphone-o-navegador)
  - [Screenshots](#Screenshots)


## ¿Para que sirve?
Antes de nada quiero decir que aunque profesionalmente me he dedicado a la programación y he estado relacionado con la electrónica, Arduino, ESP8266 y el lenguaje C++ no es mi mundo. Lo he realizado lo mejor que podía, pero seguro que encontrareis errores y mejores maneras de hacer muchos de los pasos que he programado. Sed benébolos :pray:. Gracias.

Bien, los vehículos Hyundai Ioniq PHEV o EV vendidos en Europa no incluyen ningún sistema de monitorización de la batería o del proceso de carga, por lo que decidí hacerme un sistema que me permitiera dejar mi Ioniq cargando y que mediante mi smartphone pudiera ver el estado y me avisase al finalizar de la carga. Además quería que el sistema fuera lo mas sencillo posible, aunque haya que tener conocimientos de programación y algo de electrónica básica. No es equiparable al sistema BlueLink que se suministra en USA o Corea, pero realiza las funciones básicas para las que se ha programado.

Un punto importante es que yo ni me hago responsable de la manipulación que hagas a tu OBD o en tu Ioniq. Conectar un OBD al coche no hace perder la garantía, pero si te equivocas, lo haces mal o produces un cortocircuito, es bajo tu única responsabilidad.


## ¿Como funciona?
Aunque os pueda parecer complicado, el sistema es mas sencillo de lo que parece. Mediante un lector OBD2 modificado para funcionar con WiFi como cliente, una placa de la familia ESP8266 se conecta al OBD2 y recoge los datos necesarios que envía a un broker MQTT. Se ha probado mediante los servidores gratuitos de Adafruit y CloudMQTT, aunque puede utilizarse cualquier otro. Posteriormente desde nuestro smartphone veremos a tiempo real el estado de nuestro vehículo a través de una página web que está copiada en nuestro dispositivo, y que no precisa instalar ninguna aplicación o contratar servicios externos de web o bases de datos. Además, si el usuario dispone de mínimos conocimientos de web y javascript puede crear la pantalla a su medida. Anímate a modificar los HTML's a tu gusto y visita [mi lista de Youtube](https://www.youtube.com/playlist?list=PLTzjC-Oy2s-5wAB1-0_I-gAGDHgAlO9_A) para ver algunos videos de ejemplo.


![esquema](https://user-images.githubusercontent.com/50306926/59946559-24200300-946b-11e9-8d00-6204caec7187.jpg)

## Funcionalidades
La aplicación permite monitorizar los siguientes datos:
* Carga actual de la batería en % (SOC Display)
* Potencia de carga o descarga de la batería
* Voltaje del batería
* Intensidad de carga o descarga
* Previsión de kilómetros restantes en modo eléctrico
* Temperatura máxima, mínima y media de la batería
* Estado del ventilador de las baterías
* Conector de carga conectado o desconectado
* Cantidad de energía cargada
* Tiempo estimado de finalización de carga
* Informe vía Telegram
* Cliente DDNS
* Posibilidad de enlazar con IFTTT para apagar cargadores u otras ideas. Tu imaginación manda...



## ¿Qué necesito?
* Primero de todo paciencia y leer las instrucciones. Si te cansa leer, no sigas.

* 2 placas de la familia ESP8266. Yo utilizo una Lolin MCU V3 para el OBD y una WiFi Kit 8 de [Heltec Automation](https://heltec.org/project/wifi-kit-8/) para la recogida y envío de datos. Realmente puedes utilizar las placas que te vengan en gana, ya que el software debería funcionar sin problemas. Para pruebas también he utilizado Wemos D1 mini. La WiFi Kit 8 dispone de pantalla 0.91″ OLED y cargador automático de baterías de litio. Eso permite que cuando el coche está cargando, la placa dispone de batería para varios días. Puedes poner un power bank o cualquier otro sistema que se te ocurra ya que los USB's del Ioniq no disponen de corriente durante la carga.

* 1 unidad de OBD2 **que funcione con el Hyunday Ioniq** PHEV y que sea Bluetooth o WiFi.

* El software [ESP-LINK](https://github.com/jeelabs/esp-link) de [JeeLabs](https://github.com/jeelabs) que encontrarás en  https://github.com/jeelabs/esp-link . Este software permite realizar un gateway entre la UART del OBD y nuestra WiFi y deberás instalarlo en la primera de las placas ESP, en mi caso la Lolin NodeMCU. JeeLabs tiene toda la documentación al respecto. Por favor, leela en detalle para poder instalarla.

* Un router 2, 3 ó 4G en tu vehículo para poder enviar los datos al servidor de MQTT, o tu propio smartphone con la compartición de WiFi activada. Yo dispongo de un router D-Link DWR-730 comprado de segunda mano por poco mas de 20€, pero el mínimo tráfico de datos hace que cualquiera sirva.


## Preguntas previas sobre el proyecto

### ¿Como sé que mi OBD va a funcionar?
Lo siento, pero deberás hacr pruebas previas para saber que sirve. Yo todavía no puedo darte un link correcto de compra o de un fabricante concreto, ya que el mio hace muchos años que lo compré y funciona perfectamente, pero tras numerosas pruebas y compras de diferentes OBD's sabemos que los que tienen version 1.5 de firmware disponen de una mayor probabilidad de funcionar, aunque no todos. Suelen funcionar OBD's antiguos, de mas de 3 ó 4 años. Además necesitarás sacar la placa bluetooth o WiFi que lleva para realizar la modificación necesaria, por lo que no podrá ser un OBD monoplaca, o que no permita su manipulación. Si el OBD que tienes dispone de la placa bluetooth o WiFi separable, antes de manipularlo deberás probar con Torque Pro y los ficheros PID de [JejuSoul](https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs) que encontrarás en https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs. No te voy a engañar... es una loteria encontrar uno que funcione, pero los productos de China son así. Sobre todo no te fies del exterior con las imagenes que pondré ya que no es garantia de que funcione. Si con los PID de [JejuSoul](https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs) te funciona y puedes leer parámetros de la batería, casi seguro que es garantía de éxito.
<br/>
<br/>

### ¿Pero voy a poder usar el OBD en otros programas?
Si, yo continuo utilizándolo con Torque o Hybrid Assistant desde mi smartphone o, mejor todavía, desde el multimedia de mi Ioniq. Por ese motivo utilizo dos ESP8266 y así poder utilizar otros programas. Eso si, no puedo utilizar multiples APP's y mi aplicación simultáneamente. Adjunto captura de Torque pro en mi multimedia del Ioniq:
<br/>
<br/>

![IMG_20190322_233611_294](https://user-images.githubusercontent.com/50306926/59950477-57b45a80-9476-11e9-8835-0e7f98892dc2.jpg)
<br/>
<br/>

### WTF!!! ¿Puedo montar Torque Pro o Hybrid Assitant en el multimedia de mi Ioniq?
Si, pero no es aquí donde te explicaré eso. Visita [Ioniq Spain](https://ioniqspain.wordpress.com/2019/02/20/acceso-completo-a-la-capa-android/), el blog de nuestro compañero Smoje79, y podrás ver como se hace.


### ¿Pero porqué no utilizar un OBD WiFi directamente? 
Los OBD's WiFi que hay en el mercado funcionan como punto de acceso (AP) y no como estación (STA). La placa encargada de recoger los datos se conectaría sin problemas al OBD, pero no podría salir a Internet para enviar los datos. Por ese motivo modifico el OBD poniendo un ESP8266. Si alguien tiene alguna otra idea, será bienvenida.
<br/>
<br/>

### ¿Pero porqué no usar un OBD Bluetooth directamente?
Esa fue mi primera idea, mediante un ESP32 conectar al OBD y de ahí enviar los datos al broker MQTT. Lo siento, no fuí capaz de utilizar el Bluetooth clasico con éxito mediante el IDE de Arduino. Mediante el ESP-IDF de Espressif parece que sería posible, pero mis conocimientos en el ESP32 no son suficientes por el momento.
<br/>
<br/>

### ¿Porqué Lolin NodeMCU y no otra placa de la familia ESP?
Muy simple, es la que tenía a mano. Una muy buena opción es Wemos D1 Mini ya que es pequeña, igual de barata y funciona perfectamente. Posiblemente sustituya Lolin por esta placa. Agadeceré que me digáis cual usáis vosotros. 
<br/>
<br/>

---------------------------------------------------------------


## Instalación del software ESP-LINK
Después de hacer diferentes pruebas con software propio, [ESP-LINK](https://github.com/jeelabs/esp-link) me ha parecido la mejor opción. Es un software fluido, facil de gestionar y que nos permite multitud de opciones. Tal como ya he dicho anteriormente, seguid las instrucciones de instalación y comprobad que funciona aunque os adjunto la configuración utilizada por mi compañero Ángel desde el programador de firmware del NodeMCU: 

![firmwareESPLINK](https://user-images.githubusercontent.com/50306926/59951787-40776c00-947a-11e9-9f69-225841a29907.jpg)


Una vez instalado el ESP-LINK conectaros a la Wifi que la placa levantará y que veréis que está sin seguridad, para posteriormente vía web ir a la IP 192.168.4.1. Deberéis cambiar la velocidad de conexión de la UART desde la opción de menú *"μC CONSOLE"*, dejándolo a 38400 bauds:

![photo5793915804792762996](https://user-images.githubusercontent.com/50306926/59952544-e2985380-947c-11e9-9bc5-ad3b606f6fc7.jpg)


Os recomiendo desactivar el log desde *"Debug log"* dejándolo en OFF:

![photo5793915804792762994](https://user-images.githubusercontent.com/50306926/59952545-e2985380-947c-11e9-86ba-340452ddb4fa.jpg)

  

Por último podéis poner una IP fija para que la placa encargada de recoger los datos del OBD pueda encontrar al ESP-LINK- Esta IP junto el puerto 2323 es lo que deberéis configurar mas tarde en la segunda placa ESP8266:

![photo5793915804792762995](https://user-images.githubusercontent.com/50306926/59952543-e2985380-947c-11e9-91af-2c1fb08e84d6.jpg)



# Modificación del OBD
Antes de nada, gracias a Ángel por su idea. El fué el primero en modificar su OBD y en explicame lo que había hecho :-)
En principio las modificaciones son sencillas, pero requieren habilidad en soldaduras de pequeño tamaño. Mi OBD originalente era así:

![IMG_20190126_114531](https://user-images.githubusercontent.com/50306926/59947687-33548000-946e-11e9-9c94-68ce4eea7137.jpg)


Se tiene que desoldar la placa Bluetooth para poder conectar los pines del ESP8266. En mi modelo el conexionado es el siguiente, pero dependiendo de como sea tu OBD, deberás investigar que pines utilizar. Yo no puedo explicártelo si es diferente.

![pineout](https://user-images.githubusercontent.com/50306926/59955764-85a49980-948c-11e9-807d-163d0b577c75.jpg)


Si, he probado otras placas mas pequeñas como la Wemos D1 Mini que funciona perfectamente, o la ESP-01 que cabía perfectamente dentro del OBD. Para no sobrecargar el regulador de tensión de 3.3v que lleva el propio OBD, le instalé un regulador que se alimentaba desde los 5 voltios, tal como hago con la Lolin NodeMCU, pero no acabó de convencerme la temperatura del regulador de tensión. En la siguiente imagen veréis como quedaba el montaje con ESP-01:

![ESP01](https://user-images.githubusercontent.com/50306926/59949205-b4ae1180-9472-11e9-8be2-156f9dbae6aa.jpg)



**IMPORTANTE:** Antes de comprobar que funciona revisa el cabelado 2 veces por lo menos, especialmente la alimentación de 5 voltios a y GND para evitar cortocircuitos que puedan dañar a los circuitos del OBD, del ESP8266 o del propio coche. Mi recomendación es que hagas la prueba con una fuente de laboratorio y conectes 12 voltios al OBD mediante conectores. Los pines que tienes que conectar son el 16 con 12V+ y el 4 y 5 con el negativo (GND). Si todo funciona correctamente el OBD y el ESP8266 arrancará y podrás acceder a él:

![Pineout_OBD](https://user-images.githubusercontent.com/50306926/59956185-049ad180-948f-11e9-984f-49ede28667f0.jpg)


Si todo es correcto, puedes probar a conectarte al OBD desde un navegador y acceder al menú *"μC CONSOLE"*. Desde ahí teclea el comando *ATZ* y deberás obtener la respuesta de la versión del firmware del OBD, en mi caso *ELM 327 v1.5*. Si es así, todo funciona correctamente y ya está realizado lo mas duro del montaje.

![atz](https://user-images.githubusercontent.com/50306926/59956657-cbb02c00-9491-11e9-886a-4882cf2a8678.jpg)


# Instalación de Ioniq BSO Remote Monitor en ESP8266

## Configuración de `Config.h`
Ahora ya podemos pasar a instalar el software del Ioniq BSO Remote Monitor en nuestra segunda placa ESP8266. Como ya he dicho anteriormente, puedes utilizar la que te venga en gana, pero deberás configurar el `Config.h` antes de compilar el código. En estas 4 primeras líneas puedes escoger si utilizar o no el bot de Telegram, cliente de DDNS (DNS dinámico), habilitar el modo test para pruebas sin el OBD y si utilizas un ESP WIFI Kit 8 de Heltec o cualquier otro ESP. Si no tienes esa placa, déjala comentada ya que podrás usar cualquiera.

```c#
#define ENABLE_TELEGRAM;          //Disconnect if you do not want notifications by Telegram bot
//#define ENABLE_DDNS;              //Disconnect if not use dynamic DNS
//#define ENABLE_TEST_MODE        //If enabled, for send data tests
//#define ENABLE_HELTEC_WIFI_Kit_8  //If defined, enable for compile HELTEC_WIFI_Kit_8
```
<br/>
<br/>


### Configuración de WiFi y OBD
`Config.h` lleva la posibilidad de utilizar dos configuraciones de WiFi y OBD diferentes. La primera es la que se usa si habilitas el `TEST_MODE`. Este modo permite trabajar sin un OBD, enviándose tramas de MQTT generadas manualmente y que permiten probar la aplicación y su funcionamiento en tu smartphone o en un navegador en tu ordenador. Puedes modificar, añadir o quitar tramas de prueba desde la función `testString`que está al final del código. Verás que el `TEST_MODE` incluye una IP del OBD, pero no es necesaria que exista realmente. Debes desabilitar `TEST_MODE` para el poder trabajar de forma normal. Fíjate que el puerto es el 2323, que es junto al 23, los que ESP-LINK utiliza.

```c#
// ***************************************************************************
// IMPORTANT: Enable wifi and OBD fot test or work in Ioniq
// ***************************************************************************
#ifdef ENABLE_TEST_MODE  ///Enable wifi for test in your LAN
    int testLine = 1; 
    const char* ssid = "YourSSIDForTest";
    const char* password = "YourPasswordForTest";
    int port= 2323; // Port OBD device
    IPAddress serverOBD(192,168,1,200);  //IP address OBD device
    
#else  //Enable wifi and IP for OBD in your Ioniq
    int testLine = 0; 
    const char* ssid = "YourSSIDIoniq";
    const char* password = "PasswordWifiIoniq";
    int port= 23; // Port OBD device
    IPAddress serverOBD(192,168,0,2);  //IP address OBD device
#endif
```
<br/>
<br/>
<br/>

### Configuración del broker MQTT
¿No conoces MQTT? Pues de nuevo Google puede ayudarte, pero puedes visitar [¿Qué es MQTT? Su importancia como protocolo IOT](https://www.luisllamas.es/que-es-mqtt-su-importancia-como-protocolo-iot/) del incombustible [Lluis Llamas](https://www.luisllamas.es/) que tiene este y otros artículos muy interesantes. 

Si mas o menos ya estás al dia de MQTT y si estás dado de alta en ningún servicio, puedes visitar [Cloudmqtt.com](https://cloudmqtt.com) y realizar el alta gratuita. Habilita un plan `Cute Cat` con el que tienes suficiente para este servicio. Cuando estes dado de alta, habilita un usuario desde *Users and ACL* y posteriormente un *nodo* o como ellos le llaman *ACL*. Puedes crear el usuario y nodo como tu quieras, pero si no quieres modificar la configuración que he puesto utiliza el usuario `ioniq` y el nodo `bso`, y habilita lectura y escritura. Debería quedar así:


![cloud](https://user-images.githubusercontent.com/50306926/59964449-3ea4bb80-9501-11e9-95bd-a284a6d38ed1.jpg)

La configuración en `Config.h` lleva dos ejemplos de servidores MQTT, estando el de la configuración de Adafruit comentado. Como ya he dicho, yo utilizo [CloudMQTT](https://www.cloudmqtt.com/). Todos los datos se envían en un array a un único tópic, por lo con el plan *Cute Cat* puedo trabajar sin problema. Además [CloudMQTT](https://www.cloudmqtt.com/) dispone la *persistencia*, lo que permite que aunque la placa está apagada se visualicen los últimos datos enviados. Adafruit no permite esa característica sin realizar acciones intermedias.

Por supuesto que podéis utilizar el servidor MQTT que mas os guste. Espero vuestros comentarios al respecto.

```c#
/*
// ***************************************************************************
// Very interesting to be used with IFTTT, but it has the inconveneinte that
// does not support persistence in MQTT
// ***************************************************************************
const char* mqtt_server = "io.adafruit.com";
const int mqttPort = 1883;
const char* mqttUser = "YourUsername";                           // Your Username
const char* mqttPassword = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";   // Your Active Key Key
const char* nodemqtt= "Username/f/ioniq";  //                    // Your feed
*/


// ***************************************************************************
// Config server mqttt cloudmqtt.com. Support persistence data.
// ***************************************************************************
const char* mqtt_server = "m24.cloudmqtt.com";   // Your server in CloudMQTT
const int mqttPort = 14357;                      // Your port NOT SSL
const char* mqttUser = "UserCloudMQTT";          // Your user in CloudMQTT
const char* mqttPassword = "PasswordCloudMQTT";  // Your password in CloudMQTT
const char* nodemqtt= "ioniq/bso";               // Your topiq in CloudMQTT
```

<br/>

### Configuración de Telegram
El siguiente bloque es la configuración del bot de Télegram, el cual permite recibir los informes de carga cuando esta finaliza:

![telegram](https://user-images.githubusercontent.com/50306926/59963526-59bcfe80-94f4-11e9-8425-da87d4fcc1f0.jpg)

La configuración es mu sencilla, pero dirás...¿Como diablos creo y configuro el bot de Telegram?. Pues primero de todo, Google vuelve a ayudarte y ahí encontrarás respuestas. De todas formas te dejo unos links que pueden ser de interés:
- [How do I create a bot?](https://core.telegram.org/bots#3-how-do-i-create-a-bot)
- [How to create an Telegram Bot Token & Get Chat ID?](https://www.youtube.com/watch?v=2jdsvSKVXNs)

En el apartado de `Config.h` únicamente deberás poner el idioma del mensaje, token y el chatId que corresponda.
```c#
String telegramLang= "ESP"  // ESP= Spanish |  ENG = English
String BOTtoken = "botxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  //token format is botXXXXXXXXXXXXXXXXXXXXXXXXXXX
String Chat_id = "123456789"; // Chat_id
IPAddress telegramServer(149, 154, 167, 200); // IP de api.telegram.org
```

<br/>


### Configuración de DDNS
Si lo deseas, puedes configurar el servicio de DDNS. ¿Qué para que lo necesitas? Pues por ejemplo por si quieres acceder desde el exterior a tu OBD o necesitas saber la IP de tu vehículo. Yo lo he dejado preparado ya que me interesa en un futuro próximo para modificaciones que tengo pensadas. Yo utilizo el servicio de [NOIP.com](https://www.noip.com/), pero verás que puedes usar otros.

```c#
// ***************************************************************************
// Config DDNS
// ***************************************************************************
String ddnsService = "noip"; // Enter your DDNS Service Name - "duckdns" / "noip" / "dyndns" / "dynu" / "enom".
String ddnsDomain ="YourName.ddns.net"; 
String ddnsUsername ="UserDDNS";
String ddnsPassword ="PasswordDDNS";
int ddnsUpdate = 10000; // Check for New Ip Every 10 Seconds.
```

<br/>

### Otras configuraciones
En este apartado está el nombre del vehículo que se envía en la notificación vía Telegram, el tamaño de la batería del nuestro Ioniq y las configuraciones horarias de zona. Tengo pendiente calcular automáticamente si es verano o invierno.
```c#
String vehicleID = "My IONIQ";   // VehicleID is sended in telegram message
float kWhBattery = 8.9;          // For Ioniq PHEV
//float kWhBattery = 38,3;       // For Ioniq EV
int timeZone = 1;                // Your time zone
bool daylightSaving = true;      // False for winter time, true for summer time.
```

## Librerías
En las librerias `NTPClient.h` y `EasyDDNS.h` añado las URL's de descarga para facilitar la instalación.
```c#
#include <time.h>              //
#include <NTPClient.h>         // https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>           //
#include <WiFiClientSecure.h>  //
#include <EasyDDNS.h>          // https://github.com/ayushsharma82/EasyDDNS
#include <ESP8266WiFi.h>  
#include <PubSubClient.h>      // IMPORTANT: Modify #define MQTT_MAX_PACKET_SIZE 256 in PubSubClient.h file from library directory
```

**IMPORTANTE:** 
Hace falta destacar un punto importante sobre la libería `PubSubClient.h`, que es la encargada de enviar los paquetes MQTT al broker. Como se envía en un único *topic* y el tamaño supera los 127 bits máximos que permite, debe modificarse el tamaño de `MQTT_MAX_PACKET_SIZE`a 256. Deberás buscar el fichero `PubSubClient.h`que debería estar en *Arduino/libraries/PubSubClient/src* y editarlo:

```c#
// MQTT_MAX_PACKET_SIZE : Maximum packet size
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 256 //<---- modificar aquí 
#endif
```
<br/>
<br/>

# Compilado e instalación
Si has seguido los pasos indicados, puedes compilar e instalar. Te recomiendo que habilites el `TEST_MODE` para poder ver como funciona a través de la cónsola serie y así podrás ver los datos en tu broker de MQTT a tiempo real.

![console](https://user-images.githubusercontent.com/50306926/59964207-02bc2700-94fe-11e9-987e-66417af0bc00.jpg)


# Visualización de datos en tu smartphone o navegador
Ahora es momento de abrir la carpeta HTML de la distribución. Deberás abrir el fichero `index.html` con un editor de texto y localizar las siguientes líneas, donde debes colocar los datos de conexión a servidor del broker MQTT:

```javascript
/// CONFIGURATION MQTT BROKER
topic = 'ioniq/bso';  // Configure your topic
usuario = 'YourUser';
contrasena = 'YourPassword';
var client = new Paho.MQTT.Client("m24.cloudmqtt.com", 12345, clientId);  // Edit port number
```

Ahora abre `index.html` con un navegador y si tienes la placa con el Ioniq BSO Remote Monitor en funcionamiento y en `TEST_MODE`, deberías ver como los datos van actualizándose. Si es así, copia la carpeta `HTML` a tu smartphone mediante algunos de los programas existentes en la APP Store y deja esa carpeta en algún lugar accesible, como por ejemplo la carpeta *Downloads*. Ahora puedes abrir el `index.html` desde el navegador de tu móvil y ver que todo funciona.

Estamos acabando.... Comenta la linea correspondiente en `Config.h` para desactivar `TEST_MODE`, compila y envía el programa de nuevo a tu placa ESP. Recuerda poner los datos de la WiFi de tu vehículo para que funcione, ya sea compartiendo desde tu smartphone o mediante un router. Conecta el OBD modificado en el Ioniq, alimenta la placa ESP con el programa todo y deberías ver los datos de tu vehículo. 

¿Funciona? Espero que si.


# Screenshots
## **SIN CARGAR**

![Screenshot_IONIQ_BSO_not_charge](https://user-images.githubusercontent.com/50306926/59957781-2d759380-949c-11e9-91c6-4f71c3bdcddf.jpg)
<br/>
<br/>
<br/>

## **DURANTE LA CARGA**

![Screenshot_IONIQ_BSO_charging](https://user-images.githubusercontent.com/50306926/59957752-c9eb6600-949b-11e9-9f2a-82c575926ec2.jpg)
<br/>
<br/>
<br/>

## **EJEMPLO DE PERSONALIZACIÓN**

![Screenshot_IONIQ_BSO_example](https://user-images.githubusercontent.com/50306926/59941543-b91bff80-945d-11e9-8704-061b4d2b4040.png)
<br/>
<br/>
<br/>

## **OTRA PERSONALIZACIÓN**

![Screenshot_IONIQ_BSO_example1](https://user-images.githubusercontent.com/50306926/59941542-b91bff80-945d-11e9-92df-750d3fc649ae.png)
<br/>
<br/>
<br/>

## Descarga de responsabilidad
*Ioniq BSO Remote Monitor  ("El Software") se proporciona como está. Utilice el software bajo su propio riesgo. Los autores no garantizan el rendimiento o la adecuación para un propósito particular, o ninguna otra garantía expresa o implícita. Ninguna comunicación oral o escrita de o autorización de los autores creará una garantía. Bajo ninguna circunstancia, los autores serán responsables por daños directos, indirectos, especiales, incidentales o consecuentes resultantes del uso, mal uso o incapacidad de usar el software, incluso si el autor ha sido avisado de la posibilidad de tal daño. Estas exclusiones y limitaciones pueden no aplicarse en todas las jurisdicciones. Puede tener derechos adicionales y algunas de estas limitaciones pueden no aplicarse a usted. Este software solo está diseñado para uso científico.*
<br/>
<br/>
----------------------------------------------------------
<br/>
<br/>
# ENGLISH (Soon)

## Formal disclaimer
*Ioniq BSO Remote Monitor (“The Software”) is provided as is. Use the software at your own risk. The authors make no warranties as to performance or fitness for a particular purpose, or any other warranties whether expressed or implied. no oral or written communication from or information provided by the authors shall create a warranty. Under no circumstances shall the authors be liable for direct, indirect, special, incidental, or consequential damages resulting from the use, misuse, or inability to use the software, even if the author has been advised of the possibility of such damages. These exclusions and limitations may not apply in all jurisdictions. You may have additional rights and some of these limitations may not apply to you. This software is only intended for scientific usage.*




