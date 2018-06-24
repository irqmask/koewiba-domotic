EESchema Schematic File Version 2
LIBS:display
LIBS:modules
LIBS:power
LIBS:sensors
LIBS:standard
LIBS:transistors
LIBS:logo
LIBS:roomthermostat-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "roomthermostat"
Date "2018-06-14"
Rev "1"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L bus-module_atmega328 MOD1
U 1 1 5B2200B5
P 5100 4000
F 0 "MOD1" H 4950 3850 50  0000 C CNN
F 1 "bus-module_atmega328" V 5200 4000 40  0000 C CNN
F 2 "Modules:bus-module_atmega328" H 5100 3850 60  0001 C CNN
F 3 "" H 5100 3850 60  0000 C CNN
	1    5100 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 5050 4900 5150
Wire Wire Line
	5000 5050 5000 5150
Wire Wire Line
	5200 5050 5200 5150
Wire Wire Line
	5300 5050 5300 5150
Wire Wire Line
	5300 5150 4900 5150
Connection ~ 5000 5150
Connection ~ 5200 5150
Wire Wire Line
	5100 5150 5100 5250
Connection ~ 5100 5150
$Comp
L GND #PWR01
U 1 1 5B22013D
P 5100 5250
F 0 "#PWR01" H 5100 5050 50  0001 C CNN
F 1 "GND" H 5100 5140 50  0001 C CNN
F 2 "" H 5100 5250 60  0001 C CNN
F 3 "" H 5100 5250 60  0001 C CNN
	1    5100 5250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR02
U 1 1 5B220159
P 5100 2800
F 0 "#PWR02" H 5100 2975 50  0001 C CNN
F 1 "VCC" H 5100 2900 50  0000 C CNN
F 2 "" H 5100 2800 60  0001 C CNN
F 3 "" H 5100 2800 60  0001 C CNN
	1    5100 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3000 4900 2900
Wire Wire Line
	4900 2900 5300 2900
Wire Wire Line
	5300 2900 5300 3000
Wire Wire Line
	5100 2800 5100 2900
Connection ~ 5100 2900
Wire Wire Line
	5000 3000 5000 2900
Connection ~ 5000 2900
Wire Wire Line
	5200 3000 5200 2900
Connection ~ 5200 2900
$Comp
L TSIC206_SOIC8 U1
U 1 1 5B2201B8
P 8200 3350
F 0 "U1" H 8200 3800 50  0000 C CNN
F 1 "TSIC206_SOIC8" H 8200 3575 25  0000 C CNN
F 2 "Housings_SOIC:SO8E" H 8200 3100 25  0001 C CNN
F 3 "" H 8200 3350 60  0001 C CNN
	1    8200 3350
	1    0    0    -1  
$EndComp
$Comp
L Graphic-OLED-Display_4wireSPI MOD2
U 1 1 5B220AD1
P 6900 3750
F 0 "MOD2" H 6900 4600 50  0000 C CNN
F 1 "Graphic-OLED-Display_4wireSPI" H 6895 4500 25  0000 C CNN
F 2 "Display:GraphDisplay_1_OLED_27x27,5mm_030072BA" H 6895 3750 60  0001 C CNN
F 3 "" H 6895 3750 60  0001 C CNN
	1    6900 3750
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5B220C28
P 6450 4050
F 0 "C1" H 6500 4100 50  0000 L CNN
F 1 "100nF" H 6450 3925 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6450 4050 60  0001 C CNN
F 3 "" H 6450 4050 60  0000 C CNN
	1    6450 4050
	0    1    1    0   
$EndComp
$Comp
L C C2
U 1 1 5B220CB9
P 7900 4050
F 0 "C2" H 7950 4100 50  0000 L CNN
F 1 "100nF" H 7900 3925 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 7900 4050 60  0001 C CNN
F 3 "" H 7900 4050 60  0000 C CNN
	1    7900 4050
	0    1    1    0   
