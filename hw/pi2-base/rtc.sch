EESchema Schematic File Version 4
LIBS:pi2-base-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
Title "RTC"
Date "2016-11-28"
Rev "B"
Comp "KoeWiBa"
Comment1 "Baseboard for Raspberry PI 2"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4000 3950 4350 3950
Wire Wire Line
	5450 3950 5800 3950
Wire Wire Line
	5450 4050 6050 4050
Connection ~ 5550 3850
Wire Wire Line
	5450 3850 5550 3850
Connection ~ 7300 4350
Wire Wire Line
	7300 4350 7300 4650
Wire Wire Line
	7900 4100 7900 3950
Connection ~ 7700 4350
Wire Wire Line
	7700 4350 7700 4050
Connection ~ 8400 3900
Wire Wire Line
	8350 3900 8400 3900
Wire Wire Line
	7700 4050 7600 4050
Wire Wire Line
	8350 3700 8400 3700
Connection ~ 8050 3900
Wire Wire Line
	8050 3800 8050 3900
Wire Wire Line
	7600 3850 7700 3850
Wire Wire Line
	7700 3850 7700 3900
Wire Wire Line
	7700 3900 8050 3900
Wire Wire Line
	7700 3750 7600 3750
Wire Wire Line
	8150 3700 7700 3700
Wire Wire Line
	7700 3700 7700 3750
Connection ~ 7700 3750
Wire Wire Line
	7900 3950 7600 3950
Wire Wire Line
	6300 3750 6500 3750
Wire Wire Line
	8400 3700 8400 3900
Wire Wire Line
	8400 4350 7900 4350
Connection ~ 7900 4350
Wire Wire Line
	6300 4350 6300 4300
Wire Wire Line
	7900 4300 7900 4350
Wire Wire Line
	5550 3400 5550 3850
Wire Wire Line
	4000 4050 4350 4050
Wire Wire Line
	6300 3250 6300 3400
Connection ~ 6300 3750
Text Label 4300 4050 2    40   ~ 0
RSDA
Text Label 4300 3950 2    40   ~ 0
RSCL
NoConn ~ 6500 3850
NoConn ~ 5450 4150
Text Notes 7325 4225 2    40   ~ 0
I2C Address: 0x68
Text Label 6250 4050 2    40   ~ 0
SDA
Text Label 6250 3950 2    40   ~ 0
SCL
$Comp
L standard:C C1
U 1 1 527FA60F
P 5550 4350
F 0 "C1" H 5600 4400 50  0000 L CNN
F 1 "100nF" H 5550 4225 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 5550 4350 60  0001 C CNN
F 3 "" H 5550 4350 60  0001 C CNN
F 4 "~" H 5350 4190 50  0001 L CNN "Voltage"
F 5 "~" H 5350 4130 50  0001 L CNN "Tolerance"
	1    5550 4350
	0    -1   1    0   
$EndComp
$Comp
L standard:BATT BATT1
U 1 1 527FA283
P 7900 4200
F 0 "BATT1" H 7800 4050 50  0000 L CNN
F 1 "3V" H 7950 4145 50  0000 L CNN
F 2 "Electromechanical:CR2032H" H 7850 4150 50  0001 C CNN
F 3 "" H 7900 4200 60  0001 C CNN
F 4 "~" H 7700 4040 50  0001 L CNN "Voltage"
F 5 "~" H 7700 3980 50  0001 L CNN "Tolerance"
	1    7900 4200
	0    -1   1    0   
$EndComp
$Comp
L standard:C C2
U 1 1 527FA128
P 6300 4200
F 0 "C2" H 6350 4250 50  0000 L CNN
F 1 "100nF" H 6300 4325 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6300 4200 60  0001 C CNN
F 3 "" H 6300 4200 60  0001 C CNN
F 4 "~" H 6100 4040 50  0001 L CNN "Voltage"
F 5 "~" H 6100 3980 50  0001 L CNN "Tolerance"
	1    6300 4200
	0    -1   1    0   
$EndComp
$Comp
L standard:C C4
U 1 1 527FA124
P 8250 3900
F 0 "C4" H 8200 3675 50  0000 L CNN
F 1 "12.5pF" H 8250 3775 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 8250 3900 60  0001 C CNN
F 3 "" H 8250 3900 60  0001 C CNN
F 4 "~" H 8050 3740 50  0001 L CNN "Voltage"
F 5 "~" H 8050 3680 50  0001 L CNN "Tolerance"
	1    8250 3900
	-1   0    0    -1  
