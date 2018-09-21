EESchema Schematic File Version 4
LIBS:bus-splitter-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "bus-splitter"
Date "2018-09-21"
Rev "1"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L bus-splitter-rescue:ATTINY1634-SOIC U4
U 1 1 5B1FC4AC
P 6000 3800
F 0 "U4" H 6000 3800 50  0000 C CNN
F 1 "ATTINY1634-SOIC" H 6000 3200 50  0000 C CNN
F 2 "Housings_SOIC:SO20L" H 6000 3800 25  0001 C CNN
F 3 "" H 6000 3800 25  0001 C CNN
	1    6000 3800
	-1   0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:CRYSTAL U3
U 1 1 5B1FC51B
P 5650 5050
F 0 "U3" H 5825 5150 50  0000 C CNN
F 1 "7.3728MHz" H 5825 4950 50  0000 C CNN
F 2 "Crystal:CRY_HC49_SMD" H 5650 5050 60  0001 C CNN
F 3 "" H 5650 5050 60  0000 C CNN
	1    5650 5050
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:C C5
U 1 1 5B1FC574
P 6200 2650
F 0 "C5" H 6250 2700 50  0000 L CNN
F 1 "100nF" H 6200 2525 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6200 2650 60  0001 C CNN
F 3 "" H 6200 2650 60  0000 C CNN
	1    6200 2650
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:C C4
U 1 1 5B1FC5E5
P 5750 5250
F 0 "C4" H 5800 5300 50  0000 L CNN
F 1 "18pF" H 5750 5125 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5750 5250 60  0001 C CNN
F 3 "" H 5750 5250 60  0000 C CNN
	1    5750 5250
	0    1    1    0   
$EndComp
$Comp
L bus-splitter-rescue:C C3
U 1 1 5B1FC602
P 5200 5250
F 0 "C3" H 5250 5300 50  0000 L CNN
F 1 "18pF" H 5200 5125 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5200 5250 60  0001 C CNN
F 3 "" H 5200 5250 60  0000 C CNN
	1    5200 5250
	0    1    1    0   
$EndComp
$Comp
L bus-splitter-rescue:CONN-ISP-6 J5
U 1 1 5B1FC62B
P 8150 5150
F 0 "J5" H 8000 5400 50  0000 C CNN
F 1 "CONN-ISP-6" H 8000 4925 25  0000 L CNN
F 2 "Connector_Header:HEADER_3x2" H 8000 4900 25  0001 L CNN
F 3 "" H 8150 5150 60  0000 C CNN
	1    8150 5150
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J1
U 1 1 5B1FC7EC
P 1250 2850
F 0 "J1" H 1350 2750 50  0000 C CNN
F 1 "WAGO 243-723" H 1250 2950 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 1250 2850 60  0001 C CNN
F 3 "" H 1250 2850 60  0000 C CNN
	1    1250 2850
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J2
U 1 1 5B1FC81B
P 1250 3150
F 0 "J2" H 1350 3050 50  0000 C CNN
F 1 "WAGO 243-724" H 1250 3250 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 1250 3150 60  0001 C CNN
F 3 "" H 1250 3150 60  0000 C CNN
	1    1250 3150
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J3
U 1 1 5B1FC852
P 1250 3450
F 0 "J3" H 1350 3350 50  0000 C CNN
F 1 "WAGO 243-721" H 1250 3550 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 1250 3450 60  0001 C CNN
F 3 "" H 1250 3450 60  0000 C CNN
	1    1250 3450
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J4
U 1 1 5B1FC8B1
P 1250 3750
F 0 "J4" H 1350 3650 50  0000 C CNN
F 1 "WAGO 243-722" H 1250 3850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 1250 3750 60  0001 C CNN
F 3 "" H 1250 3750 60  0000 C CNN
	1    1250 3750
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:FUSE F1
U 1 1 5B1FC963
P 1750 2850
F 0 "F1" H 1750 2950 50  0000 C CNN
F 1 "0,15A" H 1750 2750 50  0000 C CNN
F 2 "Resistor:R_1206" H 1750 2850 60  0001 C CNN
F 3 "" H 1750 2850 60  0000 C CNN
	1    1750 2850
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:MAX487 U2
U 1 1 5B1FCAC1
P 3550 3300
F 0 "U2" H 3300 3700 50  0000 C CNN
F 1 "MAX487" H 3750 2900 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 3400 2900 25  0001 C CNN
F 3 "" H 3550 3300 60  0000 C CNN
	1    3550 3300
	-1   0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:MAX487 U5
