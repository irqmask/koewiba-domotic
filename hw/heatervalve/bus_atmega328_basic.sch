EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title "motor switch 3pole (bus_atmega328_basic)"
Date "2016-09-06"
Rev "1"
Comp "KoeWiBa"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ics_peripheral:25LCXXX U1
U 1 1 559D5893
P 5200 6850
F 0 "U1" H 5200 7000 40  0000 C CNN
F 1 "25LC256" H 5200 6700 40  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 5200 6850 40  0001 C CNN
F 3 "SPI EEPROM" H 5200 6850 40  0001 C CNN
	1    5200 6850
	1    0    0    -1  
$EndComp
$Comp
L ics_misc:MAX487 U4
U 1 1 559D596F
P 7600 2850
F 0 "U4" H 7350 3250 50  0000 C CNN
F 1 "MAX487ECSA" H 7850 2450 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 7450 2450 25  0001 C CNN
F 3 "" H 7600 2850 60  0000 C CNN
	1    7600 2850
	1    0    0    -1  
$EndComp
$Comp
L standard:C C8
U 1 1 559D5E5A
P 7800 2200
F 0 "C8" H 7850 2250 50  0000 L CNN
F 1 "100nF" H 7800 2075 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 7800 2200 60  0001 C CNN
F 3 "" H 7800 2200 60  0000 C CNN
	1    7800 2200
	-1   0    0    1   
$EndComp
$Comp
L standard:C C1
U 1 1 559D623B
P 4950 1400
F 0 "C1" H 5000 1450 50  0000 L CNN
F 1 "100nF" H 4950 1250 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 4950 1400 60  0001 C CNN
F 3 "" H 4950 1400 60  0000 C CNN
	1    4950 1400
	0    1    1    0   
$EndComp
$Comp
L standard:C C5
U 1 1 559D6400
P 6450 4650
F 0 "C5" H 6500 4700 50  0000 L CNN
F 1 "18pF" H 6550 4500 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6450 4650 60  0001 C CNN
F 3 "" H 6450 4650 60  0000 C CNN
	1    6450 4650
	1    0    0    -1  
$EndComp
$Comp
L standard:C C4
U 1 1 559D67BD
P 6450 4300
F 0 "C4" H 6500 4350 50  0000 L CNN
F 1 "18pF" H 6550 4150 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6450 4300 60  0001 C CNN
F 3 "" H 6450 4300 60  0000 C CNN
	1    6450 4300
	1    0    0    -1  
$EndComp
$Comp
L standard:C C2
U 1 1 559D6AC5
P 5200 6350
F 0 "C2" H 5250 6400 50  0000 L CNN
F 1 "100nF" H 5200 6225 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5200 6350 60  0001 C CNN
F 3 "" H 5200 6350 60  0000 C CNN
	1    5200 6350
	0    1    1    0   
$EndComp
$Comp
L connectors:CONN-ISP-6 J3
U 1 1 559D6B4E
P 7600 5600
F 0 "J3" H 7450 5850 50  0000 C CNN
F 1 "CONN-ISP-6" H 7450 5375 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x2" H 7450 5350 25  0001 L CNN
F 3 "" H 7600 5600 60  0000 C CNN
	1    7600 5600
	1    0    0    -1  
$EndComp
$Comp
L standard:CRYSTAL U3
U 1 1 559D6E2A
P 6300 4300
F 0 "U3" H 6475 4400 50  0000 C CNN
F 1 "7,3728MHz" H 6500 4150 50  0000 C CNN
F 2 "Crystal:CRY_HC49_SMD" H 6300 4300 60  0001 C CNN
F 3 "" H 6300 4300 60  0000 C CNN
	1    6300 4300
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR018
U 1 1 559D7329
P 5350 4950
F 0 "#PWR018" H 5350 4750 50  0001 C CNN
F 1 "GND" H 5350 4840 50  0001 C CNN
F 2 "" H 5350 4950 60  0000 C CNN
F 3 "" H 5350 4950 60  0000 C CNN
	1    5350 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 559D76CD
P 4550 7100
F 0 "#PWR019" H 4550 6900 50  0001 C CNN
F 1 "GND" H 4550 6990 50  0001 C CNN
F 2 "" H 4550 7100 60  0000 C CNN
F 3 "" H 4550 7100 60  0000 C CNN
	1    4550 7100
	1    0    0    -1  
