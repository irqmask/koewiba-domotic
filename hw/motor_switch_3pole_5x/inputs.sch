EESchema Schematic File Version 4
LIBS:motor_switch_3pole_5x-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "motor switch 3pole (inputs)"
Date "2018-09-17"
Rev "A"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 2600 1500 2    40   ~ 0
/button_0_up
Text Label 2600 2700 2    40   ~ 0
/button_0_down
$Comp
L power:+5V #PWR030
U 1 1 572DD72B
P 1800 2000
F 0 "#PWR030" H 1800 2200 50  0001 C CNN
F 1 "+5V" H 1800 2100 50  0000 C CNN
F 2 "" H 1800 2000 60  0000 C CNN
F 3 "" H 1800 2000 60  0000 C CNN
	1    1800 2000
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR031
U 1 1 572DD731
P 1800 2500
F 0 "#PWR031" H 1800 2300 50  0001 C CNN
F 1 "GND" H 1800 2390 50  0001 C CNN
F 2 "" H 1800 2500 60  0000 C CNN
F 3 "" H 1800 2500 60  0000 C CNN
	1    1800 2500
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR032
U 1 1 572DE6EB
P 1475 1975
F 0 "#PWR032" H 1475 1775 50  0001 C CNN
F 1 "GND" H 1475 1865 50  0001 C CNN
F 2 "" H 1475 1975 60  0000 C CNN
F 3 "" H 1475 1975 60  0000 C CNN
	1    1475 1975
	-1   0    0    -1  
$EndComp
$Comp
L logo:OPEN_HARDWARE_1 LOGO3
U 1 1 5B10FE76
P 975 7350
F 0 "LOGO3" H 975 7625 60  0000 C CNN
F 1 "OPEN_HARDWARE_1" H 975 7125 60  0000 C CNN
F 2 "Symbol:Symbol_OSHW-Logo_Copper" H 975 7350 60  0001 C CNN
F 3 "" H 975 7350 60  0000 C CNN
	1    975  7350
	1    0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D12
U 1 1 57AB8D78
P 1800 2250
F 0 "D12" H 1879 2296 50  0000 L CNN
F 1 "BAT54S" H 1879 2205 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 1800 2250 60  0001 C CNN
F 3 "" H 1800 2250 60  0000 C CNN
	1    1800 2250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1475 1600 1150 1600
Wire Wire Line
	1475 1600 1475 1800
Wire Wire Line
	1150 1700 1400 1700
$Comp
L connectors:CONN_4X1 J7
U 1 1 5B0E2195
P 950 1650
F 0 "J7" H 1025 1925 50  0000 C CNN
F 1 "WAGO 233-504" V 1125 1650 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 950 1650 60  0001 C CNN
F 3 "" H 950 1650 60  0000 C CNN
	1    950  1650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1475 1800 1150 1800
Connection ~ 1475 1800
$Comp
L standard:R R22
U 1 1 572DD721
P 1800 2700
F 0 "R22" H 1800 2600 50  0000 C CNN
F 1 "4k7" H 1800 2700 50  0000 C CNN
F 2 "Resistor:R_0805" H 1800 2700 60  0001 C CNN
F 3 "" H 1800 2700 60  0000 C CNN
	1    1800 2700
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR033
U 1 1 5B0E2682
P 1800 800
F 0 "#PWR033" H 1800 1000 50  0001 C CNN
F 1 "+5V" H 1800 900 50  0000 C CNN
F 2 "" H 1800 800 60  0000 C CNN
F 3 "" H 1800 800 60  0000 C CNN
	1    1800 800 
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR034
U 1 1 5B0E2688
P 1800 1300
F 0 "#PWR034" H 1800 1100 50  0001 C CNN
F 1 "GND" H 1800 1190 50  0001 C CNN
F 2 "" H 1800 1300 60  0000 C CNN
F 3 "" H 1800 1300 60  0000 C CNN
	1    1800 1300
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D11
U 1 1 5B0E268E
P 1800 1050
F 0 "D11" H 1879 1096 50  0000 L CNN
F 1 "BAT54S" H 1879 1005 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 1800 1050 60  0001 C CNN
F 3 "" H 1800 1050 60  0000 C CNN
	1    1800 1050
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R21
U 1 1 5B0E2694
P 1800 1500
F 0 "R21" H 1800 1400 50  0000 C CNN
F 1 "4k7" H 1800 1500 50  0000 C CNN
F 2 "Resistor:R_0805" H 1800 1500 60  0001 C CNN
F 3 "" H 1800 1500 60  0000 C CNN
	1    1800 1500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1800 800  1800 850 
