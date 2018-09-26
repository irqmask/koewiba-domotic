EESchema Schematic File Version 4
LIBS:motor_switch_3pole_5x-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "motor switch 3pole 5x"
Date "2018-09-17"
Rev "A"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+5V #PWR01
U 1 1 572DAEDB
P 4300 3050
F 0 "#PWR01" H 4300 3250 50  0001 C CNN
F 1 "+5V" H 4300 3160 50  0000 C CNN
F 2 "" H 4300 3050 60  0000 C CNN
F 3 "" H 4300 3050 60  0000 C CNN
	1    4300 3050
	-1   0    0    -1  
$EndComp
$Comp
L power:+24V #PWR02
U 1 1 572DAF09
P 2600 3050
F 0 "#PWR02" H 2600 3250 50  0001 C CNN
F 1 "+24V" H 2600 3160 50  0000 C CNN
F 2 "" H 2600 3050 60  0000 C CNN
F 3 "" H 2600 3050 60  0000 C CNN
	1    2600 3050
	-1   0    0    -1  
$EndComp
$Comp
L logo:OPEN_HARDWARE_1 LOGO1
U 1 1 572E288C
P 1025 7400
F 0 "LOGO1" H 1025 7675 60  0000 C CNN
F 1 "OPEN_HARDWARE_1" H 1025 7175 60  0000 C CNN
F 2 "Symbol:Symbol_OSHW-Logo_Copper" H 1025 7400 60  0001 C CNN
F 3 "" H 1025 7400 60  0000 C CNN
	1    1025 7400
	1    0    0    -1  
$EndComp
$Sheet
S 10100 3250 1000 1300
U 5B0D315A
F0 "Motor Relays" 60
F1 "motor_relays.sch" 60
F2 "motor_0_on" I L 10100 3350 60 
F3 "motor_0_up" I L 10100 3450 60 
F4 "motor_1_on" I L 10100 3600 60 
F5 "motor_1_up" I L 10100 3700 60 
F6 "motor_2_up" I L 10100 3950 60 
F7 "motor_2_on" I L 10100 3850 60 
F8 "motor_3_up" I L 10100 4200 60 
F9 "motor_3_on" I L 10100 4100 60 
F10 "motor_4_up" I L 10100 4450 60 
F11 "motor_4_on" I L 10100 4350 60 
$EndSheet
$Sheet
S 625  3050 925  1300
U 5B0E123E
F0 "Inputs" 40
F1 "inputs.sch" 40
F2 "button_0_up" O R 1550 3150 60 
F3 "button_0_down" O R 1550 3250 60 
F4 "button_1_up" O R 1550 3400 60 
F5 "button_1_down" O R 1550 3500 60 
F6 "button_2_up" O R 1550 3650 60 
F7 "button_2_down" O R 1550 3750 60 
F8 "button_3_up" O R 1550 3900 60 
F9 "button_3_down" O R 1550 4000 60 
F10 "button_4_up" O R 1550 4150 60 
F11 "button_4_down" O R 1550 4250 60 
$EndSheet
$Comp
L power:GND #PWR03
U 1 1 578B8717
P 4300 4600
F 0 "#PWR03" H 4300 4400 50  0001 C CNN
F 1 "GND" H 4300 4490 50  0001 C CNN
F 2 "" H 4300 4600 60  0000 C CNN
F 3 "" H 4300 4600 60  0000 C CNN
	1    4300 4600
	-1   0    0    -1  
$EndComp
$Sheet
S 3100 3075 900  1525
U 578A251D
F0 "bus_atmega328_basic" 60
F1 "bus_atmega328_basic.sch" 60
F2 "AREF" I L 3100 4025 60 
F3 "AVCC" I L 3100 4125 60 
F4 "+24V" I L 3100 3150 60 
F5 "PC1" B R 4000 3400 60 
F6 "PC3" B R 4000 3600 60 
F7 "PC2" B R 4000 3500 60 
F8 "PC0" B R 4000 3300 60 
F9 "GND" I R 4000 4500 60 
F10 "VCC" O R 4000 3150 60 
F11 "PC5" B R 4000 3800 60 
F12 "PC4" B R 4000 3700 60 
F13 "/RESET" I R 4000 4350 60 
F14 "PD4" B L 3100 3600 60 
F15 "PD3" B L 3100 3500 60 
F16 "PD5" B L 3100 3700 60 
F17 "PD7" B L 3100 3900 60 
F18 "PD6" B L 3100 3800 60 
F19 "ADC6" I L 3100 4225 60 
F20 "ADC7" I L 3100 4325 60 
F21 "PB0" B L 3100 3250 60 
F22 "PB1" B L 3100 3350 60 
F23 "/CSEEP" O R 4000 4250 60 
F24 "MOSI" O R 4000 3950 60 
F25 "MISO" I R 4000 4050 60 
F26 "SCK" O R 4000 4150 60 
$EndSheet
$Comp
L ics_74xx:74HC595 U2
U 1 1 5B0E7E5B
P 8300 3800
F 0 "U2" H 8450 4400 70  0000 C CNN
F 1 "74HC595" V 8300 3800 79  0000 C CNN
F 2 "Housings_SOIC:SO16N" H 8300 3800 60  0001 C CNN
F 3 "" H 8300 3800 60  0000 C CNN
	1    8300 3800
	1    0    0    -1  
