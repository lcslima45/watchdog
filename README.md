# Watchdog
## Projeto Final da Disciplina de Microprocessados 2019.2 - UFC
## Final Project of the Microprocessor Course 2019.2 - UFC
### Integrantes/Members:
  * Lucas Lima de Sousa
  * Dhannyell Lukas Belmino Lima
  
> ### O objetivo do projeto consiste em monitorar o uso de recursos de um computador em tempo real e transmitir os dados para o microcontrolador greenpill(STM32F030F4Px) usando o protocolo serial UART. 
> ### Em seguido os dados são exibidos em um display nokia 5110(PCD8455) usando o protocolo SPI. As informações são exibidas tanto em forma de graficos como em texto, tendo tambem um menu com algumas opções disponiveis.

> ### The objective of the project is to monitor the resource usage of a computer in real time and transmit the data to the microcontroller greenpill (STM32F030F4Px) using the serial UART protocol.
> ### The data is then displayed on a nokia 5110 (PCD8455) using the SPI protocol. Since information is defined in either graphical or textual form, see also a menu with some options available.

#### Projeto em funcionamento
#### Project in operation
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/j1izv3b1LiM/0.jpg)](https://www.youtube.com/watch?v=j1izv3b1LiM)

## Materias Usados/Materials Used:
* STM32F030F4 Cortex M0 Microcontroller.
* Display SPI Nokia 5110
* Conversor TTL USB-Serial CH340
* 4 Push Buttons
* ST-LinkV2

## Display Pins
![LCDSchematic](https://lastminuteengineers.com/wp-content/uploads/2018/11/Nokia-5110-PCD8544-LCD-DDRAM-Memory-Map.png)
![LCDPINS](https://lastminuteengineers.com/wp-content/uploads/2018/11/Nokia-5110-LCD-Module-Pinout.png)

## STM32 Pins
![STM32Schematic](https://i.imgur.com/aN8i9SQ.png)