Wire Wire Line
	1800 1250 1800 1300
Wire Wire Line
	1600 1500 1150 1500
Wire Wire Line
	1600 2700 1400 2700
Wire Wire Line
	1400 2700 1400 1700
Wire Wire Line
	1800 2000 1800 2050
Wire Wire Line
	1800 2450 1800 2500
Wire Wire Line
	2700 1500 2100 1500
Wire Wire Line
	2100 1500 2100 1350
Wire Wire Line
	2100 1050 1900 1050
Wire Wire Line
	2700 2700 2100 2700
Wire Wire Line
	2100 2700 2100 2550
Wire Wire Line
	2100 2250 1900 2250
Connection ~ 2100 1500
Connection ~ 2100 2700
Text HLabel 2700 1500 2    60   Output ~ 0
button_0_up
Text HLabel 2700 2700 2    60   Output ~ 0
button_0_down
Text Label 2600 3900 2    40   ~ 0
/button_1_up
Text Label 2600 5100 2    40   ~ 0
/button_1_down
$Comp
L power:+5V #PWR035
U 1 1 5B0E2FF8
P 1800 4400
F 0 "#PWR035" H 1800 4600 50  0001 C CNN
F 1 "+5V" H 1800 4500 50  0000 C CNN
F 2 "" H 1800 4400 60  0000 C CNN
F 3 "" H 1800 4400 60  0000 C CNN
	1    1800 4400
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR036
U 1 1 5B0E2FFE
P 1800 4900
F 0 "#PWR036" H 1800 4700 50  0001 C CNN
F 1 "GND" H 1800 4790 50  0001 C CNN
F 2 "" H 1800 4900 60  0000 C CNN
F 3 "" H 1800 4900 60  0000 C CNN
	1    1800 4900
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR037
U 1 1 5B0E3004
P 1475 4375
F 0 "#PWR037" H 1475 4175 50  0001 C CNN
F 1 "GND" H 1475 4265 50  0001 C CNN
F 2 "" H 1475 4375 60  0000 C CNN
F 3 "" H 1475 4375 60  0000 C CNN
	1    1475 4375
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D14
U 1 1 5B0E300A
P 1800 4650
F 0 "D14" H 1879 4696 50  0000 L CNN
F 1 "BAT54S" H 1879 4605 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 1800 4650 60  0001 C CNN
F 3 "" H 1800 4650 60  0000 C CNN
	1    1800 4650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1475 4000 1150 4000
Wire Wire Line
	1475 4000 1475 4200
Wire Wire Line
	1150 4100 1400 4100
$Comp
L connectors:CONN_4X1 J8
U 1 1 5B0E3013
P 950 4050
F 0 "J8" H 1025 4325 50  0000 C CNN
F 1 "WAGO 233-504" V 1125 4050 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 950 4050 60  0001 C CNN
F 3 "" H 950 4050 60  0000 C CNN
	1    950  4050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1475 4200 1150 4200
