EESchema Schematic File Version 4
LIBS:switch_3pole_5x-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "switch 3pole (inputs)"
Date "2018-09-18"
Rev "A"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 5050 2400 2    40   ~ 0
/button_0
Text Label 5050 3600 2    40   ~ 0
/button_1
$Comp
L power:+5V #PWR014
U 1 1 572DD72B
P 4250 2900
F 0 "#PWR014" H 4250 3100 50  0001 C CNN
F 1 "+5V" H 4250 3050 50  0000 C CNN
F 2 "" H 4250 2900 60  0000 C CNN
F 3 "" H 4250 2900 60  0000 C CNN
	1    4250 2900
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 572DD731
P 4250 3400
F 0 "#PWR015" H 4250 3200 50  0001 C CNN
F 1 "GND" H 4250 3290 50  0001 C CNN
F 2 "" H 4250 3400 60  0000 C CNN
F 3 "" H 4250 3400 60  0000 C CNN
	1    4250 3400
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 572DE6EB
P 3925 2875
F 0 "#PWR016" H 3925 2675 50  0001 C CNN
F 1 "GND" H 3925 2765 50  0001 C CNN
F 2 "" H 3925 2875 60  0000 C CNN
F 3 "" H 3925 2875 60  0000 C CNN
	1    3925 2875
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
P 4250 3150
F 0 "D12" H 4329 3196 50  0000 L CNN
F 1 "BAT54S" H 4329 3105 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4250 3150 60  0001 C CNN
F 3 "" H 4250 3150 60  0000 C CNN
	1    4250 3150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3925 2500 3600 2500
Wire Wire Line
	3925 2500 3925 2700
Wire Wire Line
	3600 2600 3850 2600
$Comp
L connectors:CONN_4X1 J7
U 1 1 5B0E2195
P 3400 2550
F 0 "J7" H 3475 2825 50  0000 C CNN
F 1 "WAGO 233-504" V 3575 2550 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 3400 2550 60  0001 C CNN
F 3 "" H 3400 2550 60  0000 C CNN
	1    3400 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3925 2700 3600 2700
Connection ~ 3925 2700
$Comp
L standard:R R22
U 1 1 572DD721
P 4250 3600
F 0 "R22" H 4250 3500 50  0000 C CNN
F 1 "4k7" H 4250 3600 50  0000 C CNN
F 2 "Resistor:R_0805" H 4250 3600 60  0001 C CNN
F 3 "" H 4250 3600 60  0000 C CNN
	1    4250 3600
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 5B0E2682
P 4250 1700
F 0 "#PWR017" H 4250 1900 50  0001 C CNN
F 1 "+5V" H 4250 1850 50  0000 C CNN
F 2 "" H 4250 1700 60  0000 C CNN
F 3 "" H 4250 1700 60  0000 C CNN
	1    4250 1700
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5B0E2688
P 4250 2200
F 0 "#PWR018" H 4250 2000 50  0001 C CNN
F 1 "GND" H 4250 2090 50  0001 C CNN
F 2 "" H 4250 2200 60  0000 C CNN
F 3 "" H 4250 2200 60  0000 C CNN
	1    4250 2200
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D11
U 1 1 5B0E268E
P 4250 1950
F 0 "D11" H 4329 1996 50  0000 L CNN
F 1 "BAT54S" H 4329 1905 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4250 1950 60  0001 C CNN
F 3 "" H 4250 1950 60  0000 C CNN
	1    4250 1950
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R21
U 1 1 5B0E2694
P 4250 2400
F 0 "R21" H 4250 2300 50  0000 C CNN
F 1 "4k7" H 4250 2400 50  0000 C CNN
F 2 "Resistor:R_0805" H 4250 2400 60  0001 C CNN
F 3 "" H 4250 2400 60  0000 C CNN
	1    4250 2400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4250 1700 4250 1750
Wire Wire Line
	4250 2150 4250 2200
Wire Wire Line
	4050 2400 3600 2400
Wire Wire Line
	4050 3600 3850 3600