$EndComp
$Comp
L standard:R R11
U 1 1 559D7BAA
P 4900 5700
F 0 "R11" H 4100 5700 50  0000 C CNN
F 1 "100" H 4900 5700 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 5700 60  0001 C CNN
F 3 "" H 4900 5700 60  0000 C CNN
	1    4900 5700
	1    0    0    -1  
$EndComp
$Comp
L standard:R R10
U 1 1 559D7C90
P 4900 5600
F 0 "R10" H 4100 5600 50  0000 C CNN
F 1 "100" H 4900 5600 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 5600 60  0001 C CNN
F 3 "" H 4900 5600 60  0000 C CNN
	1    4900 5600
	1    0    0    -1  
$EndComp
$Comp
L standard:R R9
U 1 1 559D7D33
P 4900 5500
F 0 "R9" H 4100 5500 50  0000 C CNN
F 1 "100" H 4900 5500 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 5500 60  0001 C CNN
F 3 "" H 4900 5500 60  0000 C CNN
	1    4900 5500
	1    0    0    -1  
$EndComp
Entry Wire Line
	4200 2950 4300 2850
Entry Wire Line
	4200 3050 4300 2950
Entry Wire Line
	4200 3150 4300 3050
Entry Wire Line
	4200 3250 4300 3150
Wire Wire Line
	4300 2850 4550 2850
Wire Wire Line
	4300 2950 4550 2950
Wire Wire Line
	4300 3050 4550 3050
Wire Wire Line
	4300 3150 4550 3150
Wire Wire Line
	4650 7000 4550 7000
Wire Wire Line
	4550 7000 4550 7100
Wire Wire Line
	7600 2100 7600 2200
Wire Wire Line
	7700 2200 7600 2200
Connection ~ 7600 2200
$Comp
L power:GND #PWR021
U 1 1 55A00C4E
P 7600 3450
F 0 "#PWR021" H 7600 3250 50  0001 C CNN
F 1 "GND" H 7600 3340 50  0001 C CNN
F 2 "" H 7600 3450 60  0000 C CNN
F 3 "" H 7600 3450 60  0000 C CNN
	1    7600 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3400 7600 3450
Wire Wire Line
	6850 2800 7000 2800
Wire Wire Line
	7000 2800 7000 2900
Wire Wire Line
	7000 2900 7100 2900
Wire Wire Line
	8100 2700 8750 2700
$Comp
L power:GND #PWR022
U 1 1 55A03238
P 4950 1700
F 0 "#PWR022" H 4950 1500 50  0001 C CNN
F 1 "GND" H 4950 1590 50  0001 C CNN
F 2 "" H 4950 1700 60  0000 C CNN
F 3 "" H 4950 1700 60  0000 C CNN
	1    4950 1700
	1    0    0    -1  
$EndComp
Text Label 4300 3150 0    40   ~ 0
PC3
Text Label 4300 3350 0    40   ~ 0
PC5
Text Label 4300 3650 0    40   ~ 0
RXD0
Text Label 4300 3750 0    40   ~ 0
TXD0
Wire Wire Line
	6850 3000 7100 3000
Text Label 6850 3000 0    40   ~ 0
RXD0
Wire Wire Line
	6850 2700 7100 2700
Connection ~ 7000 2800
Text Label 6850 2800 0    40   ~ 0
TXD0EN
Text Label 6850 2700 0    40   ~ 0
TXD0
Entry Wire Line
	6750 2800 6850 2700
Entry Wire Line
	6750 2900 6850 2800
Entry Wire Line
	6750 3100 6850 3000
Wire Wire Line
	8350 2900 9350 2900
Text Label 4300 3850 0    40   ~ 0
TXD0EN
Text Label 4300 4150 0    40   ~ 0
PD5
Wire Wire Line
	4300 3250 4550 3250
Wire Wire Line
	4300 3350 4550 3350
Entry Wire Line
	4200 3350 4300 3250
Entry Wire Line
	4200 3450 4300 3350
Wire Wire Line
	4300 4150 4550 4150
Text Label 6350 3350 0    40   ~ 0
/CSEEP
Entry Wire Line
	4200 5800 4300 5700
Entry Wire Line
	4200 5700 4300 5600
Entry Wire Line
	4200 5600 4300 5500
Entry Wire Line
	5300 5500 5400 5600
Entry Wire Line
	4200 4250 4300 4150
Entry Wire Line
	4200 4350 4300 4250
Wire Wire Line
	4300 4250 4550 4250