Connection ~ 1475 4200
$Comp
L standard:R R24
U 1 1 5B0E301B
P 1800 5100
F 0 "R24" H 1800 5000 50  0000 C CNN
F 1 "4k7" H 1800 5100 50  0000 C CNN
F 2 "Resistor:R_0805" H 1800 5100 60  0001 C CNN
F 3 "" H 1800 5100 60  0000 C CNN
	1    1800 5100
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR038
U 1 1 5B0E3021
P 1800 3200
F 0 "#PWR038" H 1800 3400 50  0001 C CNN
F 1 "+5V" H 1800 3300 50  0000 C CNN
F 2 "" H 1800 3200 60  0000 C CNN
F 3 "" H 1800 3200 60  0000 C CNN
	1    1800 3200
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR039
U 1 1 5B0E3027
P 1800 3700
F 0 "#PWR039" H 1800 3500 50  0001 C CNN
F 1 "GND" H 1800 3590 50  0001 C CNN
F 2 "" H 1800 3700 60  0000 C CNN
F 3 "" H 1800 3700 60  0000 C CNN
	1    1800 3700
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D13
U 1 1 5B0E302D
P 1800 3450
F 0 "D13" H 1879 3496 50  0000 L CNN
F 1 "BAT54S" H 1879 3405 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 1800 3450 60  0001 C CNN
F 3 "" H 1800 3450 60  0000 C CNN
	1    1800 3450
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R23
U 1 1 5B0E3033
P 1800 3900
F 0 "R23" H 1800 3800 50  0000 C CNN
F 1 "4k7" H 1800 3900 50  0000 C CNN
F 2 "Resistor:R_0805" H 1800 3900 60  0001 C CNN
F 3 "" H 1800 3900 60  0000 C CNN
	1    1800 3900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1800 3200 1800 3250
Wire Wire Line
	1800 3650 1800 3700
Wire Wire Line
	1600 3900 1150 3900
Wire Wire Line
	1600 5100 1400 5100
Wire Wire Line
	1400 5100 1400 4100
Wire Wire Line
	1800 4400 1800 4450
Wire Wire Line
	1800 4850 1800 4900
Wire Wire Line
	2700 3900 2100 3900
Wire Wire Line
	2100 3900 2100 3750
Wire Wire Line
	2100 3450 1900 3450
Wire Wire Line
	2700 5100 2100 5100
Wire Wire Line
	2100 5100 2100 4950
Wire Wire Line
	2100 4650 1900 4650
Connection ~ 2100 3900
Connection ~ 2100 5100
Text HLabel 2700 3900 2    60   Output ~ 0
button_1_up
Text HLabel 2700 5100 2    60   Output ~ 0
button_1_down
Text Label 5700 1500 2    40   ~ 0
/button_2_up
Text Label 5700 2700 2    40   ~ 0
/button_2_down
$Comp
L power:+5V #PWR040
U 1 1 5B0E32B0
P 4900 2000
F 0 "#PWR040" H 4900 2200 50  0001 C CNN
F 1 "+5V" H 4900 2100 50  0000 C CNN
F 2 "" H 4900 2000 60  0000 C CNN
F 3 "" H 4900 2000 60  0000 C CNN
	1    4900 2000
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR041
U 1 1 5B0E32B6
P 4900 2500
F 0 "#PWR041" H 4900 2300 50  0001 C CNN
F 1 "GND" H 4900 2390 50  0001 C CNN
F 2 "" H 4900 2500 60  0000 C CNN
F 3 "" H 4900 2500 60  0000 C CNN
	1    4900 2500
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR042
U 1 1 5B0E32BC
P 4575 1975
F 0 "#PWR042" H 4575 1775 50  0001 C CNN
F 1 "GND" H 4575 1865 50  0001 C CNN
F 2 "" H 4575 1975 60  0000 C CNN
F 3 "" H 4575 1975 60  0000 C CNN
	1    4575 1975
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D16
U 1 1 5B0E32C2
P 4900 2250
F 0 "D16" H 4979 2296 50  0000 L CNN
F 1 "BAT54S" H 4979 2205 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4900 2250 60  0001 C CNN
F 3 "" H 4900 2250 60  0000 C CNN
	1    4900 2250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4575 1600 4250 1600
