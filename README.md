# Watchdog
## Projeto Final da Disciplina de Microprocessados 2019.2 - UFC
### Integrantes
  * Lucas Lima de Sousa
  * Dhannyell Lukas Belmino Lima
  
### O objetivo do projeto consiste em monitorar o uso de recursos de um computador em tempo real e transmitir os dados para o microcontrolador greenpill(STM32F030F4Px) usando o protocolo serial UART. 
### Em seguido os dados são exibidos em um display nokia 5110(PCD8455) usando o protocolo SPI. As informações são exibidas tanto em forma de graficos como em texto, tendo tambem um menu com algumas opções disponiveis.

#### projeto em funcionamento
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/j1izv3b1LiM/0.jpg)](https://www.youtube.com/watch?v=j1izv3b1LiM)

## Materias Usados:
* STM32F030F4 Cortex M0 Microcontroller.
* Display SPI Nokia 5110
* Conversor TTL USB-Serial CH340

## Display Pins
![LCDSchematic](https://lastminuteengineers.com/wp-content/uploads/2018/11/Nokia-5110-PCD8544-LCD-DDRAM-Memory-Map.png)
![LCDPINS](https://lastminuteengineers.com/wp-content/uploads/2018/11/Nokia-5110-LCD-Module-Pinout.png)

## STM32 Pins
![STM32Schematic](https://i.imgur.com/aN8i9SQ.png)
