EESchema Schematic File Version 2
LIBS:connectors
LIBS:ics_controller_atmel
LIBS:ics_misc
LIBS:ics_peripheral
LIBS:ics_regulator
LIBS:logo
LIBS:power
LIBS:standard
LIBS:diodes
LIBS:bus-module_atmega324-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "bus-module_atmega324"
Date "2015-10-19"
Rev "1"
Comp "KoeWiBa"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATMEGA324-TQFP44 U1
U 1 1 559D5818
P 5350 3650
F 0 "U1" H 5350 3650 50  0000 C CNN
F 1 "ATMEGA324-TQFP44" H 5350 2550 50  0000 C CNN
F 2 "Housings_QFP:TQFP-44_10x10mm_Pitch0.8mm" H 5350 3650 25  0001 C CNN
F 3 "DOCUMENTATION" H 5350 3650 25  0001 C CNN
	1    5350 3650
	1    0    0    -1  
$EndComp
$Comp
L 25LCXXX U3
U 1 1 559D5893
P 5200 6800
F 0 "U3" H 5200 6950 40  0000 C CNN
F 1 "25LCXXX" H 5200 6650 40  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 5200 6800 40  0001 C CNN
F 3 "SPI EEPROM" H 5200 6800 40  0001 C CNN
	1    5200 6800
	1    0    0    -1  
$EndComp
$Comp
L MAX487 U4
U 1 1 559D596F
P 7600 2850
F 0 "U4" H 7350 3250 50  0000 C CNN
F 1 "MAX3485" H 7800 2450 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 7450 2450 25  0001 C CNN
F 3 "" H 7600 2850 60  0000 C CNN
	1    7600 2850
	1    0    0    -1  
$EndComp
$Comp
L MAX487 U5
U 1 1 559D5A22
P 7600 4450
F 0 "U5" H 7350 4850 50  0000 C CNN
F 1 "MAX3485" H 7800 4050 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 7450 4050 25  0001 C CNN
F 3 "" H 7600 4450 60  0000 C CNN
	1    7600 4450
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 J5
U 1 1 559D5A6B
P 9900 2500
F 0 "J5" H 9900 2575 50  0000 C CNN
F 1 "CONN_1" H 9900 2425 25  0000 C CNN
F 2 "Connector_Miscelleaneus:WAGO243" H 9900 2500 60  0001 C CNN
F 3 "" H 9900 2500 60  0000 C CNN
	1    9900 2500
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 J6
U 1 1 559D5ACC
P 9900 2700
F 0 "J6" H 9900 2775 50  0000 C CNN
F 1 "CONN_1" H 9900 2625 25  0000 C CNN
F 2 "Connector_Miscelleaneus:WAGO243" H 9900 2700 60  0001 C CNN
F 3 "" H 9900 2700 60  0000 C CNN
	1    9900 2700
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 J7
U 1 1 559D5B5B
P 9900 2900
F 0 "J7" H 9900 2975 50  0000 C CNN
F 1 "CONN_1" H 9900 2825 25  0000 C CNN
F 2 "Connector_Miscelleaneus:WAGO243" H 9900 2900 60  0001 C CNN
F 3 "" H 9900 2900 60  0000 C CNN
	1    9900 2900
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 J8
U 1 1 559D5B94
P 9900 3100
F 0 "J8" H 9900 3175 50  0000 C CNN
F 1 "CONN_1" H 9900 3025 25  0000 C CNN
F 2 "Connector_Miscelleaneus:WAGO243" H 9900 3100 60  0001 C CNN
F 3 "" H 9900 3100 60  0000 C CNN
	1    9900 3100
	1    0    0    -1  
$EndComp
$Comp
L AM1S-2403 U6
U 1 1 559D5CD1
P 8050 1300
F 0 "U6" H 8050 1300 50  0000 C CNN
F 1 "AM1S-2403" H 8050 1100 50  0000 C CNN
F 2 "DCDC_Converter:AM1S-XXXX_SIL4" H 8050 1500 25  0001 C CNN
F 3 "" H 7300 1300 25  0001 C CNN
	1    8050 1300
	-1   0    0    -1  
$EndComp
$Comp
L CP C9
U 1 1 559D5D78
P 7200 1300
F 0 "C9" H 7200 1175 50  0000 C CNN
F 1 "100µF" H 7200 1425 50  0000 C CNN
F 2 "Capacitors:cp_smd_6.3" H 7200 1300 60  0001 C CNN
F 3 "" H 7200 1300 60  0000 C CNN
F 4 "16V" H 7200 1490 50  0000 C CNN "Voltage"
F 5 "+-10%" H 7200 1555 50  0000 C CNN "Tolrance"
	1    7200 1300
	0    1    1    0   
$EndComp
$Comp
L C C10
U 1 1 559D5E5A
P 7800 2200
F 0 "C10" H 7850 2250 50  0000 L CNN
F 1 "100nF" H 7800 2075 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 7800 2200 60  0001 C CNN
F 3 "" H 7800 2200 60  0000 C CNN
	1    7800 2200
	-1   0    0    1   
$EndComp
$Comp
L C C11
U 1 1 559D5F93
P 7800 3800
F 0 "C11" H 7850 3850 50  0000 L CNN
F 1 "100nF" H 7800 3675 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 7800 3800 60  0001 C CNN
F 3 "" H 7800 3800 60  0000 C CNN
	1    7800 3800
	-1   0    0    1   
$EndComp
$Comp
L C C3
U 1 1 559D60B0
P 4650 1800
F 0 "C3" H 4700 1850 50  0000 L CNN
F 1 "100nF" H 4500 1750 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 4650 1800 60  0001 C CNN
F 3 "" H 4650 1800 60  0000 C CNN
	1    4650 1800
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 559D623B
P 4850 1800
F 0 "C4" H 4900 1850 50  0000 L CNN
F 1 "100nF" H 4700 1750 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 4850 1800 60  0001 C CNN
F 3 "" H 4850 1800 60  0000 C CNN
	1    4850 1800
	0    1    1    0   