Wire Wire Line
	3850 3600 3850 2600
Wire Wire Line
	4250 2900 4250 2950
Wire Wire Line
	4250 3350 4250 3400
Wire Wire Line
	5150 2400 4550 2400
Wire Wire Line
	4550 2400 4550 2200
Wire Wire Line
	4550 1950 4350 1950
Wire Wire Line
	5150 3600 4550 3600
Wire Wire Line
	4550 3600 4550 3400
Wire Wire Line
	4550 3150 4350 3150
Connection ~ 4550 2400
Connection ~ 4550 3600
Text HLabel 5150 2400 2    60   Output ~ 0
button_0
Text HLabel 5150 3600 2    60   Output ~ 0
button_1
Text Label 5050 4800 2    40   ~ 0
/button_2
Text Label 5050 6000 2    40   ~ 0
/button_3
$Comp
L power:+5V #PWR019
U 1 1 5B0E2FF8
P 4250 5300
F 0 "#PWR019" H 4250 5500 50  0001 C CNN
F 1 "+5V" H 4250 5450 50  0000 C CNN
F 2 "" H 4250 5300 60  0000 C CNN
F 3 "" H 4250 5300 60  0000 C CNN
	1    4250 5300
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5B0E2FFE
P 4250 5800
F 0 "#PWR020" H 4250 5600 50  0001 C CNN
F 1 "GND" H 4250 5690 50  0001 C CNN
F 2 "" H 4250 5800 60  0000 C CNN
F 3 "" H 4250 5800 60  0000 C CNN
	1    4250 5800
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5B0E3004
P 3925 5275
F 0 "#PWR021" H 3925 5075 50  0001 C CNN
F 1 "GND" H 3925 5165 50  0001 C CNN
F 2 "" H 3925 5275 60  0000 C CNN
F 3 "" H 3925 5275 60  0000 C CNN
	1    3925 5275
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D14
U 1 1 5B0E300A
P 4250 5550
F 0 "D14" H 4329 5596 50  0000 L CNN
F 1 "BAT54S" H 4329 5505 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4250 5550 60  0001 C CNN
F 3 "" H 4250 5550 60  0000 C CNN
	1    4250 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3925 4900 3600 4900
Wire Wire Line
	3925 4900 3925 5100
Wire Wire Line
	3600 5000 3850 5000
$Comp
L connectors:CONN_4X1 J8
U 1 1 5B0E3013
P 3400 4950
F 0 "J8" H 3475 5225 50  0000 C CNN
F 1 "WAGO 233-504" V 3575 4950 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 3400 4950 60  0001 C CNN
F 3 "" H 3400 4950 60  0000 C CNN
	1    3400 4950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3925 5100 3600 5100
Connection ~ 3925 5100
$Comp
L standard:R R24
U 1 1 5B0E301B
P 4250 6000
F 0 "R24" H 4250 5900 50  0000 C CNN
F 1 "4k7" H 4250 6000 50  0000 C CNN
F 2 "Resistor:R_0805" H 4250 6000 60  0001 C CNN
F 3 "" H 4250 6000 60  0000 C CNN
	1    4250 6000
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR022
U 1 1 5B0E3021
P 4250 4100
F 0 "#PWR022" H 4250 4300 50  0001 C CNN
F 1 "+5V" H 4250 4250 50  0000 C CNN
F 2 "" H 4250 4100 60  0000 C CNN
F 3 "" H 4250 4100 60  0000 C CNN
	1    4250 4100
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5B0E3027
P 4250 4600
F 0 "#PWR023" H 4250 4400 50  0001 C CNN
F 1 "GND" H 4250 4490 50  0001 C CNN
F 2 "" H 4250 4600 60  0000 C CNN
F 3 "" H 4250 4600 60  0000 C CNN
	1    4250 4600
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D13
U 1 1 5B0E302D
P 4250 4350
F 0 "D13" H 4329 4396 50  0000 L CNN
F 1 "BAT54S" H 4329 4305 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 4250 4350 60  0001 C CNN
F 3 "" H 4250 4350 60  0000 C CNN
	1    4250 4350
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R23
U 1 1 5B0E3033
P 4250 4800
F 0 "R23" H 4250 4700 50  0000 C CNN
F 1 "4k7" H 4250 4800 50  0000 C CNN
F 2 "Resistor:R_0805" H 4250 4800 60  0001 C CNN
F 3 "" H 4250 4800 60  0000 C CNN
	1    4250 4800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4250 4100 4250 4150
