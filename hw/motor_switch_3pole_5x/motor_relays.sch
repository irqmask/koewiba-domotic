EESchema Schematic File Version 2
LIBS:diodes
LIBS:ics_controller_atmel
LIBS:ics_misc
LIBS:ics_peripheral
LIBS:logo
LIBS:analog_devices
LIBS:connectors
LIBS:display
LIBS:encoder
LIBS:ftdi
LIBS:ics_40xx
LIBS:ics_74xx
LIBS:ics_opamp
LIBS:ics_opto
LIBS:ics_regulator
LIBS:ics_signal
LIBS:modules
LIBS:power
LIBS:relays
LIBS:sensors
LIBS:standard
LIBS:transformer
LIBS:transistors
LIBS:motor_switch_3pole_5x-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title "motor switch 3pole 5x (motor relays)"
Date "2018-05-29"
Rev "1"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +24V #PWR010
U 1 1 572DA356
P 3850 1750
F 0 "#PWR010" H 3850 1950 50  0001 C CNN
F 1 "+24V" H 3850 1860 50  0000 C CNN
F 2 "" H 3850 1750 60  0000 C CNN
F 3 "" H 3850 1750 60  0000 C CNN
	1    3850 1750
	1    0    0    -1  
$EndComp
$Comp
L D D1
U 1 1 572DA3A8
P 4150 1150
F 0 "D1" H 4150 1250 50  0000 C CNN
F 1 "1N4148" H 4150 1050 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 4150 1150 60  0001 C CNN
F 3 "" H 4150 1150 60  0000 C CNN
	1    4150 1150
	0    -1   -1   0   
$EndComp
$Comp
L D D2
U 1 1 572DA4C3
P 4150 2050
F 0 "D2" H 4150 2150 50  0000 C CNN
F 1 "1N4148" H 4150 1950 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 4150 2050 60  0001 C CNN
F 3 "" H 4150 2050 60  0000 C CNN
	1    4150 2050
	0    -1   -1   0   
$EndComp
$Comp
L BSS138 Q2
U 1 1 572DAB4D
P 4850 2000
F 0 "Q2" H 5000 1900 50  0000 L CNN
F 1 "BSS138" H 5000 2100 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5040 2000 25  0001 C CNN
F 3 "" H 4850 2000 60  0000 C CNN
	1    4850 2000
	-1   0    0    -1  
$EndComp
$Comp
L BSS138 Q1
U 1 1 572DAC4A
P 4850 1050
F 0 "Q1" H 5000 950 50  0000 L CNN
F 1 "BSS138" H 5000 1150 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5040 1050 25  0001 C CNN
F 3 "" H 4850 1050 60  0000 C CNN
	1    4850 1050
	-1   0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 572DAC83
P 5450 1050
F 0 "R7" H 5450 950 50  0000 C CNN
F 1 "100" H 5450 1050 50  0000 C CNN
F 2 "Resistor:R_0805" H 5450 1050 60  0001 C CNN
F 3 "" H 5450 1050 60  0000 C CNN
	1    5450 1050
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 572DAD70
P 5150 1350
F 0 "R1" H 5150 1250 50  0000 C CNN
F 1 "10k" H 5150 1350 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 1350 60  0001 C CNN
F 3 "" H 5150 1350 60  0000 C CNN
	1    5150 1350
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 572DADE3
P 5150 2300
F 0 "R2" H 5150 2200 50  0000 C CNN
F 1 "10k" H 5150 2300 50  0000 C CNN
F 2 "Resistor:R_0805" H 5150 2300 60  0001 C CNN
F 3 "" H 5150 2300 60  0000 C CNN
	1    5150 2300
	0    1    1    0   
$EndComp
$Comp
L R R8
U 1 1 572DAE46
P 5450 2000
F 0 "R8" H 5450 1900 50  0000 C CNN
F 1 "100" H 5450 2000 50  0000 C CNN
F 2 "Resistor:R_0805" H 5450 2000 60  0001 C CNN
F 3 "" H 5450 2000 60  0000 C CNN
	1    5450 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 572DB59F
P 4750 2600
F 0 "#PWR011" H 4750 2400 50  0001 C CNN
F 1 "GND" H 4750 2490 50  0001 C CNN
F 2 "" H 4750 2600 60  0000 C CNN
F 3 "" H 4750 2600 60  0000 C CNN
	1    4750 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 572DB5CD
P 4750 1650
F 0 "#PWR012" H 4750 1450 50  0001 C CNN
F 1 "GND" H 4750 1540 50  0001 C CNN
F 2 "" H 4750 1650 60  0000 C CNN
F 3 "" H 4750 1650 60  0000 C CNN
	1    4750 1650
	1    0    0    -1  
$EndComp
Text Label 6050 1050 2    40   ~ 0
motor_0_up
Text Label 6050 2000 2    40   ~ 0
motor_0_on
$Comp
L OPEN_HARDWARE_1 LOGO2
U 1 1 5B10FE77
P 975 7350
F 0 "LOGO2" H 975 7625 60  0000 C CNN
F 1 "OPEN_HARDWARE_1" H 975 7125 60  0000 C CNN
F 2 "Symbol:Symbol_OSHW-Logo_Copper" H 975 7350 60  0001 C CNN
F 3 "" H 975 7350 60  0000 C CNN
	1    975  7350
	1    0    0    -1  
