# Lycee Des Andaines

L'objectif est de connaître l'environnement extérieur et intérieur d'une ruche.

L'idée est de pouvoir anticiper au possible l'essaimage.

Arduino est choisi afin d'initier les élèves au développement logiciel,
à l'électricité, à l'électronique et à la mesure physique.

## Sketchs/programmes exemples

Nous avons besoin de connaître quelques facteurs environnementaux :

- température
- masse de la ruche
- activité de la ruche

Nous disposons de capteurs de type DS18B20.

La masse est estimée avec un capteur mesurant la pression exercée par la ruche
sur un liquide contenu dans des sachets étanches (type compote).

L'activité globale sera estimée à l'aide de capteurs infra-rouge placés
à l'entrée de la ruche.

## Difficultés (techniques) rencontrées

### Capteur de température

Thermomètre numérique DS18B20, version waterproof,
avec interface 1-Wire (One-wire).

### Compteur d'abeilles

Le compteur nécessite deux capteurs IR par couloir. (QRE1113 FSC SENSOR REFL 5MM PHOTOTRANS THRU)
Nous devons comparer le passage devant un capteur puis l'autre pour déduire s'il
s'agit d'une entrée ou d'une sortie.

La lecture des capteurs est assez rapide, elle effectue de nombreuses lectures
par secondes.

Pour une prote de ruche de près de 30cm, avec un couloir de 1cm de large, il
nous faudrait donc 60 capteurs IR.

Nous décidones de réduire cette porte, toutefois Arduino ne possède toujours
pas assez d'entrée. Nous décidons d'utiliser un/des multiplexeurs du type :
CD74HC4067

### Capteur de pression / mesure de l' évolution de la masse de la ruche

La précision du convertisseur ADC de l'Arduino est insuffisante,
nous décidons d'utiliser un convertisseur Analogique/Numérique ADS1015
avec une précision sur 12 bit à 3300 samples/seconde via I2C.