$EndComp
$Comp
L C C2
U 1 1 559D6400
P 3900 2650
F 0 "C2" H 3950 2700 50  0000 L CNN
F 1 "18pF" H 3900 2525 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 3900 2650 60  0001 C CNN
F 3 "" H 3900 2650 60  0000 C CNN
	1    3900 2650
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 559D67BD
P 3900 2200
F 0 "C1" H 3950 2250 50  0000 L CNN
F 1 "18pF" H 3900 2075 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 3900 2200 60  0001 C CNN
F 3 "" H 3900 2200 60  0000 C CNN
	1    3900 2200
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 559D6AC5
P 5200 6300
F 0 "C6" H 5250 6350 50  0000 L CNN
F 1 "100nF" H 5200 6175 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 5200 6300 60  0001 C CNN
F 3 "" H 5200 6300 60  0000 C CNN
	1    5200 6300
	0    1    1    0   
$EndComp
$Comp
L CONN-ISP-6 J3
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
L CRYSTAL U2
U 1 1 559D6E2A
P 4250 2250
F 0 "U2" H 4425 2350 50  0000 C CNN
F 1 "7,3728MHz" H 4425 2150 50  0000 C CNN
F 2 "Crystal:HC49-SMD" H 4250 2250 60  0001 C CNN
F 3 "" H 4250 2250 60  0000 C CNN
	1    4250 2250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR01
U 1 1 559D7329
P 5350 5400
F 0 "#PWR01" H 5350 5200 50  0001 C CNN
F 1 "GND" H 5350 5290 50  0001 C CNN
F 2 "" H 5350 5400 60  0000 C CNN
F 3 "" H 5350 5400 60  0000 C CNN
	1    5350 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 559D76CD
P 4550 7050
F 0 "#PWR02" H 4550 6850 50  0001 C CNN
F 1 "GND" H 4550 6940 50  0001 C CNN
F 2 "" H 4550 7050 60  0000 C CNN
F 3 "" H 4550 7050 60  0000 C CNN
	1    4550 7050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 559D775F
P 7600 5050
F 0 "#PWR03" H 7600 4850 50  0001 C CNN
F 1 "GND" H 7600 4940 50  0001 C CNN
F 2 "" H 7600 5050 60  0000 C CNN
F 3 "" H 7600 5050 60  0000 C CNN
	1    7600 5050
	1    0    0    -1  
$EndComp
$Comp
L +24V #PWR04
U 1 1 559D77F1
P 9250 2400
F 0 "#PWR04" H 9250 2600 50  0001 C CNN
F 1 "+24V" H 9250 2510 50  0000 C CNN
F 2 "" H 9250 2400 60  0000 C CNN
F 3 "" H 9250 2400 60  0000 C CNN
	1    9250 2400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 559D7883
P 7600 2100
F 0 "#PWR05" H 7600 2300 50  0001 C CNN
F 1 "+3.3V" H 7600 2210 50  0000 C CNN
F 2 "" H 7600 2100 60  0000 C CNN
F 3 "" H 7600 2100 60  0000 C CNN
	1    7600 2100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 559D7915
P 4850 1400
F 0 "#PWR06" H 4850 1600 50  0001 C CNN
F 1 "+3.3V" H 4850 1510 50  0000 C CNN
F 2 "" H 4850 1400 60  0000 C CNN
F 3 "" H 4850 1400 60  0000 C CNN
	1    4850 1400
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 559D7BAA
P 4700 5700
F 0 "R4" H 4000 5700 50  0000 C CNN
F 1 "1k" H 4700 5700 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 4700 5700 60  0001 C CNN
F 3 "" H 4700 5700 60  0000 C CNN
	1    4700 5700
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 559D7C90
P 4700 5600
F 0 "R3" H 4000 5600 50  0000 C CNN
F 1 "1k" H 4700 5600 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 4700 5600 60  0001 C CNN
F 3 "" H 4700 5600 60  0000 C CNN
	1    4700 5600
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 559D7D33
P 4700 5500
F 0 "R2" H 4000 5500 50  0000 C CNN
F 1 "1k" H 4700 5500 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 4700 5500 60  0001 C CNN
F 3 "" H 4700 5500 60  0000 C CNN
	1    4700 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2550 4500 2550
Wire Wire Line
	4450 2200 4450 2550
Wire Wire Line
	4000 2200 4450 2200
Wire Wire Line
	4250 2200 4250 2250
Wire Wire Line
	4000 2650 4500 2650
Wire Wire Line
	4250 2650 4250 2600
Wire Wire Line
	3800 2200 3700 2200
Wire Wire Line
	3700 2200 3700 2650
Wire Wire Line
	3700 2650 3800 2650
Connection ~ 4250 2200
Connection ~ 4250 2650
$Comp
L GND #PWR07
U 1 1 559F6928
P 3550 2500
F 0 "#PWR07" H 3550 2300 50  0001 C CNN
F 1 "GND" H 3550 2390 50  0001 C CNN
F 2 "" H 3550 2500 60  0000 C CNN
F 3 "" H 3550 2500 60  0000 C CNN
	1    3550 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2400 3550 2400
Wire Wire Line
	3550 2400 3550 2500
Connection ~ 3700 2400
Wire Bus Line
	4200 2950 4200 7200
Entry Wire Line
	4200 2950 4300 2850
Entry Wire Line
	4200 3050 4300 2950
Entry Wire Line
	4200 3150 4300 3050
Entry Wire Line
	4200 3250 4300 3150
Wire Wire Line
	4300 2850 4500 2850