$EndComp
Text Label 3000 800  0    40   ~ 0
MOTOR_0_230V_UP
Text Label 3000 900  0    40   ~ 0
MOTOR_0_230V_DOWN
Text Label 3650 1600 1    40   ~ 0
M0_230V_ON
$Comp
L FINDER_34.51.7.024 REL2
U 1 1 5842EAE4
P 3850 2050
F 0 "REL2" V 4000 2000 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 4350 2475 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 3850 2050 60  0001 C CNN
F 3 "" H 3850 2050 60  0001 C CNN
	1    3850 2050
	0    -1   -1   0   
$EndComp
$Comp
L FINDER_34.51.7.024 REL1
U 1 1 5842E555
P 3850 1150
F 0 "REL1" V 4000 1100 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 4000 1650 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 3850 1150 60  0001 C CNN
F 3 "" H 3850 1150 60  0001 C CNN
	1    3850 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 1250 4750 1650
Connection ~ 4150 1400
Connection ~ 4150 2300
Wire Wire Line
	5050 1050 5250 1050
Wire Wire Line
	5150 1150 5150 1050
Connection ~ 5150 1050
Wire Wire Line
	5050 2000 5250 2000
Wire Wire Line
	5150 2100 5150 2000
Connection ~ 5150 2000
Wire Wire Line
	5150 2500 5150 2550
Wire Wire Line
	5150 2550 4750 2550
Wire Wire Line
	4750 2200 4750 2600
Connection ~ 4750 2550
Wire Wire Line
	4750 1800 4750 1750
Wire Wire Line
	3850 2300 3850 2250
Wire Wire Line
	3850 1850 3850 1750
Wire Wire Line
	3850 1800 4150 1800
Wire Wire Line
	4150 1800 4150 1850
Connection ~ 3850 1800
Wire Wire Line
	4150 2250 4150 2300
Wire Wire Line
	5150 1550 5150 1600
Wire Wire Line
	5150 1600 4750 1600
Connection ~ 4750 1600
Wire Wire Line
	4150 1350 4150 1400
Wire Wire Line
	3850 1350 3850 1400
Wire Wire Line
	4150 950  4150 900 
Wire Wire Line
	4150 900  3850 900 
Wire Wire Line
	3850 850  3850 950 
Connection ~ 3850 900 
Wire Wire Line
	3650 2350 3650 2250
Wire Wire Line
	3100 2350 3650 2350
$Comp
L +24V #PWR013
U 1 1 584354C0
P 3850 850
F 0 "#PWR013" H 3850 1050 50  0001 C CNN
F 1 "+24V" H 3850 1004 50  0000 C CNN
F 2 "" H 3850 850 60  0001 C CNN
F 3 "" H 3850 850 60  0001 C CNN
	1    3850 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 950  3600 900 
Wire Wire Line
	3600 900  2900 900 
Wire Wire Line
	3700 950  3700 800 
Wire Wire Line
	3700 800  2900 800 
$Comp
L CONN_2X1 J2
U 1 1 5B0D4D4D
P 2700 850
F 0 "J2" H 2775 1025 50  0000 C CNN
F 1 "WAGO 2081-1122" V 2875 850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO2081-1122" H 2700 850 60  0001 C CNN
F 3 "" H 2700 850 60  0000 C CNN
	1    2700 850 
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 2300 4350 2300
Wire Wire Line
	4350 2300 4350 1750
Wire Wire Line
	4350 1750 4750 1750
Wire Wire Line
	3850 1400 4350 1400
Wire Wire Line
	4350 1400 4350 850 
Wire Wire Line
	4350 850  4750 850 
Wire Wire Line
	3700 1850 3700 1600
Wire Wire Line
	3700 1600 3650 1600
Wire Wire Line
	3650 1600 3650 1350
Text Label 3100 2350 0    40   ~ 0
230V
$Comp
L +24V #PWR014
U 1 1 5B0D5EB1
P 3900 3750
F 0 "#PWR014" H 3900 3950 50  0001 C CNN
F 1 "+24V" H 3900 3900 50  0000 C CNN
F 2 "" H 3900 3750 60  0000 C CNN
F 3 "" H 3900 3750 60  0000 C CNN
	1    3900 3750
	1    0    0    -1  
$EndComp
$Comp
L D D3
U 1 1 5B0D5EB7
P 4200 3150
F 0 "D3" H 4200 3250 50  0000 C CNN
F 1 "1N4148" H 4200 3050 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 4200 3150 60  0001 C CNN
F 3 "" H 4200 3150 60  0000 C CNN
	1    4200 3150
	0    -1   -1   0   
$EndComp
$Comp
L D D4
U 1 1 5B0D5EBD
P 4200 4050
F 0 "D4" H 4200 4150 50  0000 C CNN
F 1 "1N4148" H 4200 3950 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 4200 4050 60  0001 C CNN
F 3 "" H 4200 4050 60  0000 C CNN
	1    4200 4050
	0    -1   -1   0   
$EndComp
$Comp
L BSS138 Q4
U 1 1 5B0D5EC3
P 4900 4000
F 0 "Q4" H 5050 3900 50  0000 L CNN
F 1 "BSS138" H 5050 4100 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5090 4000 25  0001 C CNN
F 3 "" H 4900 4000 60  0000 C CNN
	1    4900 4000
	-1   0    0    -1  
$EndComp
$Comp
L BSS138 Q3
U 1 1 5B0D5EC9
P 4900 3050
F 0 "Q3" H 5050 2950 50  0000 L CNN
F 1 "BSS138" H 5050 3150 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5090 3050 25  0001 C CNN
F 3 "" H 4900 3050 60  0000 C CNN
	1    4900 3050
	-1   0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5B0D5ECF
