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
Sheet 5 5
Title "2 Relays"
Date "2016-12-02"
Rev "1"
Comp "KoeWiBa"
Comment1 "Baseboard for Raspberry PI 2B"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4550 4700 4600 4700
Connection ~ 4050 4700
Wire Wire Line
	4150 4700 3975 4700
Wire Wire Line
	7400 4700 7350 4700
Connection ~ 6850 5300
Wire Wire Line
	6850 5200 6850 5300
Connection ~ 7275 3000
Wire Wire Line
	7275 3600 7275 3000
Connection ~ 6475 5300
Connection ~ 6875 4050
Connection ~ 6875 4500
Wire Wire Line
	6875 4050 6875 4500
Wire Wire Line
	7275 4500 6475 4500
Wire Wire Line
	6475 5300 6475 4900
Wire Wire Line
	8075 5300 3675 5300
Wire Wire Line
	3675 5300 3675 4900
Wire Wire Line
	4475 4500 3675 4500
Wire Wire Line
	4225 3600 4225 2900
Wire Wire Line
	4225 2900 8025 2900
Wire Wire Line
	4225 4000 4225 4050
Wire Wire Line
	4225 4050 3925 4050
Wire Wire Line
	3925 4050 3925 3975
Wire Wire Line
	4475 4000 4475 4100
Wire Wire Line
	3725 3975 3725 4050
Wire Wire Line
	3725 4050 3525 4050
Wire Wire Line
	3525 4050 3525 3900
Wire Wire Line
	3525 3900 3375 3900
Wire Wire Line
	3675 3625 3675 3600
Wire Wire Line
	3675 3600 3475 3600
Wire Wire Line
	3475 3600 3475 3700
Wire Wire Line
	3475 3700 3375 3700
Wire Wire Line
	3375 3500 3775 3500
Wire Wire Line
	3775 3500 3775 3625
Wire Wire Line
	3925 3625 3925 3550
Wire Wire Line
	3925 3550 4225 3550
Connection ~ 4225 3550
Connection ~ 7025 3550
Wire Wire Line
	7025 3550 6725 3550
Wire Wire Line
	6725 3550 6725 3625
Wire Wire Line
	6575 3625 6575 3500
Wire Wire Line
	6575 3500 6175 3500
Wire Wire Line
	6175 3700 6275 3700
Wire Wire Line
	6275 3700 6275 3600
Wire Wire Line
	6275 3600 6475 3600
Wire Wire Line
	6475 3600 6475 3625
Wire Wire Line
	6175 3900 6325 3900
Wire Wire Line
	6325 3900 6325 4050
Wire Wire Line
	6325 4050 6525 4050
Wire Wire Line
	6525 4050 6525 3975
Wire Wire Line
	7275 4000 7275 4100
Wire Wire Line
	6725 3975 6725 4050
Wire Wire Line
	6725 4050 7025 4050
Wire Wire Line
	7025 4050 7025 4000
Wire Wire Line
	7025 2900 7025 3600
Connection ~ 7025 2900
Wire Wire Line
	4075 4050 4075 4500
Connection ~ 4075 4500
Connection ~ 4075 4050
Wire Wire Line
	4475 3600 4475 3000
Wire Wire Line
	4475 3000 8025 3000
Wire Wire Line
	6850 4700 6850 4800
Wire Wire Line
	6950 4700 6775 4700
Connection ~ 6850 4700
Wire Wire Line
	4050 4700 4050 4800
Wire Wire Line
	4050 5200 4050 5300
Connection ~ 4050 5300
Text HLabel 8025 3000 2    50   Input ~ 0
LEDVCC
$Comp
L BSS138 Q2
U 1 1 527208FD
P 6575 4700
F 0 "Q2" H 6725 4600 50  0000 L CNN
F 1 "BSS138" H 6725 4800 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 6765 4700 25  0001 C CNN
F 3 "" H 6575 4700 60  0001 C CNN
	1    6575 4700
	-1   0    0    -1  
$EndComp
$Comp
L R R25
U 1 1 527208FC
P 7150 4700
F 0 "R25" H 7150 4600 50  0000 C CNN
F 1 "100" H 7150 4700 50  0000 C CNN
F 2 "Resistor:R_0805" H 7150 4700 60  0001 C CNN
F 3 "" H 7150 4700 60  0001 C CNN
F 4 "~" H 7275 4690 50  0001 L CNN "Tolerance"
F 5 "~" H 7275 4630 50  0001 L CNN "Nom. Power"
	1    7150 4700
	1    0    0    -1  
$EndComp
$Comp
L R R22
U 1 1 527208A4
P 4350 4700
F 0 "R22" H 4350 4600 50  0000 C CNN
F 1 "100" H 4350 4700 50  0000 C CNN
F 2 "Resistor:R_0805" H 4350 4700 60  0001 C CNN
F 3 "" H 4350 4700 60  0001 C CNN
F 4 "~" H 4475 4690 50  0001 L CNN "Tolerance"
F 5 "~" H 4475 4630 50  0001 L CNN "Nom. Power"
	1    4350 4700
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q1
U 1 1 5272082F
P 3775 4700
F 0 "Q1" H 3925 4600 50  0000 L CNN
F 1 "BSS138" H 3925 4800 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 3965 4700 25  0001 C CNN
F 3 "" H 3775 4700 60  0001 C CNN
	1    3775 4700
	-1   0    0    -1  