Wire Wire Line
	4575 1600 4575 1800
Wire Wire Line
	4250 1700 4500 1700
$Comp
L connectors:CONN_4X1 J9
U 1 1 5B0E32CB
P 4050 1650
F 0 "J9" H 4125 1925 50  0000 C CNN
F 1 "WAGO 233-504" V 4225 1650 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 4050 1650 60  0001 C CNN
F 3 "" H 4050 1650 60  0000 C CNN
	1    4050 1650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4575 1800 4250 1800
Connection ~ 4575 1800
$Comp
L standard:R R26
U 1 1 5B0E32D3
P 4900 2700
F 0 "R26" H 4900 2600 50  0000 C CNN
F 1 "4k7" H 4900 2700 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 2700 60  0001 C CNN
F 3 "" H 4900 2700 60  0000 C CNN
	1    4900 2700
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR043
U 1 1 5B0E32D9
P 4900 800
F 0 "#PWR043" H 4900 1000 50  0001 C CNN
F 1 "+5V" H 4900 900 50  0000 C CNN
F 2 "" H 4900 800 60  0000 C CNN
F 3 "" H 4900 800 60  0000 C CNN
	1    4900 800 
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR044
U 1 1 5B0E32DF
P 4900 1300
F 0 "#PWR044" H 4900 1100 50  0001 C CNN
F 1 "GND" H 4900 1190 50  0001 C CNN
F 2 "" H 4900 1300 60  0000 C CNN
F 3 "" H 4900 1300 60  0000 C CNN
	1    4900 1300
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D15
U 1 1 5B0E32E5
P 4900 1050
F 0 "D15" H 4979 1096 50  0000 L CNN
F 1 "BAT54S" H 4979 1005 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4900 1050 60  0001 C CNN
F 3 "" H 4900 1050 60  0000 C CNN
	1    4900 1050
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R25
U 1 1 5B0E32EB
P 4900 1500
F 0 "R25" H 4900 1400 50  0000 C CNN
F 1 "4k7" H 4900 1500 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 1500 60  0001 C CNN
F 3 "" H 4900 1500 60  0000 C CNN
	1    4900 1500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4900 800  4900 850 
Wire Wire Line
	4900 1250 4900 1300
Wire Wire Line
	4700 1500 4250 1500
Wire Wire Line
	4700 2700 4500 2700
Wire Wire Line
	4500 2700 4500 1700
Wire Wire Line
	4900 2000 4900 2050
Wire Wire Line
	4900 2450 4900 2500
Wire Wire Line
	5800 1500 5200 1500
Wire Wire Line
	5200 1500 5200 1350
Wire Wire Line
	5200 1050 5000 1050
Wire Wire Line
	5800 2700 5200 2700
Wire Wire Line
	5200 2700 5200 2550
Wire Wire Line
	5200 2250 5000 2250