P 5500 3050
F 0 "R9" H 5500 2950 50  0000 C CNN
F 1 "100" H 5500 3050 50  0000 C CNN
F 2 "Resistor:R_0805" H 5500 3050 60  0001 C CNN
F 3 "" H 5500 3050 60  0000 C CNN
	1    5500 3050
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5B0D5ED5
P 5200 3350
F 0 "R3" H 5200 3250 50  0000 C CNN
F 1 "10k" H 5200 3350 50  0000 C CNN
F 2 "Resistor:R_0805" H 5200 3350 60  0001 C CNN
F 3 "" H 5200 3350 60  0000 C CNN
	1    5200 3350
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 5B0D5EDB
P 5200 4300
F 0 "R4" H 5200 4200 50  0000 C CNN
F 1 "10k" H 5200 4300 50  0000 C CNN
F 2 "Resistor:R_0805" H 5200 4300 60  0001 C CNN
F 3 "" H 5200 4300 60  0000 C CNN
	1    5200 4300
	0    1    1    0   
$EndComp
$Comp
L R R10
U 1 1 5B0D5EE1
P 5500 4000
F 0 "R10" H 5500 3900 50  0000 C CNN
F 1 "100" H 5500 4000 50  0000 C CNN
F 2 "Resistor:R_0805" H 5500 4000 60  0001 C CNN
F 3 "" H 5500 4000 60  0000 C CNN
	1    5500 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5B0D5EE7
P 4800 4600
F 0 "#PWR015" H 4800 4400 50  0001 C CNN
F 1 "GND" H 4800 4490 50  0001 C CNN
F 2 "" H 4800 4600 60  0000 C CNN
F 3 "" H 4800 4600 60  0000 C CNN
	1    4800 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5B0D5EED
P 4800 3650
F 0 "#PWR016" H 4800 3450 50  0001 C CNN
F 1 "GND" H 4800 3540 50  0001 C CNN
F 2 "" H 4800 3650 60  0000 C CNN
F 3 "" H 4800 3650 60  0000 C CNN
	1    4800 3650
	1    0    0    -1  
$EndComp
Text Label 6100 3050 2    40   ~ 0
motor_1_up
Text Label 6100 4000 2    40   ~ 0
motor_1_on
Text Label 3050 2800 0    40   ~ 0
MOTOR_1_230V_UP
Text Label 3050 2900 0    40   ~ 0
MOTOR_1_230V_DOWN
Text Label 3700 3600 1    40   ~ 0
M1_230V_ON
$Comp
L FINDER_34.51.7.024 REL4
U 1 1 5B0D5EF8
P 3900 4050
F 0 "REL4" V 4050 4000 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 4400 4475 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 3900 4050 60  0001 C CNN
F 3 "" H 3900 4050 60  0001 C CNN
	1    3900 4050
	0    -1   -1   0   
$EndComp
$Comp
L FINDER_34.51.7.024 REL3
U 1 1 5B0D5EFE
P 3900 3150
F 0 "REL3" V 4050 3100 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 4050 3650 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 3900 3150 60  0001 C CNN
F 3 "" H 3900 3150 60  0001 C CNN
	1    3900 3150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4800 3250 4800 3650
Connection ~ 4200 3400
Connection ~ 4200 4300
Wire Wire Line
	5100 3050 5300 3050
Wire Wire Line
	5200 3150 5200 3050
Connection ~ 5200 3050
Wire Wire Line
	5100 4000 5300 4000
Wire Wire Line
	5200 4100 5200 4000
Connection ~ 5200 4000
Wire Wire Line
	5200 4500 5200 4550
Wire Wire Line
	5200 4550 4800 4550
Wire Wire Line
	4800 4200 4800 4600
Connection ~ 4800 4550
Wire Wire Line
	4800 3800 4800 3750
Wire Wire Line
	3900 4300 3900 4250
Wire Wire Line
	3900 3850 3900 3750
Wire Wire Line
	3900 3800 4200 3800
Wire Wire Line
	4200 3800 4200 3850
Connection ~ 3900 3800
Wire Wire Line
	4200 4250 4200 4300
Wire Wire Line
	5200 3550 5200 3600
Wire Wire Line
	5200 3600 4800 3600
Connection ~ 4800 3600
Wire Wire Line
	4200 3350 4200 3400
Wire Wire Line
	3900 3350 3900 3400
Wire Wire Line
	4200 2950 4200 2900
Wire Wire Line
	4200 2900 3900 2900
Wire Wire Line
	3900 2850 3900 2950
Connection ~ 3900 2900
Wire Wire Line
	3700 4350 3700 4250
Wire Wire Line
	3150 4350 3700 4350
$Comp
L +24V #PWR017
U 1 1 5B0D5F25
P 3900 2850
F 0 "#PWR017" H 3900 3050 50  0001 C CNN
F 1 "+24V" H 3900 3004 50  0000 C CNN
F 2 "" H 3900 2850 60  0001 C CNN
F 3 "" H 3900 2850 60  0001 C CNN
	1    3900 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2950 3650 2900
Wire Wire Line
	3650 2900 2950 2900
Wire Wire Line
	3750 2950 3750 2800
Wire Wire Line
	3750 2800 2950 2800
$Comp
L CONN_2X1 J3
U 1 1 5B0D5F33
P 2750 2850
F 0 "J3" H 2825 3025 50  0000 C CNN
F 1 "WAGO 2081-1122" V 2925 2850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO2081-1122" H 2750 2850 60  0001 C CNN
F 3 "" H 2750 2850 60  0000 C CNN
	1    2750 2850
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 4300 4400 4300
Wire Wire Line
	4400 4300 4400 3750
Wire Wire Line
	4400 3750 4800 3750
Wire Wire Line
	3900 3400 4400 3400
Wire Wire Line
	4400 3400 4400 2850