Text Label 6350 3450 0    40   ~ 0
PB3_MOSI
Text Label 6350 3650 0    40   ~ 0
PB4_MISO
Text Label 6350 3750 0    40   ~ 0
PB5_SCK
Text Label 4300 5500 0    40   ~ 0
PB4_MISO
Text Label 4300 5600 0    40   ~ 0
PB3_MOSI
Text Label 4300 5700 0    40   ~ 0
PB5_SCK
Text Label 5150 5500 0    40   ~ 0
MISO
Text Label 5150 5600 0    40   ~ 0
MOSI
Text Label 5150 5700 0    40   ~ 0
SCK
Entry Wire Line
	5300 5600 5400 5700
Entry Wire Line
	5300 5700 5400 5800
Wire Wire Line
	5300 5700 5100 5700
Wire Wire Line
	5100 5600 5300 5600
Wire Wire Line
	5300 5500 5100 5500
Wire Wire Line
	4300 5500 4700 5500
Wire Wire Line
	4300 5600 4700 5600
Wire Wire Line
	4300 5700 4700 5700
Text Label 4300 6700 0    40   ~ 0
/CSEEP
Text Label 4300 6800 0    40   ~ 0
MISO
Text Label 5800 7000 0    40   ~ 0
MOSI
Text Label 5800 6900 0    40   ~ 0
SCK
$Comp
L standard:R R8
U 1 1 55A05C69
P 4550 6400
F 0 "R8" H 4550 6300 50  0000 C CNN
F 1 "10k" H 4550 6400 50  0000 C CNN
F 2 "Resistor:R_0805" H 4550 6400 60  0001 C CNN
F 3 "" H 4550 6400 60  0000 C CNN
	1    4550 6400
	0    1    1    0   
$EndComp
$Comp
L standard:R R12
U 1 1 55A05DA4
P 5900 6400
F 0 "R12" H 5900 6300 50  0000 C CNN
F 1 "10k" H 5900 6400 50  0000 C CNN
F 2 "Resistor:R_0805" H 5900 6400 60  0001 C CNN
F 3 "" H 5900 6400 60  0000 C CNN
	1    5900 6400
	0    1    1    0   
$EndComp
Entry Wire Line
	5950 6900 6050 7000
Entry Wire Line
	5950 7000 6050 7100
Entry Wire Line
	4200 6800 4300 6700
Entry Wire Line
	4200 6900 4300 6800
Wire Wire Line
	4300 6700 4650 6700
Wire Wire Line
	4650 6800 4300 6800
Wire Wire Line
	4650 6900 4550 6900
Wire Wire Line
	4550 6900 4550 6600
Wire Wire Line
	5900 6600 5900 6800
Wire Wire Line
	5900 6800 5750 6800
Wire Wire Line
	5900 6150 5900 6200
Wire Wire Line
	4550 6150 5200 6150
Wire Wire Line
	4550 6150 4550 6200
Wire Wire Line
	5750 6700 5750 6150
Connection ~ 5750 6150
Wire Wire Line
	5950 6900 5750 6900
Wire Wire Line
	5750 7000 5950 7000
$Comp
L power:GND #PWR023
U 1 1 55A076FA
P 5200 6500
F 0 "#PWR023" H 5200 6300 50  0001 C CNN
F 1 "GND" H 5200 6390 50  0001 C CNN
F 2 "" H 5200 6500 60  0000 C CNN
F 3 "" H 5200 6500 60  0000 C CNN
	1    5200 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 6050 5200 6150
Connection ~ 5200 6150
Wire Wire Line
	5200 6450 5200 6500
Wire Bus Line
	6050 7250 4200 7250
Entry Wire Line
	6750 5800 6850 5700
Entry Wire Line
	6750 5700 6850 5600
Entry Wire Line
	6750 5600 6850 5500
Text Label 6850 5500 0    40   ~ 0
PB4_MISO
$Comp
L power:GND #PWR024
U 1 1 55A08693
P 8000 5800
F 0 "#PWR024" H 8000 5600 50  0001 C CNN
F 1 "GND" H 8000 5690 50  0001 C CNN
F 2 "" H 8000 5800 60  0000 C CNN
F 3 "" H 8000 5800 60  0000 C CNN
	1    8000 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 5500 8000 5500
Wire Wire Line
	8000 5500 8000 5400
Wire Wire Line
	7900 5700 8000 5700
Wire Wire Line
	8000 5700 8000 5800
Entry Wire Line
	6750 5300 6850 5200
Wire Wire Line
	6850 5200 8200 5200