$EndComp
$Comp
L ics_74xx:74HC595 U1
U 1 1 5B0EF04A
P 6200 3800
F 0 "U1" H 6350 4400 70  0000 C CNN
F 1 "74HC595" V 6200 3800 79  0000 C CNN
F 2 "Housings_SOIC:SO16N" H 6200 3800 60  0001 C CNN
F 3 "" H 6200 3800 60  0000 C CNN
	1    6200 3800
	1    0    0    -1  
$EndComp
$Comp
L standard:C C2
U 1 1 5B0F03CD
P 8100 2950
F 0 "C2" H 8150 3000 50  0000 L CNN
F 1 "100nF" H 8100 2825 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 8100 2950 60  0001 C CNN
F 3 "" H 8100 2950 60  0000 C CNN
	1    8100 2950
	0    1    1    0   
$EndComp
$Comp
L standard:C C1
U 1 1 5B0F0568
P 6000 2950
F 0 "C1" H 6050 3000 50  0000 L CNN
F 1 "100nF" H 6000 2825 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 6000 2950 60  0001 C CNN
F 3 "" H 6000 2950 60  0000 C CNN
	1    6000 2950
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR04
U 1 1 5B0F0960
P 8300 2750
F 0 "#PWR04" H 8300 2950 50  0001 C CNN
F 1 "+5V" H 8300 2860 50  0000 C CNN
F 2 "" H 8300 2750 40  0001 C CNN
F 3 "" H 8300 2750 40  0001 C CNN
	1    8300 2750
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 5B0F0D67
P 6200 2750
F 0 "#PWR05" H 6200 2950 50  0001 C CNN
F 1 "+5V" H 6200 2860 50  0000 C CNN
F 2 "" H 6200 2750 40  0001 C CNN
F 3 "" H 6200 2750 40  0001 C CNN
	1    6200 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5B0F16A2
P 6000 3150
F 0 "#PWR06" H 6000 2950 50  0001 C CNN
F 1 "GND" H 6000 3040 50  0001 C CNN
F 2 "" H 6000 3150 40  0001 C CNN
F 3 "" H 6000 3150 40  0001 C CNN
	1    6000 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5B0F17D2
P 8100 3150
F 0 "#PWR07" H 8100 2950 50  0001 C CNN
F 1 "GND" H 8100 3040 50  0001 C CNN
F 2 "" H 8100 3150 40  0001 C CNN
F 3 "" H 8100 3150 40  0001 C CNN
	1    8100 3150
	1    0    0    -1  
$EndComp
Text Label 6800 3950 0    40   ~ 0
motor_0_on
Text Label 6800 4050 0    40   ~ 0
motor_0_up
Text Label 6800 3750 0    40   ~ 0
motor_1_on
Text Label 6800 3850 0    40   ~ 0
motor_1_up
Text Label 6800 3550 0    40   ~ 0
motor_2_on
Text Label 6800 3650 0    40   ~ 0
motor_2_up
Text Label 6800 3350 0    40   ~ 0
motor_3_on
Text Label 6800 3450 0    40   ~ 0
motor_3_up
Text Label 8900 3950 0    40   ~ 0
motor_4_on
Text Label 8900 4050 0    40   ~ 0
motor_4_up
NoConn ~ 8900 3550
NoConn ~ 8900 3650
NoConn ~ 8900 3750
NoConn ~ 8900 3850
NoConn ~ 8900 3350
NoConn ~ 8900 3450
NoConn ~ 8900 4250
$Comp
L power:GND #PWR08
U 1 1 5B0F354D
P 8300 4650
F 0 "#PWR08" H 8300 4450 50  0001 C CNN
F 1 "GND" H 8300 4540 50  0001 C CNN
F 2 "" H 8300 4650 40  0001 C CNN
F 3 "" H 8300 4650 40  0001 C CNN
	1    8300 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5B0F3C09
