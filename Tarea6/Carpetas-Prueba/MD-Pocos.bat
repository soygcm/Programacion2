:: MD-Pocos.bat ==> Genera poquito archivos de prueba para la tarea
@echo off

mkdir pocos
cd    pocos
mkdir DCIM
    mkdir "DCIM\IMS0001"
        touch ".\DCIM\IMS0001\dsc00200.jpg"
        touch ".\DCIM\IMS0001\dsc00201.jpg"
        touch ".\DCIM\IMS0001\dsc00202.jpg"
        touch ".\DCIM\IMS0001\dsc00203.jpg"
        touch ".\DCIM\IMS0001\dsc00204.jpg"
    mkdir "DCIM\IMS0002"
        touch ".\DCIM\IMS0002\dsc00205.jpg"
        touch ".\DCIM\IMS0002\dsc00206.jpg"
        touch ".\DCIM\IMS0002\dsc00207.jpg"

mkdir ALBUM
    mkdir "ALBUM\2015-03 Boda Juana"
        touch ".\ALBUM\2015-03 Boda Juana\dsc00200 Dele Beso.jpg"
        touch ".\ALBUM\2015-03 Boda Juana\dsc00201 El ramo.jpg"
        touch ".\ALBUM\2015-03 Boda Juana\dsc00203 Se cayo la suegra.jpg"
        touch ".\ALBUM\2015-03 Boda Juana\dsc00205 Guaro Guaro.jpg"
    mkdir "ALBUM\2015-06 Paso Canoas"
        touch ".\ALBUM\2015-06 Paso Canoas\dsc00202 Reloj.jpg"
        touch ".\ALBUM\2015-06 Paso Canoas\dsc00204 Copias Rolex.jpg"
        touch ".\ALBUM\2015-06 Paso Canoas\dsc00206 Casio de 2da.jpg"
        touch ".\ALBUM\2015-06 Paso Canoas\dsc00207 Omega.jpg"
    mkdir ""ALBUM\etc"

:_out
cd ..
:: MD-Pocos.bat ==> Fin de archivo