Wire Wire Line
	8200 5200 8200 5600
Wire Wire Line
	8200 5600 7900 5600
Wire Wire Line
	7300 5500 6850 5500
Wire Wire Line
	6850 5600 7300 5600
Wire Wire Line
	7300 5700 6850 5700
$Comp
L power:GND #PWR025
U 1 1 55A0C631
P 8100 2300
F 0 "#PWR025" H 8100 2100 50  0001 C CNN
F 1 "GND" H 8100 2190 50  0001 C CNN
F 2 "" H 8100 2300 60  0000 C CNN
F 3 "" H 8100 2300 60  0000 C CNN
	1    8100 2300
	1    0    0    -1  
$EndComp
$Comp
L standard:C C6
U 1 1 55A0D462
P 6550 2000
F 0 "C6" H 6600 2050 50  0000 L CNN
F 1 "4,7nF" H 6650 2150 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6550 2000 60  0001 C CNN
F 3 "" H 6550 2000 60  0000 C CNN
	1    6550 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	6550 1700 6550 1800
$Comp
L standard:R R13
U 1 1 55A0DE32
P 6550 1500
F 0 "R13" H 6550 1400 50  0000 C CNN
F 1 "10k" H 6550 1500 50  0000 C CNN
F 2 "Resistor:R_0805" H 6550 1500 60  0001 C CNN
F 3 "" H 6550 1500 60  0000 C CNN
	1    6550 1500
	0    1    1    0   
$EndComp
Connection ~ 6550 1800
$Comp
L power:GND #PWR026
U 1 1 55A0E9DD
P 6550 2300
F 0 "#PWR026" H 6550 2100 50  0001 C CNN
F 1 "GND" H 6550 2190 50  0001 C CNN
F 2 "" H 6550 2300 60  0000 C CNN
F 3 "" H 6550 2300 60  0000 C CNN
	1    6550 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 1200 6550 1300
$Comp
L power:+24V #PWR027
U 1 1 55A0FF09
P 8100 1100
F 0 "#PWR027" H 8100 1300 50  0001 C CNN
F 1 "+24V" H 8100 1250 50  0000 C CNN
F 2 "" H 8100 1100 60  0000 C CNN
F 3 "" H 8100 1100 60  0000 C CNN
	1    8100 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR028
U 1 1 55A10203
P 7600 1700
F 0 "#PWR028" H 7600 1500 50  0001 C CNN
F 1 "GND" H 7600 1590 50  0001 C CNN
F 2 "" H 7600 1700 60  0000 C CNN
F 3 "" H 7600 1700 60  0000 C CNN
	1    7600 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 55A10383
P 7000 1700
F 0 "#PWR029" H 7000 1500 50  0001 C CNN
F 1 "GND" H 7000 1590 50  0001 C CNN
F 2 "" H 7000 1700 60  0000 C CNN
F 3 "" H 7000 1700 60  0000 C CNN
	1    7000 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 55A105FF
P 9050 4200
F 0 "#PWR030" H 9050 4000 50  0001 C CNN
F 1 "GND" H 9050 4090 50  0001 C CNN
F 2 "" H 9050 4200 60  0000 C CNN
F 3 "" H 9050 4200 60  0000 C CNN
	1    9050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 1100 8100 1200
Wire Wire Line
	8100 1200 8000 1200
Entry Wire Line
	1500 3200 1600 3100
Entry Wire Line
	2775 2700 2875 2800
Entry Wire Line
	2775 2800 2875 2900
Entry Wire Line
	2775 3000 2875 3100
Entry Wire Line
	2775 3100 2875 3200
Entry Wire Line
	2775 2900 2875 3000
Entry Wire Line
	6650 2850 6750 2950
Entry Wire Line
	6650 2950 6750 3050
Entry Wire Line
	6650 3150 6750 3250
Entry Wire Line
	6650 3250 6750 3350
Entry Wire Line
	6650 3350 6750 3450
Entry Wire Line
	6650 3450 6750 3550
Entry Wire Line
	6650 3650 6750 3750
Entry Wire Line
	6650 3750 6750 3850
Entry Wire Line
	6650 4150 6750 4250
Wire Wire Line
	6150 2850 6650 2850
Wire Wire Line
	6150 2950 6650 2950
Wire Wire Line
	6150 3150 6650 3150
Wire Wire Line
	6150 3250 6650 3250
Wire Wire Line
	6150 3350 6650 3350
Wire Wire Line
	6150 3450 6650 3450
