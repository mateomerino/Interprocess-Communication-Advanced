Sistemas Operativos II - Laboratorio II IPC
###  Ingeniería en Computación - FCEFyN - UNC - 2023
# Laboratorio 2 - Interprocess Communication Advanced

El presente trabajo se desarrolló en lenguaje C con el propósito de crear una estructura Cliente-Servidor haciendo uso de Sockets. El Servidor atiende a tres tipos de clientes,
cada cual responde a distintas solicitudes.

Los archivos del proyecto están organizados de la siguiente manera:

![image](https://user-images.githubusercontent.com/66422394/236014175-6a8f3b5f-4fea-425c-9d9e-474780cf67b7.png)

Para compilar el proyecto, se debe hacer uso de la herramienta CMake. Los pasos a seguir son:

1) En el directorio raíz del proyecto, crear una carpeta llamada "build"
  
  mkdir build
  
2) Ingresar a esta carpeta

  cd build
  
3) Llamar a CMake para que compile todos los archivos del proyecto

  cmake ..
  
  make
  
Se crearán los archivos ejecutables "server.c", "client_unix.c", "client_inet.c" y "client_inet_6.c"

![image](https://user-images.githubusercontent.com/66422394/236014818-81ea8790-6675-4030-a914-0ee271d06b16.png)

Para la ejecución del programa, se debe ejecutar "server.c" con las direcciones de los tres puertos de los clientes. Por ejemplo:

  ./server.c 1111 2222 3333
  
Esto ejecutará el Servidor que estará escuchando al client_unix por el puerto 1111, al client_inet por el puerto 2222 y al client_inet_6 por el puerto 3333

Luego, repetir el proceso de ejecución para cada cliente utilizando el puerto asignado correspondiente. Por ejemplo, para ejecutar el Cliente Unix:

  ./client_unix 1111
  
Y ya estaremos conectados al Servidor.

A continuación; se detallan los tipos de Clientes del proyecto y como resuelve sus peticiones el Servidor

### Client Unix

  El cliente Unix interactúa con journalctl por consola. Hace peticiones utilizando el comando "journalctl -n <X>" y el Servidor le devuelve la información correspondiente siendo X un número entero.

### Client Inet

  El cliente Inet interactúa con journalctl haciendo uso de los LOGS del sistema, logeando en él. El cliente puede ingresar strings con información 
  para logear en el sistema y el Servidor realiza dicha tarea. Luego de ingresar la cantidad de información a logear por consola, el Servidor finaliza su conexión y le envía un archivo .ZIP con la información que el cliente logeó; detallando la hora y el ID del proceso que logeó la información. 
  
### Client Inet 6
  
  El cliente Inet 6 puede solicitar información del sistema al servidor. Está limitado a dos comandos; "getmemory" y "getload" para los cuales el sevidor
  le devuelve la información de la memoria disponible en el sistema y de la información de carga del sistema, respectivamente.
  
Todos los clientes deben ingresar el comando "end" para finalizar su conexión.
  
Cada cliente puede instanciarse N veces, siendo N el número máximo de archivos posibles abiertos en el Sistema.