P 6200 4650
F 0 "#PWR09" H 6200 4450 50  0001 C CNN
F 1 "GND" H 6200 4540 50  0001 C CNN
F 2 "" H 6200 4650 40  0001 C CNN
F 3 "" H 6200 4650 40  0001 C CNN
	1    6200 4650
	1    0    0    -1  
$EndComp
Entry Wire Line
	9250 3950 9350 4050
Entry Wire Line
	9250 4050 9350 4150
Entry Wire Line
	7150 3350 7250 3450
Entry Wire Line
	7150 3450 7250 3550
Entry Wire Line
	7150 3550 7250 3650
Entry Wire Line
	7150 3650 7250 3750
Entry Wire Line
	7150 3750 7250 3850
Entry Wire Line
	7150 3850 7250 3950
Entry Wire Line
	7150 3950 7250 4050
Entry Wire Line
	7150 4050 7250 4150
Text Label 9650 3350 0    40   ~ 0
motor_0_on
Text Label 9650 3450 0    40   ~ 0
motor_0_up
Text Label 9650 3600 0    40   ~ 0
motor_1_on
Text Label 9650 3700 0    40   ~ 0
motor_1_up
Text Label 9650 3850 0    40   ~ 0
motor_2_on
Text Label 9650 3950 0    40   ~ 0
motor_2_up
Text Label 9650 4100 0    40   ~ 0
motor_3_on
Text Label 9650 4200 0    40   ~ 0
motor_3_up
Text Label 9650 4350 0    40   ~ 0
motor_4_on
Text Label 9650 4450 0    40   ~ 0
motor_4_up
Entry Wire Line
	9550 3450 9650 3350
Entry Wire Line
	9550 3550 9650 3450
Entry Wire Line
	9550 3700 9650 3600
Entry Wire Line
	9550 3800 9650 3700
Entry Wire Line
	9550 3950 9650 3850
Entry Wire Line
	9550 4050 9650 3950
Entry Wire Line
	9550 4200 9650 4100
Entry Wire Line
	9550 4300 9650 4200
Entry Wire Line
	9550 4450 9650 4350
Entry Wire Line
	9550 4550 9650 4450
Wire Wire Line
	4000 3150 4300 3150
Wire Wire Line
	4300 3150 4300 3050
Wire Wire Line
	3100 3150 2600 3150
Wire Wire Line
	2600 3150 2600 3050
Wire Wire Line
	4000 4500 4300 4500
Wire Wire Line
	4300 4500 4300 4600
Wire Wire Line
	6000 2850 6200 2850
Wire Wire Line
	6200 2750 6200 2850
Connection ~ 6200 2850
Wire Wire Line
	8300 2750 8300 2850
Wire Wire Line
	8100 2850 8300 2850
Connection ~ 8300 2850
Wire Wire Line
	8100 3050 8100 3150
Wire Wire Line
	6000 3050 6000 3150
Wire Wire Line
	6200 4550 6200 4600
Wire Wire Line
	8300 4550 8300 4600
Wire Wire Line
	6800 3350 7150 3350
Wire Wire Line
	7150 3450 6800 3450
Wire Wire Line
	6800 3550 7150 3550
Wire Wire Line
	6800 3650 7150 3650
Wire Wire Line
	6800 3750 7150 3750
Wire Wire Line
	6800 3850 7150 3850
Wire Wire Line
	6800 3950 7150 3950
Wire Wire Line
	6800 4050 7150 4050
Wire Wire Line
	10100 3350 9650 3350
Wire Wire Line
	10100 3450 9650 3450
Wire Wire Line
	10100 3600 9650 3600
Wire Wire Line
	10100 3700 9650 3700
Wire Wire Line
	10100 3850 9650 3850
Wire Wire Line
	10100 3950 9650 3950
Wire Wire Line
	10100 4100 9650 4100
Wire Wire Line
	10100 4200 9650 4200
Wire Wire Line
	10100 4350 9650 4350
Wire Wire Line
	10100 4450 9650 4450
Wire Wire Line
	6800 4250 7350 4250
Wire Wire Line
	7350 4250 7350 3350
Wire Wire Line
	7700 3950 7600 3950
Wire Wire Line
	7600 3950 7600 4600
Wire Wire Line
	7600 4600 8300 4600
Connection ~ 8300 4600
Wire Wire Line
	5600 3950 5500 3950
Wire Wire Line
	5500 3950 5500 4600
Wire Wire Line
	5500 4600 6200 4600
Connection ~ 6200 4600
Wire Wire Line
	7700 3850 7550 3850
Wire Wire Line
	7550 3850 7550 4850