Wire Wire Line
	6150 3750 6650 3750
Wire Wire Line
	6150 4150 6650 4150
Wire Wire Line
	4300 3750 4550 3750
Wire Wire Line
	4300 3850 4550 3850
Entry Wire Line
	4200 3850 4300 3750
Entry Wire Line
	4200 3950 4300 3850
Text Label 4300 4250 0    40   ~ 0
PD6
Text Label 4300 3250 0    40   ~ 0
PC4
Text Label 6250 1800 0    40   ~ 0
/RESET
Text Label 6850 5700 0    40   ~ 0
/RESET
Text Label 6850 5600 0    40   ~ 0
PB5_SCK
Text Label 7900 5600 0    40   ~ 0
PB3_MOSI
$Comp
L logo:OPEN_HARDWARE_1 LOGO2
U 1 1 55B4D774
P 1300 7350
F 0 "LOGO2" H 1300 7625 60  0001 C CNN
F 1 "Open Hardware" H 1300 7125 60  0000 C CNN
F 2 "Symbol:Symbol_OSHW-Logo_SilkScreen" H 1300 7350 60  0001 C CNN
F 3 "" H 1300 7350 60  0000 C CNN
	1    1300 7350
	1    0    0    -1  
$EndComp
Entry Wire Line
	6650 2750 6750 2850
Text Label 6350 2750 0    40   ~ 0
AREF
Text Label 6350 2850 0    40   ~ 0
ADC6
Text Label 6350 2950 0    40   ~ 0
ADC7
Entry Wire Line
	4200 3550 4300 3450
Entry Wire Line
	4200 3750 4300 3650
Wire Wire Line
	4300 3450 4550 3450
Text Label 6350 4150 0    40   ~ 0
PD7
Text Label 4300 3950 0    40   ~ 0
PD3
Text Label 4300 4050 0    40   ~ 0
PD4
Entry Wire Line
	4200 4050 4300 3950
Entry Wire Line
	4200 4150 4300 4050
Wire Wire Line
	4300 3950 4550 3950
Wire Wire Line
	4300 4050 4550 4050
Entry Wire Line
	2775 3900 2875 4000
Entry Wire Line
	2775 4000 2875 4100
Entry Wire Line
	2775 4100 2875 4200
Entry Wire Line
	2775 4200 2875 4300
Entry Wire Line
	2775 4400 2875 4500
Entry Wire Line
	2775 4500 2875 4600
Entry Wire Line
	2775 4300 2875 4400
$Comp
L power:GND #PWR031
U 1 1 55D48C16
P 1800 3700
F 0 "#PWR031" H 1800 3500 50  0001 C CNN
F 1 "GND" H 1800 3590 50  0001 C CNN
F 2 "" H 1800 3700 60  0000 C CNN
F 3 "" H 1800 3700 60  0000 C CNN
	1    1800 3700
	0    1    1    0   
$EndComp
Text Label 2575 2700 0    40   ~ 0
AVCC
Text Label 1700 2800 0    40   ~ 0
ADC6
Text Label 2575 2800 0    40   ~ 0
ADC7
Wire Wire Line
	2475 3100 2775 3100
Wire Wire Line
	2475 3900 2775 3900
Wire Wire Line
	2775 4000 2475 4000
Wire Wire Line
	2475 4100 2775 4100
Wire Wire Line
	2775 4200 2475 4200
Wire Wire Line
	2475 4300 2775 4300
Wire Wire Line
	2775 4400 2475 4400
Wire Wire Line
	2475 4500 2775 4500