Connection ~ 5200 1500
Connection ~ 5200 2700
Text HLabel 5800 1500 2    60   Output ~ 0
button_2_up
Text HLabel 5800 2700 2    60   Output ~ 0
button_2_down
Text Label 5700 3900 2    40   ~ 0
/button_3_up
Text Label 5700 5100 2    40   ~ 0
/button_3_down
$Comp
L power:+5V #PWR045
U 1 1 5B0E3304
P 4900 4400
F 0 "#PWR045" H 4900 4600 50  0001 C CNN
F 1 "+5V" H 4900 4500 50  0000 C CNN
F 2 "" H 4900 4400 60  0000 C CNN
F 3 "" H 4900 4400 60  0000 C CNN
	1    4900 4400
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR046
U 1 1 5B0E330A
P 4900 4900
F 0 "#PWR046" H 4900 4700 50  0001 C CNN
F 1 "GND" H 4900 4790 50  0001 C CNN
F 2 "" H 4900 4900 60  0000 C CNN
F 3 "" H 4900 4900 60  0000 C CNN
	1    4900 4900
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5B0E3310
P 4575 4375
F 0 "#PWR047" H 4575 4175 50  0001 C CNN
F 1 "GND" H 4575 4265 50  0001 C CNN
F 2 "" H 4575 4375 60  0000 C CNN
F 3 "" H 4575 4375 60  0000 C CNN
	1    4575 4375
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D18
U 1 1 5B0E3316
P 4900 4650
F 0 "D18" H 4979 4696 50  0000 L CNN
F 1 "BAT54S" H 4979 4605 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4900 4650 60  0001 C CNN
F 3 "" H 4900 4650 60  0000 C CNN
	1    4900 4650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4575 4000 4250 4000
Wire Wire Line
	4575 4000 4575 4200
Wire Wire Line
	4250 4100 4500 4100
$Comp
L connectors:CONN_4X1 J10
U 1 1 5B0E331F
P 4050 4050
F 0 "J10" H 4125 4325 50  0000 C CNN
F 1 "WAGO 233-504" V 4225 4050 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 4050 4050 60  0001 C CNN
F 3 "" H 4050 4050 60  0000 C CNN
	1    4050 4050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4575 4200 4250 4200
Connection ~ 4575 4200
$Comp
L standard:R R28
U 1 1 5B0E3327
P 4900 5100
F 0 "R28" H 4900 5000 50  0000 C CNN
F 1 "4k7" H 4900 5100 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 5100 60  0001 C CNN
F 3 "" H 4900 5100 60  0000 C CNN
	1    4900 5100
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR048
U 1 1 5B0E332D
P 4900 3200
F 0 "#PWR048" H 4900 3400 50  0001 C CNN
F 1 "+5V" H 4900 3300 50  0000 C CNN
F 2 "" H 4900 3200 60  0000 C CNN
F 3 "" H 4900 3200 60  0000 C CNN
	1    4900 3200
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR049
U 1 1 5B0E3333
P 4900 3700
F 0 "#PWR049" H 4900 3500 50  0001 C CNN
F 1 "GND" H 4900 3590 50  0001 C CNN
F 2 "" H 4900 3700 60  0000 C CNN
F 3 "" H 4900 3700 60  0000 C CNN
	1    4900 3700
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D17
U 1 1 5B0E3339
P 4900 3450
F 0 "D17" H 4979 3496 50  0000 L CNN
F 1 "BAT54S" H 4979 3405 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4900 3450 60  0001 C CNN
F 3 "" H 4900 3450 60  0000 C CNN
	1    4900 3450
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R27
U 1 1 5B0E333F
P 4900 3900
F 0 "R27" H 4900 3800 50  0000 C CNN
F 1 "4k7" H 4900 3900 50  0000 C CNN
F 2 "Resistor:R_0805" H 4900 3900 60  0001 C CNN
F 3 "" H 4900 3900 60  0000 C CNN
	1    4900 3900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4900 3200 4900 3250
Wire Wire Line
	4900 3650 4900 3700
Wire Wire Line
	4700 3900 4250 3900
Wire Wire Line
	4700 5100 4500 5100
Wire Wire Line
	4500 5100 4500 4100
Wire Wire Line
	4900 4400 4900 4450
Wire Wire Line
	4900 4850 4900 4900
Wire Wire Line
	5800 3900 5200 3900
Wire Wire Line
	5200 3900 5200 3750
Wire Wire Line
	5200 3450 5000 3450
Wire Wire Line
	5800 5100 5200 5100
Wire Wire Line
	5200 5100 5200 4950
Wire Wire Line
	5200 4650 5000 4650
