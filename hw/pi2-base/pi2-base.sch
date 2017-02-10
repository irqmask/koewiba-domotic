EESchema Schematic File Version 2
LIBS:pi2-base-rescue
LIBS:connectors
LIBS:diodes
LIBS:ics_regulator
LIBS:logo
LIBS:modules
LIBS:power
LIBS:relays
LIBS:standard
LIBS:transistors
LIBS:ics_peripheral
LIBS:pi2-base-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title "pi2-base"
Date "2017-02-10"
Rev "1"
Comp "KoeWiBa"
Comment1 "Base board for Raspberry Pi B2"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L bus-module_atmega324 MOD1
U 1 1 583C04FF
P 4750 2900
F 0 "MOD1" H 4750 2900 50  0000 C CNN
F 1 "bus-module_atmega324" V 4900 2900 50  0000 C CNN
F 2 "Modules:bus-module_atmega324" H 4750 2900 60  0001 C CNN
F 3 "" H 4750 2900 60  0000 C CNN
	1    4750 2900
	-1   0    0    -1  
$EndComp
$Comp
L Raspberry_Pi_B2 MOD2
U 1 1 583C214C
P 8100 2900
F 0 "MOD2" H 8100 4115 50  0000 C CNN
F 1 "Raspberry_Pi_B2" H 8100 4024 50  0000 C CNN
F 2 "Modules:raspberry_pi2" H 8100 2850 60  0001 C CNN
F 3 "" H 8100 2850 60  0001 C CNN
	1    8100 2900
	1    0    0    -1  
$EndComp
$Sheet
S 6400 1000 550  600 
U 583C418C
F0 "RTC" 50
F1 "rtc.sch" 50
F2 "SCL_3V3" I R 6950 1400 50 
F3 "SDA_3V3" I R 6950 1300 50 
F4 "+3V3_IN" I R 6950 1150 50 
F5 "+5V_IN" I R 6950 1050 50 
F6 "GND" I R 6950 1550 50 
$EndSheet
$Sheet
S 2000 1000 850  300 
U 583C4215
F0 "DC/DC" 39
F1 "dcdc.sch" 39
F2 "+24V_IN" I L 2000 1100 50 
F3 "GND" I L 2000 1200 50 
F4 "+24V_OUT" O R 2850 1100 50 
F5 "+5V_OUT" O R 2850 1200 50 
$EndSheet
$Sheet
S 2000 6300 800  800 
U 583C42B2
F0 "4 Inputs" 39
F1 "4inputs.sch" 39
F2 "+5V" I R 2800 6400 50 
F3 "GND" I R 2800 7000 50 
F4 "IN1" I L 2000 6400 50 
F5 "IN1_PROT" O R 2800 6550 50 
F6 "IN2" I L 2000 6600 50 
F7 "IN2_PROT" O R 2800 6650 50 
F8 "IN3" I L 2000 6800 50 
F9 "IN3_PROT" O R 2800 6750 50 
F10 "IN4" I L 2000 7000 50 
F11 "IN4_PROT" O R 2800 6850 50 
$EndSheet
$Sheet
S 2000 3150 800  800 
U 583C43EA
F0 "2 Relays" 39
F1 "2relays.sch" 39
F2 "LEDVCC" I R 2800 3350 50 
F3 "GND" I R 2800 3850 50 
F4 "VCC" I R 2800 3250 50 
F5 "REL2_NC" U L 2000 3850 50 
F6 "REL2_NO" U L 2000 3750 50 
F7 "REL2_COM" U L 2000 3650 50 
F8 "REL2" I R 2800 3650 50 
F9 "REL1" I R 2800 3500 50 
F10 "REL1_COM" U L 2000 3250 50 
F11 "REL1_NO" U L 2000 3350 50 
F12 "REL1_NC" U L 2000 3450 50 
$EndSheet
$Comp
L +3.3V #PWR01
U 1 1 583C4681
P 7400 950
F 0 "#PWR01" H 7400 1150 50  0001 C CNN
F 1 "+3.3V" H 7409 1103 50  0000 C CNN
F 2 "" H 7400 950 60  0001 C CNN
F 3 "" H 7400 950 60  0001 C CNN
	1    7400 950 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 583C470B
P 7350 4050
F 0 "#PWR02" H 7350 3850 50  0001 C CNN
F 1 "GND" H 7350 3940 50  0001 C CNN
F 2 "" H 7350 4050 60  0001 C CNN
F 3 "" H 7350 4050 60  0001 C CNN
	1    7350 4050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 583C4A5B
P 8800 950
F 0 "#PWR03" H 8800 1150 50  0001 C CNN
F 1 "+5V" H 8796 1094 50  0000 C CNN
F 2 "" H 8800 950 60  0001 C CNN
F 3 "" H 8800 950 60  0001 C CNN
	1    8800 950 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 583C4A7F
P 8800 4100
F 0 "#PWR04" H 8800 3900 50  0001 C CNN
F 1 "GND" H 8800 3990 50  0001 C CNN
F 2 "" H 8800 4100 60  0001 C CNN
F 3 "" H 8800 4100 60  0001 C CNN
	1    8800 4100
	1    0    0    -1  
$EndComp
NoConn ~ 8700 3250
NoConn ~ 7500 3250
$Comp
L GND #PWR05
U 1 1 583C4C25
P 4750 4300
F 0 "#PWR05" H 4750 4100 50  0001 C CNN
F 1 "GND" H 4750 4190 50  0001 C CNN
F 2 "" H 4750 4300 60  0001 C CNN
F 3 "" H 4750 4300 60  0001 C CNN
	1    4750 4300
	-1   0    0    -1  