Text Label 6350 3150 0    40   ~ 0
PB0
Text Label 6350 3250 0    40   ~ 0
PB1
Text Label 4300 2850 0    40   ~ 0
PC0
Text Label 4300 2950 0    40   ~ 0
PC1
Text Label 4300 3050 0    40   ~ 0
PC2
Text Label 4300 3450 0    40   ~ 0
/RESET
Text Label 1700 2900 0    40   ~ 0
PB0
Text Label 2575 2900 0    40   ~ 0
PB1
Text Label 1700 3000 0    40   ~ 0
/CSEEP
Text Label 2575 3000 0    40   ~ 0
MOSI
Text Label 1700 3100 0    40   ~ 0
MISO
Text Label 2575 3100 0    40   ~ 0
SCK
Text Label 2575 3900 0    40   ~ 0
PC0
Text Label 1700 3900 0    40   ~ 0
PC1
Text Label 2575 4000 0    40   ~ 0
PC2
Text Label 1700 4000 0    40   ~ 0
PC3
Text Label 2575 4100 0    40   ~ 0
PC4
Text Label 1700 4100 0    40   ~ 0
PC5
Text Label 2575 4200 0    40   ~ 0
/RESET
Text Label 2575 4300 0    40   ~ 0
PD3
Text Label 1700 4300 0    40   ~ 0
PD4
Text Label 2575 4400 0    40   ~ 0
PD5
Text Label 1700 4400 0    40   ~ 0
PD6
Text Label 2575 4500 0    40   ~ 0
PD7
$Comp
L ics_controller_atmel:ATMEGA88-TQFP32 U2
U 1 1 55D48DEF
P 5350 3550
F 0 "U2" H 5350 3550 50  0000 C CNN
F 1 "ATMEGA328-TQFP32" H 5350 2750 50  0000 C CNN
F 2 "Housings_QFP:TQFP-32_7x7mm_Pitch0.8mm" H 5350 3550 25  0001 C CNN
F 3 "DOCUMENTATION" H 5350 3550 25  0001 C CNN
	1    5350 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3650 6650 3650
Wire Wire Line
	5250 4750 5250 4850
Wire Wire Line
	5250 4850 5350 4850
Wire Wire Line
	5450 4850 5450 4750
Wire Wire Line
	5350 4750 5350 4850
Connection ~ 5350 4850
Wire Wire Line
	6150 3950 6200 3950
Wire Wire Line
	6200 3950 6200 4650
Wire Wire Line
	6250 4300 6250 3850
Wire Wire Line
	6250 3850 6150 3850
Wire Wire Line
	6550 4300 6650 4300
Wire Wire Line
	6650 4300 6650 4650
Connection ~ 5450 4850
Wire Wire Line
	6550 4650 6650 4650
Connection ~ 6650 4650
Connection ~ 5250 1200
Connection ~ 5350 1200
Wire Wire Line
	6150 2750 6650 2750
Wire Wire Line
	4550 3650 4300 3650
Text Label 5450 2050 1    40   ~ 0
AVCC
NoConn ~ 9050 3400
Wire Wire Line
	8800 3700 8750 3700
Wire Wire Line
	8750 3700 8750 2700
Wire Wire Line
	9300 3700 9350 3700
Wire Wire Line
	9350 3700 9350 2900
Wire Wire Line
	8350 2900 8350 3000
Entry Wire Line
	1500 2800 1600 2700
Text Label 1700 2700 0    40   ~ 0
AREF
Entry Wire Line
	1500 2900 1600 2800
Entry Wire Line
	1500 3000 1600 2900
Entry Wire Line
	1500 3100 1600 3000
Wire Wire Line
	1950 2700 1600 2700
Entry Wire Line
	1500 4000 1600 3900
Entry Wire Line
	1500 4100 1600 4000
Entry Wire Line
	1500 4200 1600 4100
Entry Wire Line
	1500 4400 1600 4300
Entry Wire Line
	1500 4500 1600 4400
Wire Wire Line
	1600 3900 1950 3900
Wire Wire Line
	1800 3700 1950 3700
Wire Wire Line
	1950 3800 1800 3800
Wire Wire Line
	1950 4000 1600 4000
Wire Wire Line
	1600 4100 1950 4100
Wire Wire Line
	1600 4300 1950 4300
Wire Wire Line
	1950 4400 1600 4400
Text Notes 4750 7200 0    40   ~ 0
32kByte EEProm for Firmware
Text Notes 8850 4500 0    40   ~ 0
Protection varistor and\noptional RS485 termination resistor\nare plugged into the WAGO clamps.
Wire Wire Line
	1600 2800 1950 2800
Wire Wire Line
	2475 2700 2775 2700
Wire Wire Line
	2475 2800 2775 2800
Wire Wire Line
	2775 2900 2475 2900
Wire Wire Line
	2475 3000 2775 3000
Wire Wire Line
	1950 3100 1600 3100
Wire Wire Line
	1600 3000 1950 3000
Wire Wire Line
	1950 2900 1600 2900
Wire Bus Line
	1500 5800 4200 5800
$Comp
L diodes:SEMTECH_SR05 D10
U 1 1 55EC2FF2
P 9050 3700
F 0 "D10" H 9125 3450 50  0000 C CNN
F 1 "SEMTECH SR05" H 9050 4050 50  0000 C CNN
F 2 "Diode_SMD:D_SOT143" H 9050 3700 60  0001 C CNN
F 3 "" H 9050 3700 60  0000 C CNN
	1    9050 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3000 8100 3000