Connection ~ 5200 3900
Connection ~ 5200 5100
Text HLabel 5800 3900 2    60   Output ~ 0
button_3_up
Text HLabel 5800 5100 2    60   Output ~ 0
button_3_down
Text Label 8750 1500 2    40   ~ 0
/button_4_up
Text Label 8750 2700 2    40   ~ 0
/button_4_down
$Comp
L power:+5V #PWR050
U 1 1 5B0E35E6
P 7950 2000
F 0 "#PWR050" H 7950 2200 50  0001 C CNN
F 1 "+5V" H 7950 2100 50  0000 C CNN
F 2 "" H 7950 2000 60  0000 C CNN
F 3 "" H 7950 2000 60  0000 C CNN
	1    7950 2000
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR051
U 1 1 5B0E35EC
P 7950 2500
F 0 "#PWR051" H 7950 2300 50  0001 C CNN
F 1 "GND" H 7950 2390 50  0001 C CNN
F 2 "" H 7950 2500 60  0000 C CNN
F 3 "" H 7950 2500 60  0000 C CNN
	1    7950 2500
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR052
U 1 1 5B0E35F2
P 7625 1975
F 0 "#PWR052" H 7625 1775 50  0001 C CNN
F 1 "GND" H 7625 1865 50  0001 C CNN
F 2 "" H 7625 1975 60  0000 C CNN
F 3 "" H 7625 1975 60  0000 C CNN
	1    7625 1975
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D20
U 1 1 5B0E35F8
P 7950 2250
F 0 "D20" H 8029 2296 50  0000 L CNN
F 1 "BAT54S" H 8029 2205 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 7950 2250 60  0001 C CNN
F 3 "" H 7950 2250 60  0000 C CNN
	1    7950 2250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7625 1600 7300 1600
Wire Wire Line
	7625 1600 7625 1800
Wire Wire Line
	7300 1700 7550 1700
$Comp
L connectors:CONN_4X1 J11
U 1 1 5B0E3601
P 7100 1650
F 0 "J11" H 7175 1925 50  0000 C CNN
F 1 "WAGO 233-504" V 7275 1650 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 7100 1650 60  0001 C CNN
F 3 "" H 7100 1650 60  0000 C CNN
	1    7100 1650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7625 1800 7300 1800
Connection ~ 7625 1800
$Comp
L standard:R R30
U 1 1 5B0E3609
P 7950 2700
F 0 "R30" H 7950 2600 50  0000 C CNN
F 1 "4k7" H 7950 2700 50  0000 C CNN
F 2 "Resistor:R_0805" H 7950 2700 60  0001 C CNN
F 3 "" H 7950 2700 60  0000 C CNN
	1    7950 2700
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR053
U 1 1 5B0E360F
P 7950 800
F 0 "#PWR053" H 7950 1000 50  0001 C CNN
F 1 "+5V" H 7950 900 50  0000 C CNN
F 2 "" H 7950 800 60  0000 C CNN
F 3 "" H 7950 800 60  0000 C CNN
	1    7950 800 
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR054
U 1 1 5B0E3615
P 7950 1300
F 0 "#PWR054" H 7950 1100 50  0001 C CNN
F 1 "GND" H 7950 1190 50  0001 C CNN
F 2 "" H 7950 1300 60  0000 C CNN
F 3 "" H 7950 1300 60  0000 C CNN
	1    7950 1300
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D19
U 1 1 5B0E361B
P 7950 1050
F 0 "D19" H 8029 1096 50  0000 L CNN
F 1 "BAT54S" H 8029 1005 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 7950 1050 60  0001 C CNN
F 3 "" H 7950 1050 60  0000 C CNN
	1    7950 1050
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R29
U 1 1 5B0E3621
P 7950 1500
F 0 "R29" H 7950 1400 50  0000 C CNN
F 1 "4k7" H 7950 1500 50  0000 C CNN
F 2 "Resistor:R_0805" H 7950 1500 60  0001 C CNN
F 3 "" H 7950 1500 60  0000 C CNN
	1    7950 1500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7950 800  7950 850 