Wire Wire Line
	4500 2950 4300 2950
Wire Wire Line
	4300 3050 4500 3050
Wire Wire Line
	4500 3150 4300 3150
Wire Wire Line
	5200 5250 5200 5350
Wire Wire Line
	5200 5350 5500 5350
Wire Wire Line
	5500 5350 5500 5250
Wire Wire Line
	5400 5250 5400 5350
Connection ~ 5400 5350
Wire Wire Line
	5300 5250 5300 5350
Connection ~ 5300 5350
Wire Wire Line
	5350 5350 5350 5400
Connection ~ 5350 5350
Wire Wire Line
	4650 6950 4550 6950
Wire Wire Line
	4550 6950 4550 7050
Wire Wire Line
	7600 2100 7600 2300
Wire Wire Line
	7700 2200 7600 2200
Connection ~ 7600 2200
Wire Wire Line
	7700 3800 7600 3800
Wire Wire Line
	7600 3700 7600 3900
$Comp
L GND #PWR08
U 1 1 55A00C4E
P 7600 3450
F 0 "#PWR08" H 7600 3250 50  0001 C CNN
F 1 "GND" H 7600 3340 50  0001 C CNN
F 2 "" H 7600 3450 60  0000 C CNN
F 3 "" H 7600 3450 60  0000 C CNN
	1    7600 3450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 55A00CB6
P 8000 3900
F 0 "#PWR09" H 8000 3700 50  0001 C CNN
F 1 "GND" H 8000 3790 50  0001 C CNN
F 2 "" H 8000 3900 60  0000 C CNN
F 3 "" H 8000 3900 60  0000 C CNN
	1    8000 3900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR010
U 1 1 55A00E8D
P 7600 3700
F 0 "#PWR010" H 7600 3900 50  0001 C CNN
F 1 "+3.3V" H 7600 3810 50  0000 C CNN
F 2 "" H 7600 3700 60  0000 C CNN
F 3 "" H 7600 3700 60  0000 C CNN
	1    7600 3700
	1    0    0    -1  
$EndComp
Connection ~ 7600 3800
Wire Wire Line
	7900 3800 8000 3800
Wire Wire Line
	8000 3800 8000 3900
Wire Wire Line
	7600 5000 7600 5050
Wire Wire Line
	7600 3400 7600 3450
Wire Wire Line
	6850 2800 7100 2800
Wire Wire Line
	7000 2800 7000 2900
Wire Wire Line
	7000 2900 7100 2900
Wire Wire Line
	6850 4400 7100 4400
Wire Wire Line
	7050 4400 7050 4500
Wire Wire Line
	7050 4500 7100 4500
Wire Wire Line
	8100 2700 9800 2700
Wire Wire Line
	8300 2700 8300 4300
Wire Wire Line
	8300 4300 8100 4300
Wire Wire Line
	8350 2900 8350 4600
Wire Wire Line
	8350 3000 8100 3000
Wire Wire Line
	4650 1700 4650 1450
Wire Wire Line
	4650 1450 6750 1450
Wire Wire Line
	4650 1900 4650 2000
Wire Wire Line
	4650 2000 5050 2000
Wire Wire Line
	4850 2000 4850 1900
Wire Wire Line
	5400 1450 5400 2050
Connection ~ 4850 1450
Wire Wire Line
	5300 2050 5300 1450
Connection ~ 5300 1450
Wire Wire Line
	5200 2050 5200 1450
Connection ~ 5200 1450
$Comp
L GND #PWR011
U 1 1 55A03238
P 4750 2050
F 0 "#PWR011" H 4750 1850 50  0001 C CNN
F 1 "GND" H 4750 1940 50  0001 C CNN
F 2 "" H 4750 2050 60  0000 C CNN
F 3 "" H 4750 2050 60  0000 C CNN
	1    4750 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2000 4750 2050
Connection ~ 4750 2000
Wire Wire Line
	4850 1400 4850 1700
Text Label 4300 3050 0    40   ~ 0
PD2
Text Label 4300 3150 0    40   ~ 0
PD3
Text Label 4300 2850 0    40   ~ 0
RXD0
Text Label 4300 2950 0    40   ~ 0
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
Text Label 6850 4400 0    40   ~ 0
BUSTXDEN
Text Label 6850 4600 0    40   ~ 0
BUSRXD
Text Label 6850 4300 0    40   ~ 0
BUSTXD
Entry Wire Line
	6750 2800 6850 2700
Entry Wire Line
	6750 2900 6850 2800
Entry Wire Line
	6750 3100 6850 3000
Entry Wire Line
	6750 4400 6850 4300
Entry Wire Line
	6750 4500 6850 4400
Entry Wire Line
	6750 4700 6850 4600
Wire Wire Line
	6850 4300 7100 4300
Wire Wire Line
	6850 4600 7100 4600
Connection ~ 7050 4400
Wire Bus Line
	6750 5800 6750 2750
Connection ~ 8300 2700
Wire Wire Line
	8350 2900 9800 2900
Connection ~ 8350 3000
Text Label 4300 3250 0    40   ~ 0
TXD0EN
Text Label 4300 3350 0    40   ~ 0
PD5
Wire Wire Line
	4500 3250 4300 3250
Wire Wire Line
	4300 3350 4500 3350
Entry Wire Line
	4200 3350 4300 3250
Entry Wire Line
	4200 3450 4300 3350
Wire Wire Line
	4500 4150 4300 4150
Text Label 4300 4150 0    40   ~ 0
/CSEEP
Entry Wire Line
	4200 5800 4300 5700
Entry Wire Line
	4200 5700 4300 5600
Entry Wire Line
	4200 5600 4300 5500
Entry Wire Line
	5100 5500 5200 5600
Entry Wire Line
	4200 4250 4300 4150
Entry Wire Line
	4200 4350 4300 4250