Wire Wire Line
	4250 4550 4250 4600
Wire Wire Line
	4050 4800 3600 4800
Wire Wire Line
	4050 6000 3850 6000
Wire Wire Line
	3850 6000 3850 5000
Wire Wire Line
	4250 5300 4250 5350
Wire Wire Line
	4250 5750 4250 5800
Wire Wire Line
	5150 4800 4550 4800
Wire Wire Line
	4550 4800 4550 4600
Wire Wire Line
	4550 4350 4350 4350
Wire Wire Line
	5150 6000 4550 6000
Wire Wire Line
	4550 6000 4550 5800
Wire Wire Line
	4550 5550 4350 5550
Connection ~ 4550 4800
Connection ~ 4550 6000
Text HLabel 5150 4800 2    60   Output ~ 0
button_2
Text HLabel 5150 6000 2    60   Output ~ 0
button_3
Text Label 8150 2400 2    40   ~ 0
/button_4
$Comp
L power:+5V #PWR024
U 1 1 5B0E32D9
P 7350 1700
F 0 "#PWR024" H 7350 1900 50  0001 C CNN
F 1 "+5V" H 7350 1850 50  0000 C CNN
F 2 "" H 7350 1700 60  0000 C CNN
F 3 "" H 7350 1700 60  0000 C CNN
	1    7350 1700
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5B0E32DF
P 7350 2200
F 0 "#PWR025" H 7350 2000 50  0001 C CNN
F 1 "GND" H 7350 2090 50  0001 C CNN
F 2 "" H 7350 2200 60  0000 C CNN
F 3 "" H 7350 2200 60  0000 C CNN
	1    7350 2200
	-1   0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D15
U 1 1 5B0E32E5
P 7350 1950
F 0 "D15" H 7429 1996 50  0000 L CNN
F 1 "BAT54S" H 7429 1905 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 7350 1950 60  0001 C CNN
F 3 "" H 7350 1950 60  0000 C CNN
	1    7350 1950
	-1   0    0    -1  
$EndComp
$Comp
L standard:R R25
U 1 1 5B0E32EB
P 7350 2400
F 0 "R25" H 7350 2300 50  0000 C CNN
F 1 "4k7" H 7350 2400 50  0000 C CNN
F 2 "Resistor:R_0805" H 7350 2400 60  0001 C CNN
F 3 "" H 7350 2400 60  0000 C CNN
	1    7350 2400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7350 1700 7350 1750
Wire Wire Line
	7350 2150 7350 2200
Wire Wire Line
	7150 2400 6700 2400
Wire Wire Line
	8250 2400 7650 2400
Wire Wire Line
	7650 2400 7650 2200
Wire Wire Line
	7650 1950 7450 1950
Connection ~ 7650 2400
Text HLabel 8250 2400 2    60   Output ~ 0
button_4
$Comp
L standard:ZD D21
U 1 1 5B0E3DD6
P 7550 4800
F 0 "D21" H 7550 4900 50  0000 C CNN
F 1 "ZD" H 7550 4725 50  0000 C CNN
F 2 "Diode_SMD:D_MINIMELF" H 7550 4800 60  0001 C CNN
F 3 "" H 7550 4800 60  0000 C CNN
	1    7550 4800
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR026
U 1 1 5B0E3F0E
P 7550 4500
F 0 "#PWR026" H 7550 4700 50  0001 C CNN
F 1 "+5V" H 7550 4650 50  0000 C CNN
F 2 "" H 7550 4500 60  0001 C CNN
F 3 "" H 7550 4500 60  0001 C CNN
	1    7550 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5B0E3FFA