Wire Wire Line
	7950 1250 7950 1300
Wire Wire Line
	7750 1500 7300 1500
Wire Wire Line
	7750 2700 7550 2700
Wire Wire Line
	7550 2700 7550 1700
Wire Wire Line
	7950 2000 7950 2050
Wire Wire Line
	7950 2450 7950 2500
Wire Wire Line
	8850 1500 8250 1500
Wire Wire Line
	8250 1500 8250 1350
Wire Wire Line
	8250 1050 8050 1050
Wire Wire Line
	8850 2700 8250 2700
Wire Wire Line
	8250 2700 8250 2550
Wire Wire Line
	8250 2250 8050 2250
Connection ~ 8250 1500
Connection ~ 8250 2700
Text HLabel 8850 1500 2    60   Output ~ 0
button_4_up
Text HLabel 8850 2700 2    60   Output ~ 0
button_4_down
$Comp
L standard:ZD D21
U 1 1 5B0E3DD6
P 8000 4350
F 0 "D21" H 8000 4450 50  0000 C CNN
F 1 "ZD" H 8000 4275 50  0000 C CNN
F 2 "Diode_SMD:D_MINIMELF" H 8000 4350 60  0001 C CNN
F 3 "" H 8000 4350 60  0000 C CNN
	1    8000 4350
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR055
U 1 1 5B0E3F0E
P 8000 4050
F 0 "#PWR055" H 8000 4250 50  0001 C CNN
F 1 "+5V" H 8000 4160 50  0000 C CNN
F 2 "" H 8000 4050 60  0001 C CNN
F 3 "" H 8000 4050 60  0001 C CNN
	1    8000 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR056
U 1 1 5B0E3FFA
P 8000 4650
F 0 "#PWR056" H 8000 4450 50  0001 C CNN
F 1 "GND" H 8000 4540 50  0001 C CNN
F 2 "" H 8000 4650 60  0001 C CNN
F 3 "" H 8000 4650 60  0001 C CNN
	1    8000 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4050 8000 4150
Wire Wire Line
	8000 4550 8000 4650
Text Notes 8850 4350 2    40   ~ 0
Place close to inputs!
Wire Wire Line
	1475 1800 1475 1975
Wire Wire Line
	2100 1500 2000 1500
Wire Wire Line
	2100 2700 2000 2700
Wire Wire Line
	1475 4200 1475 4375
Wire Wire Line
	2100 3900 2000 3900
Wire Wire Line
	2100 5100 2000 5100
Wire Wire Line
	4575 1800 4575 1975
Wire Wire Line
	5200 1500 5100 1500
Wire Wire Line
	5200 2700 5100 2700
Wire Wire Line
	4575 4200 4575 4375
Wire Wire Line
	5200 3900 5100 3900
Wire Wire Line
	5200 5100 5100 5100
Wire Wire Line
	7625 1800 7625 1975
Wire Wire Line
	8250 1500 8150 1500
Wire Wire Line
	8250 2700 8150 2700