Wire Wire Line
	4400 2850 4800 2850
Wire Wire Line
	3750 3850 3750 3600
Wire Wire Line
	3750 3600 3700 3600
Wire Wire Line
	3700 3600 3700 3350
Text Label 3150 4350 0    40   ~ 0
230V
$Comp
L +24V #PWR018
U 1 1 5B0D642B
P 8400 1750
F 0 "#PWR018" H 8400 1950 50  0001 C CNN
F 1 "+24V" H 8400 1900 50  0000 C CNN
F 2 "" H 8400 1750 60  0000 C CNN
F 3 "" H 8400 1750 60  0000 C CNN
	1    8400 1750
	1    0    0    -1  
$EndComp
$Comp
L D D7
U 1 1 5B0D6431
P 8700 1150
F 0 "D7" H 8700 1250 50  0000 C CNN
F 1 "1N4148" H 8700 1050 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 8700 1150 60  0001 C CNN
F 3 "" H 8700 1150 60  0000 C CNN
	1    8700 1150
	0    -1   -1   0   
$EndComp
$Comp
L D D8
U 1 1 5B0D6437
P 8700 2050
F 0 "D8" H 8700 2150 50  0000 C CNN
F 1 "1N4148" H 8700 1950 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 8700 2050 60  0001 C CNN
F 3 "" H 8700 2050 60  0000 C CNN
	1    8700 2050
	0    -1   -1   0   
$EndComp
$Comp
L BSS138 Q8
U 1 1 5B0D643D
P 9400 2000
F 0 "Q8" H 9550 1900 50  0000 L CNN
F 1 "BSS138" H 9550 2100 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 9590 2000 25  0001 C CNN
F 3 "" H 9400 2000 60  0000 C CNN
	1    9400 2000
	-1   0    0    -1  
$EndComp
$Comp
L BSS138 Q7
U 1 1 5B0D6443
P 9400 1050
F 0 "Q7" H 9550 950 50  0000 L CNN
F 1 "BSS138" H 9550 1150 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 9590 1050 25  0001 C CNN
F 3 "" H 9400 1050 60  0000 C CNN
	1    9400 1050
	-1   0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 5B0D6449
P 10000 1050
F 0 "R17" H 10000 950 50  0000 C CNN
F 1 "100" H 10000 1050 50  0000 C CNN
F 2 "Resistor:R_0805" H 10000 1050 60  0001 C CNN
F 3 "" H 10000 1050 60  0000 C CNN
	1    10000 1050
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 5B0D644F
P 9700 1350
F 0 "R13" H 9700 1250 50  0000 C CNN
F 1 "10k" H 9700 1350 50  0000 C CNN
F 2 "Resistor:R_0805" H 9700 1350 60  0001 C CNN
F 3 "" H 9700 1350 60  0000 C CNN
	1    9700 1350
	0    1    1    0   
$EndComp
$Comp
L R R14
U 1 1 5B0D6455
P 9700 2300
F 0 "R14" H 9700 2200 50  0000 C CNN
F 1 "10k" H 9700 2300 50  0000 C CNN
F 2 "Resistor:R_0805" H 9700 2300 60  0001 C CNN
F 3 "" H 9700 2300 60  0000 C CNN
	1    9700 2300
	0    1    1    0   
$EndComp
$Comp
L R R18
U 1 1 5B0D645B
P 10000 2000
F 0 "R18" H 10000 1900 50  0000 C CNN
F 1 "100" H 10000 2000 50  0000 C CNN
F 2 "Resistor:R_0805" H 10000 2000 60  0001 C CNN
F 3 "" H 10000 2000 60  0000 C CNN
	1    10000 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5B0D6461
P 9300 2600
F 0 "#PWR019" H 9300 2400 50  0001 C CNN
F 1 "GND" H 9300 2490 50  0001 C CNN
F 2 "" H 9300 2600 60  0000 C CNN
F 3 "" H 9300 2600 60  0000 C CNN
	1    9300 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 5B0D6467
P 9300 1650
F 0 "#PWR020" H 9300 1450 50  0001 C CNN
F 1 "GND" H 9300 1540 50  0001 C CNN
F 2 "" H 9300 1650 60  0000 C CNN
F 3 "" H 9300 1650 60  0000 C CNN
	1    9300 1650
	1    0    0    -1  
$EndComp
Text Label 10600 1050 2    40   ~ 0
motor_3_up
Text Label 10600 2000 2    40   ~ 0
motor_3_on
Text Label 7550 800  0    40   ~ 0
MOTOR_3_230V_UP
Text Label 7550 900  0    40   ~ 0
MOTOR_3_230V_DOWN
Text Label 8200 1600 1    40   ~ 0
M3_230V_ON
$Comp
L FINDER_34.51.7.024 REL8
U 1 1 5B0D6472
P 8400 2050
F 0 "REL8" V 8550 2000 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 8900 2475 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 8400 2050 60  0001 C CNN
F 3 "" H 8400 2050 60  0001 C CNN
	1    8400 2050
	0    -1   -1   0   
$EndComp
$Comp
L FINDER_34.51.7.024 REL7
U 1 1 5B0D6478
P 8400 1150
F 0 "REL7" V 8550 1100 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 8550 1650 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 8400 1150 60  0001 C CNN
F 3 "" H 8400 1150 60  0001 C CNN
	1    8400 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9300 1250 9300 1650
Connection ~ 8700 1400
Connection ~ 8700 2300
Wire Wire Line
	9600 1050 9800 1050
Wire Wire Line
	9700 1150 9700 1050
Connection ~ 9700 1050
Wire Wire Line
	9600 2000 9800 2000
