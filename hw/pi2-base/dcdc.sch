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
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
Title "DC/DC Converter"
Date "2016-11-28"
Rev "1"
Comp "KoeWiBa"
Comment1 "Baseboard for Raspberry Pi 2"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5250 3350 5250 3900
Wire Wire Line
	4600 3650 4550 3650
Wire Wire Line
	6850 4200 6850 4450
Wire Wire Line
	5950 4000 5950 4450
Connection ~ 5950 4450
Wire Wire Line
	6850 4450 5250 4450
Wire Wire Line
	6400 3850 6250 3850
Connection ~ 5850 4450
Connection ~ 5250 3650
Wire Wire Line
	4100 3750 4150 3750
Wire Wire Line
	4150 3750 4150 3800
Wire Wire Line
	5450 3650 5000 3650
Wire Wire Line
	5850 4450 5850 4525
Connection ~ 5750 4450
Wire Wire Line
	6850 3850 6800 3850
Connection ~ 6850 3850
Connection ~ 6850 3650
Wire Wire Line
	6850 3650 6850 3900
Wire Wire Line
	5250 4450 5250 4200
Wire Wire Line
	5750 4450 5750 4000
Wire Wire Line
	6325 4300 6325 4450
Connection ~ 6325 4450
Wire Wire Line
	6325 3850 6325 3900
Connection ~ 6325 3850
Wire Wire Line
	4100 3650 4150 3650
$Comp
L LM2576 U4
U 1 1 52AD6D47
P 5850 3750
F 0 "U4" H 5850 4050 50  0000 C CNN
F 1 "LM2576" H 5850 3950 50  0000 C CNN
F 2 "Housings_TO:TO263_5" H 5850 3450 25  0001 C CNN
F 3 "" H 5850 3750 60  0001 C CNN
	1    5850 3750
	1    0    0    -1  
$EndComp
$Comp
L SCHOTTKY D1
U 1 1 526EC28B
P 6325 4100
F 0 "D1" H 6325 4200 50  0000 C CNN
F 1 "B 340 F" H 6325 4000 50  0000 C CNN
F 2 "Diode_SMD:D_SMC" H 6325 4100 60  0001 C CNN
F 3 "" H 6325 4100 60  0001 C CNN
	1    6325 4100
	0    -1   -1   0   
$EndComp
$Comp
L CP C6
U 1 1 526EBEBD
P 6850 4050
F 0 "C6" H 6850 3925 50  0000 C CNN
F 1 "1000uF" H 6850 4175 50  0000 C CNN
F 2 "Capacitors_THT:CP_RM5.08mm_D13.5mm" H 6850 4050 60  0001 C CNN
F 3 "" H 6850 4050 60  0001 C CNN
F 4 "25V" H 6850 4240 50  0000 C CNN "Voltage"
F 5 "+-10%" H 6850 4305 50  0000 C CNN "Tolrance"
	1    6850 4050
	0    1    1    0   
$EndComp
$Comp
L L L1
U 1 1 526EBE7B
P 6600 3850
F 0 "L1" H 6500 3750 50  0000 L CNN
F 1 "100uH" H 6500 3935 50  0000 L CNN
F 2 "Inductor:L_PISR" H 6600 3850 60  0001 C CNN
F 3 "" H 6600 3850 60  0001 C CNN
	1    6600 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 526EBE53
P 5850 4525
F 0 "#PWR024" H 5850 4325 50  0001 C CNN
F 1 "GND" H 5850 4415 50  0001 C CNN
F 2 "" H 5850 4525 60  0001 C CNN
F 3 "" H 5850 4525 60  0001 C CNN
	1    5850 4525
	1    0    0    -1  
$EndComp
$Comp
L CP C5
U 1 1 526EBE2E
P 5250 4050
F 0 "C5" H 5250 3925 50  0000 C CNN
F 1 "100uF" H 5250 4175 50  0000 C CNN
F 2 "Capacitors_THT:CP_RM5.08mm_D10.5mm" H 5250 4050 60  0001 C CNN
F 3 "" H 5250 4050 60  0001 C CNN
F 4 "63V" H 5250 4240 50  0000 C CNN "Voltage"
F 5 "+-10%" H 5250 4305 50  0000 C CNN "Tolrance"
	1    5250 4050
	0    1    1    0   
$EndComp
$Comp
L SWITCH SW2
U 1 1 526EA5A3
P 4100 3750
F 0 "SW2" H 4275 3925 50  0000 L BNN
F 1 "MAIN" H 4275 3725 50  0000 L BNN
F 2 "Connector_Header:PCB_CON2" H 4100 3750 60  0001 C CNN
F 3 "" H 4100 3750 60  0001 C CNN
	1    4100 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 526EA53B
P 4150 3800
F 0 "#PWR025" H 4150 3600 50  0001 C CNN
F 1 "GND" H 4150 3690 50  0001 C CNN
F 2 "" H 4150 3800 60  0001 C CNN
F 3 "" H 4150 3800 60  0001 C CNN
	1    4150 3800
	1    0    0    -1  
$EndComp
$Comp
L FUSE F3
U 1 1 583C6A9F
P 7150 3650
AR Path="/583C6A9F" Ref="F3"  Part="1" 
AR Path="/583C4215/583C6A9F" Ref="F3"  Part="1" 
F 0 "F3" H 7150 3865 50  0000 C CNN
F 1 "250V/2,5A T" H 7150 3774 50  0000 C CNN
F 2 "Electromechanical:FUSE5-20" H 7150 3650 60  0001 C CNN
F 3 "" H 7150 3650 60  0000 C CNN
	1    7150 3650
	1    0    0    -1  
$EndComp
$Comp
L FUSE F2
U 1 1 583C71B1
P 4800 3650
AR Path="/583C71B1" Ref="F2"  Part="1" 
AR Path="/583C4215/583C71B1" Ref="F2"  Part="1" 
F 0 "F2" H 4800 3865 50  0000 C CNN
F 1 "250V/2,5A T" H 4800 3774 50  0000 C CNN
F 2 "Electromechanical:FUSE5-20" H 4800 3650 60  0001 C CNN
F 3 "" H 4800 3650 60  0000 C CNN
	1    4800 3650
	1    0    0    -1  
$EndComp
Text HLabel 4100 3650 0    50   Input ~ 0
+24V_IN
Text HLabel 4100 3750 0    50   Input ~ 0
GND
Wire Wire Line
	6250 3650 6950 3650
Wire Wire Line
	7350 3650 7500 3650
Text HLabel 7500 3350 2    50   Output ~ 0
+24V_OUT
Text HLabel 7500 3650 2    50   Output ~ 0
+5V_OUT
Wire Wire Line
	7500 3350 5250 3350
Text Notes 5250 3200 0    60   ~ 0
Inductor selected for 24V input voltage and 2A output current
$EndSCHEMATC