Entry Wire Line
	4200 4450 4300 4350
Entry Wire Line
	4200 4550 4300 4450
Wire Wire Line
	4300 4250 4500 4250
Wire Wire Line
	4500 4350 4300 4350
Wire Wire Line
	4300 4450 4500 4450
Text Label 4300 4250 0    40   ~ 0
PB5_MOSI
Text Label 4300 4350 0    40   ~ 0
PB6_MISO
Text Label 4300 4450 0    40   ~ 0
PB7_SCK
Text Label 4300 5500 0    40   ~ 0
PB6_MISO
Text Label 4300 5600 0    40   ~ 0
PB5_MOSI
Text Label 4300 5700 0    40   ~ 0
PB7_SCK
Text Label 4950 5500 0    40   ~ 0
MISO
Text Label 4950 5600 0    40   ~ 0
MOSI
Text Label 4950 5700 0    40   ~ 0
SCK
Entry Wire Line
	5100 5600 5200 5700
Entry Wire Line
	5100 5700 5200 5800
Wire Wire Line
	5100 5700 4900 5700
Wire Wire Line
	4900 5600 5100 5600
Wire Wire Line
	5100 5500 4900 5500
Wire Wire Line
	4300 5500 4500 5500
Wire Wire Line
	4500 5600 4300 5600
Wire Wire Line
	4300 5700 4500 5700
Wire Bus Line
	5200 5800 5200 5600
Text Label 4300 6650 0    40   ~ 0
/CSEEP
Text Label 4300 6750 0    40   ~ 0
MISO
Text Label 5800 6950 0    40   ~ 0
MOSI
Text Label 5800 6850 0    40   ~ 0
SCK
$Comp
L R R1
U 1 1 55A05C69
P 4550 6350
F 0 "R1" H 4550 6250 50  0000 C CNN
F 1 "10k" H 4550 6350 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 4550 6350 60  0001 C CNN
F 3 "" H 4550 6350 60  0000 C CNN
	1    4550 6350
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 55A05DA4
P 5900 6350
F 0 "R5" H 5900 6250 50  0000 C CNN
F 1 "10k" H 5900 6350 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 5900 6350 60  0001 C CNN
F 3 "" H 5900 6350 60  0000 C CNN
	1    5900 6350
	0    1    1    0   
$EndComp
Entry Wire Line
	5950 6850 6050 6950
Entry Wire Line
	5950 6950 6050 7050
Entry Wire Line
	4200 6750 4300 6650
Entry Wire Line
	4200 6850 4300 6750
Wire Wire Line
	4300 6650 4650 6650
Wire Wire Line
	4650 6750 4300 6750
Wire Wire Line
	4650 6850 4550 6850
Wire Wire Line
	4550 6850 4550 6550
Wire Wire Line
	5900 6550 5900 6750
Wire Wire Line
	5900 6750 5750 6750
Wire Wire Line
	5900 6100 5900 6150
Wire Wire Line
	4550 6100 5900 6100
Wire Wire Line
	4550 6100 4550 6150
Wire Wire Line
	5750 6650 5750 6100
Connection ~ 5750 6100
Wire Wire Line
	5950 6850 5750 6850
Wire Wire Line
	5750 6950 5950 6950
$Comp
L GND #PWR012
U 1 1 55A076FA
P 5200 6450
F 0 "#PWR012" H 5200 6250 50  0001 C CNN
F 1 "GND" H 5200 6340 50  0001 C CNN
F 2 "" H 5200 6450 60  0000 C CNN
F 3 "" H 5200 6450 60  0000 C CNN
	1    5200 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 6200 5200 6100
Connection ~ 5200 6100
Wire Wire Line
	5200 6400 5200 6450
Wire Bus Line
	6050 6950 6050 7200
Wire Bus Line
	6050 7200 4200 7200
Entry Wire Line
	6750 5800 6850 5700
Entry Wire Line
	6750 5700 6850 5600
Entry Wire Line
	6750 5600 6850 5500
Text Label 6850 5500 0    40   ~ 0
PB6_MISO
$Comp
L +3.3V #PWR013
U 1 1 55A085DC
P 8000 5400
F 0 "#PWR013" H 8000 5600 50  0001 C CNN
F 1 "+3.3V" H 8000 5510 50  0000 C CNN
F 2 "" H 8000 5400 60  0000 C CNN
F 3 "" H 8000 5400 60  0000 C CNN
	1    8000 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 55A08693
P 8000 5800
F 0 "#PWR014" H 8000 5600 50  0001 C CNN
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
L GND #PWR015
U 1 1 55A0C631
P 8000 2300
F 0 "#PWR015" H 8000 2100 50  0001 C CNN
F 1 "GND" H 8000 2190 50  0001 C CNN
F 2 "" H 8000 2300 60  0000 C CNN
F 3 "" H 8000 2300 60  0000 C CNN
	1    8000 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 2200 8000 2200
Wire Wire Line
	8000 2200 8000 2300
$Comp
L C C5
U 1 1 55A0C93D
P 5050 1800
F 0 "C5" H 5100 1850 50  0000 L CNN
F 1 "100nF" H 4900 1750 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 5050 1800 60  0001 C CNN
F 3 "" H 5050 1800 60  0000 C CNN
	1    5050 1800
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 2000 5050 1900
Connection ~ 4850 2000
Wire Wire Line
	5050 1700 5050 1450
Connection ~ 5050 1450
$Comp
L C C7
U 1 1 55A0D0CD
P 6350 2250
F 0 "C7" H 6400 2300 50  0000 L CNN
F 1 "100nF" H 6500 2200 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 6350 2250 60  0001 C CNN
F 3 "" H 6350 2250 60  0000 C CNN
	1    6350 2250
	0    1    1    0   