Wire Wire Line
	9700 2100 9700 2000
Connection ~ 9700 2000
Wire Wire Line
	9700 2500 9700 2550
Wire Wire Line
	9700 2550 9300 2550
Wire Wire Line
	9300 2200 9300 2600
Connection ~ 9300 2550
Wire Wire Line
	9300 1800 9300 1750
Wire Wire Line
	8400 2300 8400 2250
Wire Wire Line
	8400 1850 8400 1750
Wire Wire Line
	8400 1800 8700 1800
Wire Wire Line
	8700 1800 8700 1850
Connection ~ 8400 1800
Wire Wire Line
	8700 2250 8700 2300
Wire Wire Line
	9700 1550 9700 1600
Wire Wire Line
	9700 1600 9300 1600
Connection ~ 9300 1600
Wire Wire Line
	8700 1350 8700 1400
Wire Wire Line
	8400 1350 8400 1400
Wire Wire Line
	8700 950  8700 900 
Wire Wire Line
	8700 900  8400 900 
Wire Wire Line
	8400 850  8400 950 
Connection ~ 8400 900 
Wire Wire Line
	8200 2350 8200 2250
Wire Wire Line
	7650 2350 8200 2350
$Comp
L +24V #PWR021
U 1 1 5B0D649F
P 8400 850
F 0 "#PWR021" H 8400 1050 50  0001 C CNN
F 1 "+24V" H 8400 1004 50  0000 C CNN
F 2 "" H 8400 850 60  0001 C CNN
F 3 "" H 8400 850 60  0001 C CNN
	1    8400 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 950  8150 900 
Wire Wire Line
	8150 900  7450 900 
Wire Wire Line
	8250 950  8250 800 
Wire Wire Line
	8250 800  7450 800 
$Comp
L CONN_2X1 J5
U 1 1 5B0D64AD
P 7250 850
F 0 "J5" H 7325 1025 50  0000 C CNN
F 1 "WAGO 2081-1122" V 7425 850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO2081-1122" H 7250 850 60  0001 C CNN
F 3 "" H 7250 850 60  0000 C CNN
	1    7250 850 
	-1   0    0    1   
$EndComp
Wire Wire Line
	8400 2300 8900 2300
Wire Wire Line
	8900 2300 8900 1750
Wire Wire Line
	8900 1750 9300 1750
Wire Wire Line
	8400 1400 8900 1400
Wire Wire Line
	8900 1400 8900 850 
Wire Wire Line
	8900 850  9300 850 
Wire Wire Line
	8250 1850 8250 1600
Wire Wire Line
	8250 1600 8200 1600
Wire Wire Line
	8200 1600 8200 1350
Text Label 7650 2350 0    40   ~ 0
230V
$Comp
L +24V #PWR022
U 1 1 5B0D64BD
P 8450 3750
F 0 "#PWR022" H 8450 3950 50  0001 C CNN
F 1 "+24V" H 8450 3900 50  0000 C CNN
F 2 "" H 8450 3750 60  0000 C CNN
F 3 "" H 8450 3750 60  0000 C CNN
	1    8450 3750
	1    0    0    -1  
$EndComp
$Comp
L D D9
U 1 1 5B0D64C3
P 8750 3150
F 0 "D9" H 8750 3250 50  0000 C CNN
F 1 "1N4148" H 8750 3050 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 8750 3150 60  0001 C CNN
F 3 "" H 8750 3150 60  0000 C CNN
	1    8750 3150
	0    -1   -1   0   
$EndComp
$Comp
L D D10
U 1 1 5B0D64C9
P 8750 4050
F 0 "D10" H 8750 4150 50  0000 C CNN
F 1 "1N4148" H 8750 3950 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 8750 4050 60  0001 C CNN
F 3 "" H 8750 4050 60  0000 C CNN
	1    8750 4050
	0    -1   -1   0   
$EndComp
$Comp
L BSS138 Q10
U 1 1 5B0D64CF
P 9450 4000
F 0 "Q10" H 9600 3900 50  0000 L CNN
F 1 "BSS138" H 9600 4100 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 9640 4000 25  0001 C CNN
F 3 "" H 9450 4000 60  0000 C CNN
	1    9450 4000
	-1   0    0    -1  
$EndComp
$Comp
L BSS138 Q9
U 1 1 5B0D64D5
P 9450 3050
F 0 "Q9" H 9600 2950 50  0000 L CNN
F 1 "BSS138" H 9600 3150 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 9640 3050 25  0001 C CNN
F 3 "" H 9450 3050 60  0000 C CNN
	1    9450 3050
	-1   0    0    -1  
$EndComp
$Comp
L R R19
U 1 1 5B0D64DB
P 10050 3050
F 0 "R19" H 10050 2950 50  0000 C CNN
F 1 "100" H 10050 3050 50  0000 C CNN
F 2 "Resistor:R_0805" H 10050 3050 60  0001 C CNN
F 3 "" H 10050 3050 60  0000 C CNN
	1    10050 3050
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 5B0D64E1
P 9750 3350
F 0 "R15" H 9750 3250 50  0000 C CNN
F 1 "10k" H 9750 3350 50  0000 C CNN
F 2 "Resistor:R_0805" H 9750 3350 60  0001 C CNN
F 3 "" H 9750 3350 60  0000 C CNN
	1    9750 3350
	0    1    1    0   
$EndComp
$Comp
L R R16
U 1 1 5B0D64E7
P 9750 4300
F 0 "R16" H 9750 4200 50  0000 C CNN
F 1 "10k" H 9750 4300 50  0000 C CNN
F 2 "Resistor:R_0805" H 9750 4300 60  0001 C CNN
F 3 "" H 9750 4300 60  0000 C CNN
	1    9750 4300
	0    1    1    0   
