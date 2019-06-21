# Ioniq-BSO-Remote-Monitor
**Monitor the charge of your Ioniq PHEV remotely from your smartphone**

![Screenshot_IONIQ_BSO](https://user-images.githubusercontent.com/50306926/59940566-5cb7e080-945b-11e9-9088-f2885e057302.png)

# VERSIÓN EN ESPAÑOL
## ¿Para que sirve?
Los vehículos Hyundai Ioniq PHEV o EV vendidos en Europa no incluyen ningún sistema de monitorización de la batería o del proceso de carga, por lo que decidí hacerme un sistema que me permitiera dejar mi Ioniq cargando y que mediante mi smartphone pudiera ver el proceso y me avise al finalizar con el resumen de dicha carga. Además quería que el sistema fuera lo mas sencillo posible, aunque haya que tener conocimientos de programación y algo de electrónica básica. No es equiparable al sistema BlueLink que se suministra en USA o Corea, pero realiza las funciones básicas para las que se ha programado.

## ¿Como funciona?
Mediante un lector OBD2 modificado para funcionar con WiFi como cliente, una placa de la familia ESP8266 se conecta al OBD2 y recoge los datos necesarios que envía a un servidor MQTT. Se ha probado mediante los servidores gratuitos de Adafruit y CloudMQTT, auunque puede utilizarse cualquier otro. Posteriormente desde nuestro smartphone vemos a tiempo real el estado de nuestro vehículo a través de una página web que está copiada en nuestro dispositivo, y que no es necesario instalar nnguna aplicación o contratar servicios externos de web o bases de datos. Además, si el usuario dispone de mínimos conocimientos de web y javascript puede crear la pantalla a su medida y gusto. Anímate!

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
* Cantidad de energía gargada durante el proceso
* Tiempo estimado de carga para completar el 100%
* Informe de carga vía Telegram bot



## ¿Qué necesito?
* Primero de todo paciencia y leer las instrucciones. Si te cansa leer, no sigas.

* 2 placas de la familia ESP8266. Yo utilizo una Lolin NodeMCU V3 para el OBD y una WiFi Kit 8 de [Heltec Automation](https://heltec.org/project/wifi-kit-8/) para la recogida y envío de datos. La WiFi Kit 8 dispone de pantalla 0.91″ OLED y cargador automático de baterías de litio.

* 1 unidad de OBD2 que funcione con el Hyunda Ioniq PHEV y que sea Bluetooth o WiFi. 

* EL software [ESP-LINK](https://github.com/jeelabs/esp-link) de [JeeLabs](https://github.com/jeelabs) que encontrarás en  https://github.com/jeelabs/esp-link

**¿Como sé que el OBD va a funcionar?**  Lo siento, pero todavía no puedo darte un link correcto de compra o de un fabricante, ya que el mio hace muchos años que lo compré y funciona perfectamente, pero tras numerosas pruebas y compras de diferentes OBD's sabemos que los que tienen version 1.5 de firmware disponen de una mayor probabilidad de funcionar, aunque no todos. Suelen funcionar OBD's antiguos, de mas de 3 ó 4 años. Además necesitarás sacar la placa bluetooth o WiFi que lleva para realizar la modificación necesaria, por lo que no podrá ser un OBD monoplaca, o que no permita su manipulación. Si el OBD que tienes dispone de la placa bluetooth o WiFi separable, antes de manipularlo deberás probar con Torque Pro y los ficheros PID de [JejuSoul](https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs) que encontrarás en https://github.com/JejuSoul/OBD-PIDs-for-HKMC-EVs. No te voy a engañar... es una loteria encontrar uno que funcione, pero los productos de China son así. Sobre todo no te fies del exterior con las imagenes que pondré. No es garantia de que funcione.


## Screenshots
**SIN CARGAR**

![Screenshot_IONIQ_BSO_not_charge](https://user-images.githubusercontent.com/50306926/59941540-b91bff80-945d-11e9-98c9-408788477c5f.png)


**DURANTE LA CARGA**

![Screenshot_IONIQ_BSO_charging](https://user-images.githubusercontent.com/50306926/59941541-b91bff80-945d-11e9-9719-e03ffd199031.png)


**EJEMPLO DE PERSONALIZACIÓN**

![Screenshot_IONIQ_BSO_example](https://user-images.githubusercontent.com/50306926/59941543-b91bff80-945d-11e9-8704-061b4d2b4040.png)


**OTRA PERSONALIZACIÓN**

![Screenshot_IONIQ_BSO_example1](https://user-images.githubusercontent.com/50306926/59941542-b91bff80-945d-11e9-92df-750d3fc649ae.png)



## Descarga de responsabilidad
Ioniq BSO Remote Monitor  ("EL SOFTWARE") SE PROPORCIONA COMO ESTÁ. UTILICE EL SOFTWARE BAJO SU PROPIO RIESGO. LOS AUTORES NO GARANTIZAN EL RENDIMIENTO O LA ADECUACIÓN PARA UN PROPÓSITO PARTICULAR, O NINGUNA OTRA GARANTÍA EXPRESA O IMPLÍCITA. NINGUNA COMUNICACIÓN ORAL O ESCRITA DE O AUTORIZACIÓN DE LOS AUTORES CREARÁ UNA GARANTÍA. BAJO NINGUNA CIRCUNSTANCIA, LOS AUTORES SERÁN RESPONSABLES POR DAÑOS DIRECTOS, INDIRECTOS, ESPECIALES, INCIDENTALES O CONSECUENTES RESULTANTES DEL USO, MAL USO O INCAPACIDAD DE USAR EL SOFTWARE, INCLUSO SI EL AUTOR HA SIDO AVISO DE LA POSIBILIDAD DE TAL DAMÁ. ESTAS EXCLUSIONES Y LIMITACIONES PUEDEN NO APLICARSE EN TODAS LAS JURISDICCIONES. PUEDE TENER DERECHOS ADICIONALES Y ALGUNAS DE ESTAS LIMITACIONES PUEDEN NO APLICARSE A USTED. ESTE SOFTWARE SOLO ESTÁ DISEÑADO PARA USO CIENTÍFICO.

# ENGLISH (Soon)
# Formal disclaimer
Ioniq BSO Remote Monitor (“THE SOFTWARE”) IS PROVIDED AS IS. USE THE SOFTWARE AT YOUR OWN RISK. THE AUTHORS MAKE NO WARRANTIES AS TO PERFORMANCE OR FITNESS FOR A PARTICULAR PURPOSE, OR ANY OTHER WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO ORAL OR WRITTEN COMMUNICATION FROM OR INFORMATION PROVIDED BY THE AUTHORS SHALL CREATE A WARRANTY. UNDER NO CIRCUMSTANCES SHALL THE AUTHORS BE LIABLE FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES RESULTING FROM THE USE, MISUSE, OR INABILITY TO USE THE SOFTWARE, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. THESE EXCLUSIONS AND LIMITATIONS MAY NOT APPLY IN ALL JURISDICTIONS. YOU MAY HAVE ADDITIONAL RIGHTS AND SOME OF THESE LIMITATIONS MAY NOT APPLY TO YOU. THIS SOFTWARE IS ONLY INTENDED FOR SCIENTIFIC USAGE.
