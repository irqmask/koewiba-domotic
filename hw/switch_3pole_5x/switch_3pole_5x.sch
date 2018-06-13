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
LIBS:switch_3pole_5x-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "switch 3pole 5x"
Date "2018-06-08"
Rev "1"
Comp "koewiba"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +5V #PWR01
U 1 1 572DAEDB
P 6450 3350
F 0 "#PWR01" H 6450 3550 50  0001 C CNN
F 1 "+5V" H 6450 3460 50  0000 C CNN
F 2 "" H 6450 3350 60  0000 C CNN
F 3 "" H 6450 3350 60  0000 C CNN
	1    6450 3350
	-1   0    0    -1  
$EndComp
$Comp
L +24V #PWR02
U 1 1 572DAF09
P 4750 3350
F 0 "#PWR02" H 4750 3550 50  0001 C CNN
F 1 "+24V" H 4750 3460 50  0000 C CNN
F 2 "" H 4750 3350 60  0000 C CNN
F 3 "" H 4750 3350 60  0000 C CNN
	1    4750 3350
	-1   0    0    -1  
$EndComp
$Comp
L OPEN_HARDWARE_1 LOGO1
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
S 7600 3350 650  800 
U 5B0D315A
F0 "Relays" 60
F1 "relays.sch" 60
F2 "relay_0" I L 7600 3450 60 
F3 "relay_1" I L 7600 3600 60 
F4 "relay_2" I L 7600 3750 60 
F5 "relay_3" I L 7600 3900 60 
F6 "relay_4" I L 7600 4050 60 
$EndSheet
$Sheet
S 2775 3350 925  800 
U 5B0E123E
F0 "Inputs" 40
F1 "inputs.sch" 40
F2 "button_0" O R 3700 3450 60 
F3 "button_1" O R 3700 3600 60 
F4 "button_2" O R 3700 3750 60 
F5 "button_3" O R 3700 3900 60 
F6 "button_4" O R 3700 4050 60 
$EndSheet
$Comp
L GND #PWR03
U 1 1 578B8717
P 6450 4900
F 0 "#PWR03" H 6450 4700 50  0001 C CNN
F 1 "GND" H 6450 4790 50  0001 C CNN
F 2 "" H 6450 4900 60  0000 C CNN
F 3 "" H 6450 4900 60  0000 C CNN
	1    6450 4900
	-1   0    0    -1  
$EndComp
$Sheet
S 5250 3375 900  1525
U 578A251D
F0 "bus_atmega328_basic" 60
F1 "bus_atmega328_basic.sch" 60
F2 "AREF" I L 5250 4325 60 
F3 "AVCC" I L 5250 4425 60 
F4 "+24V" I L 5250 3450 60 
F5 "PC1" B R 6150 3700 60 
F6 "PC3" B R 6150 3900 60 
F7 "PC2" B R 6150 3800 60 
F8 "PC0" B R 6150 3600 60 
F9 "GND" I R 6150 4800 60 
F10 "VCC" O R 6150 3450 60 
F11 "PC5" B R 6150 4100 60 
F12 "PC4" B R 6150 4000 60 
F13 "/RESET" I R 6150 4650 60 
F14 "PD4" B L 5250 3900 60 
F15 "PD3" B L 5250 3800 60 
F16 "PD5" B L 5250 4000 60 
F17 "PD7" B L 5250 4200 60 
F18 "PD6" B L 5250 4100 60 
F19 "ADC6" I L 5250 4525 60 
F20 "ADC7" I L 5250 4625 60 
F21 "PB0" B L 5250 3550 60 
F22 "PB1" B L 5250 3650 60 
F23 "/CSEEP" O R 6150 4550 60 
F24 "MOSI" O R 6150 4250 60 
F25 "MISO" I R 6150 4350 60 
F26 "SCK" O R 6150 4450 60 
$EndSheet
Text Label 7150 3450 0    40   ~ 0
relay_0
Text Label 7150 3750 0    40   ~ 0
relay_2
Text Label 7150 3900 0    40   ~ 0
relay_3
Text Label 7150 4050 0    40   ~ 0
relay_4
Entry Wire Line
	7050 3550 7150 3450
