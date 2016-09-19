# Moniteur/enregistreur de l'environement de la ruche

Un Arduino Nano permet de relever la température, l'humidité extérieure,
la pression atmosphérique aux alentours de la ruche.

Deux capteurs de température DS18B20, un fixé au plancher devant
la porte d'entrée, l'autre sous le toit entre la grille et le couvre toit.

En attendant de pouvoir mesurer la hauteur des précipitations,
un capteur de chute de pluie est en test.

La plupart des capteurs sont reliés sur le bus I2C, correspondant
aux pins A4 et A5.

## Baromètre.

BMP 280 relié en I2C

## Température et humidité

Si7021 relié en I2C

## Capteur de chute de pluie

relié en A7