$Comp
L power:VCC #PWR032
U 1 1 5605057B
P 7000 1100
F 0 "#PWR032" H 7000 1275 50  0001 C CNN
F 1 "VCC" H 7000 1250 50  0000 C CNN
F 2 "" H 7000 1100 60  0000 C CNN
F 3 "" H 7000 1100 60  0000 C CNN
	1    7000 1100
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR033
U 1 1 560505CB
P 5350 1050
F 0 "#PWR033" H 5350 1225 50  0001 C CNN
F 1 "VCC" H 5350 1150 50  0000 C CNN
F 2 "" H 5350 1050 60  0000 C CNN
F 3 "" H 5350 1050 60  0000 C CNN
	1    5350 1050
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR034
U 1 1 5605061B
P 7600 2100
F 0 "#PWR034" H 7600 2275 50  0001 C CNN
F 1 "VCC" H 7600 2250 50  0000 C CNN
F 2 "" H 7600 2100 60  0000 C CNN
F 3 "" H 7600 2100 60  0000 C CNN
	1    7600 2100
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR035
U 1 1 560506A7
P 5200 6050
F 0 "#PWR035" H 5200 6225 50  0001 C CNN
F 1 "VCC" H 5200 6150 50  0000 C CNN
F 2 "" H 5200 6050 60  0000 C CNN
F 3 "" H 5200 6050 60  0000 C CNN
	1    5200 6050
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR036
U 1 1 56050805
P 1800 3800
F 0 "#PWR036" H 1800 3975 50  0001 C CNN
F 1 "VCC" V 1800 4000 50  0000 C CNN
F 2 "" H 1800 3800 60  0000 C CNN
F 3 "" H 1800 3800 60  0000 C CNN
	1    1800 3800
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR037
U 1 1 5605145B
P 8000 5400
F 0 "#PWR037" H 8000 5575 50  0001 C CNN
F 1 "VCC" H 8000 5500 50  0000 C CNN
F 2 "" H 8000 5400 60  0000 C CNN
F 3 "" H 8000 5400 60  0000 C CNN
	1    8000 5400
	1    0    0    -1  
$EndComp
Text Label 10000 2700 0    40   ~ 0
A
Text Label 10000 2900 0    40   ~ 0
B
$Comp
L standard:FUSE F1
U 1 1 56648405
P 8750 1200
AR Path="/56648405" Ref="F1"  Part="1" 
AR Path="/578A251D/56648405" Ref="F1"  Part="1" 
AR Path="/61E7F4A8/56648405" Ref="F1"  Part="1" 
F 0 "F1" H 8750 1300 50  0000 C CNN
F 1 "FUSE" H 8750 1100 50  0000 C CNN
F 2 "Resistor:R_1812" H 8750 1200 60  0001 C CNN
F 3 "" H 8750 1200 60  0000 C CNN
	1    8750 1200
	1    0    0    -1  
$EndComp
Text HLabel 1950 2700 2    40   Input ~ 0
AREF
Text HLabel 2475 2700 0    40   Output ~ 0
AVCC
Text HLabel 2475 2575 0    40   Output ~ 0
+24V
Text HLabel 1950 3900 2    40   BiDi ~ 0
PC1
Text HLabel 1950 4000 2    40   BiDi ~ 0
PC3
Text HLabel 2475 4000 0    40   BiDi ~ 0
PC2
Text HLabel 2475 3900 0    40   BiDi ~ 0
PC0
Text HLabel 1950 3700 2    40   Input ~ 0
GND
Text HLabel 1950 3800 2    40   Output ~ 0
VCC
Text HLabel 1950 4100 2    40   BiDi ~ 0
PC5
Text HLabel 2475 4100 0    40   BiDi ~ 0
PC4
Text HLabel 2475 4200 0    40   Input ~ 0
/RESET
Text HLabel 1950 4300 2    40   BiDi ~ 0
PD4
Text HLabel 2475 4300 0    40   BiDi ~ 0
PD3
Text HLabel 2475 4400 0    40   BiDi ~ 0
PD5
Text HLabel 2475 4500 0    40   BiDi ~ 0
PD7
Text HLabel 1950 4400 2    40   BiDi ~ 0
PD6
Text HLabel 1950 2800 2    40   Input ~ 0
ADC6
Text HLabel 2475 2800 0    40   Input ~ 0
ADC7
Text HLabel 1950 2900 2    40   BiDi ~ 0
PB0
Text HLabel 2475 2900 0    40   BiDi ~ 0
PB1
Text HLabel 1950 3000 2    40   Output ~ 0
/CSEEP
Text HLabel 2475 3000 0    40   Output ~ 0
MOSI
Text HLabel 1950 3100 2    40   Input ~ 0
MISO
Text HLabel 2475 3100 0    40   Output ~ 0
SCK
$Comp
L power:+24V #PWR038
U 1 1 578A4CD7
P 2775 2575
F 0 "#PWR038" H 2775 2775 50  0001 C CNN
F 1 "+24V" H 2775 2685 50  0000 C CNN
F 2 "" H 2775 2575 60  0000 C CNN
F 3 "" H 2775 2575 60  0000 C CNN
	1    2775 2575
	0    1    1    0   