$EndComp
$Comp
L R R20
U 1 1 5B0D64ED
P 10050 4000
F 0 "R20" H 10050 3900 50  0000 C CNN
F 1 "100" H 10050 4000 50  0000 C CNN
F 2 "Resistor:R_0805" H 10050 4000 60  0001 C CNN
F 3 "" H 10050 4000 60  0000 C CNN
	1    10050 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5B0D64F3
P 9350 4600
F 0 "#PWR023" H 9350 4400 50  0001 C CNN
F 1 "GND" H 9350 4490 50  0001 C CNN
F 2 "" H 9350 4600 60  0000 C CNN
F 3 "" H 9350 4600 60  0000 C CNN
	1    9350 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 5B0D64F9
P 9350 3650
F 0 "#PWR024" H 9350 3450 50  0001 C CNN
F 1 "GND" H 9350 3540 50  0001 C CNN
F 2 "" H 9350 3650 60  0000 C CNN
F 3 "" H 9350 3650 60  0000 C CNN
	1    9350 3650
	1    0    0    -1  
$EndComp
Text Label 10650 3050 2    40   ~ 0
motor_4_up
Text Label 10650 4000 2    40   ~ 0
motor_4_on
Text Label 7600 2800 0    40   ~ 0
MOTOR_4_230V_UP
Text Label 7600 2900 0    40   ~ 0
MOTOR_4_230V_DOWN
Text Label 8250 3600 1    40   ~ 0
M4_230V_ON
$Comp
L FINDER_34.51.7.024 REL10
U 1 1 5B0D6504
P 8450 4050
F 0 "REL10" V 8600 4000 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 8950 4475 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 8450 4050 60  0001 C CNN
F 3 "" H 8450 4050 60  0001 C CNN
	1    8450 4050
	0    -1   -1   0   
$EndComp
$Comp
L FINDER_34.51.7.024 REL9
U 1 1 5B0D650A
P 8450 3150
F 0 "REL9" V 8600 3100 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 8600 3650 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 8450 3150 60  0001 C CNN
F 3 "" H 8450 3150 60  0001 C CNN
	1    8450 3150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9350 3250 9350 3650
Connection ~ 8750 3400
Connection ~ 8750 4300
Wire Wire Line
	9650 3050 9850 3050
Wire Wire Line
	9750 3150 9750 3050
Connection ~ 9750 3050
Wire Wire Line
	9650 4000 9850 4000
Wire Wire Line
	9750 4100 9750 4000
Connection ~ 9750 4000
Wire Wire Line
	9750 4500 9750 4550
Wire Wire Line
	9750 4550 9350 4550
Wire Wire Line
	9350 4200 9350 4600
Connection ~ 9350 4550
Wire Wire Line
	9350 3800 9350 3750
Wire Wire Line
	8450 4300 8450 4250
Wire Wire Line
	8450 3850 8450 3750
Wire Wire Line
	8450 3800 8750 3800
Wire Wire Line
	8750 3800 8750 3850
Connection ~ 8450 3800
Wire Wire Line
	8750 4250 8750 4300
Wire Wire Line
	9750 3550 9750 3600
Wire Wire Line
	9750 3600 9350 3600
Connection ~ 9350 3600
Wire Wire Line
	8750 3350 8750 3400
Wire Wire Line
	8450 3350 8450 3400
Wire Wire Line
	8750 2950 8750 2900
Wire Wire Line
	8750 2900 8450 2900
Wire Wire Line
	8450 2850 8450 2950
Connection ~ 8450 2900
Wire Wire Line
	8250 4350 8250 4250
Wire Wire Line
	7700 4350 8250 4350
$Comp
L +24V #PWR025
U 1 1 5B0D6531
P 8450 2850
F 0 "#PWR025" H 8450 3050 50  0001 C CNN
F 1 "+24V" H 8450 3004 50  0000 C CNN
F 2 "" H 8450 2850 60  0001 C CNN
F 3 "" H 8450 2850 60  0001 C CNN
	1    8450 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 2950 8200 2900
Wire Wire Line
	8200 2900 7500 2900
Wire Wire Line
	8300 2950 8300 2800
Wire Wire Line
	8300 2800 7500 2800
$Comp
L CONN_2X1 J6
U 1 1 5B0D653F
P 7300 2850
F 0 "J6" H 7375 3025 50  0000 C CNN
F 1 "WAGO 2081-1122" V 7475 2850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO2081-1122" H 7300 2850 60  0001 C CNN
F 3 "" H 7300 2850 60  0000 C CNN
	1    7300 2850
	-1   0    0    1   
$EndComp
Wire Wire Line
	8450 4300 8950 4300
Wire Wire Line
	8950 4300 8950 3750
Wire Wire Line
	8950 3750 9350 3750
Wire Wire Line
	8450 3400 8950 3400
Wire Wire Line
	8950 3400 8950 2850
Wire Wire Line
	8950 2850 9350 2850
Wire Wire Line
	8300 3850 8300 3600
Wire Wire Line
	8300 3600 8250 3600
Wire Wire Line
	8250 3600 8250 3350
Text Label 7700 4350 0    40   ~ 0
230V
$Comp
L +24V #PWR026
U 1 1 5B0D6798
P 3900 5800
F 0 "#PWR026" H 3900 6000 50  0001 C CNN
F 1 "+24V" H 3900 5910 50  0000 C CNN
F 2 "" H 3900 5800 60  0000 C CNN
F 3 "" H 3900 5800 60  0000 C CNN
	1    3900 5800
	1    0    0    -1  