$EndComp
NoConn ~ 5650 4550
Text Label 5700 4450 0    40   ~ 0
DISP_MOSI
Text Label 5700 4650 0    40   ~ 0
DISP_SCK
Text Label 5700 3800 0    40   ~ 0
/DISP_CS
Text Label 5700 3900 0    40   ~ 0
DISP_A0
Wire Wire Line
	5650 4450 6050 4450
Wire Wire Line
	5650 4650 6050 4650
Wire Wire Line
	5650 3900 6050 3900
Wire Wire Line
	5650 3800 6050 3800
Text Label 4250 4000 0    40   ~ 0
/RESET
Wire Wire Line
	6700 3850 6450 3850
Wire Wire Line
	6600 3750 6600 4350
Wire Wire Line
	8100 3850 7900 3850
Wire Wire Line
	7900 2850 7900 3950
Wire Wire Line
	8300 3750 8300 4350
$Comp
L GND #PWR03
U 1 1 5B221010
P 6600 4350
F 0 "#PWR03" H 6600 4150 50  0001 C CNN
F 1 "GND" H 6600 4240 50  0001 C CNN
F 2 "" H 6600 4350 60  0001 C CNN
F 3 "" H 6600 4350 60  0001 C CNN
	1    6600 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5B22102E
P 8300 4350
F 0 "#PWR04" H 8300 4150 50  0001 C CNN
F 1 "GND" H 8300 4240 50  0001 C CNN
F 2 "" H 8300 4350 60  0001 C CNN
F 3 "" H 8300 4350 60  0001 C CNN
	1    8300 4350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR05
U 1 1 5B22104C
P 7900 2850
F 0 "#PWR05" H 7900 3025 50  0001 C CNN
F 1 "VCC" H 7900 2950 50  0000 C CNN
F 2 "" H 7900 2850 60  0001 C CNN
F 3 "" H 7900 2850 60  0001 C CNN
	1    7900 2850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR06
U 1 1 5B22106A
P 6450 2850
F 0 "#PWR06" H 6450 3025 50  0001 C CNN
F 1 "VCC" H 6450 2950 50  0000 C CNN
F 2 "" H 6450 2850 60  0001 C CNN
F 3 "" H 6450 2850 60  0001 C CNN
	1    6450 2850
	1    0    0    -1  
$EndComp
Text Label 6800 3800 3    40   ~ 0
DISP_SCK
Text Label 6900 3800 3    40   ~ 0
DISP_MOSI
Text Label 7000 3800 3    40   ~ 0
/RESET
Text Label 7100 3800 3    40   ~ 0
DISP_A0
Text Label 7200 3800 3    40   ~ 0
/DISP_CS
Wire Wire Line
	6450 2850 6450 3950
Connection ~ 6450 3850
Wire Wire Line
	6700 3750 6700 3850
Wire Wire Line
	6450 4150 6450 4250
Wire Wire Line
	6450 4250 6600 4250
Connection ~ 6600 4250
Wire Wire Line
	8100 3750 8100 3850
Connection ~ 7900 3850
Wire Wire Line
	7900 4150 7900 4250
Wire Wire Line
	7900 4250 8300 4250
Connection ~ 8300 4250
Wire Wire Line
	6800 3750 6800 4200
Wire Wire Line
	6900 3750 6900 4200
Wire Wire Line
	7000 3750 7000 4200
Wire Wire Line
	7100 3750 7100 4200
Wire Wire Line
	7200 3750 7200 4200
Entry Wire Line
	6800 4200 6900 4300
Entry Wire Line
	6900 4200 7000 4300
Entry Wire Line
	7000 4200 7100 4300
Entry Wire Line
	7100 4200 7200 4300
Entry Wire Line
	7200 4200 7300 4300
Entry Wire Line
	8400 4000 8500 4100
Text Label 8200 3950 1    40   ~ 0
TEMP
Wire Wire Line
	8200 3750 8200 4000
Wire Wire Line
	8200 4000 8400 4000
Wire Bus Line
	8500 5500 8500 4100
Wire Bus Line
	6900 4300 7300 4300
Wire Bus Line
	7300 4300 7300 5500
Entry Wire Line
	6050 3800 6150 3900
Entry Wire Line
	6050 3900 6150 4000
Entry Wire Line
	6050 4450 6150 4550