$EndComp
NoConn ~ 4200 3700
NoConn ~ 4200 3600
NoConn ~ 4200 3500
NoConn ~ 4200 3000
Text Notes 2000 800  0    50   ~ 0
DC/DC Converter with fuses and connectors
Text Notes 6400 900  0    50   ~ 0
Real time clock
$Comp
L GND #PWR06
U 1 1 583DA21C
P 7050 1650
F 0 "#PWR06" H 7050 1450 50  0001 C CNN
F 1 "GND" H 7050 1540 50  0001 C CNN
F 2 "" H 7050 1650 60  0001 C CNN
F 3 "" H 7050 1650 60  0001 C CNN
	1    7050 1650
	1    0    0    -1  
$EndComp
Text Notes 4300 1500 0    50   ~ 0
Gateway BUS<->RS232
Text Notes 2000 2950 0    50   ~ 0
Relays for general purpose
Text Notes 2000 6200 0    50   ~ 0
Inputs for general purpose
$Comp
L R R6
U 1 1 583D8EBC
P 5800 3100
F 0 "R6" H 6050 3150 50  0000 C CNN
F 1 "10k" H 5800 3100 50  0000 C CNN
F 2 "Resistor:R_0805" H 5800 3100 60  0001 C CNN
F 3 "" H 5800 3100 60  0000 C CNN
	1    5800 3100
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 583D95CC
P 5800 3250
F 0 "R7" H 6050 3300 50  0000 C CNN
F 1 "10k" H 5800 3250 50  0000 C CNN
F 2 "Resistor:R_0805" H 5800 3250 60  0001 C CNN
F 3 "" H 5800 3250 60  0000 C CNN
	1    5800 3250
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 583D9BC2
P 6100 3550
F 0 "R9" H 6100 3450 50  0000 L CNN
F 1 "18k" H 6100 3550 50  0000 C CNN
F 2 "Resistor:R_0805" H 6100 3550 60  0001 C CNN
F 3 "" H 6100 3550 60  0000 C CNN
	1    6100 3550
	0    1    1    0   
$EndComp
$Comp
L R R10
U 1 1 583D9D30
P 6300 3550
F 0 "R10" H 6300 3650 50  0000 L CNN
F 1 "18k" H 6300 3550 50  0000 C CNN
F 2 "Resistor:R_0805" H 6300 3550 60  0001 C CNN
F 3 "" H 6300 3550 60  0000 C CNN
	1    6300 3550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR07
U 1 1 583DA8D5
P 6300 4050
F 0 "#PWR07" H 6300 3850 50  0001 C CNN
F 1 "GND" H 6300 3940 50  0001 C CNN
F 2 "" H 6300 4050 60  0001 C CNN
F 3 "" H 6300 4050 60  0001 C CNN
	1    6300 4050
	1    0    0    -1  
$EndComp
Entry Wire Line
	9100 2250 9200 2350
Entry Wire Line
	9100 2350 9200 2450
Entry Wire Line
	6550 3100 6650 3200
Entry Wire Line
	6550 3250 6650 3350
Text Label 6350 3100 0    40   ~ 0
PI_RXD
Text Label 6350 3250 0    40   ~ 0
PI_CTS
Text Label 6350 2950 0    40   ~ 0
PI_TXD
Entry Wire Line
	6550 2950 6650 3050
Text Label 5350 3100 0    40   ~ 0
BM_TXD
Text Label 5350 3200 0    40   ~ 0
BM_CTS
Text Label 9100 2250 2    40   ~ 0
PI_TXD
Text Label 9100 2350 2    40   ~ 0
PI_RXD
NoConn ~ 4550 1750
NoConn ~ 4650 1750
NoConn ~ 4850 1750
NoConn ~ 4950 1750
Text Notes 4350 1700 0    40   ~ 0
5V generated by \nbus-module is not used!
$Comp
L FUSE F1
U 1 1 583E2953
P 2200 1650
F 0 "F1" H 2200 1750 50  0000 C CNN
F 1 "250V 1A T" H 2200 1550 50  0000 C CNN
F 2 "Electromechanical:FUSE5-20" H 2200 1650 60  0001 C CNN
F 3 "" H 2200 1650 60  0000 C CNN
	1    2200 1650
	1    0    0    -1  
$EndComp
$Comp
L SWITCH SW1
U 1 1 583E2A61
P 2450 1750
F 0 "SW1" H 2700 1950 50  0000 C CNN
F 1 "SWITCH" H 2700 1750 50  0000 C CNN
F 2 "Connector_Header:PCB_CON2" H 2450 1750 60  0001 C CNN
F 3 "" H 2450 1750 60  0000 C CNN
	1    2450 1750
	1    0    0    -1  
$EndComp
Text Notes 2000 1500 0    50   ~ 0
Bus power
Text Notes 2000 2000 0    50   ~ 0
Bus pull-up / pull-down
$Comp
L +24V #PWR08
U 1 1 583E4182
P 3050 1000
F 0 "#PWR08" H 3050 1200 50  0001 C CNN
F 1 "+24V" H 2950 1150 50  0000 C CNN
F 2 "" H 3050 1000 60  0001 C CNN
F 3 "" H 3050 1000 60  0001 C CNN
	1    3050 1000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR09
