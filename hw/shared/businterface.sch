EESchema Schematic File Version 2  date So 05 Mai 2013 15:34:25 CEST
LIBS:ics_controller_atmel
LIBS:ics_misc
LIBS:ics_regulator
LIBS:power
LIBS:standard
LIBS:connectors
LIBS:ics_peripheral
LIBS:display
LIBS:buscom-cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 3 3
Title ""
Date "5 may 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3150 1350 3150 3600
Connection ~ 3150 1450
Wire Wire Line
	2300 5150 2300 4300
Wire Wire Line
	3350 1850 3350 1750
Wire Wire Line
	3350 2850 3350 2750
Wire Wire Line
	2550 3350 2550 2750
Wire Wire Line
	1300 5950 1300 5050
Wire Wire Line
	3150 5850 3150 4700
Connection ~ 3150 5100
Wire Wire Line
	3150 5100 2550 5100
Wire Wire Line
	2550 5100 2550 4950
Wire Wire Line
	4200 3800 4200 1450
Wire Wire Line
	4300 4500 4200 4500
Wire Wire Line
	4300 4300 3650 4300
Wire Wire Line
	3650 4000 4300 4000
Connection ~ 1800 1900
Wire Wire Line
	2400 1900 1200 1900
Wire Wire Line
	2400 1900 2400 1650
Wire Wire Line
	2450 1450 2300 1450
Wire Wire Line
	2400 1650 2300 1650
Wire Wire Line
	1300 1450 1000 1450
Connection ~ 1000 3900
Wire Wire Line
	1300 4550 1300 3900
Wire Wire Line
	1300 3900 1000 3900
Connection ~ 3150 2750
Connection ~ 2300 4300
Wire Wire Line
	740  4100 1100 4100
Wire Wire Line
	2100 4000 2100 4550
Wire Wire Line
	740  4000 1000 4000
Connection ~ 2550 4300
Wire Wire Line
	2550 4550 2550 4300
Wire Wire Line
	2100 4950 2100 5600
Connection ~ 2300 4300
Wire Wire Line
	2650 4000 1100 4000
Wire Wire Line
	2650 4300 1100 4300
Connection ~ 2100 4000
Wire Wire Line
	1100 4300 1100 4200
Wire Wire Line
	740  4300 1000 4300
Wire Wire Line
	1100 4200 740  4200
Wire Wire Line
	2100 5600 2300 5600
Wire Wire Line
	2550 4000 2550 3750
Connection ~ 2550 4000
Wire Wire Line
	1300 1650 1200 1650
Wire Wire Line
	1000 4000 1000 2550
Wire Wire Line
	3650 4100 3700 4100
Wire Wire Line
	3700 4100 3700 4200
Wire Wire Line
	3700 4200 3650 4200
Wire Wire Line
	1800 2000 1800 1900
Wire Wire Line
	1200 1650 1200 1900
Wire Wire Line
	4300 4150 3700 4150
Connection ~ 3700 4150
Wire Wire Line
	4200 3800 4300 3800
Wire Wire Line
	4200 1450 2850 1450
Wire Wire Line
	1000 1450 1000 2150
Wire Wire Line
	1000 4300 1000 5850
Connection ~ 1300 5850
Wire Wire Line
	4200 4500 4200 5850
Connection ~ 3150 5850
Wire Wire Line
	3350 1450 3350 1550
Connection ~ 3350 1450
Wire Wire Line
	3350 3150 3350 3050
Wire Wire Line
	2300 5600 2300 5450
Wire Wire Line
	2550 2750 3350 2750
Wire Wire Line
	1100 4000 1100 4100
Wire Wire Line
	4200 5850 1000 5850
$Comp
L GND #PWR04
U 1 1 517A5C83
P 3350 3150
F 0 "#PWR04" H 3350 3150 30  0001 C CNN
F 1 "GND" H 3350 3080 30  0001 C CNN
	1    3350 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 517A5C80
P 3350 1850
F 0 "#PWR05" H 3350 1850 30  0001 C CNN
F 1 "GND" H 3350 1780 30  0001 C CNN
	1    3350 1850
	1    0    0    -1  
$EndComp
$Comp
L C C301
U 1 1 517A5C6F
P 3350 1650
F 0 "C301" H 3400 1700 50  0000 L CNN
F 1 "2,2uF" H 3350 1550 40  0000 C CNN
F 4 "~" H 3150 1490 50  0001 L CNN "Voltage"
F 5 "~" H 3150 1430 50  0001 L CNN "Tolerance"
	1    3350 1650
	0    1    1    0   
$EndComp
$Comp
L C C302
U 1 1 517A5C42
P 3350 2950
F 0 "C302" H 3400 3000 50  0000 L CNN
F 1 "100nF" H 3350 2850 40  0000 C CNN
F 4 "~" H 3150 2790 50  0001 L CNN "Voltage"
F 5 "~" H 3150 2730 50  0001 L CNN "Tolerance"
	1    3350 2950
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR06
U 1 1 517A5B06
P 3150 1350
F 0 "#PWR06" H 3150 1525 50  0001 C CNN
F 1 "VCC" H 3150 1450 50  0000 C CNN
	1    3150 1350
	1    0    0    -1  