U 1 1 5B1FCB38
P 8850 3300
F 0 "U5" H 8600 3700 50  0000 C CNN
F 1 "MAX487" H 9050 2900 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 8700 2900 25  0001 C CNN
F 3 "" H 8850 3300 60  0000 C CNN
	1    8850 3300
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:C C2
U 1 1 5B1FCBA9
P 3750 2650
F 0 "C2" H 3800 2700 50  0000 L CNN
F 1 "100nF" H 3750 2525 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 3750 2650 60  0001 C CNN
F 3 "" H 3750 2650 60  0000 C CNN
	1    3750 2650
	-1   0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:R R2
U 1 1 5B1FD0F2
P 6900 4750
F 0 "R2" H 6900 4650 50  0000 C CNN
F 1 "10k" H 6900 4750 50  0000 C CNN
F 2 "Resistor:R_0805" H 6900 4750 60  0001 C CNN
F 3 "" H 6900 4750 60  0000 C CNN
	1    6900 4750
	0    1    1    0   
$EndComp
$Comp
L bus-splitter-rescue:C C6
U 1 1 5B1FD1F5
P 6900 5250
F 0 "C6" H 6950 5300 50  0000 L CNN
F 1 "4,7nF" H 6900 5125 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6900 5250 60  0001 C CNN
F 3 "" H 6900 5250 60  0000 C CNN
	1    6900 5250
	0    1    1    0   
$EndComp
$Comp
L bus-splitter-rescue:D D2
U 1 1 5B1FD25A
P 6700 4750
F 0 "D2" H 6710 4830 50  0000 C CNN
F 1 "1N4148" H 6700 4675 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 6700 4750 60  0001 C CNN
F 3 "" H 6700 4750 60  0000 C CNN
	1    6700 4750
	0    -1   -1   0   
$EndComp
$Comp
L bus-splitter-rescue:SEMTECH_SR05 D1
U 1 1 5B1FD34A
P 2600 4050
F 0 "D1" H 2675 3800 50  0000 C CNN
F 1 "SEMTECH_SR05" H 2600 4425 50  0000 C CNN
F 2 "Diode_SMD:D_SOT143" H 2600 4050 60  0001 C CNN
F 3 "" H 2600 4050 60  0000 C CNN
	1    2600 4050
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:SEMTECH_SR05 D3
U 1 1 5B1FD3F5
P 9800 4050
F 0 "D3" H 9875 3800 50  0000 C CNN
F 1 "SEMTECH_SR05" H 9800 4400 50  0000 C CNN
F 2 "Diode_SMD:D_SOT143" H 9800 4050 60  0001 C CNN
F 3 "" H 9800 4050 60  0000 C CNN
	1    9800 4050
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:Varistor R1
U 1 1 5B1FD4A0
P 2050 4100
F 0 "R1" H 1895 4010 50  0000 L CNN
F 1 "Varistor" H 2050 4100 50  0000 C CNN
F 2 "Resistor:R_RM10.16mm" H 2050 4100 60  0001 C CNN
F 3 "" H 2050 4100 60  0000 C CNN
	1    2050 4100
	0    1    1    0   
$EndComp
$Comp
L bus-splitter-rescue:R R4
U 1 1 5B1FD567
P 8050 3450
F 0 "R4" H 8050 3350 50  0000 C CNN
F 1 "100" H 8050 3450 50  0000 C CNN
F 2 "Resistor:R_0805" H 8050 3450 60  0001 C CNN
F 3 "" H 8050 3450 60  0000 C CNN
	1    8050 3450
	1    0    0    -1  
$EndComp
Text Notes 800  2900 0    60   ~ 0
+24V
Text Notes 850  3200 0    60   ~ 0
A
Text Notes 850  3500 0    60   ~ 0
B
Text Notes 850  3800 0    60   ~ 0
GND
$Comp
L bus-splitter-rescue:GND #PWR01
U 1 1 5B1FECB7
P 2050 4550
F 0 "#PWR01" H 2050 4350 50  0001 C CNN
F 1 "GND" H 2050 4440 50  0001 C CNN
F 2 "" H 2050 4550 60  0001 C CNN
F 3 "" H 2050 4550 60  0001 C CNN
	1    2050 4550
	1    0    0    -1  