$EndComp
$Comp
L C C8
U 1 1 55A0D462
P 6550 2250
F 0 "C8" H 6600 2300 50  0000 L CNN
F 1 "4,7nF" H 6650 2400 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 6550 2250 60  0001 C CNN
F 3 "" H 6550 2250 60  0000 C CNN
	1    6550 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	6550 1950 6550 2150
Wire Wire Line
	6550 2350 6550 2450
$Comp
L R R6
U 1 1 55A0DE32
P 6550 1750
F 0 "R6" H 6550 1650 50  0000 C CNN
F 1 "10k" H 6550 1750 50  0000 C CNN
F 2 "Housings_SOIC:SM0805" H 6550 1750 60  0001 C CNN
F 3 "" H 6550 1750 60  0000 C CNN
	1    6550 1750
	0    1    1    0   
$EndComp
$Comp
L L L1
U 1 1 55A0E16B
P 5600 1750
F 0 "L1" H 5500 1650 50  0000 L CNN
F 1 "10µH" H 5500 1835 50  0000 L CNN
F 2 "Housings_SOIC:SM0805" H 5600 1750 60  0001 C CNN
F 3 "" H 5600 1750 60  0000 C CNN
	1    5600 1750
	0    1    1    0   
$EndComp
Connection ~ 5400 1450
Connection ~ 5600 1450
Wire Wire Line
	5600 1950 5600 2050
Connection ~ 6550 2050
$Comp
L GND #PWR016
U 1 1 55A0E9DD
P 6550 2450
F 0 "#PWR016" H 6550 2250 50  0001 C CNN
F 1 "GND" H 6550 2340 50  0001 C CNN
F 2 "" H 6550 2450 60  0000 C CNN
F 3 "" H 6550 2450 60  0000 C CNN
	1    6550 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2050 6750 2050
Wire Wire Line
	6200 2050 6200 2550
Wire Wire Line
	6200 2650 6650 2650
Wire Wire Line
	6250 2650 6250 2150
Wire Wire Line
	6250 2150 6350 2150
Wire Wire Line
	6350 2350 6350 2400
Wire Wire Line
	6350 2400 6550 2400
Connection ~ 6550 2400
Wire Wire Line
	6550 1450 6550 1550
Wire Wire Line
	7550 1200 7550 1100
Wire Wire Line
	7550 1100 7200 1100
Wire Wire Line
	7200 1000 7200 1150
Wire Wire Line
	7550 1400 7550 1500
Wire Wire Line
	7550 1500 7200 1500
Wire Wire Line
	7200 1450 7200 1600
$Comp
L +24V #PWR017
U 1 1 55A0FF09
P 8650 1100
F 0 "#PWR017" H 8650 1300 50  0001 C CNN
F 1 "+24V" H 8650 1210 50  0000 C CNN
F 2 "" H 8650 1100 60  0000 C CNN
F 3 "" H 8650 1100 60  0000 C CNN
	1    8650 1100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 55A10203
P 8650 1500
F 0 "#PWR018" H 8650 1300 50  0001 C CNN
F 1 "GND" H 8650 1390 50  0001 C CNN
F 2 "" H 8650 1500 60  0000 C CNN
F 3 "" H 8650 1500 60  0000 C CNN
	1    8650 1500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 55A10383
P 7200 1600
F 0 "#PWR019" H 7200 1400 50  0001 C CNN
F 1 "GND" H 7200 1490 50  0001 C CNN
F 2 "" H 7200 1600 60  0000 C CNN
F 3 "" H 7200 1600 60  0000 C CNN
	1    7200 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 55A105FF
P 9450 3700
F 0 "#PWR020" H 9450 3500 50  0001 C CNN
F 1 "GND" H 9450 3590 50  0001 C CNN
F 2 "" H 9450 3700 60  0000 C CNN
F 3 "" H 9450 3700 60  0000 C CNN
	1    9450 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2400 9250 2500
Wire Wire Line
	9800 3100 9700 3100
Wire Wire Line
	9700 3100 9700 3650
Wire Wire Line
	8650 1100 8650 1200
Wire Wire Line
	8650 1200 8550 1200
Wire Wire Line
	8550 1400 8650 1400
Wire Wire Line
	8650 1400 8650 1500
Connection ~ 7200 1500
$Comp
L +3.3V #PWR021
U 1 1 55A113A0
P 7200 1000
F 0 "#PWR021" H 7200 1200 50  0001 C CNN
F 1 "+3.3V" H 7200 1110 50  0000 C CNN
F 2 "" H 7200 1000 60  0000 C CNN
F 3 "" H 7200 1000 60  0000 C CNN
	1    7200 1000
	1    0    0    -1  
$EndComp
Connection ~ 7200 1100
Text Notes 10100 3100 0    60   ~ 0
+24V\n\nA\n\nB \n\nGND
Entry Wire Line
	1550 2800 1650 2700
Entry Wire Line
	1550 2900 1650 2800
Entry Wire Line
	1550 3000 1650 2900
Entry Wire Line
	1550 3100 1650 3000
Entry Wire Line
	1550 3200 1650 3100
Entry Wire Line
	1550 3300 1650 3200
Entry Wire Line
	1550 3400 1650 3300
Entry Wire Line
	2650 2700 2750 2800
Entry Wire Line
	2650 2800 2750 2900
Entry Wire Line
	2650 2900 2750 3000
Entry Wire Line
	2650 3000 2750 3100
Entry Wire Line
	2650 3200 2750 3300
Entry Wire Line
	2650 3300 2750 3400
Entry Wire Line
	2650 3400 2750 3500
Entry Wire Line
	2650 3100 2750 3200
Entry Wire Line
	6650 2850 6750 2950
Entry Wire Line
	6650 2950 6750 3050
Entry Wire Line
	6650 3050 6750 3150
Entry Wire Line
	6650 3150 6750 3250