$EndComp
Text HLabel 8075 5300 2    50   Input ~ 0
GND
Text HLabel 8025 2900 2    50   Input ~ 0
VCC
Text HLabel 6175 3700 0    50   UnSpc ~ 0
REL2_NC
Text HLabel 6175 3500 0    50   UnSpc ~ 0
REL2_NO
Text HLabel 6175 3900 0    50   UnSpc ~ 0
REL2_COM
$Comp
L OMRON_G5LE-1-RESCUE-pi2-base REL2
U 1 1 526EA20C
P 6725 3800
F 0 "REL2" H 6750 3700 50  0000 C CNN
F 1 "OMRON_G5LE-1" V 7075 3925 50  0000 C CNN
F 2 "Relay:omron_G5LE-1" H 6725 3800 60  0001 C CNN
F 3 "" H 6725 3800 60  0001 C CNN
	1    6725 3800
	0    -1   -1   0   
$EndComp
$Comp
L D D8
U 1 1 526EA20B
P 7025 3800
F 0 "D8" H 7035 3880 50  0000 C CNN
F 1 "1N4148" H 7025 3725 50  0000 C CNN
F 2 "Diode_SMD:D_MINIMELF" H 7025 3800 60  0001 C CNN
F 3 "" H 7025 3800 60  0001 C CNN
	1    7025 3800
	0    -1   -1   0   
$EndComp
$Comp
L R R24
U 1 1 526EA20A
P 6850 5000
F 0 "R24" H 6850 4900 50  0000 C CNN
F 1 "10k" H 6850 5000 50  0000 C CNN
F 2 "Resistor:R_0805" H 6850 5000 60  0001 C CNN
F 3 "" H 6850 5000 60  0001 C CNN
F 4 "~" H 6975 4990 50  0001 L CNN "Tolerance"
F 5 "~" H 6975 4930 50  0001 L CNN "Nom. Power"
	1    6850 5000
	0    -1   -1   0   
$EndComp
$Comp
L R R26
U 1 1 526EA209
P 7275 3800
F 0 "R26" H 7275 3700 50  0000 C CNN
F 1 "1k" H 7275 3800 50  0000 C CNN
F 2 "Resistor:R_0805" H 7275 3800 60  0001 C CNN
F 3 "" H 7275 3800 60  0001 C CNN
F 4 "~" H 7400 3790 50  0001 L CNN "Tolerance"
F 5 "~" H 7400 3730 50  0001 L CNN "Nom. Power"
	1    7275 3800
	0    -1   -1   0   
$EndComp
$Comp
L LED D9
U 1 1 526EA208
P 7125 4100
F 0 "D9" H 7125 4300 50  0000 L CNN
F 1 "LED 2,2V 25mA" H 7000 4425 50  0000 L CNN
F 2 "LED:LED_RM2.54mm_D3mm" H 7125 4100 25  0001 C CNN
F 3 "" H 7125 4100 60  0001 C CNN
	1    7125 4100
	0    1    1    0   
$EndComp
Text HLabel 7400 4700 2    50   Input ~ 0
REL2
Text HLabel 4600 4700 2    50   Input ~ 0
REL1
$Comp
L LED D7
U 1 1 526EA113
P 4325 4100
F 0 "D7" H 4325 4300 50  0000 L CNN
F 1 "LED 2,2V 25mA" H 4225 4425 50  0000 L CNN
F 2 "LED:LED_RM2.54mm_D3mm" H 4325 4100 60  0001 C CNN
F 3 "" H 4325 4100 60  0001 C CNN
	1    4325 4100
	0    1    1    0   
$EndComp
$Comp
L R R23
U 1 1 526EA0FD
P 4475 3800
F 0 "R23" H 4475 3700 50  0000 C CNN
F 1 "1k" H 4475 3800 50  0000 C CNN
F 2 "Resistor:R_0805" H 4475 3800 60  0001 C CNN
F 3 "" H 4475 3800 60  0001 C CNN
F 4 "~" H 4600 3790 50  0001 L CNN "Tolerance"
F 5 "~" H 4600 3730 50  0001 L CNN "Nom. Power"
	1    4475 3800
	0    -1   -1   0   
$EndComp
$Comp
L R R21
U 1 1 526EA09B
P 4050 5000
F 0 "R21" H 4050 4900 50  0000 C CNN
F 1 "10k" H 4050 5000 50  0000 C CNN
F 2 "Resistor:R_0805" H 4050 5000 60  0001 C CNN
F 3 "" H 4050 5000 60  0001 C CNN
F 4 "~" H 4175 4990 50  0001 L CNN "Tolerance"
F 5 "~" H 4175 4930 50  0001 L CNN "Nom. Power"
	1    4050 5000
	0    -1   -1   0   
$EndComp
$Comp
L D D6
U 1 1 526EA082
P 4225 3800
F 0 "D6" H 4235 3880 50  0000 C CNN
F 1 "1N4148" H 4225 3725 50  0000 C CNN
F 2 "Diode_SMD:D_MINIMELF" H 4225 3800 60  0001 C CNN
F 3 "" H 4225 3800 60  0001 C CNN
	1    4225 3800
	0    -1   -1   0   
$EndComp
$Comp
L OMRON_G5LE-1-RESCUE-pi2-base REL1
U 1 1 526EA068
P 3925 3800
F 0 "REL1" H 3950 3700 50  0000 C CNN
F 1 "OMRON_G5LE-1" V 4275 3925 50  0000 C CNN
F 2 "Relay:omron_G5LE-1" H 3925 3800 60  0001 C CNN
F 3 "" H 3925 3800 60  0001 C CNN
	1    3925 3800
	0    -1   -1   0   
$EndComp
Text HLabel 3375 3900 0    50   UnSpc ~ 0
REL1_COM
Text HLabel 3375 3500 0    50   UnSpc ~ 0
REL1_NO
Text HLabel 3375 3700 0    50   UnSpc ~ 0
REL1_NC
$EndSCHEMATC