$EndComp
$Comp
L power1:+24V #PWR02
U 1 1 5B1FED11
P 2050 2750
F 0 "#PWR02" H 2050 2950 50  0001 C CNN
F 1 "+24V" H 2050 2900 50  0000 C CNN
F 2 "" H 2050 2750 60  0001 C CNN
F 3 "" H 2050 2750 60  0001 C CNN
	1    2050 2750
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR03
U 1 1 5B1FF42F
P 3550 3950
F 0 "#PWR03" H 3550 3750 50  0001 C CNN
F 1 "GND" H 3550 3840 50  0001 C CNN
F 2 "" H 3550 3950 60  0001 C CNN
F 3 "" H 3550 3950 60  0001 C CNN
	1    3550 3950
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR04
U 1 1 5B1FF487
P 3950 2750
F 0 "#PWR04" H 3950 2550 50  0001 C CNN
F 1 "GND" H 3950 2640 50  0001 C CNN
F 2 "" H 3950 2750 60  0001 C CNN
F 3 "" H 3950 2750 60  0001 C CNN
	1    3950 2750
	1    0    0    -1  
$EndComp
$Comp
L power1:VCC #PWR05
U 1 1 5B1FF4DF
P 3550 2550
F 0 "#PWR05" H 3550 2725 50  0001 C CNN
F 1 "VCC" H 3550 2700 50  0000 C CNN
F 2 "" H 3550 2550 60  0001 C CNN
F 3 "" H 3550 2550 60  0001 C CNN
	1    3550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 2750 2050 2850
Wire Wire Line
	1450 2850 1500 2850
Connection ~ 2050 2850
Wire Wire Line
	1450 3750 1550 3750
Wire Wire Line
	1550 3750 1550 4450
Wire Wire Line
	2050 4350 2050 4450
Connection ~ 2050 4450
Wire Wire Line
	1550 4450 2050 4450
Wire Wire Line
	2600 4450 2600 4350
Wire Wire Line
	1450 3150 2250 3150
Wire Wire Line
	1450 3450 2950 3450
Wire Wire Line
	2350 4050 2250 4050
Wire Wire Line
	2250 4050 2250 3150
Connection ~ 2250 3150
Wire Wire Line
	2850 4050 2950 4050
Wire Wire Line
	2950 4050 2950 3450
Connection ~ 2950 3450
Wire Wire Line
	3550 2750 3550 2650
Wire Wire Line
	3650 2650 3550 2650
Connection ~ 3550 2650
Wire Wire Line
	3850 2650 3950 2650
Wire Wire Line
	3950 2650 3950 2750
Wire Wire Line
	3550 3850 3550 3950
$Comp
L power1:+24V #PWR06
U 1 1 5B1FF8D2
P 2050 4850
F 0 "#PWR06" H 2050 5050 50  0001 C CNN
F 1 "+24V" H 2050 5000 50  0000 C CNN
F 2 "" H 2050 4850 60  0001 C CNN
F 3 "" H 2050 4850 60  0001 C CNN
	1    2050 4850
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR07
U 1 1 5B1FF92A
P 2050 5450
F 0 "#PWR07" H 2050 5250 50  0001 C CNN
F 1 "GND" H 2050 5340 50  0001 C CNN
F 2 "" H 2050 5450 60  0001 C CNN
F 3 "" H 2050 5450 60  0001 C CNN
	1    2050 5450
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:C C1
U 1 1 5B1FD7C3
P 3350 5150
F 0 "C1" H 3400 5200 50  0000 L CNN
F 1 "100µF/6V" H 3350 5300 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 3350 5150 60  0001 C CNN
F 3 "" H 3350 5150 60  0000 C CNN
	1    3350 5150
	0    1    1    0   
$EndComp
$Comp
L bus-splitter-rescue:AM1S-2405 U1
U 1 1 5B1FC8F2
P 2650 5150
F 0 "U1" H 2650 5150 50  0000 C CNN
F 1 "AM1S-2405" H 2650 4950 50  0000 C CNN
F 2 "DCDC_Converter:AM1S-XXXX_SIL4" H 2650 5350 25  0001 C CNN
F 3 "" H 1900 5150 25  0001 C CNN
	1    2650 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 5050 2150 5050
Wire Wire Line
	2050 5250 2150 5250
Wire Wire Line
	1950 2850 2050 2850
NoConn ~ 2600 3750
NoConn ~ 1800 -2100
Wire Wire Line
	3150 5050 3250 5050
Wire Wire Line
	3250 5050 3250 4950
Wire Wire Line
	3250 4950 3350 4950
Wire Wire Line
	3350 4850 3350 4950
Wire Wire Line
	3150 5250 3250 5250
Wire Wire Line
	3250 5250 3250 5350