$EndComp
$Comp
L D D5
U 1 1 5B0D679E
P 4200 5200
F 0 "D5" H 4200 5300 50  0000 C CNN
F 1 "1N4148" H 4200 5100 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 4200 5200 60  0001 C CNN
F 3 "" H 4200 5200 60  0000 C CNN
	1    4200 5200
	0    -1   -1   0   
$EndComp
$Comp
L D D6
U 1 1 5B0D67A4
P 4200 6100
F 0 "D6" H 4200 6200 50  0000 C CNN
F 1 "1N4148" H 4200 6000 50  0000 C CNN
F 2 "Diode_SMD:D_SOD323" H 4200 6100 60  0001 C CNN
F 3 "" H 4200 6100 60  0000 C CNN
	1    4200 6100
	0    -1   -1   0   
$EndComp
$Comp
L BSS138 Q6
U 1 1 5B0D67AA
P 4900 6050
F 0 "Q6" H 5050 5950 50  0000 L CNN
F 1 "BSS138" H 5050 6150 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5090 6050 25  0001 C CNN
F 3 "" H 4900 6050 60  0000 C CNN
	1    4900 6050
	-1   0    0    -1  
$EndComp
$Comp
L BSS138 Q5
U 1 1 5B0D67B0
P 4900 5100
F 0 "Q5" H 5050 5000 50  0000 L CNN
F 1 "BSS138" H 5050 5200 50  0000 L CNN
F 2 "Housings_SOT:SOT23-3" H 5090 5100 25  0001 C CNN
F 3 "" H 4900 5100 60  0000 C CNN
	1    4900 5100
	-1   0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 5B0D67B6
P 5500 5100
F 0 "R11" H 5500 5000 50  0000 C CNN
F 1 "100" H 5500 5100 50  0000 C CNN
F 2 "Resistor:R_0805" H 5500 5100 60  0001 C CNN
F 3 "" H 5500 5100 60  0000 C CNN
	1    5500 5100
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5B0D67BC
P 5200 5400
F 0 "R5" H 5200 5300 50  0000 C CNN
F 1 "10k" H 5200 5400 50  0000 C CNN
F 2 "Resistor:R_0805" H 5200 5400 60  0001 C CNN
F 3 "" H 5200 5400 60  0000 C CNN
	1    5200 5400
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 5B0D67C2
P 5200 6350
F 0 "R6" H 5200 6250 50  0000 C CNN
F 1 "10k" H 5200 6350 50  0000 C CNN
F 2 "Resistor:R_0805" H 5200 6350 60  0001 C CNN
F 3 "" H 5200 6350 60  0000 C CNN
	1    5200 6350
	0    1    1    0   
$EndComp
$Comp
L R R12
U 1 1 5B0D67C8
P 5500 6050
F 0 "R12" H 5500 5950 50  0000 C CNN
F 1 "100" H 5500 6050 50  0000 C CNN
F 2 "Resistor:R_0805" H 5500 6050 60  0001 C CNN
F 3 "" H 5500 6050 60  0000 C CNN
	1    5500 6050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 5B0D67CE
P 4800 6650
F 0 "#PWR027" H 4800 6450 50  0001 C CNN
F 1 "GND" H 4800 6540 50  0001 C CNN
F 2 "" H 4800 6650 60  0000 C CNN
F 3 "" H 4800 6650 60  0000 C CNN
	1    4800 6650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 5B0D67D4
P 4800 5700
F 0 "#PWR028" H 4800 5500 50  0001 C CNN
F 1 "GND" H 4800 5590 50  0001 C CNN
F 2 "" H 4800 5700 60  0000 C CNN
F 3 "" H 4800 5700 60  0000 C CNN
	1    4800 5700
	1    0    0    -1  
$EndComp
Text Label 6100 5100 2    40   ~ 0
motor_2_up
Text Label 6100 6050 2    40   ~ 0
motor_2_on
Text Label 3050 4850 0    40   ~ 0
MOTOR_2_230V_UP
Text Label 3050 4950 0    40   ~ 0
MOTOR_2_230V_DOWN
Text Label 3700 5650 1    40   ~ 0
M2_230V_ON
$Comp
L FINDER_34.51.7.024 REL6
U 1 1 5B0D67DF
P 3900 6100
F 0 "REL6" V 4050 6050 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 4400 6525 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 3900 6100 60  0001 C CNN
F 3 "" H 3900 6100 60  0001 C CNN
	1    3900 6100
	0    -1   -1   0   
$EndComp
$Comp
L FINDER_34.51.7.024 REL5
U 1 1 5B0D67E5
P 3900 5200
F 0 "REL5" V 4050 5150 50  0000 R CNN
F 1 "FINDER_34.51.7.024" H 4050 5700 50  0000 R CNN
F 2 "Relay:finder_34.51_1xUM" H 3900 5200 60  0001 C CNN
F 3 "" H 3900 5200 60  0001 C CNN
	1    3900 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4800 5300 4800 5700
Connection ~ 4200 5450
Connection ~ 4200 6350
Wire Wire Line
	5100 5100 5300 5100
Wire Wire Line
	5200 5200 5200 5100
Connection ~ 5200 5100
Wire Wire Line
	5100 6050 5300 6050
Wire Wire Line
	5200 6150 5200 6050
Connection ~ 5200 6050
Wire Wire Line
	5200 6550 5200 6600
Wire Wire Line
	5200 6600 4800 6600
Wire Wire Line
	4800 6250 4800 6650
Connection ~ 4800 6600
Wire Wire Line
	4800 5850 4800 5800