U 1 1 583E425A
P 3150 1000
F 0 "#PWR09" H 3150 1200 50  0001 C CNN
F 1 "+5V" H 3250 1150 50  0000 C CNN
F 2 "" H 3150 1000 60  0001 C CNN
F 3 "" H 3150 1000 60  0001 C CNN
	1    3150 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 583E4C14
P 1900 1300
F 0 "#PWR010" H 1900 1100 50  0001 C CNN
F 1 "GND" H 1900 1190 50  0001 C CNN
F 2 "" H 1900 1300 60  0001 C CNN
F 3 "" H 1900 1300 60  0001 C CNN
	1    1900 1300
	1    0    0    -1  
$EndComp
$Comp
L JUMPER J9
U 1 1 583E4D9B
P 2150 2150
F 0 "J9" H 2150 2250 50  0000 C CNN
F 1 "BUS Pull-Up" H 2350 2250 25  0000 C CNN
F 2 "Connector_Header:HEADER_2x1" H 2150 2150 60  0001 C CNN
F 3 "" H 2150 2150 60  0000 C CNN
	1    2150 2150
	1    0    0    -1  
$EndComp
$Comp
L JUMPER J10
U 1 1 583E4E8B
P 2150 2400
F 0 "J10" H 2150 2500 50  0000 C CNN
F 1 "BUS Pull-down" H 2350 2500 25  0000 C CNN
F 2 "Connector_Header:HEADER_2x1" H 2150 2400 60  0001 C CNN
F 3 "" H 2150 2400 60  0000 C CNN
	1    2150 2400
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 583E641B
P 2600 2150
F 0 "R1" H 2600 2250 50  0000 C CNN
F 1 "1k" H 2600 2150 50  0000 C CNN
F 2 "Resistor:R_0805" H 2600 2150 60  0001 C CNN
F 3 "" H 2600 2150 60  0000 C CNN
	1    2600 2150
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 583E651D
P 2600 2400
F 0 "R2" H 2600 2500 50  0000 C CNN
F 1 "1k" H 2600 2400 50  0000 C CNN
F 2 "Resistor:R_0805" H 2600 2400 60  0001 C CNN
F 3 "" H 2600 2400 60  0000 C CNN
	1    2600 2400
	1    0    0    -1  
$EndComp
Text Label 1550 2150 0    40   ~ 0
BUS_A
Text Label 1550 2400 0    40   ~ 0
BUS_B
$Comp
L +5V #PWR011
U 1 1 583E7D9E
P 3150 2050
F 0 "#PWR011" H 3150 2250 50  0001 C CNN
F 1 "+5V" H 3146 2194 50  0000 C CNN
F 2 "" H 3150 2050 60  0001 C CNN
F 3 "" H 3150 2050 60  0001 C CNN
	1    3150 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 583E7E7E
P 3150 2500
F 0 "#PWR012" H 3150 2300 50  0001 C CNN
F 1 "GND" H 3150 2390 50  0001 C CNN
F 2 "" H 3150 2500 60  0001 C CNN
F 3 "" H 3150 2500 60  0001 C CNN
	1    3150 2500
	1    0    0    -1  
$EndComp
$Comp
L CONN_2X1 J1
U 1 1 583E80F6
P 1050 1150
F 0 "J1" H 1228 1176 50  0000 L CNN
F 1 "WAGO 2081-1122" H 1228 1104 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_2081-1122" H 1050 1150 60  0001 C CNN
F 3 "" H 1050 1150 60  0000 C CNN
	1    1050 1150
	-1   0    0    -1  
$EndComp
$Comp
L CONN_3X1 J6
U 1 1 583E8284
P 1050 3350
F 0 "J6" H 1228 3376 50  0000 L CNN
F 1 "WAGO 2081-1123" H 1250 3450 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_2081-1123" H 1050 3350 60  0001 C CNN
F 3 "" H 1050 3350 60  0000 C CNN
	1    1050 3350
	-1   0    0    -1  
$EndComp
$Comp
L CONN_3X1 J7
U 1 1 583E840B
P 1050 3750
F 0 "J7" H 1228 3776 50  0000 L CNN
F 1 "WAGO 2081-1123" H 1250 3850 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_2081-1123" H 1050 3750 60  0001 C CNN
F 3 "" H 1050 3750 60  0000 C CNN
	1    1050 3750
	-1   0    0    -1  
$EndComp
$Comp
L CONN_8X1 J8
U 1 1 583E874C
P 1050 6750
F 0 "J8" H 1228 6776 50  0000 L CNN
F 1 "WAGO 233-508" H 1228 6704 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_233-8" H 1050 6750 60  0001 C CNN
F 3 "" H 1050 6750 60  0000 C CNN
	1    1050 6750
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1X1 J2
U 1 1 583E88D1
P 1050 1800
F 0 "J2" H 1228 1826 50  0000 L CNN
F 1 "WAGO 243 orange" H 1228 1754 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_243" H 1050 1800 60  0001 C CNN
F 3 "" H 1050 1800 60  0000 C CNN
	1    1050 1800
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1X1 J3
U 1 1 583E8A4D
P 1050 2000
F 0 "J3" H 1228 2026 50  0000 L CNN
F 1 "WAGO 243 green" H 1228 1954 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_243" H 1050 2000 60  0001 C CNN
F 3 "" H 1050 2000 60  0000 C CNN
	1    1050 2000
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1X1 J4
U 1 1 583E8BB4
P 1050 2200
F 0 "J4" H 1228 2226 50  0000 L CNN
F 1 "WAGO 243 grey" H 1228 2154 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_243" H 1050 2200 60  0001 C CNN
F 3 "" H 1050 2200 60  0000 C CNN
	1    1050 2200
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1X1 J5
U 1 1 583E8D22
P 1050 2400
F 0 "J5" H 1228 2426 50  0000 L CNN
F 1 "WAGO 243 blue" H 1228 2354 25  0000 L CNN
F 2 "Connector_WAGO:WAGO_243" H 1050 2400 60  0001 C CNN
F 3 "" H 1050 2400 60  0000 C CNN
	1    1050 2400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7500 1950 7400 1950