Wire Wire Line
	3250 5350 3350 5350
Wire Wire Line
	3350 5250 3350 5350
Connection ~ 3350 5350
$Comp
L bus-splitter-rescue:GND #PWR08
U 1 1 5B20E396
P 3350 5450
F 0 "#PWR08" H 3350 5250 50  0001 C CNN
F 1 "GND" H 3350 5340 50  0001 C CNN
F 2 "" H 3350 5450 60  0001 C CNN
F 3 "" H 3350 5450 60  0001 C CNN
	1    3350 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 5250 2050 5450
Connection ~ 3350 4950
$Comp
L power1:VCC #PWR09
U 1 1 5B20E5DF
P 3350 4850
F 0 "#PWR09" H 3350 5025 50  0001 C CNN
F 1 "VCC" H 3350 5000 50  0000 C CNN
F 2 "" H 3350 4850 60  0001 C CNN
F 3 "" H 3350 4850 60  0001 C CNN
	1    3350 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 4850 2050 5050
Text Label 4100 3150 0    40   ~ 0
TXD0
Text Label 4100 3450 0    40   ~ 0
RXD0
Text Label 4100 3250 0    40   ~ 0
TXDEN0
Entry Wire Line
	4350 3150 4450 3250
Entry Wire Line
	4350 3250 4450 3350
Entry Wire Line
	4350 3450 4450 3550
Wire Wire Line
	4050 3150 4100 3150
Wire Wire Line
	4050 3250 4150 3250
Wire Wire Line
	4050 3350 4150 3350
Wire Wire Line
	4150 3350 4150 3250
Connection ~ 4150 3250
Wire Wire Line
	4050 3450 4100 3450
Text Label 4950 3350 0    40   ~ 0
TXD0
Text Label 4950 3450 0    40   ~ 0
RXD1_MISO
Text Label 4950 3550 0    40   ~ 0
TXD1_MOSI
Text Label 4950 3650 0    40   ~ 0
TXDEN0
Text Label 4950 3750 0    40   ~ 0
TXDEN1
Entry Wire Line
	4800 3450 4900 3350
Entry Wire Line
	4800 3550 4900 3450
Entry Wire Line
	4800 3650 4900 3550
Entry Wire Line
	4800 3750 4900 3650
Entry Wire Line
	4800 3850 4900 3750
$Comp
L bus-splitter-rescue:R R3
U 1 1 5B20FAC2
P 8050 3150
F 0 "R3" H 8050 3250 50  0000 C CNN
F 1 "100" H 8050 3150 50  0000 C CNN
F 2 "Resistor:R_0805" H 8050 3150 60  0001 C CNN
F 3 "" H 8050 3150 60  0000 C CNN
	1    8050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3350 5300 3350
Wire Wire Line
	4900 3450 5300 3450
Wire Wire Line
	4900 3550 5300 3550
Wire Wire Line
	4900 3650 5300 3650
Wire Wire Line
	4900 3750 5300 3750
Wire Wire Line
	4900 3850 5300 3850
Text Label 4950 3850 0    40   ~ 0
SCK
Text Label 4950 4050 0    40   ~ 0
/RESET
Wire Wire Line
	5300 4050 4900 4050
NoConn ~ 5300 3950
NoConn ~ 6700 3450
NoConn ~ 6700 3550
NoConn ~ 6700 3650
NoConn ~ 6700 3750
NoConn ~ 6700 3850
NoConn ~ 6700 3950
NoConn ~ 6700 4050
$Comp
L power1:VCC #PWR010
U 1 1 5B210DC4
P 6000 2550
F 0 "#PWR010" H 6000 2725 50  0001 C CNN
F 1 "VCC" H 6000 2700 50  0000 C CNN
F 2 "" H 6000 2550 60  0001 C CNN
F 3 "" H 6000 2550 60  0001 C CNN
	1    6000 2550
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR011
U 1 1 5B210E18
P 6400 2750
F 0 "#PWR011" H 6400 2550 50  0001 C CNN
F 1 "GND" H 6400 2640 50  0001 C CNN
F 2 "" H 6400 2750 60  0001 C CNN
F 3 "" H 6400 2750 60  0001 C CNN
	1    6400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2650 6400 2650
Wire Wire Line
	6400 2650 6400 2750
Wire Wire Line
	6000 2550 6000 2650
Wire Wire Line
	6100 2650 6000 2650
Connection ~ 6000 2650
Entry Wire Line
	4800 3950 4900 3850
Entry Wire Line
	4800 4150 4900 4050
