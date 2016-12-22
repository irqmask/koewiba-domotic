EESchema Schematic File Version 2
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
Sheet 4 5
Title "4 Inputs"
Date "2016-11-29"
Rev "1"
Comp "KoeWiBa"
Comment1 "Baseboard for Raspberry PI 2B"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R R13
U 1 1 52AD89E5
P 4950 2400
F 0 "R13" H 4950 2300 50  0000 C CNN
F 1 "4k7" H 4950 2400 50  0000 C CNN
F 2 "Resistor:R_0805" H 4950 2400 60  0001 C CNN
F 3 "" H 4950 2400 60  0001 C CNN
F 4 "~" H 5075 2390 50  0001 L CNN "Tolerance"
F 5 "~" H 5075 2330 50  0001 L CNN "Nom. Power"
	1    4950 2400
	1    0    0    -1  
$EndComp
Text HLabel 5550 1300 2    50   Input ~ 0
+5V
Text HLabel 5450 2100 3    50   Input ~ 0
GND
Text HLabel 4650 2400 0    50   Input ~ 0
IN1
$Comp
L BAT54S D2
U 1 1 583D5AEB
P 5450 1800
F 0 "D2" H 5529 1846 50  0000 L CNN
F 1 "BAT54S" H 5529 1755 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5450 1800 60  0001 C CNN
F 3 "" H 5450 1800 60  0000 C CNN
	1    5450 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 1800 5350 1800
Wire Wire Line
	5250 1800 5250 2400
Wire Wire Line
	5150 2400 5550 2400
Connection ~ 5250 2400
Wire Wire Line
	5450 1300 5450 1600
Wire Wire Line
	4650 2400 4750 2400
Text HLabel 5550 2400 2    50   Output ~ 0
IN1_PROT
Wire Wire Line
	5450 2100 5450 2000
$Comp
L R R14
U 1 1 583D5D3C
P 4950 3700
F 0 "R14" H 4950 3600 50  0000 C CNN
F 1 "4k7" H 4950 3700 50  0000 C CNN
F 2 "Resistor:R_0805" H 4950 3700 60  0001 C CNN
F 3 "" H 4950 3700 60  0001 C CNN
F 4 "~" H 5075 3690 50  0001 L CNN "Tolerance"
F 5 "~" H 5075 3630 50  0001 L CNN "Nom. Power"
	1    4950 3700
	1    0    0    -1  
$EndComp
Text HLabel 5550 2600 2    50   Input ~ 0
+5V
Text HLabel 5450 3400 3    50   Input ~ 0
GND
Text HLabel 4650 3700 0    50   Input ~ 0
IN2
$Comp
L BAT54S D3
U 1 1 583D5D45
P 5450 3100
F 0 "D3" H 5529 3146 50  0000 L CNN
F 1 "BAT54S" H 5529 3055 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5450 3100 60  0001 C CNN
F 3 "" H 5450 3100 60  0000 C CNN
	1    5450 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3100 5350 3100
Wire Wire Line
	5250 3100 5250 3700
Wire Wire Line
	5150 3700 5550 3700
Connection ~ 5250 3700
Wire Wire Line
	5450 2600 5450 2900
Wire Wire Line
	4650 3700 4750 3700
Text HLabel 5550 3700 2    50   Output ~ 0
IN2_PROT
Wire Wire Line
	5450 3400 5450 3300
$Comp
L R R15
U 1 1 583D5EDE
P 4950 5050
F 0 "R15" H 4950 4950 50  0000 C CNN
F 1 "4k7" H 4950 5050 50  0000 C CNN
F 2 "Resistor:R_0805" H 4950 5050 60  0001 C CNN
F 3 "" H 4950 5050 60  0001 C CNN
F 4 "~" H 5075 5040 50  0001 L CNN "Tolerance"
F 5 "~" H 5075 4980 50  0001 L CNN "Nom. Power"
	1    4950 5050
	1    0    0    -1  
$EndComp
Text HLabel 5550 3900 2    50   Input ~ 0
+5V
Text HLabel 5450 4750 3    50   Input ~ 0
GND
Text HLabel 4650 5050 0    50   Input ~ 0
IN3
$Comp
L BAT54S D4
U 1 1 583D5EE7
P 5450 4450
F 0 "D4" H 5529 4496 50  0000 L CNN
F 1 "BAT54S" H 5529 4405 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5450 4450 60  0001 C CNN
F 3 "" H 5450 4450 60  0000 C CNN
	1    5450 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4450 5350 4450