Wire Wire Line
	7400 950  7400 2750
Wire Wire Line
	7400 2750 7500 2750
Connection ~ 7400 1950
Wire Wire Line
	7500 2350 7350 2350
Wire Wire Line
	7350 2350 7350 4050
Wire Wire Line
	7350 3850 7500 3850
Wire Wire Line
	7500 3150 7350 3150
Connection ~ 7350 3150
Connection ~ 7350 3850
Wire Wire Line
	7500 2050 7250 2050
Wire Wire Line
	7250 2050 7250 1300
Wire Wire Line
	7250 1300 6950 1300
Wire Wire Line
	6950 1400 7200 1400
Wire Wire Line
	7200 1400 7200 2150
Wire Wire Line
	7200 2150 7500 2150
Wire Wire Line
	8800 2050 8700 2050
Wire Wire Line
	8800 950  8800 2050
Wire Wire Line
	8800 1950 8700 1950
Connection ~ 8800 1950
Wire Wire Line
	8700 2150 8800 2150
Wire Wire Line
	8800 2150 8800 4100
Wire Wire Line
	8700 3550 8800 3550
Connection ~ 8800 3550
Wire Wire Line
	8700 3350 8800 3350
Connection ~ 8800 3350
Wire Wire Line
	8700 2850 8800 2850
Connection ~ 8800 2850
Wire Wire Line
	8700 2550 8800 2550
Connection ~ 8800 2550
Wire Wire Line
	4950 4100 4950 4200
Wire Wire Line
	4950 4200 4550 4200
Wire Wire Line
	4550 4200 4550 4100
Wire Wire Line
	4650 4100 4650 4200
Connection ~ 4650 4200
Wire Wire Line
	4850 4100 4850 4200
Connection ~ 4850 4200
Wire Wire Line
	4750 4300 4750 4200
Connection ~ 4750 4200
Wire Wire Line
	6950 1150 7400 1150
Wire Wire Line
	6950 1050 8800 1050
Connection ~ 8800 1050
Connection ~ 7400 1150
Wire Wire Line
	6950 1550 7050 1550
Wire Wire Line
	7050 1550 7050 1650
Wire Wire Line
	6300 3750 6300 4050
Wire Wire Line
	6300 3950 6100 3950
Wire Wire Line
	6100 3950 6100 3750
Connection ~ 6300 3950
Wire Wire Line
	6000 3250 6550 3250
Wire Wire Line
	6100 3250 6100 3350
Wire Wire Line
	5300 3100 5600 3100
Wire Wire Line
	5300 3200 5500 3200
Wire Wire Line
	5500 3200 5500 3250
Wire Wire Line
	5500 3250 5600 3250
Wire Bus Line
	6650 4350 9200 4350
Wire Wire Line
	5300 3000 5500 3000
Wire Wire Line
	5500 3000 5500 2950
Wire Wire Line
	5500 2950 6550 2950
Connection ~ 6100 3250
Wire Wire Line
	6000 3100 6550 3100
Wire Wire Line
	6300 3350 6300 3100
Connection ~ 6300 3100
Wire Bus Line
	6650 7500 6650 3050
Wire Wire Line
	8700 2250 9100 2250
Wire Wire Line
	8700 2350 9100 2350
Wire Bus Line
	9200 4350 9200 2350
Wire Wire Line
	2400 1650 2500 1650
Wire Wire Line
	2850 1100 3050 1100
Wire Wire Line
	3050 1000 3050 1650
Wire Wire Line
	3050 1650 2900 1650
Wire Wire Line
	2850 1200 3150 1200
Wire Wire Line
	3150 1200 3150 1000
Connection ~ 3050 1100
Wire Wire Line
	2000 1200 1900 1200
Wire Wire Line
	1900 1200 1900 1300
Wire Wire Line
	2300 2150 2400 2150
Wire Wire Line
	2300 2400 2400 2400
Wire Wire Line
	2800 2150 3150 2150
Wire Wire Line
	3150 2150 3150 2050
Wire Wire Line
	2800 2400 3150 2400
Wire Wire Line
	3150 2400 3150 2500
Wire Wire Line
	1250 1100 2000 1100
Wire Wire Line
	1250 1200 1350 1200
Wire Wire Line
	1350 1200 1350 1300
Wire Wire Line
	1250 2400 1350 2400
Wire Wire Line
	1350 2400 1350 2500
Wire Wire Line
	1250 2200 1400 2200
Wire Wire Line
	1400 2200 1400 2400
Wire Wire Line
	1400 2400 2000 2400
Wire Wire Line
	1250 1800 1350 1800
Wire Wire Line
	1350 1800 1350 1650