Entry Wire Line
	6650 3250 6750 3350
Entry Wire Line
	6650 3350 6750 3450
Entry Wire Line
	6650 3450 6750 3550
Entry Wire Line
	6650 3550 6750 3650
Entry Wire Line
	6650 3750 6750 3850
Entry Wire Line
	6650 3850 6750 3950
Entry Wire Line
	6650 3950 6750 4050
Entry Wire Line
	6650 4050 6750 4150
Entry Wire Line
	6650 4150 6750 4250
Entry Wire Line
	6650 4250 6750 4350
Entry Wire Line
	6650 4350 6750 4450
Entry Wire Line
	6650 4450 6750 4550
Wire Wire Line
	6200 2850 6650 2850
Wire Wire Line
	6650 2950 6200 2950
Wire Wire Line
	6200 3050 6650 3050
Wire Wire Line
	6650 3150 6200 3150
Wire Wire Line
	6200 3250 6650 3250
Wire Wire Line
	6650 3350 6200 3350
Wire Wire Line
	6200 3450 6650 3450
Wire Wire Line
	6650 3550 6200 3550
Wire Wire Line
	6200 3750 6650 3750
Wire Wire Line
	6650 3850 6200 3850
Wire Wire Line
	6650 3950 6200 3950
Wire Wire Line
	6200 4050 6650 4050
Wire Wire Line
	6650 4150 6200 4150
Wire Wire Line
	6200 4250 6650 4250
Wire Wire Line
	6650 4350 6200 4350
Wire Wire Line
	6200 4450 6650 4450
$Comp
L +3.3V #PWR022
U 1 1 55A147BE
P 1850 2600
F 0 "#PWR022" H 1850 2800 50  0001 C CNN
F 1 "+3.3V" V 1850 2800 50  0000 C CNN
F 2 "" H 1850 2600 60  0000 C CNN
F 3 "" H 1850 2600 60  0000 C CNN
	1    1850 2600
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR023
U 1 1 55A149AB
P 1850 2500
F 0 "#PWR023" H 1850 2300 50  0001 C CNN
F 1 "GND" H 1850 2390 50  0001 C CNN
F 2 "" H 1850 2500 60  0000 C CNN
F 3 "" H 1850 2500 60  0000 C CNN
	1    1850 2500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR024
U 1 1 55A14C98
P 2400 2500
F 0 "#PWR024" H 2400 2300 50  0001 C CNN
F 1 "GND" H 2400 2390 50  0001 C CNN
F 2 "" H 2400 2500 60  0000 C CNN
F 3 "" H 2400 2500 60  0000 C CNN
	1    2400 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2350 2500 2400 2500
Wire Wire Line
	2350 2600 2650 2600
Wire Wire Line
	4500 3750 4300 3750
Wire Wire Line
	4500 3850 4300 3850
Entry Wire Line
	4200 3850 4300 3750
Entry Wire Line
	4200 3950 4300 3850
Text Label 4300 3750 0    40   ~ 0
PB0
Text Label 4300 3850 0    40   ~ 0
PB1
$Comp
L +3.3V #PWR025
U 1 1 55A16808
P 5400 6050
F 0 "#PWR025" H 5400 6250 50  0001 C CNN
F 1 "+3.3V" H 5400 6200 50  0000 C CNN
F 2 "" H 5400 6050 60  0000 C CNN
F 3 "" H 5400 6050 60  0000 C CNN
	1    5400 6050
	1    0    0    -1  
$EndComp
Connection ~ 5400 6100
Text Label 6250 2050 0    40   ~ 0
/RESET
Text Label 6850 5700 0    40   ~ 0
/RESET
Text Label 6850 5600 0    40   ~ 0
PB7_SCK
Text Label 7950 5600 0    40   ~ 0
PB5_MOSI
$Comp
L HOLE H1
U 1 1 55A11650
P 1600 6200
F 0 "H1" H 1700 6200 50  0000 L CNN
F 1 "HOLE" H 1900 6200 50  0001 L CNN
F 2 "Mechanical:HOLE_2.5mm" H 1600 6200 60  0001 C CNN
F 3 "" H 1600 6200 60  0000 C CNN
	1    1600 6200
	1    0    0    -1  
$EndComp
NoConn ~ 1550 6200
$Comp
L OPEN_HARDWARE_1 LOGO1
U 1 1 55B4D774
P 1300 7350
F 0 "LOGO1" H 1300 7625 60  0001 C CNN
F 1 "Open Hardware" H 1300 7125 60  0000 C CNN
F 2 "Symbol:Symbol_OSHW-Logo_SilkScreen" H 1300 7350 60  0001 C CNN
F 3 "" H 1300 7350 60  0000 C CNN
	1    1300 7350
	1    0    0    -1  
$EndComp
Entry Wire Line
	6650 2650 6750 2750
Entry Wire Line
	6650 2650 6750 2750
Connection ~ 6250 2650
Text Label 6500 2650 0    40   ~ 0
AREF
Text Label 6500 2850 0    40   ~ 0
PA0
Text Label 6500 2950 0    40   ~ 0
PA1
Entry Wire Line
	4200 3550 4300 3450
Entry Wire Line
	4200 3650 4300 3550
Wire Wire Line
	4500 3450 4300 3450
Wire Wire Line
	4500 3550 4300 3550
Text Label 4300 3450 0    40   ~ 0
PD6
Text Label 4300 3550 0    40   ~ 0
PD7
Text Label 4300 3950 0    40   ~ 0
PB2
Text Label 4300 4050 0    40   ~ 0
PB3
Entry Wire Line
	4200 4050 4300 3950
Entry Wire Line
	4200 4150 4300 4050
Wire Wire Line
	4300 3950 4500 3950
Wire Wire Line
	4300 4050 4500 4050