Wire Wire Line
	5450 4850 5450 3850
Wire Wire Line
	4950 3850 5450 3850
Wire Wire Line
	4950 3650 5400 3650
Wire Wire Line
	5400 3650 5400 4800
Wire Wire Line
	7500 4800 7500 3650
Wire Wire Line
	7500 3650 7700 3650
Wire Wire Line
	7700 3550 7450 3550
Wire Wire Line
	7450 3550 7450 4750
Wire Wire Line
	5350 4750 5350 3550
Wire Wire Line
	4950 3550 5350 3550
Text Label 1600 3150 0    40   ~ 0
/button_0_up
Text Label 1600 3250 0    40   ~ 0
/button_0_down
Text Label 1600 3400 0    40   ~ 0
/button_1_up
Text Label 1600 3500 0    40   ~ 0
/button_1_down
Text Label 1600 3650 0    40   ~ 0
/button_2_up
Text Label 1600 3750 0    40   ~ 0
/button_2_down
Text Label 1600 4000 0    40   ~ 0
/button_3_down
Entry Wire Line
	2100 3150 2200 3250
Entry Wire Line
	2100 3250 2200 3350
Entry Wire Line
	2100 3400 2200 3500
Entry Wire Line
	2100 3500 2200 3600
Entry Wire Line
	2100 3650 2200 3750
Entry Wire Line
	2100 3750 2200 3850
Entry Wire Line
	2100 3900 2200 4000
Entry Wire Line
	2100 4000 2200 4100
Entry Wire Line
	2100 4150 2200 4250
Entry Wire Line
	2100 4250 2200 4350
Text Label 1600 3900 0    40   ~ 0
/button_3_up
Text Label 1600 4150 0    40   ~ 0
/button_4_up
Text Label 1600 4250 0    40   ~ 0
/button_4_down
Wire Wire Line
	1550 3150 2100 3150
Wire Wire Line
	1550 3250 2100 3250
Wire Wire Line
	2100 3400 1550 3400
Wire Wire Line
	1550 3500 2100 3500
Wire Wire Line
	2100 3650 1550 3650
Wire Wire Line
	1550 3750 2100 3750
Wire Wire Line
	2100 3900 1550 3900
Wire Wire Line
	1550 4000 2100 4000
Wire Wire Line
	2100 4150 1550 4150
Wire Wire Line
	1550 4250 2100 4250
Text Label 4050 3400 0    40   ~ 0
/button_0_up
Text Label 4050 3300 0    40   ~ 0
/button_0_down
Text Label 4050 3600 0    40   ~ 0
/button_1_up
Text Label 4050 3500 0    40   ~ 0
/button_1_down
Text Label 4050 3800 0    40   ~ 0
/button_2_up
Text Label 4050 3700 0    40   ~ 0
/button_2_down
Text Label 2600 3500 0    40   ~ 0
/button_3_down
Entry Wire Line
	4550 3400 4650 3500
Entry Wire Line
	4550 3300 4650 3400
Entry Wire Line
	4550 3500 4650 3600
Entry Wire Line
	4550 3600 4650 3700
Entry Wire Line
	4550 3700 4650 3800
Entry Wire Line
	4550 3800 4650 3900
Entry Wire Line
	2500 3600 2600 3500
Entry Wire Line
	2500 3700 2600 3600
Entry Wire Line
	2500 3800 2600 3700
Entry Wire Line
	2500 3900 2600 3800
Text Label 2600 3600 0    40   ~ 0
/button_3_up
Text Label 2600 3800 0    40   ~ 0
/button_4_up
Text Label 2600 3700 0    40   ~ 0
/button_4_down
Wire Wire Line
	4000 3300 4550 3300
Wire Wire Line
	4000 3400 4550 3400
Wire Wire Line
	4550 3500 4000 3500
Wire Wire Line
	4000 3600 4550 3600
Wire Wire Line
	4000 3700 4550 3700
Wire Wire Line
	4000 3800 4550 3800
Wire Wire Line
	2600 3500 3100 3500
Wire Wire Line
	2600 3600 3100 3600
Wire Wire Line
	2600 3700 3100 3700
Wire Wire Line
	2600 3800 3100 3800
Text Label 4050 3950 0    40   ~ 0
output_data
Text Label 4050 4150 0    40   ~ 0
output_clock
Text Label 4050 4350 0    40   ~ 0
output_reset
Text Label 3050 3250 2    40   ~ 0
output_latch
Entry Wire Line
	4450 3950 4550 4050
Entry Wire Line
	4450 4150 4550 4250
Entry Wire Line
	4450 4350 4550 4450
Entry Wire Line
	2400 3350 2500 3250