Wire Wire Line
	5650 5050 5750 5050
Wire Wire Line
	5750 4750 5750 5050
Wire Wire Line
	5300 5050 5200 5050
Wire Wire Line
	5200 4850 5200 5050
Wire Wire Line
	5300 4250 5200 4250
Wire Wire Line
	5200 4250 5200 4750
Wire Wire Line
	5200 4750 5750 4750
Connection ~ 5750 5050
Wire Wire Line
	5200 4850 5100 4850
Wire Wire Line
	5100 4850 5100 4150
Wire Wire Line
	5100 4150 5300 4150
Connection ~ 5200 5050
Wire Wire Line
	5200 5350 5200 5450
Wire Wire Line
	5750 5350 5750 5450
Wire Wire Line
	6000 4850 6000 5450
$Comp
L bus-splitter-rescue:GND #PWR012
U 1 1 5B212396
P 5200 5450
F 0 "#PWR012" H 5200 5250 50  0001 C CNN
F 1 "GND" H 5200 5340 50  0001 C CNN
F 2 "" H 5200 5450 60  0001 C CNN
F 3 "" H 5200 5450 60  0001 C CNN
	1    5200 5450
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR013
U 1 1 5B2123EA
P 5750 5450
F 0 "#PWR013" H 5750 5250 50  0001 C CNN
F 1 "GND" H 5750 5340 50  0001 C CNN
F 2 "" H 5750 5450 60  0001 C CNN
F 3 "" H 5750 5450 60  0001 C CNN
	1    5750 5450
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR014
U 1 1 5B21243E
P 6000 5450
F 0 "#PWR014" H 6000 5250 50  0001 C CNN
F 1 "GND" H 6000 5340 50  0001 C CNN
F 2 "" H 6000 5450 60  0001 C CNN
F 3 "" H 6000 5450 60  0001 C CNN
	1    6000 5450
	1    0    0    -1  
$EndComp
$Comp
L power1:VCC #PWR015
U 1 1 5B2128CB
P 8550 4950
F 0 "#PWR015" H 8550 5125 50  0001 C CNN
F 1 "VCC" H 8550 5100 50  0000 C CNN
F 2 "" H 8550 4950 60  0001 C CNN
F 3 "" H 8550 4950 60  0001 C CNN
	1    8550 4950
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR016
U 1 1 5B21291F
P 8550 5450
F 0 "#PWR016" H 8550 5250 50  0001 C CNN
F 1 "GND" H 8550 5340 50  0001 C CNN
F 2 "" H 8550 5450 60  0001 C CNN
F 3 "" H 8550 5450 60  0001 C CNN
	1    8550 5450
	1    0    0    -1  
$EndComp
Text Label 7450 5050 0    40   ~ 0
RXD1_MISO
Text Label 7450 5250 0    40   ~ 0
/RESET
Text Label 7450 5150 0    40   ~ 0
SCK
Text Label 8500 5150 0    40   ~ 0
TXD1_MOSI
Wire Wire Line
	8450 5050 8550 5050
Wire Wire Line
	8550 5050 8550 4950
Wire Wire Line
	8450 5250 8550 5250
Wire Wire Line
	7850 5150 7800 5150
Wire Wire Line
	8450 5150 8850 5150
Wire Wire Line
	7850 5250 7700 5250
Wire Wire Line
	7850 5050 7400 5050
$Comp
L bus-splitter-rescue:GND #PWR017
U 1 1 5B21341E
P 6900 5450
F 0 "#PWR017" H 6900 5250 50  0001 C CNN
F 1 "GND" H 6900 5340 50  0001 C CNN
F 2 "" H 6900 5450 60  0001 C CNN
F 3 "" H 6900 5450 60  0001 C CNN
	1    6900 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 5350 6900 5450
Wire Wire Line
	6700 4950 6700 5050
Wire Wire Line
	6700 5050 6900 5050
Wire Wire Line
	6900 4950 6900 5050
Connection ~ 6900 5050
Wire Wire Line
	6700 4550 6700 4450
Wire Wire Line
	6700 4450 6800 4450
Wire Wire Line
	6900 4450 6900 4550
Wire Wire Line
	6800 4450 6800 4350
Connection ~ 6800 4450
$Comp
L power1:VCC #PWR018
U 1 1 5B2137A5
P 6800 4350
F 0 "#PWR018" H 6800 4525 50  0001 C CNN
F 1 "VCC" H 6800 4500 50  0000 C CNN
F 2 "" H 6800 4350 60  0001 C CNN
F 3 "" H 6800 4350 60  0001 C CNN
	1    6800 4350
	1    0    0    -1  