P 7550 5100
F 0 "#PWR027" H 7550 4900 50  0001 C CNN
F 1 "GND" H 7550 4990 50  0001 C CNN
F 2 "" H 7550 5100 60  0001 C CNN
F 3 "" H 7550 5100 60  0001 C CNN
	1    7550 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 4500 7550 4600
Wire Wire Line
	7550 5000 7550 5100
Text Notes 8400 4800 2    40   ~ 0
Place close to inputs!
$Comp
L connectors:CONN_2X1 J9
U 1 1 5B1A4C2C
P 6500 2450
F 0 "J9" H 6575 2625 50  0000 C CNN
F 1 "WAGO 233-502" V 6675 2450 25  0000 C CNN
F 2 "Connector_WAGO:WAGO233-4" H 6500 2450 60  0001 C CNN
F 3 "" H 6500 2450 60  0000 C CNN
	1    6500 2450
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR028
U 1 1 5B0E32BC
P 6800 2600
F 0 "#PWR028" H 6800 2400 50  0001 C CNN
F 1 "GND" H 6800 2490 50  0001 C CNN
F 2 "" H 6800 2600 60  0000 C CNN
F 3 "" H 6800 2600 60  0000 C CNN
	1    6800 2600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6700 2500 6800 2500
Wire Wire Line
	6800 2500 6800 2600
Wire Wire Line
	3925 2700 3925 2875
Wire Wire Line
	4550 2400 4450 2400
Wire Wire Line
	4550 3600 4450 3600
Wire Wire Line
	3925 5100 3925 5275
Wire Wire Line
	4550 4800 4450 4800
Wire Wire Line
	4550 6000 4450 6000
Wire Wire Line
	7650 2400 7550 2400
$Comp
L connectors:CONN_1 J19
U 1 1 5BA165F9
P 4750 2200
F 0 "J19" H 4791 2236 50  0000 L CNN
F 1 "tp_button_0" H 4791 2164 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4750 2200 60  0001 C CNN
F 3 "" H 4750 2200 60  0000 C CNN
	1    4750 2200
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J20
U 1 1 5BA166D5
P 4750 3400
F 0 "J20" H 4791 3436 50  0000 L CNN
F 1 "tp_button_1" H 4791 3364 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4750 3400 60  0001 C CNN
F 3 "" H 4750 3400 60  0000 C CNN
	1    4750 3400
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J21
U 1 1 5BA16774
P 4750 4600
F 0 "J21" H 4791 4636 50  0000 L CNN
F 1 "tp_button_2" H 4791 4564 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4750 4600 60  0001 C CNN
F 3 "" H 4750 4600 60  0000 C CNN
	1    4750 4600
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J22
U 1 1 5BA167E4
P 4750 5800
F 0 "J22" H 4791 5836 50  0000 L CNN
F 1 "tp_button_3" H 4791 5764 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 4750 5800 60  0001 C CNN
F 3 "" H 4750 5800 60  0000 C CNN
	1    4750 5800
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J23
U 1 1 5BA16905
P 7850 2200
F 0 "J23" H 7891 2236 50  0000 L CNN
F 1 "tp_button_4" H 7891 2164 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 7850 2200 60  0001 C CNN
F 3 "" H 7850 2200 60  0000 C CNN
	1    7850 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2200 4550 2200
Connection ~ 4550 2200
Wire Wire Line
	4550 2200 4550 1950
Wire Wire Line
	4650 3400 4550 3400
Connection ~ 4550 3400
Wire Wire Line
	4550 3400 4550 3150
Wire Wire Line
	7750 2200 7650 2200
Connection ~ 7650 2200
Wire Wire Line
	7650 2200 7650 1950
Wire Wire Line
	4650 4600 4550 4600
Connection ~ 4550 4600
Wire Wire Line
	4550 4600 4550 4350
Wire Wire Line
	4650 5800 4550 5800
Connection ~ 4550 5800
Wire Wire Line
	4550 5800 4550 5550
$EndSCHEMATC