Entry Wire Line
	6050 4650 6150 4750
Wire Bus Line
	6150 3900 6150 5500
Wire Bus Line
	4100 5500 8500 5500
Entry Wire Line
	4100 4100 4200 4000
Wire Wire Line
	4550 4000 4200 4000
NoConn ~ 5650 4000
NoConn ~ 5650 3500
NoConn ~ 5650 3600
NoConn ~ 5650 3400
NoConn ~ 5650 3700
$Comp
L PushButton_SPST_NO S1
U 1 1 5B221710
P 3850 3100
F 0 "S1" H 4000 3025 50  0000 C BNN
F 1 "PushButton_SPST_NO" H 4000 3225 25  0000 C BNN
F 2 "Switch_Miscelleaneus:PHAP3301" H 3850 3100 60  0001 C CNN
F 3 "" H 3850 3100 60  0000 C CNN
	1    3850 3100
	1    0    0    -1  
$EndComp
$Comp
L PushButton_SPST_NO S2
U 1 1 5B22176B
P 3850 3350
F 0 "S2" H 4000 3275 50  0000 C BNN
F 1 "PushButton_SPST_NO" H 4000 3475 25  0000 C BNN
F 2 "Switch_Miscelleaneus:PHAP3301" H 3850 3350 60  0001 C CNN
F 3 "" H 3850 3350 60  0000 C CNN
	1    3850 3350
	1    0    0    -1  
$EndComp
$Comp
L PushButton_SPST_NO S3
U 1 1 5B22178E
P 3850 3600
F 0 "S3" H 4000 3525 50  0000 C BNN
F 1 "PushButton_SPST_NO" H 4000 3725 25  0000 C BNN
F 2 "Switch_Miscelleaneus:PHAP3301" H 3850 3600 60  0001 C CNN
F 3 "" H 3850 3600 60  0000 C CNN
	1    3850 3600
	1    0    0    -1  
$EndComp
Text Label 4350 3400 0    40   ~ 0
KEY0
Text Label 4350 3500 0    40   ~ 0
KEY1
Text Label 4350 3600 0    40   ~ 0
KEY2
Wire Wire Line
	4550 3400 4300 3400
Wire Wire Line
	4300 3400 4300 3100
Wire Wire Line
	4550 3500 4250 3500
Wire Wire Line
	4250 3500 4250 3350
Wire Wire Line
	4250 3350 4150 3350
Wire Wire Line
	4550 3600 4150 3600
Wire Wire Line
	4300 3100 4150 3100
Wire Wire Line
	3850 3100 3750 3100
Wire Wire Line
	3750 3100 3750 3700
Wire Wire Line
	3850 3600 3750 3600
Connection ~ 3750 3600
Wire Wire Line
	3850 3350 3750 3350
Connection ~ 3750 3350
$Comp
L GND #PWR07
U 1 1 5B221CED
P 3750 3700
F 0 "#PWR07" H 3750 3500 50  0001 C CNN
F 1 "GND" H 3750 3590 50  0001 C CNN
F 2 "" H 3750 3700 60  0001 C CNN
F 3 "" H 3750 3700 60  0001 C CNN
	1    3750 3700
	1    0    0    -1  
$EndComp
Text Label 4250 4450 0    40   ~ 0
TEMP
Wire Wire Line
	4550 4450 4200 4450
Entry Wire Line
	4100 4550 4200 4450
Wire Bus Line
	4100 4100 4100 5500
NoConn ~ 4550 3700
NoConn ~ 4550 3800
NoConn ~ 4550 3900
NoConn ~ 4550 4250
NoConn ~ 4550 4350
NoConn ~ 4550 4550
NoConn ~ 4550 4650
$Comp
L OPEN_HARDWARE_1 LOGO1
U 1 1 5B225797
P 3250 5700
F 0 "LOGO1" H 3250 5975 50  0000 C CNN
F 1 "OPEN_HARDWARE_1" H 3250 5475 25  0000 C CNN
F 2 "" H 3250 5700 60  0001 C CNN
F 3 "" H 3250 5700 60  0001 C CNN
	1    3250 5700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