$EndComp
Text Label 6750 4150 0    40   ~ 0
RXD0
Text Label 6950 5050 0    40   ~ 0
/RESET
Wire Wire Line
	7200 4150 6700 4150
Entry Wire Line
	7200 4150 7300 4250
Entry Wire Line
	7200 5050 7300 5150
Wire Bus Line
	4450 5700 8950 5700
Text Label 7450 3450 0    40   ~ 0
RXD1_MISO
Text Label 7450 3150 0    40   ~ 0
TXD1_MOSI
Wire Wire Line
	8350 3250 8250 3250
Wire Wire Line
	7850 3150 7800 3150
Wire Wire Line
	8350 3150 8250 3150
Wire Wire Line
	8350 3450 8250 3450
Wire Wire Line
	8350 3350 8250 3350
Wire Wire Line
	8250 3350 8250 3250
Connection ~ 8250 3250
Wire Wire Line
	7850 3450 7800 3450
Text Label 7450 3250 0    40   ~ 0
TXDEN1
Entry Wire Line
	7300 3250 7400 3150
Entry Wire Line
	7300 3350 7400 3250
Entry Wire Line
	7300 3550 7400 3450
$Comp
L bus-splitter-rescue:C C7
U 1 1 5B215009
P 9050 2650
F 0 "C7" H 9100 2700 50  0000 L CNN
F 1 "100nF" H 9050 2525 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 9050 2650 60  0001 C CNN
F 3 "" H 9050 2650 60  0000 C CNN
	1    9050 2650
	-1   0    0    1   
$EndComp
$Comp
L power1:VCC #PWR019
U 1 1 5B21500F
P 8850 2550
F 0 "#PWR019" H 8850 2725 50  0001 C CNN
F 1 "VCC" H 8850 2700 50  0000 C CNN
F 2 "" H 8850 2550 60  0001 C CNN
F 3 "" H 8850 2550 60  0001 C CNN
	1    8850 2550
	1    0    0    -1  
$EndComp
$Comp
L bus-splitter-rescue:GND #PWR020
U 1 1 5B215015
P 9250 2750
F 0 "#PWR020" H 9250 2550 50  0001 C CNN
F 1 "GND" H 9250 2640 50  0001 C CNN
F 2 "" H 9250 2750 60  0001 C CNN
F 3 "" H 9250 2750 60  0001 C CNN
	1    9250 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 2650 9250 2650
Wire Wire Line
	9250 2650 9250 2750
Wire Wire Line
	8850 2550 8850 2650
Wire Wire Line
	8950 2650 8850 2650
Connection ~ 8850 2650
$Comp
L bus-splitter-rescue:GND #PWR021
U 1 1 5B2152E0
P 8850 3950
F 0 "#PWR021" H 8850 3750 50  0001 C CNN
F 1 "GND" H 8850 3840 50  0001 C CNN
F 2 "" H 8850 3950 60  0001 C CNN
F 3 "" H 8850 3950 60  0001 C CNN
	1    8850 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 3850 8850 3950
$Comp
L bus-splitter-rescue:CONN_1X1 J6
U 1 1 5B215E52
P 10550 2850
F 0 "J6" H 10650 2750 50  0000 C CNN
F 1 "WAGO 243-723" H 10550 2950 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 10550 2850 60  0001 C CNN
F 3 "" H 10550 2850 60  0000 C CNN
	1    10550 2850
	1    0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J7
U 1 1 5B215E58
P 10550 3150
F 0 "J7" H 10650 3050 50  0000 C CNN
F 1 "WAGO 243-724" H 10550 3250 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 10550 3150 60  0001 C CNN
F 3 "" H 10550 3150 60  0000 C CNN
	1    10550 3150
	1    0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J8
U 1 1 5B215E5E
P 10550 3450
F 0 "J8" H 10650 3350 50  0000 C CNN
F 1 "WAGO 243-721" H 10550 3550 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 10550 3450 60  0001 C CNN
F 3 "" H 10550 3450 60  0000 C CNN
	1    10550 3450
	1    0    0    1   
$EndComp
$Comp
L bus-splitter-rescue:CONN_1X1 J9
U 1 1 5B215E64
P 10550 3750
F 0 "J9" H 10650 3650 50  0000 C CNN
F 1 "WAGO 243-722" H 10550 3850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO243" H 10550 3750 60  0001 C CNN
F 3 "" H 10550 3750 60  0000 C CNN
	1    10550 3750
	1    0    0    1   