$Comp
L connectors:CONN_1 J33
U 1 1 5B9EEE13
P 2250 2550
F 0 "J33" H 2291 2586 50  0000 L CNN
F 1 "tp_output_button_0_down" H 2291 2514 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 2250 2550 60  0001 C CNN
F 3 "" H 2250 2550 60  0000 C CNN
	1    2250 2550
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J32
U 1 1 5B9EF0FF
P 2250 1350
F 0 "J32" H 2291 1386 50  0000 L CNN
F 1 "tp_output_button_0_up" H 2291 1314 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 2250 1350 60  0001 C CNN
F 3 "" H 2250 1350 60  0000 C CNN
	1    2250 1350
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J34
U 1 1 5B9EF182
P 2250 3750
F 0 "J34" H 2291 3786 50  0000 L CNN
F 1 "tp_output_button_1_up" H 2291 3714 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 2250 3750 60  0001 C CNN
F 3 "" H 2250 3750 60  0000 C CNN
	1    2250 3750
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J35
U 1 1 5B9EF22A
P 2250 4950
F 0 "J35" H 2291 4986 50  0000 L CNN
F 1 "tp_output_button_1_down" H 2291 4914 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 2250 4950 60  0001 C CNN
F 3 "" H 2250 4950 60  0000 C CNN
	1    2250 4950
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J36
U 1 1 5B9EF4BE
P 5350 1350
F 0 "J36" H 5391 1386 50  0000 L CNN
F 1 "tp_output_button_2_up" H 5391 1314 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 5350 1350 60  0001 C CNN
F 3 "" H 5350 1350 60  0000 C CNN
	1    5350 1350
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J37
U 1 1 5B9EF828
P 5350 2550
F 0 "J37" H 5391 2586 50  0000 L CNN
F 1 "tp_output_button_2_down" H 5391 2514 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 5350 2550 60  0001 C CNN
F 3 "" H 5350 2550 60  0000 C CNN
	1    5350 2550
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J38
U 1 1 5B9EF905
P 5350 3750
F 0 "J38" H 5391 3786 50  0000 L CNN
F 1 "tp_output_button_3_up" H 5391 3714 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 5350 3750 60  0001 C CNN
F 3 "" H 5350 3750 60  0000 C CNN
	1    5350 3750
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J39
U 1 1 5B9EFB4F
P 5350 4950
F 0 "J39" H 5391 4986 50  0000 L CNN
F 1 "tp_output_button_3_down" H 5391 4914 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 5350 4950 60  0001 C CNN
F 3 "" H 5350 4950 60  0000 C CNN
	1    5350 4950
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J40
U 1 1 5B9EFD23
P 8400 1350
F 0 "J40" H 8441 1386 50  0000 L CNN
F 1 "tp_output_button_4_up" H 8441 1314 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 8400 1350 60  0001 C CNN
F 3 "" H 8400 1350 60  0000 C CNN
	1    8400 1350
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J41
U 1 1 5B9EFE0B
P 8400 2550
F 0 "J41" H 8441 2586 50  0000 L CNN
F 1 "tp_output_button_4_down" H 8441 2514 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 8400 2550 60  0001 C CNN
F 3 "" H 8400 2550 60  0000 C CNN
	1    8400 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1350 2100 1350
Connection ~ 2100 1350
Wire Wire Line
	2100 1350 2100 1050
Wire Wire Line
	2150 2550 2100 2550
Connection ~ 2100 2550
Wire Wire Line
	2100 2550 2100 2250
Wire Wire Line
	2150 3750 2100 3750
Connection ~ 2100 3750
Wire Wire Line
	2100 3750 2100 3450
Wire Wire Line
	2150 4950 2100 4950
Connection ~ 2100 4950
Wire Wire Line
	2100 4950 2100 4650
Wire Wire Line
	5250 4950 5200 4950
Connection ~ 5200 4950
Wire Wire Line
	5200 4950 5200 4650
Wire Wire Line
	5250 3750 5200 3750
Connection ~ 5200 3750
Wire Wire Line
	5200 3750 5200 3450
Wire Wire Line
	5250 2550 5200 2550
Connection ~ 5200 2550
Wire Wire Line
	5200 2550 5200 2250
Wire Wire Line
	5250 1350 5200 1350
Connection ~ 5200 1350
Wire Wire Line
	5200 1350 5200 1050
Wire Wire Line
	8300 1350 8250 1350
Connection ~ 8250 1350
Wire Wire Line
	8250 1350 8250 1050
Wire Wire Line
	8300 2550 8250 2550
Connection ~ 8250 2550
Wire Wire Line
	8250 2550 8250 2250
$EndSCHEMATC