$EndComp
$Comp
L standard:C C3
U 1 1 527FA121
P 8250 3700
F 0 "C3" H 8200 3925 50  0000 L CNN
F 1 "12.5pF" H 8250 3825 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 8250 3700 60  0001 C CNN
F 3 "" H 8250 3700 60  0001 C CNN
F 4 "~" H 8050 3540 50  0001 L CNN "Voltage"
F 5 "~" H 8050 3480 50  0001 L CNN "Tolerance"
	1    8250 3700
	-1   0    0    -1  
$EndComp
$Comp
L standard:CRYSTAL U3
U 1 1 527FA110
P 7700 3800
F 0 "U3" H 7850 4050 50  0000 C CNN
F 1 "32.768kHz" H 7850 3950 50  0000 C CNN
F 2 "Crystal:CRY_MS1V-7_SMD" H 7700 3800 60  0001 C CNN
F 3 "" H 7700 3800 60  0001 C CNN
	1    7700 3800
	1    0    0    -1  
$EndComp
$Comp
L ics_peripheral:DS1307 U2
U 1 1 527F9F98
P 7050 3900
F 0 "U2" H 7050 3800 50  0000 C CNN
F 1 "DS1307" H 7050 4000 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 7050 3900 50  0001 C CNN
F 3 "64X8 I2C Real Time Clock" H 7050 3900 50  0001 C CNN
	1    7050 3900
	-1   0    0    -1  
$EndComp
$Comp
L ics_peripheral:PCA9517 U1
U 1 1 527F9F8E
P 4900 4000
F 0 "U1" H 4900 3900 50  0000 C CNN
F 1 "PCA9517" H 4900 4100 50  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 4900 4000 50  0001 C CNN
F 3 "Level translating I2C Bus repeater" H 4900 4000 50  0001 C CNN
	1    4900 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3850 4350 3850
Wire Wire Line
	4350 4150 4250 4150
Wire Wire Line
	4250 4150 4250 4650
Wire Wire Line
	5550 4450 5550 4650
Text HLabel 3900 3900 0    50   Input ~ 0
SCL_3V3
Text HLabel 3900 4100 0    50   Input ~ 0
SDA_3V3
Wire Wire Line
	4000 3950 4000 3900
Wire Wire Line
	4000 3900 3900 3900
Wire Wire Line
	4000 4050 4000 4100
Wire Wire Line
	4000 4100 3900 4100
$Comp
L standard:R R11
U 1 1 583C425A
P 5800 3650
F 0 "R11" V 5754 3728 50  0000 L CNN
F 1 "10k" H 5800 3650 50  0000 C CNN
F 2 "Resistor:R_0805" H 5800 3650 60  0001 C CNN
F 3 "" H 5800 3650 60  0000 C CNN
	1    5800 3650
	0    1    1    0   
$EndComp
$Comp
L standard:R R12
U 1 1 583C4352
P 6050 3650
F 0 "R12" V 6004 3728 50  0000 L CNN
F 1 "10k" H 6050 3650 50  0000 C CNN
F 2 "Resistor:R_0805" H 6050 3650 60  0001 C CNN
F 3 "" H 6050 3650 60  0000 C CNN
	1    6050 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	5800 3450 5800 3400
Wire Wire Line
	5550 3400 5800 3400
Wire Wire Line
	6050 3450 6050 3400
Connection ~ 6050 3400
Connection ~ 6300 3400
Wire Wire Line
	5800 3850 5800 3950
Connection ~ 5800 3950
Wire Wire Line
	6050 3850 6050 4050
Connection ~ 6050 4050
Connection ~ 5800 3400
Text HLabel 4250 3200 1    50   Input ~ 0
+3V3_IN
Text HLabel 6300 3250 1    50   Input ~ 0
+5V_IN
Wire Wire Line
	4250 3850 4250 3200
Wire Wire Line
	4250 4650 5550 4650
Connection ~ 5550 4650
Text HLabel 5550 4750 3    50   Input ~ 0
GND
Wire Wire Line
	5550 3850 5550 4250
Wire Wire Line
	7300 4350 6300 4350
Wire Wire Line
	7700 4350 7300 4350
Wire Wire Line
	8400 3900 8400 4350
Wire Wire Line
	8050 3900 8150 3900
Wire Wire Line
	7700 3750 7700 3800
Wire Wire Line
	7900 4350 7700 4350
Wire Wire Line
	6300 3750 6300 4100
Wire Wire Line
	6050 3400 6300 3400
Wire Wire Line
	6300 3400 6300 3750
Wire Wire Line
	5800 3950 6500 3950
Wire Wire Line
	6050 4050 6500 4050
Wire Wire Line
	5800 3400 6050 3400
Wire Wire Line
	5550 4650 5550 4750
Wire Wire Line
	5550 4650 7300 4650
$EndSCHEMATC