$EndComp
Text Notes 11000 2900 2    60   ~ 0
+24V
Text Notes 10950 3200 2    60   ~ 0
A
Text Notes 10950 3500 2    60   ~ 0
B
Text Notes 10950 3800 2    60   ~ 0
GND
Wire Wire Line
	9550 4050 9450 4050
Wire Wire Line
	9450 4050 9450 3150
Connection ~ 9450 3150
Wire Wire Line
	10050 4050 10150 4050
Wire Wire Line
	10150 4050 10150 3450
Connection ~ 10150 3450
Wire Wire Line
	10350 3750 10250 3750
Wire Wire Line
	10250 3750 10250 4450
Wire Wire Line
	10250 4450 9800 4450
Wire Wire Line
	9800 4350 9800 4450
Wire Wire Line
	10350 3450 10150 3450
Wire Wire Line
	9350 3150 9450 3150
Wire Wire Line
	1500 2850 1500 2550
Connection ~ 1500 2850
Text Label 1550 2550 0    40   ~ 0
24V_UNFUSED
Wire Wire Line
	10250 2850 10250 2550
Text Label 10200 2550 2    40   ~ 0
24V_UNFUSED
Wire Wire Line
	10250 2850 10350 2850
Wire Wire Line
	10250 2550 9750 2550
Wire Wire Line
	1500 2550 2000 2550
Entry Wire Line
	8850 5150 8950 5250
Entry Wire Line
	7300 5350 7400 5250
Entry Wire Line
	7300 5250 7400 5150
Entry Wire Line
	7300 5150 7400 5050
Wire Wire Line
	8550 5250 8550 5450
Wire Bus Line
	8950 5700 8950 5250
Text Label 1550 3150 0    40   ~ 0
A0
Text Label 1550 3450 0    40   ~ 0
B0
Text Label 10300 3150 0    40   ~ 0
A1
Text Label 10300 3450 0    40   ~ 0
B1
$Comp
L bus-splitter-rescue:GND #PWR022
U 1 1 5B2108EB
P 9800 4550
F 0 "#PWR022" H 9800 4350 50  0001 C CNN
F 1 "GND" H 9800 4440 50  0001 C CNN
F 2 "" H 9800 4550 60  0001 C CNN
F 3 "" H 9800 4550 60  0001 C CNN
	1    9800 4550
	1    0    0    -1  
$EndComp
Connection ~ 9800 4450
$Comp
L bus-splitter-rescue:OPEN_HARDWARE_1 LOGO1
U 1 1 5B225B1C
P 1200 7250
F 0 "LOGO1" H 1200 7525 50  0000 C CNN
F 1 "OPEN_HARDWARE_1" H 1200 7025 25  0000 C CNN
F 2 "Symbol:Symbol_OSHW-Logo_Copper" H 1200 7250 60  0001 C CNN
F 3 "" H 1200 7250 60  0001 C CNN
	1    1200 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 2850 2050 3850
Wire Wire Line
	2050 4450 2050 4550
Wire Wire Line
	2050 4450 2600 4450
Wire Wire Line
	2250 3150 3050 3150
Wire Wire Line
	2950 3450 3050 3450
Wire Wire Line
	3550 2650 3550 2550
Wire Wire Line
	3350 5350 3350 5450
Wire Wire Line
	3350 4950 3350 5050
Wire Wire Line
	4150 3250 4350 3250
Wire Wire Line
	6000 2650 6000 2750
Wire Wire Line
	5750 5050 5750 5150
Wire Wire Line
	5200 5050 5200 5150
Wire Wire Line
	6900 5050 7200 5050
Wire Wire Line
	6900 5050 6900 5150
Wire Wire Line
	6800 4450 6900 4450
Wire Wire Line
	8250 3250 7400 3250
Wire Wire Line
	8850 2650 8850 2750
Wire Wire Line
	9450 3150 10350 3150
Wire Wire Line
	10150 3450 9350 3450
Wire Wire Line
	1500 2850 1550 2850
Wire Wire Line
	9800 4450 9800 4550
$Comp
L connectors:CONN_1 J14
U 1 1 5BA58448
P 4250 3350
F 0 "J14" H 4500 3375 50  0000 L CNN
F 1 "tp_txden0" H 4500 3325 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4250 3350 60  0001 C CNN
F 3 "" H 4250 3350 60  0000 C CNN
	1    4250 3350
	1    0    0    -1  