Wire Notes Line
	6800 3550 8200 3550
Wire Notes Line
	8200 3550 8200 5050
Wire Notes Line
	8200 5050 6800 5050
Wire Notes Line
	6800 5050 6800 3550
Text Notes 6850 3600 0    40   ~ 0
optional 
Entry Wire Line
	2650 3400 2750 3500
Entry Wire Line
	2650 3900 2750 4000
Entry Wire Line
	2650 4000 2750 4100
Entry Wire Line
	2650 4100 2750 4200
Entry Wire Line
	2650 4200 2750 4300
Entry Wire Line
	2650 4400 2750 4500
Entry Wire Line
	2650 4500 2750 4600
Entry Wire Line
	2650 4600 2750 4700
Entry Wire Line
	2650 4300 2750 4400
$Comp
L +3.3V #PWR026
U 1 1 55D48AB9
P 2400 3800
F 0 "#PWR026" H 2400 4000 50  0001 C CNN
F 1 "+3.3V" V 2400 4000 50  0000 C CNN
F 2 "" H 2400 3800 60  0000 C CNN
F 3 "" H 2400 3800 60  0000 C CNN
	1    2400 3800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR027
U 1 1 55D48ABF
P 2400 3700
F 0 "#PWR027" H 2400 3500 50  0001 C CNN
F 1 "GND" H 2400 3590 50  0001 C CNN
F 2 "" H 2400 3700 60  0000 C CNN
F 3 "" H 2400 3700 60  0000 C CNN
	1    2400 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2350 3700 2400 3700
Wire Wire Line
	2400 3800 2350 3800
Entry Wire Line
	2650 4600 2750 4700
Entry Wire Line
	2650 4600 2750 4700
Entry Wire Line
	1550 4000 1650 3900
Entry Wire Line
	1550 4100 1650 4000
Entry Wire Line
	1550 4200 1650 4100
Entry Wire Line
	1550 4300 1650 4200
Entry Wire Line
	1550 4500 1650 4400
Entry Wire Line
	1550 4600 1650 4500
Entry Wire Line
	1550 4700 1650 4600
Entry Wire Line
	1550 4400 1650 4300
$Comp
L +3.3V #PWR028
U 1 1 55D48C10
P 1850 3800
F 0 "#PWR028" H 1850 4000 50  0001 C CNN
F 1 "+3.3V" V 1850 4000 50  0000 C CNN
F 2 "" H 1850 3800 60  0000 C CNN
F 3 "" H 1850 3800 60  0000 C CNN
	1    1850 3800
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR029
U 1 1 55D48C16
P 1850 3700
F 0 "#PWR029" H 1850 3500 50  0001 C CNN
F 1 "GND" H 1850 3590 50  0001 C CNN
F 2 "" H 1850 3700 60  0000 C CNN
F 3 "" H 1850 3700 60  0000 C CNN
	1    1850 3700
	0    1    1    0   
$EndComp
Text Label 1750 3100 0    40   ~ 0
PA0
Text Label 2450 3000 0    40   ~ 0
PA1
Text Label 1750 3000 0    40   ~ 0
PA2
Text Label 2450 2900 0    40   ~ 0
PA3
Text Label 1750 2900 0    40   ~ 0
PA4
Text Label 2450 2800 0    40   ~ 0
PA5
Text Label 1750 2800 0    40   ~ 0
PA6
Text Label 2450 2700 0    40   ~ 0
PA7
Wire Wire Line
	2350 2700 2650 2700
Wire Wire Line
	2650 2800 2350 2800
Wire Wire Line
	2350 2900 2650 2900
Wire Wire Line
	2650 3000 2350 3000
Wire Wire Line
	2350 3100 2650 3100
Wire Wire Line
	2650 3200 2350 3200
Wire Wire Line
	2350 3300 2650 3300
Wire Wire Line
	2650 3400 2350 3400
Wire Wire Line
	2350 3900 2650 3900
Wire Wire Line
	2650 4000 2350 4000
Wire Wire Line
	2350 4100 2650 4100
Wire Wire Line
	2650 4200 2350 4200
Wire Wire Line
	2350 4300 2650 4300
Wire Wire Line
	2650 4400 2350 4400
Wire Wire Line
	2350 4500 2650 4500
Wire Wire Line
	2650 4600 2350 4600
Text Label 6500 3050 0    40   ~ 0
PA2
Text Label 6500 3150 0    40   ~ 0
PA3
Text Label 6500 3250 0    40   ~ 0
PA4
Text Label 6500 3350 0    40   ~ 0
PA5
Text Label 6500 3450 0    40   ~ 0
PA6
Text Label 6500 3550 0    40   ~ 0
PA7
Text Label 6500 3750 0    40   ~ 0
PC0
Text Label 6500 3850 0    40   ~ 0
PC1
Text Label 6500 3950 0    40   ~ 0
PC2
Text Label 6500 4050 0    40   ~ 0
PC3
Text Label 6500 4150 0    40   ~ 0
PC4
Text Label 6500 4250 0    40   ~ 0
PC5
Text Label 6500 4350 0    40   ~ 0
PC6
Text Label 6500 4450 0    40   ~ 0
PC7
Text Label 2450 3100 0    40   ~ 0
PB0
Text Label 1750 3200 0    40   ~ 0
PB1
Text Label 2450 3200 0    40   ~ 0
PB2
Text Label 1750 3300 0    40   ~ 0
PB3
Text Label 2450 3300 0    40   ~ 0
MOSI
Text Label 1750 3400 0    40   ~ 0
MISO
Text Label 2450 3400 0    40   ~ 0
SCK
Text Label 2450 3900 0    40   ~ 0
PC0
Text Label 1750 3900 0    40   ~ 0
PC1
Text Label 2450 4000 0    40   ~ 0
PC2
Text Label 1750 4000 0    40   ~ 0
PC3
Text Label 2450 4100 0    40   ~ 0
PC4
Text Label 1750 4100 0    40   ~ 0
PC5
Text Label 2450 4200 0    40   ~ 0
PC6
Text Label 1750 4200 0    40   ~ 0
PC7
Text Label 2450 4300 0    40   ~ 0
PD2
Text Label 1750 4300 0    40   ~ 0
PD3
Text Label 2450 4400 0    40   ~ 0
PD5
Text Label 1750 4400 0    40   ~ 0
PD6
Text Label 2450 4500 0    40   ~ 0
PD7
Text Label 1650 4500 0    40   ~ 0
BUSRXD
Text Label 2450 4600 0    40   ~ 0
BUSTXD
Text Label 1650 4600 0    40   ~ 0
BUSTXDEN
NoConn ~ 8900 3000
Wire Wire Line
	9150 3300 9250 3300