Wire Wire Line
	1350 1650 2000 1650
Wire Wire Line
	1250 2000 1450 2000
Wire Wire Line
	1450 2000 1450 2150
Wire Wire Line
	1450 2150 2000 2150
$Comp
L GND #PWR013
U 1 1 583EA06E
P 1350 2500
F 0 "#PWR013" H 1350 2300 50  0001 C CNN
F 1 "GND" H 1350 2390 50  0001 C CNN
F 2 "" H 1350 2500 60  0001 C CNN
F 3 "" H 1350 2500 60  0001 C CNN
	1    1350 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 583EA15E
P 1350 1300
F 0 "#PWR014" H 1350 1100 50  0001 C CNN
F 1 "GND" H 1350 1190 50  0001 C CNN
F 2 "" H 1350 1300 60  0001 C CNN
F 3 "" H 1350 1300 60  0001 C CNN
	1    1350 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3250 2000 3250
Wire Wire Line
	1250 3350 2000 3350
Wire Wire Line
	2000 3450 1250 3450
Wire Wire Line
	1250 3650 2000 3650
Wire Wire Line
	2000 3750 1250 3750
Wire Wire Line
	1250 3850 2000 3850
Wire Wire Line
	2000 6400 1250 6400
Wire Wire Line
	2000 6600 1250 6600
Wire Wire Line
	2000 6800 1250 6800
Wire Wire Line
	2000 7000 1250 7000
Wire Wire Line
	1250 6500 1350 6500
Wire Wire Line
	1350 6500 1350 7300
Wire Wire Line
	1250 7100 1350 7100
Connection ~ 1350 7100
Wire Wire Line
	1250 6900 1350 6900
Connection ~ 1350 6900
Wire Wire Line
	1250 6700 1350 6700
Connection ~ 1350 6700
$Comp
L GND #PWR015
U 1 1 583EB828
P 1350 7300
F 0 "#PWR015" H 1350 7100 50  0001 C CNN
F 1 "GND" H 1350 7190 50  0001 C CNN
F 2 "" H 1350 7300 60  0001 C CNN
F 3 "" H 1350 7300 60  0001 C CNN
	1    1350 7300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 583EB918
P 3150 7300
F 0 "#PWR016" H 3150 7100 50  0001 C CNN
F 1 "GND" H 3150 7190 50  0001 C CNN
F 2 "" H 3150 7300 60  0001 C CNN
F 3 "" H 3150 7300 60  0001 C CNN
	1    3150 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3250 3050 3250
Wire Wire Line
	3050 3250 3050 3050
$Comp
L +24V #PWR017
U 1 1 583EBFB0
P 3050 3050
F 0 "#PWR017" H 3050 3250 50  0001 C CNN
F 1 "+24V" H 2900 3100 50  0000 C CNN
F 2 "" H 3050 3050 60  0001 C CNN
F 3 "" H 3050 3050 60  0001 C CNN
	1    3050 3050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR018
U 1 1 583EC0A0
P 3450 3250
F 0 "#PWR018" H 3450 3450 50  0001 C CNN
F 1 "+5V" H 3550 3300 50  0000 C CNN
F 2 "" H 3450 3250 60  0001 C CNN
F 3 "" H 3450 3250 60  0001 C CNN
	1    3450 3250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR019
U 1 1 583EC93C
P 3150 6300
F 0 "#PWR019" H 3150 6500 50  0001 C CNN
F 1 "+5V" H 3146 6444 50  0000 C CNN
F 2 "" H 3150 6300 60  0001 C CNN
F 3 "" H 3150 6300 60  0001 C CNN
	1    3150 6300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 583ECFC2
P 3150 3950
F 0 "#PWR020" H 3150 3750 50  0001 C CNN
F 1 "GND" H 3150 3840 50  0001 C CNN
F 2 "" H 3150 3950 60  0001 C CNN
F 3 "" H 3150 3950 60  0001 C CNN
	1    3150 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3850 3150 3850
Wire Wire Line
	3150 3850 3150 3950
Wire Wire Line
	3150 6300 3150 6400
Wire Wire Line
	3150 6400 2800 6400
Wire Wire Line
	2800 7000 3150 7000
Wire Wire Line
	3150 7000 3150 7300
$Comp
L JUMPER J13
U 1 1 583ED84B
P 3200 3350
F 0 "J13" H 3200 3525 50  0000 C CNN
F 1 "Power LEDs" H 3200 3453 25  0000 C CNN
F 2 "Connector_Header:HEADER_2x1" H 3200 3350 60  0001 C CNN
F 3 "" H 3200 3350 60  0000 C CNN
	1    3200 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 3250 3450 3350
Wire Wire Line
	3450 3350 3350 3350
Wire Wire Line
	3050 3350 2800 3350
Text Label 1550 1650 0    40   ~ 0
BUS_24V
Text Label 1500 1100 0    40   ~ 0
+24V_IN
Text Label 1550 3250 0    40   ~ 0
REL1_COM
Text Label 1550 3450 0    40   ~ 0
REL1_NC
Text Label 1550 3350 0    40   ~ 0
REL1_NO
Text Label 1550 3650 0    40   ~ 0
REL2_COM
Text Label 1550 3750 0    40   ~ 0
REL2_NO
Text Label 1550 3850 0    40   ~ 0
REL2_NC
Text Label 1550 6400 0    40   ~ 0
IN1
Text Label 1550 6600 0    40   ~ 0
IN2
Text Label 1550 6800 0    40   ~ 0
IN3
Text Label 1550 7000 0    40   ~ 0
IN4
Entry Wire Line
	9100 3650 9200 3750