$EndComp
Wire Wire Line
	2775 2575 2475 2575
$Comp
L standard:C C7
U 1 1 578E4351
P 7000 1400
F 0 "C7" H 7050 1450 50  0000 L CNN
F 1 "100µF / 6V" H 7000 1275 50  0000 C CNN
F 2 "Capacitors_SMD:C_1206" H 7000 1400 60  0001 C CNN
F 3 "" H 7000 1400 60  0000 C CNN
	1    7000 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	6550 1200 5450 1200
Wire Wire Line
	6550 1800 6250 1800
Connection ~ 5450 1200
Wire Wire Line
	7600 2200 7600 2300
Wire Wire Line
	7000 2800 7100 2800
Wire Bus Line
	4200 5800 5400 5800
Wire Wire Line
	5750 6150 5900 6150
Wire Wire Line
	5200 6150 5750 6150
Wire Wire Line
	5200 6150 5200 6250
Wire Wire Line
	6550 1800 6550 1900
Wire Wire Line
	5350 4850 5450 4850
Wire Wire Line
	5350 4850 5350 4950
Wire Wire Line
	5450 4850 6650 4850
Wire Wire Line
	6650 4650 6650 4850
Wire Wire Line
	5250 1200 4950 1200
Wire Wire Line
	5350 1200 5250 1200
Wire Wire Line
	5450 1200 5350 1200
Wire Wire Line
	6200 4650 6350 4650
Wire Wire Line
	6250 4300 6350 4300
Wire Wire Line
	7000 1500 7000 1700
$Comp
L ics_regulator:TRS1E-2450 U5
U 1 1 62139A20
P 7600 1250
F 0 "U5" H 7600 1650 50  0000 C CNN
F 1 "TRS1E-2450" H 7600 1550 50  0000 C CNN
F 2 "DCDC_Converter:TRS1E_SIL3" H 7600 1250 60  0001 C CNN
F 3 "regulator\\TRS1E.pdf" H 7600 1481 60  0001 C CNN
	1    7600 1250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7600 1500 7600 1700
Connection ~ 8100 1200
Wire Wire Line
	8750 2700 10200 2700
Connection ~ 8750 2700
Wire Wire Line
	9350 2900 10200 2900
Connection ~ 9350 2900
Text Label 10000 1200 0    40   ~ 0
24V_IN
Wire Wire Line
	9050 4000 9050 4200
Wire Wire Line
	7000 1100 7000 1200
Wire Wire Line
	8100 2300 8100 2200
Wire Wire Line
	8100 2200 7900 2200
Wire Wire Line
	7200 1200 7000 1200
Connection ~ 7000 1200
Wire Wire Line
	7000 1200 7000 1300
Wire Wire Line
	8950 1200 10200 1200
Wire Wire Line
	8100 1200 8550 1200
Wire Wire Line
	4950 1200 4950 1300
Wire Wire Line
	4950 1500 4950 1700
Wire Wire Line
	6550 2300 6550 2100
Wire Wire Line
	5450 1200 5450 2350
Wire Wire Line
	5350 1200 5350 2350
Wire Wire Line
	5250 1200 5250 2350
Wire Wire Line
	5350 1050 5350 1200
Wire Bus Line
	6050 7000 6050 7250
Wire Bus Line
	5400 5600 5400 5800
Wire Bus Line
	4200 5800 4200 7250
Wire Bus Line
	1500 2800 1500 5800
Wire Bus Line
	2875 2800 2875 5800
Wire Bus Line
	6750 2750 6750 5800
Wire Bus Line
	4200 2950 4200 5800
$EndSCHEMATC