Wire Wire Line
	9250 3300 9250 2900
Wire Wire Line
	8650 3300 8550 3300
Wire Wire Line
	8550 3300 8550 2700
Wire Wire Line
	8900 3600 8900 3650
Wire Wire Line
	8900 3650 9700 3650
Connection ~ 9450 3650
Connection ~ 8550 2700
Connection ~ 9250 2900
Wire Wire Line
	5600 2000 6050 2000
Connection ~ 5600 2000
Wire Wire Line
	5600 1550 5600 1450
Text Label 5750 2000 0    40   ~ 0
AVCC
Text Label 2450 2600 0    40   ~ 0
AVCC
Text Label 1750 2700 0    40   ~ 0
AREF
Wire Wire Line
	1850 2500 1950 2500
Wire Wire Line
	1850 2600 1950 2600
Wire Wire Line
	1650 2700 1950 2700
Wire Wire Line
	1650 2800 1950 2800
Wire Wire Line
	1650 2900 1950 2900
Wire Wire Line
	1650 3000 1950 3000
Wire Wire Line
	1650 3100 1950 3100
Wire Wire Line
	1650 3200 1950 3200
Wire Wire Line
	1650 3300 1950 3300
Wire Wire Line
	1950 3400 1650 3400
Entry Wire Line
	1550 3500 1650 3400
Entry Wire Line
	2650 2600 2750 2700
Wire Wire Line
	1650 3900 1950 3900
Wire Wire Line
	1950 4000 1650 4000
Wire Wire Line
	1650 4100 1950 4100
Wire Wire Line
	1950 4200 1650 4200
Wire Wire Line
	1650 4300 1950 4300
Wire Wire Line
	1950 4400 1650 4400
Wire Wire Line
	1950 4500 1650 4500
Wire Wire Line
	1650 4600 1950 4600
Wire Bus Line
	1550 2800 1550 5800
Wire Wire Line
	5400 6050 5400 6100
Wire Wire Line
	1850 3700 1950 3700
Wire Wire Line
	1950 3800 1850 3800
Wire Bus Line
	2750 2700 2750 5800
$Comp
L D D1
U 1 1 55E046D4
P 6750 1750
F 0 "D1" H 6760 1830 50  0000 C CNN
F 1 "1N4148WS (SOD323)" H 6750 1675 50  0000 C CNN
F 2 "Diode:SOD323" H 6750 1750 60  0001 C CNN
F 3 "" H 6750 1750 60  0000 C CNN
	1    6750 1750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6750 2050 6750 1950
Wire Wire Line
	6750 1450 6750 1550
Connection ~ 6550 1450
Text Notes 8700 4050 0    60   ~ 0
Protection varistor and \noptional RS485 termination resistor \nare plugged into the WAGO clamps. 
Wire Bus Line
	1550 5800 6750 5800
$Comp
L SEMTECH_SR05 D2
U 1 1 55EC3163
P 8900 3300
F 0 "D2" H 8975 3050 50  0000 C CNN
F 1 "SEMTECH_SR05" H 8900 3650 50  0000 C CNN
F 2 "Diode_SMD:SOT143" H 8900 3300 60  0001 C CNN
F 3 "" H 8900 3300 60  0000 C CNN
	1    8900 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 3650 9450 3700
Wire Wire Line
	8350 4600 8100 4600
Text Label 9150 2700 0    40   ~ 0
A
Text Label 9150 2900 0    40   ~ 0
B
$Comp
L CONN_10X2-RESCUE-bus-module_atmega324 J2
U 1 1 56050AF8
P 2150 2950
F 0 "J2" H 2150 3500 50  0000 C CNN
F 1 "CONN_10X2" H 2150 2425 25  0000 C CNN
F 2 "Connector_Header:HEADER_10x2" H 2150 3050 60  0001 C CNN
F 3 "" H 2150 3050 60  0000 C CNN
	1    2150 2950
	1    0    0    -1  
$EndComp
$Comp
L CONN_10X2-RESCUE-bus-module_atmega324 J1
U 1 1 56050B83
P 2150 4150
F 0 "J1" H 2150 4700 50  0000 C CNN
F 1 "CONN_10X2" H 2150 3625 25  0000 C CNN
F 2 "Connector_Header:HEADER_10x2" H 2150 4250 60  0001 C CNN
F 3 "" H 2150 4250 60  0000 C CNN
	1    2150 4150
	1    0    0    -1  
$EndComp
$Comp
L FUSE F1
U 1 1 566474AD
P 9500 2500
F 0 "F1" H 9500 2600 50  0000 C CNN
F 1 "FUSE" H 9500 2400 50  0000 C CNN
F 2 "Resistor:R_1812" H 9500 2500 60  0001 C CNN
F 3 "" H 9500 2500 60  0000 C CNN
	1    9500 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2500 9300 2500
Wire Wire Line
	9700 2500 9800 2500
$EndSCHEMATC