Wire Wire Line
	2500 3250 3100 3250
Wire Wire Line
	4000 3950 4450 3950
Wire Wire Line
	4000 4150 4450 4150
Wire Wire Line
	4000 4350 4450 4350
Entry Wire Line
	4850 3450 4950 3350
Entry Wire Line
	4850 3650 4950 3550
Entry Wire Line
	4850 3750 4950 3650
Entry Wire Line
	4850 3950 4950 3850
Text Label 4950 3350 0    40   ~ 0
output_data
Text Label 4950 3550 0    40   ~ 0
output_clock
Text Label 4950 3650 0    40   ~ 0
output_reset
Text Label 4950 3850 0    40   ~ 0
output_latch
Wire Wire Line
	4950 3350 5600 3350
Connection ~ 5350 3550
Connection ~ 5400 3650
Connection ~ 5450 3850
Wire Bus Line
	2200 4750 4650 4750
Text Label 2950 4850 0    40   ~ 0
output
Text Label 2600 4750 0    40   ~ 0
input
Wire Wire Line
	7350 3350 7700 3350
Wire Wire Line
	7450 4750 5350 4750
Wire Wire Line
	5400 4800 7500 4800
Wire Wire Line
	7550 4850 5450 4850
Wire Bus Line
	7250 4900 9550 4900
Wire Wire Line
	9250 3950 8900 3950
Wire Wire Line
	8900 4050 9250 4050
Wire Wire Line
	6200 2850 6200 3050
Wire Wire Line
	8300 2850 8300 3050
Wire Wire Line
	8300 4600 8300 4650
Wire Wire Line
	6200 4600 6200 4650
Wire Wire Line
	5350 3550 5600 3550
Wire Wire Line
	5400 3650 5600 3650
Wire Wire Line
	5450 3850 5600 3850
Entry Wire Line
	2400 4950 2500 5050
Entry Wire Line
	2400 5050 2500 5150
Entry Wire Line
	2400 5150 2500 5250
Entry Wire Line
	2400 5250 2500 5350
Text Label 2550 5050 0    40   ~ 0
output_latch
Text Label 2550 5150 0    40   ~ 0
output_data
Text Label 2550 5250 0    40   ~ 0
output_clock
Text Label 2550 5350 0    40   ~ 0
output_reset
Wire Wire Line
	2950 5050 2500 5050
Wire Wire Line
	2500 5150 2950 5150
Wire Wire Line
	2500 5250 2950 5250
Wire Wire Line
	2500 5350 2950 5350
Wire Bus Line
	2400 4850 4850 4850
Connection ~ 2400 4850
Wire Bus Line
	2400 3350 2400 4850
$Comp
L connectors:CONN_1 J42
U 1 1 5BA1D0CF
P 3050 5050
F 0 "J42" H 3091 5086 50  0000 L CNN
F 1 "tp_output_latch" H 3250 5050 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 3050 5050 60  0001 C CNN
F 3 "" H 3050 5050 60  0000 C CNN
	1    3050 5050
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J43
U 1 1 5BA1D111
P 3050 5150
F 0 "J43" H 3091 5186 50  0000 L CNN
F 1 "tp_output_data" H 3250 5150 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 3050 5150 60  0001 C CNN
F 3 "" H 3050 5150 60  0000 C CNN
	1    3050 5150
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J44
U 1 1 5BA1D14E
P 3050 5250
F 0 "J44" H 3091 5286 50  0000 L CNN
F 1 "tp_output_clock" H 3250 5250 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 3050 5250 60  0001 C CNN
F 3 "" H 3050 5250 60  0000 C CNN
	1    3050 5250
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_1 J45
U 1 1 5BA1D18E
P 3050 5350
F 0 "J45" H 3091 5386 50  0000 L CNN
F 1 "tp_output_reset" H 3250 5350 25  0000 L CNN
F 2 "Connector_Miscelleaneus:Testpoint_1mm" H 3050 5350 60  0001 C CNN
F 3 "" H 3050 5350 60  0000 C CNN
	1    3050 5350
	1    0    0    -1  
$EndComp
Wire Bus Line
	9350 4050 9350 4900
Wire Bus Line
	4550 4050 4550 4850
Wire Bus Line
	4850 3450 4850 4850
Wire Bus Line
	2500 3600 2500 4750
Wire Bus Line
	2400 4850 2400 5250
Wire Bus Line
	4650 3400 4650 4750
Wire Bus Line
	9550 3450 9550 4900
Wire Bus Line
	2200 3250 2200 4750
Wire Bus Line
	7250 3450 7250 4900
$EndSCHEMATC