Wire Wire Line
	3900 6350 3900 6300
Wire Wire Line
	3900 5900 3900 5800
Wire Wire Line
	3900 5850 4200 5850
Wire Wire Line
	4200 5850 4200 5900
Connection ~ 3900 5850
Wire Wire Line
	4200 6300 4200 6350
Wire Wire Line
	5200 5600 5200 5650
Wire Wire Line
	5200 5650 4800 5650
Connection ~ 4800 5650
Wire Wire Line
	4200 5400 4200 5450
Wire Wire Line
	3900 5400 3900 5450
Wire Wire Line
	4200 5000 4200 4950
Wire Wire Line
	4200 4950 3900 4950
Wire Wire Line
	3900 4900 3900 5000
Connection ~ 3900 4950
Wire Wire Line
	3700 6400 3700 6300
Wire Wire Line
	3150 6400 3700 6400
$Comp
L +24V #PWR029
U 1 1 5B0D680C
P 3900 4900
F 0 "#PWR029" H 3900 5100 50  0001 C CNN
F 1 "+24V" H 3900 5054 50  0000 C CNN
F 2 "" H 3900 4900 60  0001 C CNN
F 3 "" H 3900 4900 60  0001 C CNN
	1    3900 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 5000 3650 4950
Wire Wire Line
	3650 4950 2950 4950
Wire Wire Line
	3750 5000 3750 4850
Wire Wire Line
	3750 4850 2950 4850
$Comp
L CONN_2X1 J4
U 1 1 5B0D681A
P 2750 4900
F 0 "J4" H 2825 5075 50  0000 C CNN
F 1 "WAGO 2081-1122" V 2925 4900 25  0000 C CNN
F 2 "Connector_WAGO:WAGO2081-1122" H 2750 4900 60  0001 C CNN
F 3 "" H 2750 4900 60  0000 C CNN
	1    2750 4900
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 6350 4400 6350
Wire Wire Line
	4400 6350 4400 5800
Wire Wire Line
	4400 5800 4800 5800
Wire Wire Line
	3900 5450 4400 5450
Wire Wire Line
	4400 5450 4400 4900
Wire Wire Line
	4400 4900 4800 4900
Wire Wire Line
	3750 5900 3750 5650
Wire Wire Line
	3750 5650 3700 5650
Wire Wire Line
	3700 5650 3700 5400
Text Label 3150 6400 0    40   ~ 0
230V
$Comp
L CONN_2X1 J1
U 1 1 5B0D7FEC
P 750 850
F 0 "J1" H 825 1025 50  0000 C CNN
F 1 "CONN_2X1" V 925 850 25  0000 C CNN
F 2 "Connector_WAGO:WAGO2081-1122" H 750 850 60  0001 C CNN
F 3 "" H 750 850 60  0000 C CNN
	1    750  850 
	-1   0    0    -1  
$EndComp
Wire Wire Line
	950  800  1050 800 
Wire Wire Line
	1050 800  1050 900 
Connection ~ 1050 900 
Text Label 1650 900  0    40   ~ 0
230V
Wire Wire Line
	1550 900  1850 900 
Wire Wire Line
	950  900  1150 900 
Text HLabel 6200 5100 2    60   Input ~ 0
motor_2_up
Wire Wire Line
	5700 5100 6200 5100
Text HLabel 6200 6050 2    60   Input ~ 0
motor_2_on
Text HLabel 6200 4000 2    60   Input ~ 0
motor_1_on
Text HLabel 6200 3050 2    60   Input ~ 0
motor_1_up
Text HLabel 6150 2000 2    60   Input ~ 0
motor_0_on
Text HLabel 6150 1050 2    60   Input ~ 0
motor_0_up
Wire Wire Line
	5650 1050 6150 1050
Wire Wire Line
	5650 2000 6150 2000
Wire Wire Line
	5700 3050 6200 3050
Wire Wire Line
	5700 4000 6200 4000
Wire Wire Line
	5700 6050 6200 6050
Text HLabel 10700 1050 2    60   Input ~ 0
motor_3_up
Text HLabel 10700 2000 2    60   Input ~ 0
motor_3_on
Text HLabel 10750 3050 2    60   Input ~ 0
motor_4_up
Text HLabel 10750 4000 2    60   Input ~ 0
motor_4_on
Wire Wire Line
	10250 4000 10750 4000
Wire Wire Line
	10250 3050 10750 3050
Wire Wire Line
	10200 1050 10700 1050
Wire Wire Line
	10200 2000 10700 2000
NoConn ~ 3600 1850
NoConn ~ 3650 3850
NoConn ~ 3650 5900
NoConn ~ 8200 3850
NoConn ~ 8150 1850
$Comp
L CONN_1 J17
U 1 1 5B195303
P 1250 900
F 0 "J17" H 1250 1000 50  0000 C CNN
F 1 "CONN_1" H 1250 800 25  0000 C CNN
F 2 "Connector_Miscelleaneus:CONN1_6.35" H 1250 900 60  0001 C CNN
F 3 "" H 1250 900 60  0000 C CNN
	1    1250 900 
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 J18
U 1 1 5B1953B2
P 1450 900
F 0 "J18" H 1450 800 50  0000 C CNN
F 1 "CONN_1" H 1450 1000 25  0000 C CNN
F 2 "Connector_Miscelleaneus:CONN1_6.35" H 1450 900 60  0001 C CNN
F 3 "" H 1450 900 60  0000 C CNN
	1    1450 900 
	-1   0    0    1   
$EndComp
Text Notes 1150 1150 0    40   ~ 0
Connectors to \nfuse F1 250V/4A
$EndSCHEMATC