Entry Wire Line
	9100 3650 9200 3750
Wire Wire Line
	8700 3650 9100 3650
Text Label 9100 3650 2    40   ~ 0
PI_CTS
$Comp
L JUMPER3 J14
U 1 1 583F478F
P 3650 6200
F 0 "J14" V 3550 6200 50  0000 R CNN
F 1 "JUMPER3" V 3950 6200 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x1" H 3700 6175 60  0001 C CNN
F 3 "" H 3700 6175 60  0000 C CNN
	1    3650 6200
	0    -1   1    0   
$EndComp
Entry Wire Line
	3850 7400 3950 7500
Entry Wire Line
	3250 6000 3350 6100
Wire Wire Line
	3350 6400 3550 6400
Wire Wire Line
	3550 6200 3350 6200
Wire Wire Line
	2800 6550 3250 6550
Wire Wire Line
	3250 6550 3250 6300
Text Label 3350 6400 0    40   ~ 0
PI_IN1
Text Label 2850 6550 0    40   ~ 0
IN1_PROT
Text Label 2850 6650 0    40   ~ 0
IN2_PROT
Text Label 2850 6750 0    40   ~ 0
IN3_PROT
Text Label 2850 6850 0    40   ~ 0
IN4_PROT
Text Label 3350 6200 0    40   ~ 0
BM_IN1
Wire Wire Line
	3250 6300 3550 6300
$Comp
L R R3
U 1 1 583FB9B2
P 3550 7200
F 0 "R3" H 3550 7300 50  0000 C CNN
F 1 "100k" H 3550 7200 50  0000 C CNN
F 2 "Resistor:R_0805" H 3550 7200 60  0001 C CNN
F 3 "" H 3550 7200 60  0000 C CNN
	1    3550 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 7200 3350 7200
Wire Wire Line
	3350 7000 3850 7000
Wire Wire Line
	3850 7000 3850 7400
Wire Wire Line
	3850 7200 3750 7200
Connection ~ 3850 7200
$Comp
L JUMPER3 J15
U 1 1 583FD586
P 4400 6200
F 0 "J15" V 4300 6200 50  0000 R CNN
F 1 "JUMPER3" V 4700 6200 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x1" H 4450 6175 60  0001 C CNN
F 3 "" H 4450 6175 60  0000 C CNN
	1    4400 6200
	0    -1   1    0   
$EndComp
Entry Wire Line
	4600 7400 4700 7500
Entry Wire Line
	4000 6000 4100 6100
Wire Wire Line
	4100 6400 4300 6400
Wire Wire Line
	4300 6200 4100 6200
Wire Wire Line
	4000 6300 4000 6650
Text Label 4100 6400 0    40   ~ 0
PI_IN2
Text Label 4100 6200 0    40   ~ 0
BM_IN2
Wire Wire Line
	4000 6300 4300 6300
$Comp
L R R4
U 1 1 583FD59C
P 4300 7200
F 0 "R4" H 4300 7300 50  0000 C CNN
F 1 "100k" H 4300 7200 50  0000 C CNN
F 2 "Resistor:R_0805" H 4300 7200 60  0001 C CNN
F 3 "" H 4300 7200 60  0000 C CNN
	1    4300 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 7000 4600 7000
Wire Wire Line
	4600 7000 4600 7400
Wire Wire Line
	4600 7200 4500 7200
Connection ~ 4600 7200
Wire Wire Line
	4100 7200 4000 7200
$Comp
L GND #PWR021
U 1 1 583FEE04
P 4000 7300
F 0 "#PWR021" H 4000 7100 50  0001 C CNN
F 1 "GND" H 4000 7190 50  0001 C CNN
F 2 "" H 4000 7300 60  0001 C CNN
F 3 "" H 4000 7300 60  0001 C CNN
	1    4000 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 6650 2800 6650
$Comp
L JUMPER3 J16
U 1 1 583FF80A
P 5250 6200
F 0 "J16" V 5150 6200 50  0000 R CNN
F 1 "JUMPER3" V 5550 6200 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x1" H 5300 6175 60  0001 C CNN
F 3 "" H 5300 6175 60  0000 C CNN
	1    5250 6200
	0    -1   1    0   
$EndComp
Entry Wire Line
	5450 7400 5550 7500
Entry Wire Line
	4850 6000 4950 6100
Wire Wire Line
	4950 6400 5150 6400
Wire Wire Line
	5150 6200 4950 6200
Wire Wire Line
	4850 6300 4850 6750
Text Label 4950 6400 0    40   ~ 0
PI_IN3
Text Label 4950 6200 0    40   ~ 0
BM_IN3
Wire Wire Line
	4850 6300 5150 6300
$Comp
L R R5
U 1 1 583FF820
P 5150 7200
F 0 "R5" H 5150 7300 50  0000 C CNN
F 1 "100k" H 5150 7200 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 7200 60  0001 C CNN
F 3 "" H 5150 7200 60  0000 C CNN
	1    5150 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 7000 5450 7000
Wire Wire Line
	5450 7000 5450 7400
Wire Wire Line
	5450 7200 5350 7200
