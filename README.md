# Ioniq-BSO-Remote-Monitor
### Monitor the charge of your Ioniq PHEV remotely from your smartphone

![Screenshot_IONIQ_BSO](https://user-images.githubusercontent.com/50306926/59957631-8d6b3a80-949a-11e9-94d0-610644a23630.jpg)

---------------------------------------------------------------

# VERSIÓN EN ESPAÑOL

- [¿Para que sirve?](#para-que-sirve)
- [¿Como funciona?](#como-funciona)
- [Funcionalidades](#funcionalidades)
- [¿Qué necesito?](#qué-necesito)
- [Preguntas previas sobre el proyecto](#preguntas-previas-sobre-el-proyecto)
  - [¿Como sé que el OBD va a funcionar?](#como-sé-que-el-obd-va-a-funcionar)
  - [¿Pero voy a poder usar el OBD en otros programas?](#Pero-voy-a-poder-usar-el-OBD-en-otros-programas)
  - [¿Puedo montar Torque Pro o Hybrid Assistant en mi multimedia?](#Puedo-montar-Torque-Pro-o-Hybrid-Assistant-en-mi-multimedia)
  - [¿Pero porqué no utilizar un OBD WiFi directamente?](#Pero-porqué-no-utilizar-un-OBD-WiFi-directamente)
  - [¿Pero porqué no usar un OBD Bluetooth directamente?](#Pero-porqué-no-usar-un-OBD-Bluetooth-directamente)
  - [¿Porqué Lolin NodeMCU y no otra placa de la familia ESP?](#porqué-Lolin-NodeMCU-y-no-otra-placa-de-la-familia-ESP)
  - [Instalación del software ESP-LINK](#instalación-del-software-esp-link)

- [Modificación del OBD](#modificación-del-OBD)



## ¿Para que sirve?
Los vehículos Hyundai Ioniq PHEV o EV vendidos en Europa no incluyen ningún sistema de monitorización de la batería o del proceso de carga, por lo que decidí hacerme un sistema que me permitiera dejar mi Ioniq cargando y que mediante mi smartphone pudiera ver el proceso y me avise al finalizar con el resumen de dicha carga. Además quería que el sistema fuera lo mas sencillo posible, aunque haya que tener conocimientos de programación y algo de electrónica básica. No es equiparable al sistema BlueLink que se suministra en USA o Corea, pero realiza las funciones básicas para las que se ha programado.

## ¿Como funciona?
Mediante un lector OBD2 modificado para funcionar con WiFi como cliente, una placa de la familia ESP8266 se conecta al OBD2 y recoge los datos necesarios que envía a un servidor MQTT. Se ha probado mediante los servidores gratuitos de Adafruit y CloudMQTT, auunque puede utilizarse cualquier otro. Posteriormente desde nuestro smartphone vemos a tiempo real el estado de nuestro vehículo a través de una página web que está copiada en nuestro dispositivo, y que no es necesario instalar nnguna aplicación o contratar servicios externos de web o bases de datos. Además, si el usuario dispone de mínimos conocimientos de web y javascript puede crear la pantalla a su medida y gusto. Anímate!
![esquema](https://user-images.githubusercontent.com/50306926/59946559-24200300-946b-11e9-8d00-6204caec7187.jpg)

## Funcionalidades
La aplicación permite monitorizar los siguientes datos:
* Carga actual de la batería en % (SOC Display)
* Potencia de carga o descarga de la batería
* Voltaje del batería
* Intensidad de carga o descarga
* Previsión de kilómetros restantes en modi eléctrico
* Temperatura máxima de la batería
* Temperatura mínima de la batería
* Temperatura media de la bateria
* Visualización del funcionamiento del ventilador de las baterías
* Estado de conexión del cargador
* Cantidad de energía cargada durante el proceso
* Tiempo estimado de carga para completar el 100%
* Informe de carga vía Telegram bot
* Cliente a servicios de DDNS



## ¿Qué necesito?
* Primero de todo paciencia y leer las instrucciones. Si te cansa leer, no sigas.

* 2 placas de la familia ESP8266. Yo utilizo una Lolin MCU V3 para el OBD y una WiFi Kit 8 de [Heltec Automation](https://heltec.org/project/wifi-kit-8/) para la recogida y envío de datos. Realmente puedes utilizar las placas que te vengan en gana, ya que el softeare debería funcionar. Para pruebas también he utilizado Wemos D1 mini. La WiFi Kit 8 dispone de pantalla 0.91″ OLED y cargador automático de baterías de litio. Eso permite que cuando el coche está cargando, la placa dispone de batería para varios días. Puedes poner un power bank o cualquier otro sistema que se te ocurra.

* 1 unidad de OBD2 que funcione con el Hyunda Ioniq PHEV y que sea Bluetooth o WiFi. 

* El software [ESP-LINK](https://github.com/jeelabs/esp-link) de [JeeLabs](https://github.com/jeelabs) que encontrarás en  https://github.com/jeelabs/esp-link . Este software permite realizar un gateway entre la UART del OBD y nuestra wifi. Ese software deberás instalarlo en la primera de las placas ESP, en mi caso la Lolin NodeMCU. JeeLabs tiene toda la documentación al respecto. Por favor, leela en detalle para poder instalarla.

* Un router 2, 3 ó 4G en tu vehículo para poder enviar los datos al servidor de MQTT, o tu propio smartphone con la compartición de WiFi activada. Yo dispongo de un router D-Link DWR-730 comprado de segunda mano por poco mas de 20€


## Preguntas previas sobre el proyecto

### ¿Como sé que el OBD va a funcionar?
Lo siento, pero todavía no puedo darte un link correcto de compra o de un fabricante, ya que el mio hace muchos años que lo compré y funciona perfectamente, pero tras numerosas pruebas y compras de diferentes OBD's sabemos que los que tienen version 1.5 de firmware disponen de una mayor probabilidad de funcionar, aunque no todos. Suelen funcionar OBD's antiguos, de mas de 3 ó 4 años. Además necesitarás sacar la placa bluetooth o WiFi que lleva para realizar la modificación necesaria, por lo que no podrá ser un OBD monoplaca, o que no permita su manipulación. Si el OBD que tienes dispone de la placa bluetooth o WiFi separable, antes de manipularlo deberás probar con Torque Pro y los ficheros PID de [JejuSoul](https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs) que encontrarás en https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs. No te voy a engañar... es una loteria encontrar uno que funcione, pero los productos de China son así. Sobre todo no te fies del exterior con las imagenes que pondré. No es garantia de que funcione.

### ¿Pero voy a poder usar el OBD en otros programas?
Si, yo continuo utilizándolo con Torque o Hybrid Assistant desde mi smartphone o, mejor todavía, desde el multimedia de mi Ioniq. Eso si, no puedo utilizar APP's y mi aplicación simultáneamente. Adjunto captura de Torque pro en mi multimedia del Ioniq:

![IMG_20190322_233611_294](https://user-images.githubusercontent.com/50306926/59950477-57b45a80-9476-11e9-8835-0e7f98892dc2.jpg)


### WTF!!! ¿Puedo montar Torque Pro o Hybrid Assitant en mi multimedia?
Si, pero no es aquí donde te explicaré eso. Visita [Ioniq Spain](https://ioniqspain.wordpress.com/2019/02/20/acceso-completo-a-la-capa-android/), el blog de nuestro compañero Smoje79, y podrás ver como hacerlo.



### ¿Pero porqué no utilizar un OBD WiFi directamente? 
Los OBD's WiFi que hay en el mercado funcionan como punto de acceso. La placa encargada de recoger los datos se conectaría sin problemas al OBD, pero no podría salir a Internet para enviar los datos. Por ese motivo modifico el OBD poniendo un ESP8266. Si alguien tiene alguna otra idea, será bienvenida.


### ¿Pero porqué no usar un OBD Bluetooth directamente?
Esa fue mi primera idea, mediante un ESP32 conectar al OBD y de ahí enviar los datos al broker MQTT. Lo siento, no fuí capaz de utilizar el Bluetooth clasico con éxito mediante el IDE de Arduino. Mediante el ESP-IDF de Espressif parece que sería posible, pero mis conocimientos en el ESP32 no son suficientes por el momento.


### ¿Porqué Lolin NodeMCU y no otra placa de la familia ESP?
Muy simple, es la que tenía a mano. Una muy buena opción es Wemos D1 Mini ya que es pequeña, igual de barata y funciona perfectamente. Posiblemente sustituya Lolin por esta placa. Agadeceré que me digáis cual usáis vosotros. 


---------------------------------------------------------------


## Instalación del software ESP-LINK
Después de hacer diferentes pruebas con software propio, la mejor opción es utilizar [ESP-LINK](https://github.com/jeelabs/esp-link). Es un software fluido, facil de gestionar y que nos permite multitud de opciones. Tal como ya he dicho anteriormente, seguid las instrucciones de instalación y comprobad que funciona aunque os adjunto la configuración utilizada por mi compañero Ángel desde el programador de firmware del NodeMCU: 

![firmwareESPLINK](https://user-images.githubusercontent.com/50306926/59951787-40776c00-947a-11e9-9f69-225841a29907.jpg)


Una vez instalado el ESP-LINK, conectaros a la Wifi que la placa levantará, y que veréis que está sin seguridad. Deberéis cambiar la velocidad de conexión de la UART desde la opción de menú *"μC CONSOLE"*, dejándolo a 38400 bauds:

![photo5793915804792762996](https://user-images.githubusercontent.com/50306926/59952544-e2985380-947c-11e9-9bc5-ad3b606f6fc7.jpg)


Os recomiendo desactivar el log desde *"Debug log"* dejándolo en OFF:

![photo5793915804792762994](https://user-images.githubusercontent.com/50306926/59952545-e2985380-947c-11e9-86ba-340452ddb4fa.jpg)

  

Por último podéis poner una IP fija para que la placa encargada de recoger los datos del OBD pueda encontrar al ESP-LINK:

![photo5793915804792762995](https://user-images.githubusercontent.com/50306926/59952543-e2985380-947c-11e9-91af-2c1fb08e84d6.jpg)



## Modificación del OBD
Antes de nada, gracias a Ángel por su idea. El fué el primero en modificar su OBD y en explicame lo que había hecho :-)
En principio las modificaciones son sencillas, pero requieren habilidad en soldauras de pequeño tamaño. Mi OBD originalente era así:

![IMG_20190126_114531](https://user-images.githubusercontent.com/50306926/59947687-33548000-946e-11e9-9c94-68ce4eea7137.jpg)


Se tiene que desoldar la placa Bluetooth para poder conectar los pines del ESP8266. En mi modelo el conexionado es el siguiente, pero dependiendo de como sea tu OBD, deberás investigar que pines utilizar.

![pineout](https://user-images.githubusercontent.com/50306926/59955764-85a49980-948c-11e9-807d-163d0b577c75.jpg)


Si, he probado otras placas mas pequeñas como la Wemos D1 Mini que funciona perfectamente, o la ESP-01 que cabía perfectamente dentro del OBD. Para no sobrecargar el regulador de tensión de 3.3v que lleva el propio OBD, le instalé un regulador que se alimentaba desde los 5 voltios, tal como hago con la Lolin NodeMCU, pero no acabó de convencerme la temperatura del regulador de tensión. En la siguiente imagen veréis como quedaba el montaje:

![ESP01](https://user-images.githubusercontent.com/50306926/59949205-b4ae1180-9472-11e9-8be2-156f9dbae6aa.jpg)



**IMPORTANTE:** Antes de comprobar que funciona revisa el cabelado, especialmente la alimentación de 5 voltios a y GND para evitar cortocircuitos que puedan dañar a los circuitos del OBD, del ESP8266 o del propio coche. Mi recomendación es que hagas la prueba con una fuente de laboratorio y conectes 12 voltios al OBD mediante conectores. Los pines que tienes que conectar son el 16 con 12V+ y el 4 y 5 con el negativo. Si todo funciona correctamente el OBD y el ESP8266 arrancará y podrás acceder a él:

![Pineout_OBD](https://user-images.githubusercontent.com/50306926/59956185-049ad180-948f-11e9-984f-49ede28667f0.jpg)


Si todo es correcto, puedes probar a conectarte al OBD desde un navegador y acceder al menú *"μC CONSOLE"*. Desde ahí teclea el comando *ATZ* y deberás obtener la respuesta de la versión del firmware del OBD, en mi caso *ELM 327 v1.5*. Si es así, todo funciona correctamente y ya está realizado lo mas duro.

![atz](https://user-images.githubusercontent.com/50306926/59956657-cbb02c00-9491-11e9-886a-4882cf2a8678.jpg)




## Screenshots
**SIN CARGAR**

![Screenshot_IONIQ_BSO_not_charge](https://user-images.githubusercontent.com/50306926/59957781-2d759380-949c-11e9-91c6-4f71c3bdcddf.jpg)


**DURANTE LA CARGA**

![Screenshot_IONIQ_BSO_charging](https://user-images.githubusercontent.com/50306926/59957752-c9eb6600-949b-11e9-9f2a-82c575926ec2.jpg)


**EJEMPLO DE PERSONALIZACIÓN**

![Screenshot_IONIQ_BSO_example](https://user-images.githubusercontent.com/50306926/59941543-b91bff80-945d-11e9-8704-061b4d2b4040.png)


**OTRA PERSONALIZACIÓN**

![Screenshot_IONIQ_BSO_example1](https://user-images.githubusercontent.com/50306926/59941542-b91bff80-945d-11e9-92df-750d3fc649ae.png)



## Descarga de responsabilidad
Ioniq BSO Remote Monitor  ("EL SOFTWARE") SE PROPORCIONA COMO ESTÁ. UTILICE EL SOFTWARE BAJO SU PROPIO RIESGO. LOS AUTORES NO GARANTIZAN EL RENDIMIENTO O LA ADECUACIÓN PARA UN PROPÓSITO PARTICULAR, O NINGUNA OTRA GARANTÍA EXPRESA O IMPLÍCITA. NINGUNA COMUNICACIÓN ORAL O ESCRITA DE O AUTORIZACIÓN DE LOS AUTORES CREARÁ UNA GARANTÍA. BAJO NINGUNA CIRCUNSTANCIA, LOS AUTORES SERÁN RESPONSABLES POR DAÑOS DIRECTOS, INDIRECTOS, ESPECIALES, INCIDENTALES O CONSECUENTES RESULTANTES DEL USO, MAL USO O INCAPACIDAD DE USAR EL SOFTWARE, INCLUSO SI EL AUTOR HA SIDO AVISO DE LA POSIBILIDAD DE TAL DAMÁ. ESTAS EXCLUSIONES Y LIMITACIONES PUEDEN NO APLICARSE EN TODAS LAS JURISDICCIONES. PUEDE TENER DERECHOS ADICIONALES Y ALGUNAS DE ESTAS LIMITACIONES PUEDEN NO APLICARSE A USTED. ESTE SOFTWARE SOLO ESTÁ DISEÑADO PARA USO CIENTÍFICO.

# ENGLISH (Soon)
# Formal disclaimer
Ioniq BSO Remote Monitor (“THE SOFTWARE”) IS PROVIDED AS IS. USE THE SOFTWARE AT YOUR OWN RISK. THE AUTHORS MAKE NO WARRANTIES AS TO PERFORMANCE OR FITNESS FOR A PARTICULAR PURPOSE, OR ANY OTHER WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO ORAL OR WRITTEN COMMUNICATION FROM OR INFORMATION PROVIDED BY THE AUTHORS SHALL CREATE A WARRANTY. UNDER NO CIRCUMSTANCES SHALL THE AUTHORS BE LIABLE FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES RESULTING FROM THE USE, MISUSE, OR INABILITY TO USE THE SOFTWARE, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. THESE EXCLUSIONS AND LIMITATIONS MAY NOT APPLY IN ALL JURISDICTIONS. YOU MAY HAVE ADDITIONAL RIGHTS AND SOME OF THESE LIMITATIONS MAY NOT APPLY TO YOU. THIS SOFTWARE IS ONLY INTENDED FOR SCIENTIFIC USAGE.