$EndComp
Text HLabel 4300 4500 2    60   Input ~ 0
GND
Text HLabel 4300 3800 2    60   Input ~ 0
VCC
Text HLabel 4300 4150 2    60   Input ~ 0
TEN
Text HLabel 4300 4300 2    60   Input ~ 0
RXD
Text HLabel 4300 4000 2    60   Input ~ 0
TXD
$Comp
L GND #PWR07
U 1 1 517A5A68
P 1300 5950
F 0 "#PWR07" H 1300 5950 30  0001 C CNN
F 1 "GND" H 1300 5880 30  0001 C CNN
	1    1300 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 517A5A11
P 1800 2000
F 0 "#PWR08" H 1800 2000 30  0001 C CNN
F 1 "GND" H 1800 1930 30  0001 C CNN
	1    1800 2000
	1    0    0    -1  
$EndComp
$Comp
L FUSE F302
U 1 1 517A45B7
P 1000 2350
F 0 "F302" H 1000 2450 50  0000 C CNN
F 1 "FUSE" H 1000 2250 50  0000 C CNN
	1    1000 2350
	0    1    1    0   
$EndComp
$Comp
L FUSE F301
U 1 1 517A4454
P 2650 1450
F 0 "F301" H 2650 1550 50  0000 C CNN
F 1 "FUSE" H 2650 1350 50  0000 C CNN
	1    2650 1450
	1    0    0    -1  
$EndComp
Text Label 790  4300 0    50   ~ 0
GND
Text Label 790  4200 0    50   ~ 0
Bus-B
Text Label 790  4100 0    50   ~ 0
Bus-A
Text Label 790  4000 0    50   ~ 0
+24
$Comp
L CONN_1 J301
U 1 1 51673922
P 640 4000
F 0 "J301" H 640 4040 40  0000 C CNN
F 1 "CONN_1" H 550 3970 25  0000 C CNN
	1    640  4000
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 J302
U 1 1 5167391D
P 640 4100
F 0 "J302" H 640 4140 40  0000 C CNN
F 1 "CONN_1" H 550 4070 25  0000 C CNN
	1    640  4100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 J303
U 1 1 5167391A
P 640 4200
F 0 "J303" H 640 4240 40  0000 C CNN
F 1 "CONN_1" H 550 4170 25  0000 C CNN
	1    640  4200
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 J304
U 1 1 51673909
P 640 4300
F 0 "J304" H 640 4340 40  0000 C CNN
F 1 "CONN_1" H 550 4270 25  0000 C CNN
	1    640  4300
	-1   0    0    -1  
$EndComp
$Comp
L VARISTOR R304
U 1 1 515FF5C8
P 1300 4800
F 0 "R304" H 1170 4710 50  0000 C CNN
F 1 "VARISTOR" H 1300 4800 40  0000 C CNN
F 2 "~" H 1400 4790 50  0001 L CNN
F 3 "~" H 1400 4730 50  0001 L CNN
	1    1300 4800
	0    -1   -1   0   
$EndComp
$Comp
L R R301
U 1 1 5155E63E
P 2100 4750
F 0 "R301" H 2150 4850 50  0000 L CNN
F 1 "120R" H 2000 4750 50  0000 L CNN
F 2 "~" H 2200 4740 50  0001 L CNN
F 3 "~" H 2200 4680 50  0001 L CNN
	1    2100 4750
	0    1    1    0   
$EndComp
$Comp
L JUMPER J305
U 1 1 5155E40C
P 2300 5300
F 0 "J305" H 2250 5450 60  0000 C CNN
F 1 "JUMPER" H 2300 5150 60  0000 C CNN
	1    2300 5300
	0    1    1    0   
$EndComp
$Comp
L AM1S-2405 U301
U 1 1 5155B854
P 1800 1550
F 0 "U301" H 1800 1550 40  0000 C CNN
F 1 "AM1S-2405" H 1800 1350 40  0000 C CNN
F 2 "MODULE" H 1800 1550 40  0001 C CNN
F 3 "DOCUMENTATION" H 1800 1550 40  0001 C CNN
	1    1800 1550
	1    0    0    -1  
$EndComp
$Comp
L MAX487 U302
U 1 1 5155AE3E
P 3150 4150
F 0 "U302" H 3400 4550 60  0000 C CNN
F 1 "MAX487" H 2950 3750 60  0000 C CNN
F 2 "SSOP28" H 3000 4700 40  0001 C CNN
	1    3150 4150
	-1   0    0    -1  
$EndComp
$Comp
L R R303
U 1 1 5155A28E
P 2550 4750
F 0 "R303" H 2600 4850 50  0000 L CNN
F 1 "390R" H 2450 4750 50  0000 L CNN
F 2 "~" H 2650 4740 50  0001 L CNN
F 3 "~" H 2650 4680 50  0001 L CNN
	1    2550 4750
	0    1    1    0   
$EndComp
$Comp
L R R302
U 1 1 5155A27F
P 2550 3550
F 0 "R302" H 2600 3650 50  0000 L CNN
F 1 "390R" H 2450 3550 50  0000 L CNN
F 2 "~" H 2650 3540 50  0001 L CNN
F 3 "~" H 2650 3480 50  0001 L CNN
	1    2550 3550
	0    1    1    0   
$EndComp
$EndSCHEMATC