Connection ~ 5450 7200
$Comp
L JUMPER3 J17
U 1 1 583FF82B
P 6000 6200
F 0 "J17" V 5900 6200 50  0000 R CNN
F 1 "JUMPER3" V 6300 6200 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x1" H 6050 6175 60  0001 C CNN
F 3 "" H 6050 6175 60  0000 C CNN
	1    6000 6200
	0    -1   1    0   
$EndComp
Entry Wire Line
	6200 7400 6300 7500
Entry Wire Line
	5600 6000 5700 6100
Wire Wire Line
	5700 6400 5900 6400
Wire Wire Line
	5900 6200 5700 6200
Wire Wire Line
	5600 6300 5600 6850
Text Label 5700 6400 0    40   ~ 0
PI_IN4
Text Label 5700 6200 0    40   ~ 0
BM_IN4
Wire Wire Line
	5600 6300 5900 6300
$Comp
L R R8
U 1 1 583FF840
P 5900 7200
F 0 "R8" H 5900 7300 50  0000 C CNN
F 1 "100k" H 5900 7200 50  0000 C CNN
F 2 "Resistor:R_0805" H 5900 7200 60  0001 C CNN
F 3 "" H 5900 7200 60  0000 C CNN
	1    5900 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 7000 6200 7000
Wire Wire Line
	6200 7000 6200 7400
Wire Wire Line
	6200 7200 6100 7200
Connection ~ 6200 7200
Wire Wire Line
	5700 7200 5600 7200
$Comp
L GND #PWR022
U 1 1 583FF84C
P 5600 7300
F 0 "#PWR022" H 5600 7100 50  0001 C CNN
F 1 "GND" H 5600 7190 50  0001 C CNN
F 2 "" H 5600 7300 60  0001 C CNN
F 3 "" H 5600 7300 60  0001 C CNN
	1    5600 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 6750 2800 6750
Wire Wire Line
	5600 6850 2800 6850
Connection ~ 3150 7200
Wire Wire Line
	5600 7200 5600 7300
Wire Wire Line
	4950 7200 4850 7200
Wire Wire Line
	4850 7200 4850 7300
Wire Wire Line
	4000 7200 4000 7300
Wire Bus Line
	3950 7500 6650 7500
Wire Wire Line
	5700 6200 5700 6100
Wire Wire Line
	4950 6200 4950 6100
Wire Wire Line
	3350 6200 3350 6100
Wire Wire Line
	4100 6200 4100 6100
$Comp
L JUMPER3 J11
U 1 1 58408CCD
P 2750 4350
F 0 "J11" V 2650 4350 50  0000 R CNN
F 1 "JUMPER3" V 3050 4350 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x1" H 2800 4325 60  0001 C CNN
F 3 "" H 2800 4325 60  0000 C CNN
	1    2750 4350
	0    1    -1   0   
$EndComp
Text Label 3150 4150 2    40   ~ 0
BM_REL1
$Comp
L JUMPER3 J12
U 1 1 58408CDB
P 2750 4750
F 0 "J12" V 2650 4750 50  0000 R CNN
F 1 "JUMPER3" V 3050 4750 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x1" H 2800 4725 60  0001 C CNN
F 3 "" H 2800 4725 60  0000 C CNN
	1    2750 4750
	0    1    -1   0   
$EndComp
Text Label 3150 4550 2    40   ~ 0
BM_REL2
Wire Wire Line
	2800 3500 3300 3500
Wire Wire Line
	3300 3500 3300 4250
Wire Wire Line
	3300 4250 2850 4250
Wire Wire Line
	2850 4650 3350 4650
Wire Wire Line
	3350 4650 3350 3650
Wire Wire Line
	3350 3650 2800 3650
NoConn ~ 4200 3300
NoConn ~ 4200 3400
NoConn ~ 5300 3400
NoConn ~ 5300 3300
Text Label 3900 2150 0    40   ~ 0
BM_IN1
Text Label 3900 2250 0    40   ~ 0
BM_IN2
Text Label 3900 2350 0    40   ~ 0
BM_IN3
Text Label 3900 2450 0    40   ~ 0
BM_IN4
Text Label 3900 3100 0    40   ~ 0
BM_REL1
Text Label 3900 3200 0    40   ~ 0
BM_REL2
Entry Wire Line
	3800 2250 3900 2150
Entry Wire Line
	3800 2350 3900 2250
Entry Wire Line
	3800 2450 3900 2350
Entry Wire Line
	3800 2550 3900 2450
Entry Wire Line
	3800 3200 3900 3100
Entry Wire Line
	3800 3300 3900 3200
Wire Bus Line
	3800 2250 3800 6000
Wire Wire Line
	3900 2150 4200 2150
Wire Wire Line
	4200 2250 3900 2250
Wire Wire Line
	4200 2350 3900 2350
Wire Wire Line
	4200 2450 3900 2450
Wire Wire Line
	4200 3100 3900 3100
Wire Wire Line
	4200 3200 3900 3200
Wire Wire Line
	2850 4150 3700 4150
Wire Wire Line
	2850 4550 3700 4550
Entry Wire Line
	3700 4150 3800 4250
Entry Wire Line
	3700 4550 3800 4650
Wire Wire Line
	3350 7000 3350 6400
Wire Wire Line
	4100 7000 4100 6400
Wire Wire Line
	4950 7000 4950 6400
Wire Wire Line
	5700 7000 5700 6400