Wire Wire Line
	5250 4450 5250 5050
Wire Wire Line
	5150 5050 5550 5050
Connection ~ 5250 5050
Wire Wire Line
	5450 3900 5450 4250
Wire Wire Line
	4650 5050 4750 5050
Text HLabel 5550 5050 2    50   Output ~ 0
IN3_PROT
Wire Wire Line
	5450 4750 5450 4650
$Comp
L R R16
U 1 1 583D5EF7
P 4950 6400
F 0 "R16" H 4950 6300 50  0000 C CNN
F 1 "4k7" H 4950 6400 50  0000 C CNN
F 2 "Resistor:R_0805" H 4950 6400 60  0001 C CNN
F 3 "" H 4950 6400 60  0001 C CNN
F 4 "~" H 5075 6390 50  0001 L CNN "Tolerance"
F 5 "~" H 5075 6330 50  0001 L CNN "Nom. Power"
	1    4950 6400
	1    0    0    -1  
$EndComp
Text HLabel 5550 5250 2    50   Input ~ 0
+5V
Text HLabel 5450 6100 3    50   Input ~ 0
GND
Text HLabel 4650 6400 0    50   Input ~ 0
IN4
$Comp
L BAT54S D5
U 1 1 583D5F00
P 5450 5800
F 0 "D5" H 5529 5846 50  0000 L CNN
F 1 "BAT54S" H 5529 5755 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5450 5800 60  0001 C CNN
F 3 "" H 5450 5800 60  0000 C CNN
	1    5450 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5800 5350 5800
Wire Wire Line
	5250 5800 5250 6400
Wire Wire Line
	5150 6400 5550 6400
Connection ~ 5250 6400
Wire Wire Line
	5450 5250 5450 5600
Wire Wire Line
	4650 6400 4750 6400
Text HLabel 5550 6400 2    50   Output ~ 0
IN4_PROT
Wire Wire Line
	5450 6100 5450 6000
$Comp
L R R18
U 1 1 584153DB
P 5150 2850
F 0 "R18" V 5104 2928 50  0000 L CNN
F 1 "56k" H 5150 2850 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 2850 60  0001 C CNN
F 3 "" H 5150 2850 60  0000 C CNN
	1    5150 2850
	0    1    1    0   
$EndComp
$Comp
L R R17
U 1 1 58415503
P 5150 1550
F 0 "R17" V 5104 1628 50  0000 L CNN
F 1 "56k" H 5150 1550 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 1550 60  0001 C CNN
F 3 "" H 5150 1550 60  0000 C CNN
	1    5150 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 2600 5550 2600
Wire Wire Line
	5150 1300 5550 1300
Connection ~ 5450 1300
Connection ~ 5450 2600
Connection ~ 5250 1800
Connection ~ 5250 3100
Wire Wire Line
	5150 3100 5150 3050
Wire Wire Line
	5150 2600 5150 2650
Wire Wire Line
	5150 1800 5150 1750
Wire Wire Line
	5150 1300 5150 1350
$Comp
L R R19
U 1 1 5841585B
P 5150 4200
F 0 "R19" V 5104 4278 50  0000 L CNN
F 1 "56k" H 5150 4200 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 4200 60  0001 C CNN
F 3 "" H 5150 4200 60  0000 C CNN
	1    5150 4200
	0    1    1    0   
$EndComp
$Comp
L R R20
U 1 1 584158D3
P 5150 5550
F 0 "R20" V 5104 5628 50  0000 L CNN
F 1 "56k" H 5150 5550 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 5550 60  0001 C CNN
F 3 "" H 5150 5550 60  0000 C CNN
	1    5150 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 5800 5150 5750
Connection ~ 5250 5800
Wire Wire Line
	5150 4450 5150 4400
Connection ~ 5250 4450
Wire Wire Line
	5150 4000 5150 3900
Wire Wire Line
	5150 3900 5550 3900
Connection ~ 5450 3900
Wire Wire Line
	5150 5250 5550 5250
Wire Wire Line
	5150 5350 5150 5250
Connection ~ 5450 5250
Text Notes 4800 1150 0    40   ~ 0
Inputs are with integrated pull-up to 5V. \nDisable the internal pull-up resistors of a \nconnected mikrocontroller to ensure correct \nfunction of the input protection!
Text Notes 6100 1550 0    40   ~ 0
Pulled to ground, a remaining voltage of \n(4700/(4700+56000))*VCC = 0,077 x VCC = 0,38V remains as low voltage.
$EndSCHEMATC