$EndComp
Connection ~ 4150 3350
$Comp
L connectors:CONN_1 J12
U 1 1 5BA585E7
P 4100 2950
F 0 "J12" V 4100 2900 50  0000 R CNN
F 1 "tp_txd0" V 4050 2900 25  0000 R CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4100 2950 60  0001 C CNN
F 3 "" H 4100 2950 60  0000 C CNN
	1    4100 2950
	0    -1   -1   0   
$EndComp
$Comp
L connectors:CONN_1 J13
U 1 1 5BA586F8
P 4100 3650
F 0 "J13" V 4100 3700 50  0000 L CNN
F 1 "tp_rxd0" V 4050 3700 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4100 3650 60  0001 C CNN
F 3 "" H 4100 3650 60  0000 C CNN
	1    4100 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 3050 4100 3150
Connection ~ 4100 3150
Wire Wire Line
	4100 3150 4350 3150
Wire Wire Line
	4100 3550 4100 3450
Connection ~ 4100 3450
Wire Wire Line
	4100 3450 4350 3450
$Comp
L connectors:CONN_1 J19
U 1 1 5BA612F5
P 8050 3350
F 0 "J19" H 8375 3350 50  0000 C CNN
F 1 "tp_txden1" H 8200 3350 25  0000 C CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 8050 3350 60  0001 C CNN
F 3 "" H 8050 3350 60  0000 C CNN
	1    8050 3350
	-1   0    0    1   
$EndComp
$Comp
L connectors:CONN_1 J16
U 1 1 5BA61712
P 7800 2950
F 0 "J16" V 7800 2900 50  0000 R CNN
F 1 "tp_txd1_mosi" V 7800 3250 25  0000 R CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 7800 2950 60  0001 C CNN
F 3 "" H 7800 2950 60  0000 C CNN
	1    7800 2950
	0    -1   -1   0   
$EndComp
$Comp
L connectors:CONN_1 J17
U 1 1 5BA6183D
P 7800 3650
F 0 "J17" V 7800 3700 50  0000 L CNN
F 1 "tp_rxd1_miso" V 7800 3350 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 7800 3650 60  0001 C CNN
F 3 "" H 7800 3650 60  0000 C CNN
	1    7800 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 3050 7800 3150
Connection ~ 7800 3150
Wire Wire Line
	7800 3150 7400 3150
Wire Wire Line
	7800 3550 7800 3450
Connection ~ 7800 3450
Wire Wire Line
	7800 3450 7400 3450
Wire Wire Line
	8150 3350 8250 3350
Connection ~ 8250 3350
$Comp
L connectors:CONN_1 J18
U 1 1 5BA767BD
P 7800 5500
F 0 "J18" V 7724 5560 50  0000 L CNN
F 1 "tp_sck" V 7796 5560 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 7800 5500 60  0001 C CNN
F 3 "" H 7800 5500 60  0000 C CNN
	1    7800 5500
	0    1    1    0   
$EndComp
$Comp
L connectors:CONN_1 J15
U 1 1 5BA76912
P 7700 5500
F 0 "J15" V 7625 5325 50  0000 L CNN
F 1 "tp_reset" V 7700 5300 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 7700 5500 60  0001 C CNN
F 3 "" H 7700 5500 60  0000 C CNN
	1    7700 5500
	0    1    1    0   
$EndComp
Wire Wire Line
	7700 5400 7700 5250
Connection ~ 7700 5250
Wire Wire Line
	7700 5250 7400 5250
Wire Wire Line
	7800 5400 7800 5150
Connection ~ 7800 5150
Wire Wire Line
	7800 5150 7400 5150
$Comp
L connectors:CONN_1 J10
U 1 1 5BA7F978
P 3550 4950
F 0 "J10" H 3591 4986 50  0000 L CNN
F 1 "tp_vcc" H 3591 4914 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 3550 4950 60  0001 C CNN
F 3 "" H 3550 4950 60  0000 C CNN
	1    3550 4950
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J11
U 1 1 5BA7FA49
P 3550 5350
F 0 "J11" H 3591 5386 50  0000 L CNN
F 1 "tp_gnd" H 3591 5314 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 3550 5350 60  0001 C CNN
F 3 "" H 3550 5350 60  0000 C CNN
	1    3550 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 4950 3350 4950
Wire Wire Line
	3450 5350 3350 5350
Wire Bus Line
	4450 3250 4450 5700
Wire Bus Line
	4800 3450 4800 5700
Wire Bus Line
	7300 3250 7300 5700
$EndSCHEMATC