Entry Wire Line
	7050 3700 7150 3600
Entry Wire Line
	7050 3850 7150 3750
Entry Wire Line
	7050 4000 7150 3900
Entry Wire Line
	7050 4150 7150 4050
Wire Wire Line
	6150 3450 6450 3450
Wire Wire Line
	6450 3450 6450 3350
Wire Wire Line
	5250 3450 4750 3450
Wire Wire Line
	4750 3450 4750 3350
Wire Wire Line
	6150 4800 6450 4800
Wire Wire Line
	6450 4800 6450 4900
Wire Wire Line
	7600 3450 7150 3450
Wire Bus Line
	7050 3550 7050 5150
Text Label 3750 3450 0    40   ~ 0
/button_0
Text Label 3750 3750 0    40   ~ 0
/button_2
Text Label 3750 3900 0    40   ~ 0
/button_3
Text Label 3750 4050 0    40   ~ 0
/button_4
Entry Wire Line
	4250 3450 4350 3550
Entry Wire Line
	4250 3600 4350 3700
Entry Wire Line
	4250 3750 4350 3850
Entry Wire Line
	4250 3900 4350 4000
Entry Wire Line
	4250 4050 4350 4150
Wire Wire Line
	3700 3450 4250 3450
Text Label 6200 3700 0    40   ~ 0
/button_1
Text Label 6200 3600 0    40   ~ 0
/button_0
Text Label 6200 3900 0    40   ~ 0
/button_3
Text Label 6200 3800 0    40   ~ 0
/button_2
Text Label 6200 4000 0    40   ~ 0
/button_4
Text Label 4800 4200 0    40   ~ 0
relay_0
Entry Wire Line
	6700 3700 6800 3800
Entry Wire Line
	6700 3600 6800 3700
Entry Wire Line
	6700 3800 6800 3900
Entry Wire Line
	6700 3900 6800 4000
Entry Wire Line
	6700 4000 6800 4100
Entry Wire Line
	4650 3900 4750 3800
Entry Wire Line
	4650 4000 4750 3900
Entry Wire Line
	4650 4100 4750 4000
Entry Wire Line
	4650 4200 4750 4100
Text Label 4800 4100 0    40   ~ 0
relay_1
Text Label 4800 3900 0    40   ~ 0
relay_3
Text Label 4800 4000 0    40   ~ 0
relay_2
Wire Wire Line
	6150 3600 6700 3600
Wire Wire Line
	6150 3700 6700 3700
Wire Wire Line
	6700 3800 6150 3800
Wire Wire Line
	6150 3900 6700 3900
Wire Wire Line
	6150 4000 6700 4000
Wire Wire Line
	4750 3800 5250 3800
Wire Wire Line
	4750 3900 5250 3900
Wire Wire Line
	4750 4000 5250 4000
Wire Wire Line
	4750 4100 5250 4100
Wire Bus Line
	6800 5050 6800 3700
Wire Bus Line
	4350 3550 4350 5050
Wire Bus Line
	4350 5050 6800 5050
Text Label 5100 5150 0    40   ~ 0
output
Text Label 4750 5050 0    40   ~ 0
input
Text Label 3750 3600 0    40   ~ 0
/button_1
Wire Wire Line
	3700 3600 4250 3600
Wire Wire Line
	3700 3750 4250 3750
Wire Wire Line
	3700 3900 4250 3900
Wire Wire Line
	3700 4050 4250 4050
Wire Wire Line
	5250 4200 4750 4200
Entry Wire Line
	4650 4300 4750 4200
Text Label 4800 3800 0    40   ~ 0
relay_4
Wire Bus Line
	4650 3900 4650 5150
Wire Bus Line
	4650 5150 7050 5150
Text Label 7150 3600 0    40   ~ 0
relay_1
Wire Wire Line
	7150 3600 7600 3600
Wire Wire Line
	7150 3750 7600 3750
Wire Wire Line
	7150 3900 7600 3900
Wire Wire Line
	7150 4050 7600 4050
$EndSCHEMATC