$Comp
L GND #PWR023
U 1 1 58417FEB
P 4850 7300
F 0 "#PWR023" H 4850 7100 50  0001 C CNN
F 1 "GND" H 4850 7190 50  0001 C CNN
F 2 "" H 4850 7300 60  0001 C CNN
F 3 "" H 4850 7300 60  0001 C CNN
	1    4850 7300
	1    0    0    -1  
$EndComp
Text Notes 3400 7750 0    40   ~ 0
Voltage divider for raspberry PI inputs:\n56k pullup from input protection in series \nwith 100k.
Wire Bus Line
	3250 6000 5600 6000
Wire Wire Line
	2850 4350 3900 4350
Wire Wire Line
	2850 4750 3900 4750
Entry Wire Line
	3900 4350 4000 4450
Entry Wire Line
	3900 4750 4000 4850
Wire Bus Line
	4000 4450 4000 5800
Wire Bus Line
	4000 5800 6650 5800
Text Label 2900 4350 0    40   ~ 0
PI_REL1
Text Label 2900 4750 0    40   ~ 0
PI_REL2
NoConn ~ 5300 2150
NoConn ~ 5300 2250
NoConn ~ 5300 2350
NoConn ~ 5300 2450
NoConn ~ 5300 2550
NoConn ~ 5300 2650
NoConn ~ 5300 2750
NoConn ~ 5300 2850
NoConn ~ 4200 2850
NoConn ~ 4200 2750
NoConn ~ 4200 2650
NoConn ~ 4200 2550
Entry Wire Line
	7000 2750 7100 2650
Entry Wire Line
	9100 2650 9200 2750
Entry Wire Line
	9100 2750 9200 2850
Entry Wire Line
	9100 2950 9200 3050
Entry Wire Line
	9100 3750 9200 3850
Entry Wire Line
	9100 3850 9200 3950
Wire Wire Line
	8700 3750 9100 3750
Wire Wire Line
	8700 3850 9100 3850
Text Label 9100 3750 2    40   ~ 0
PI_REL1
Text Label 9100 3850 2    40   ~ 0
PI_REL2
Wire Wire Line
	8700 2650 9100 2650
Wire Wire Line
	9100 2750 8700 2750
Wire Wire Line
	9100 2950 8700 2950
Wire Wire Line
	7500 2650 7100 2650
Text Label 7100 2650 0    40   ~ 0
PI_IN1
Text Label 9100 2650 2    40   ~ 0
PI_IN2
Text Label 9100 2750 2    40   ~ 0
PI_IN3
Text Label 9100 2950 2    40   ~ 0
PI_IN4
NoConn ~ 7500 2250
NoConn ~ 7500 2450
NoConn ~ 7500 2550
NoConn ~ 7500 2850
NoConn ~ 7500 2950
NoConn ~ 7500 3050
NoConn ~ 7500 3350
NoConn ~ 7500 3450
NoConn ~ 7500 3550
NoConn ~ 7500 3650
NoConn ~ 7500 3750
NoConn ~ 8700 3150
NoConn ~ 8700 3050
NoConn ~ 8700 3450
NoConn ~ 8700 2450
Wire Bus Line
	7000 2750 7000 4350
NoConn ~ 5300 3500
NoConn ~ 5300 3600
NoConn ~ 5300 3700
$Comp
L HOLE H1
U 1 1 58416DF0
P 8150 5600
F 0 "H1" H 8278 5600 50  0000 L CNN
F 1 "HOLE" H 8450 5600 50  0001 L CNN
F 2 "Mechanical:HOLE_3.5mm" H 8150 5600 60  0001 C CNN
F 3 "" H 8150 5600 60  0000 C CNN
	1    8150 5600
	1    0    0    -1  
$EndComp
$Comp
L HOLE H2
U 1 1 5841770C
P 8150 5800
F 0 "H2" H 8278 5800 50  0000 L CNN
F 1 "HOLE" H 8450 5800 50  0001 L CNN
F 2 "Mechanical:HOLE_3.5mm" H 8150 5800 60  0001 C CNN
F 3 "" H 8150 5800 60  0000 C CNN
	1    8150 5800
	1    0    0    -1  
$EndComp
$Comp
L HOLE H3
U 1 1 584179EA
P 8500 5600
F 0 "H3" H 8628 5600 50  0000 L CNN
F 1 "HOLE" H 8800 5600 50  0001 L CNN
F 2 "Mechanical:HOLE_3.5mm" H 8500 5600 60  0001 C CNN
F 3 "" H 8500 5600 60  0000 C CNN
	1    8500 5600
	1    0    0    -1  
$EndComp
$Comp
L HOLE H4
U 1 1 584179F0
P 8500 5800
F 0 "H4" H 8628 5800 50  0000 L CNN
F 1 "HOLE" H 8800 5800 50  0001 L CNN
F 2 "Mechanical:HOLE_3.5mm" H 8500 5800 60  0001 C CNN
F 3 "" H 8500 5800 60  0000 C CNN
	1    8500 5800
	1    0    0    -1  
$EndComp
$Comp
L OPEN_HARDWARE_1 LOGO1
U 1 1 5846B9BF
P 10750 6200
F 0 "LOGO1" H 10750 6550 60  0001 C CNN
F 1 "OPEN HARDWARE" H 10400 5950 60  0000 L CNN
F 2 "Symbol:Symbol_OSHW-Logo_Copper" H 10750 6200 60  0001 C CNN
F 3 "" H 10750 6200 60  0001 C CNN
	1    10750 6200
	1    0    0    -1  
$EndComp
$EndSCHEMATC
