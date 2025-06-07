# EnchentesZetta – Sistema de Proteção contra Enchentes

As enchentes urbanas são desastres frequentes que afetam milhões de pessoas, causando prejuízos materiais,
perdas de vidas e colapsos na infraestrutura. A ausência de um sistema de alerta rápido e acessível compromete a capacidade de resposta da população.

Este projeto tem como objetivo o monitoramento dos bueiros para evitar o risco de enchentes, utilizando sensores de distância,
LEDs indicativos e um display LCD. Quando identificado risco alto, os dados são salvos na EEPROM com data e hora.

## Funcionalidades

- Monitoramento contínuo com sensor.
- Indicação visual com LEDs:
  - Verde: seguro
  - Amarelo: atenção
  - Vermelho: risco alto
- Ativação de buzzer em situações críticas.
- Exibição de informações em display LCD I2C.
- Registro automático de riscos altos com data e hora na EEPROM.
- Animação de logo personalizada ao iniciar o sistema.

## Componentes Utilizados

- 1x Arduino UNO ou Nano
- 1x Sensor Ultrassônico HC-SR04
- 1x Módulo RTC DS1307
- 1x Display LCD I2C 16x2
- 3x LEDs (verde, amarelo, vermelho)
- 1x Buzzer
- 3x Resistores de 220Ω
- 1x Protoboard
- Jumpers diversos

## Registro de Eventos

Em caso de problemas de obstrução de algum bueiro, o sistema salva automaticamente:

- Dia  
- Mês  
- Ano  
- Hora  
- Minuto  
- Segundo  

A EEPROM reutiliza o espaço automaticamente quando o limite é atingido.

## Critérios de Risco

- Distância > 15 cm → Risco Baixo → LED Verde
- Distância entre 8 cm e 15 cm → Risco Médio → LED Amarelo
- Distância ≤ 7 cm → Risco Alto → LED Vermelho + Buzzer + Registro na EEPROM

## Demonstração

Video: https://youtu.be/G1rUNWlMmro
Wokwi(Simulador): https://wokwi.com/projects/432888510808619009
