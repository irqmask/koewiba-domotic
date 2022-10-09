EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "heatervalve"
Date "2022-02-28"
Rev "1"
Comp "KoeWiBa"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6200 2850 6200 2700
Wire Wire Line
	7200 2700 7200 2850
Wire Wire Line
	6200 3800 6200 4000
Wire Wire Line
	7200 4000 7200 3800
$Comp
L standard:R R1
U 1 1 61E4D630
P 5600 3050
F 0 "R1" H 5850 3100 50  0000 C CNN
F 1 "680" H 5600 3050 50  0000 C CNN
F 2 "Resistor:R_0805" H 5600 3050 60  0001 C CNN
F 3 "" H 5600 3050 60  0000 C CNN
	1    5600 3050
	1    0    0    -1  
$EndComp
$Comp
L standard:R R4
U 1 1 61E4DD24
P 5600 3900
F 0 "R4" H 5850 3950 50  0000 C CNN
F 1 "680" H 5600 3900 50  0000 C CNN
F 2 "Resistor:R_0805" H 5600 3900 60  0001 C CNN
F 3 "" H 5600 3900 60  0000 C CNN
	1    5600 3900
	1    0    0    -1  
$EndComp
$Comp
L standard:R R3
U 1 1 61E4E449
P 5600 3750
F 0 "R3" H 5850 3800 50  0000 C CNN
F 1 "680" H 5600 3750 50  0000 C CNN
F 2 "Resistor:R_0805" H 5600 3750 60  0001 C CNN
F 3 "" H 5600 3750 60  0000 C CNN
	1    5600 3750
	1    0    0    -1  
$EndComp
$Comp
L connectors:CONN_2X1 J1
U 1 1 61E41AF2
P 7700 3350
F 0 "J1" H 7800 3100 50  0000 C CNN
F 1 "CONN_2X1" H 7800 3150 25  0000 C CNN
F 2 "Connector_Header:HEADER_2x1" H 7700 3350 60  0001 C CNN
F 3 "" H 7700 3350 60  0000 C CNN
	1    7700 3350
	1    0    0    1   
$EndComp
Wire Wire Line
	7200 3250 7200 3350
Wire Wire Line
	6200 3250 6200 3300
Connection ~ 6200 3300
Wire Wire Line
	6200 3300 6200 3400
Wire Wire Line
	7500 3400 7500 3350
Wire Wire Line
	7500 3350 7200 3350
Connection ~ 7200 3350
Wire Wire Line
	7200 3350 7200 3400
$Sheet
S 3600 2600 950  1550
U 61E7F4A8
F0 "bus_atmega328_basic" 50
F1 "bus_atmega328_basic.sch" 50
F2 "AREF" I L 3600 3650 50 
F3 "AVCC" O R 4550 2750 50 
F4 "+24V" O L 3600 2700 50 
F5 "PC1" B R 4550 2950 50 
F6 "PC3" B R 4550 3150 50 
F7 "PC2" B R 4550 3050 50 
F8 "PC0" B R 4550 2850 50 
F9 "GND" I R 4550 4050 50 
F10 "VCC" O R 4550 2650 50 
F11 "PC5" B R 4550 3350 50 
F12 "PC4" B R 4550 3250 50 
F13 "/RESET" I R 4550 3950 50 
F14 "PD4" B L 3600 3200 50 
F15 "PD3" B L 3600 3100 50 
F16 "PD5" B L 3600 3300 50 
F17 "PD7" B L 3600 3500 50 
F18 "PD6" B L 3600 3400 50 
F19 "ADC6" I L 3600 3950 50 
F20 "ADC7" I L 3600 4050 50 
F21 "PB0" B L 3600 2850 50 
F22 "PB1" B L 3600 2950 50 
F23 "/CSEEP" O R 4550 3850 50 
F24 "MOSI" O R 4550 3500 50 
F25 "MISO" I R 4550 3600 50 
F26 "SCK" O R 4550 3700 50 
$EndSheet
$Comp
L encoder:SimpleEncoder ENC1
U 1 1 61F84537
P 1900 2950
F 0 "ENC1" V 1800 3100 50  0000 L CNN
F 1 "SimpleEncoder" V 1900 3100 50  0000 L CNN
F 2 "Switch_Encoder:EncodeNonameHeatervalve" H 1900 2950 50  0001 C CNN
F 3 "" H 1900 2950 50  0001 C CNN
	1    1900 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	1950 3050 1850 3050
Wire Wire Line
	1850 3050 1850 3200
Wire Wire Line
	1950 3200 1850 3200
Connection ~ 1850 3200
Wire Wire Line
	1850 3200 1850 3300
$Comp
L power:GND #PWR01
U 1 1 61F8641D
P 1850 3300
F 0 "#PWR01" H 1850 3100 50  0001 C CNN
F 1 "GND" H 1850 3190 50  0001 C CNN
F 2 "" H 1850 3300 50  0001 C CNN
F 3 "" H 1850 3300 50  0001 C CNN
	1    1850 3300
	1    0    0    -1  
$EndComp
$Comp
L standard:SWITCH SW1
U 1 1 61F86BE1
P 1900 3900
F 0 "SW1" H 2150 4100 50  0000 C CNN
F 1 "SWITCH" H 2450 4100 50  0000 C CNN
F 2 "Switch_Miscelleaneus:PushButton_SPST_THT_5x5mm" H 1900 3900 60  0001 C CNN
F 3 "" H 1900 3900 60  0000 C CNN
	1    1900 3900
	1    0    0    -1  
$EndComp
$Comp
L standard:SWITCH SW2
U 1 1 61F8739A
P 1900 4100
F 0 "SW2" H 2150 4300 50  0000 C CNN
F 1 "SWITCH" H 2450 4300 50  0000 C CNN
F 2 "Switch_Miscelleaneus:PushButton_SPST_THT_5x5mm" H 1900 4100 60  0001 C CNN
F 3 "" H 1900 4100 60  0000 C CNN
	1    1900 4100
	1    0    0    -1  
$EndComp
$Comp
L standard:SWITCH SW3
U 1 1 61F87756
P 1900 4300
F 0 "SW3" H 2150 4500 50  0000 C CNN
F 1 "SWITCH" H 2450 4500 50  0000 C CNN
F 2 "Switch_Miscelleaneus:PushButton_SPST_THT_5x5mm" H 1900 4300 60  0001 C CNN
F 3 "" H 1900 4300 60  0000 C CNN
	1    1900 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 3800 1950 3800
Wire Wire Line
	1850 3800 1850 4000
Wire Wire Line
	1850 4000 1950 4000
Connection ~ 1850 4000
Wire Wire Line
	1850 4000 1850 4200
Wire Wire Line
	1850 4200 1950 4200
Connection ~ 1850 4200
Wire Wire Line
	1850 4200 1850 4300
$Comp
L power:GND #PWR02
U 1 1 61F8D20A
P 1850 4300
F 0 "#PWR02" H 1850 4100 50  0001 C CNN
F 1 "GND" H 1850 4190 50  0001 C CNN
F 2 "" H 1850 4300 50  0001 C CNN
F 3 "" H 1850 4300 50  0001 C CNN
	1    1850 4300
	1    0    0    -1  
$EndComp
$Comp
L display:Graphic-OLED-Display_4wireSPI MOD1
U 1 1 61F8E5AA
P 2850 5400
F 0 "MOD1" H 2500 6150 50  0000 L CNN
F 1 "Graphic-OLED-Display_4wireSPI" H 2200 5750 50  0001 L CNN
F 2 "Connector_Header:HEADER_7x1" H 2845 5400 60  0001 C CNN
F 3 "" H 2845 5400 60  0001 C CNN
	1    2850 5400
	1    0    0    -1  
$EndComp
Text Notes 3350 4750 0    50   ~ 0
optional Display.\nOnly as connector on PCB
Wire Wire Line
	2650 3300 2650 3200
Wire Wire Line
	2700 3300 2700 3800
Wire Wire Line
	2700 3800 2350 3800
Wire Wire Line
	2350 4000 2750 4000
Wire Wire Line
	2750 4000 2750 3400
Wire Wire Line
	2350 4200 2800 4200
Wire Wire Line
	2800 4200 2800 3500
Text Label 2800 3100 0    40   ~ 0
Enc_A
Text Label 2800 3200 0    40   ~ 0
Enc_B
Text Label 2800 3300 0    40   ~ 0
Key0
Text Label 2800 3400 0    40   ~ 0
Key1
Text Label 2800 3500 0    40   ~ 0
Key2
Text Label 4600 3050 0    40   ~ 0
MotA
Text Label 4600 3150 0    40   ~ 0
-MotA
Text Label 4600 3250 0    40   ~ 0
MotB
Text Label 4600 3350 0    40   ~ 0
-MotB
$Comp
L standard:R R2
U 1 1 61E4BE9D
P 5600 3200
F 0 "R2" H 5850 3250 50  0000 C CNN
F 1 "680" H 5600 3200 50  0000 C CNN
F 2 "Resistor:R_0805" H 5600 3200 60  0001 C CNN
F 3 "" H 5600 3200 60  0000 C CNN
	1    5600 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 3600 6900 3900
Wire Wire Line
	6900 3900 5800 3900
Wire Wire Line
	5800 3750 6850 3750
Wire Wire Line
	6850 3750 6850 3050
Wire Wire Line
	5900 3050 5800 3050
Wire Wire Line
	5800 3200 5850 3200
Wire Wire Line
	5850 3200 5850 3600
Wire Wire Line
	5850 3600 5900 3600
Wire Wire Line
	5400 3050 4550 3050
Wire Wire Line
	5400 3200 5350 3200
Wire Wire Line
	5350 3200 5350 3150
Wire Wire Line
	5350 3150 4550 3150
Wire Wire Line
	5400 3750 5300 3750
Wire Wire Line
	5300 3750 5300 3250
Wire Wire Line
	5300 3250 4550 3250
Wire Wire Line
	5400 3900 5250 3900
Wire Wire Line
	5250 3900 5250 3350
Wire Wire Line
	5250 3350 4550 3350
$Comp
L power:GND #PWR06
U 1 1 61FDF63A
P 4650 4300
F 0 "#PWR06" H 4650 4100 50  0001 C CNN
F 1 "GND" H 4650 4190 50  0001 C CNN
F 2 "" H 4650 4300 50  0001 C CNN
F 3 "" H 4650 4300 50  0001 C CNN
	1    4650 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 4050 4650 4050
Wire Wire Line
	4650 4050 4650 4300
$Comp
L power:+5V #PWR05
U 1 1 61FE47EA
P 4750 2200
F 0 "#PWR05" H 4750 2400 50  0001 C CNN
F 1 "+5V" H 4746 2343 50  0000 C CNN
F 2 "" H 4750 2200 50  0001 C CNN
F 3 "" H 4750 2200 50  0001 C CNN
	1    4750 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5400 2550 5700
$Comp
L power:GND #PWR03
U 1 1 61FFA21E
P 2550 5700
F 0 "#PWR03" H 2550 5500 50  0001 C CNN
F 1 "GND" H 2550 5590 50  0001 C CNN
F 2 "" H 2550 5700 50  0001 C CNN
F 3 "" H 2550 5700 50  0001 C CNN
	1    2550 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR04
U 1 1 61FFA462
P 2650 5500
F 0 "#PWR04" H 2650 5700 50  0001 C CNN
F 1 "+5V" H 2647 5643 50  0000 C CNN
F 2 "" H 2650 5500 50  0001 C CNN
F 3 "" H 2650 5500 50  0001 C CNN
	1    2650 5500
	-1   0    0    1   
$EndComp
Wire Wire Line
	2650 5400 2650 5500
Entry Wire Line
	2750 5600 2850 5700
Entry Wire Line
	2850 5600 2950 5700
Entry Wire Line
	3150 5600 3250 5700
Text Label 2750 5600 1    40   ~ 0
SCK
Text Label 2850 5600 1    40   ~ 0
MOSI
Text Label 4600 3500 0    40   ~ 0
MOSI
Entry Wire Line
	4750 3500 4850 3600
Entry Wire Line
	4750 3700 4850 3800
Wire Wire Line
	4550 3500 4750 3500
Wire Wire Line
	4550 3700 4750 3700
Text Label 4600 3700 0    40   ~ 0
SCK
NoConn ~ 4550 3850
Text Label 4600 3950 0    40   ~ 0
-Reset
Text Label 2950 5600 1    40   ~ 0
-Reset
Entry Wire Line
	2950 5600 3050 5700
Entry Wire Line
	4750 3950 4850 4050
Wire Wire Line
	4750 3950 4550 3950
Text Label 3150 5600 1    40   ~ 0
LCDCS
Wire Wire Line
	2750 5400 2750 5600
Wire Wire Line
	2850 5400 2850 5600
Wire Wire Line
	2950 5400 2950 5600
Wire Wire Line
	3150 5400 3150 5600
Wire Notes Line
	2400 4550 2400 5800
Wire Notes Line
	2400 5800 3300 5800
Wire Notes Line
	3300 5800 3300 4550
Wire Notes Line
	3300 4550 2400 4550
$Comp
L power:+5V #PWR07
U 1 1 62037F25
P 6700 2600
F 0 "#PWR07" H 6700 2800 50  0001 C CNN
F 1 "+5V" H 6696 2743 50  0000 C CNN
F 2 "" H 6700 2600 50  0001 C CNN
F 3 "" H 6700 2600 50  0001 C CNN
	1    6700 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 62039745
P 6900 4650
F 0 "#PWR08" H 6900 4450 50  0001 C CNN
F 1 "GND" H 6900 4540 50  0001 C CNN
F 2 "" H 6900 4650 50  0001 C CNN
F 3 "" H 6900 4650 50  0001 C CNN
	1    6900 4650
	1    0    0    -1  
$EndComp
Text Label 2900 2950 0    40   ~ 0
LCDCS
NoConn ~ 4550 3600
NoConn ~ 3600 2700
$Comp
L standard:R R6
U 1 1 6205A59A
P 6600 4250
F 0 "R6" H 6600 4150 50  0000 C CNN
F 1 "30" H 6600 4250 50  0000 C CNN
F 2 "Resistor:R_1206" H 6600 4250 60  0001 C CNN
F 3 "" H 6600 4250 60  0000 C CNN
	1    6600 4250
	0    1    1    0   
$EndComp
$Comp
L standard:R R5
U 1 1 6205BA7C
P 6400 4250
F 0 "R5" H 6400 4150 50  0000 C CNN
F 1 "30" H 6400 4250 50  0000 C CNN
F 2 "Resistor:R_1206" H 6400 4250 60  0001 C CNN
F 3 "" H 6400 4250 60  0000 C CNN
	1    6400 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 2700 6700 2700
Wire Wire Line
	6200 3300 7500 3300
Wire Wire Line
	6900 3050 6850 3050
$Comp
L connectors:CONN_3X1 J2
U 1 1 620BDEE2
P 7700 5700
F 0 "J2" H 7800 5400 50  0000 C CNN
F 1 "CONN_3X1" H 7800 5450 25  0000 C CNN
F 2 "Connector_Header:HEADER_3x1" H 7700 5700 60  0001 C CNN
F 3 "" H 7700 5700 60  0000 C CNN
	1    7700 5700
	1    0    0    1   
$EndComp
$Bitmap
Pos 8200 5700
Scale 1.000000
Data
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 00 00 00 A6 00 00 00 90 08 02 00 00 00 0D C9 87 
90 00 00 00 03 73 42 49 54 08 08 08 DB E1 4F E0 00 00 00 09 70 48 59 73 00 00 0E 74 00 00 0E 74 
01 6B 24 B3 D6 00 00 04 3D 49 44 41 54 78 9C ED DD 5B 72 E2 30 10 85 61 79 62 F6 C1 4E 60 67 21 
4B CD 3A 80 F2 3C A8 46 E5 B1 B0 90 D1 AD BB CF F9 DE 86 64 80 E4 8F 1A 63 8C 99 96 65 71 84 E4 
CF E8 3B 40 BD 31 39 1C 26 87 C3 E4 70 98 1C CF D2 D2 E8 1F 4E 8D EB F5 FA FD FD DD B4 45 30 B7 
FE 61 18 3E C7 34 4D 97 CB A5 CF 6D 71 B0 C3 61 72 38 4C 0E 87 C9 E1 30 39 1C 26 87 C3 E4 70 98 
1C 0E 93 C3 61 72 38 4C 0E 87 C9 E1 30 39 1C 26 87 C3 E4 70 0E 24 9F E7 79 FA DF E3 F1 68 77 CF 
A8 91 E6 87 48 50 B9 79 9E 9F CF 67 E6 37 DF EF F7 79 4E 65 E5 60 87 F3 7E 95 1F FA 13 23 F9 B8 
CA E5 0A 1B 4F 61 C9 DD EF F7 BD 83 18 BF BE BE FC F7 9C 4E A7 F4 96 16 93 C3 D9 1D EC 9C E7 56 
71 8B 5D AE 46 CF 81 39 D8 E1 EC AE F2 F8 4F 8C A3 DE 06 0E 76 DD E2 75 C8 5D 31 B4 C5 E4 70 38 
D8 55 FA 60 9E 07 5C E5 70 44 AF F2 69 9A 12 5F AD FE 36 E6 C4 CD 59 7A CB B4 E8 E4 E9 5F F4 34 
4D 15 4B D4 BD B6 46 4A E6 79 A0 7B B0 A7 C7 40 FF EB 51 41 F4 2A 4F 5B 96 C5 BF 64 F4 F2 4B F9 
D7 E3 AF 41 FE 12 AF 45 71 72 F7 AF FA E6 92 43 D7 A0 A2 77 95 79 1E E8 1E EC 1B 26 7B 57 67 2A 
F9 21 98 BD 9D F6 C1 1E 84 C7 F5 CC 84 2A 7A D7 9D E7 81 9D 55 EE FB E5 6C 7B AB E8 DD 8E 85 E4 
21 5E 4E 75 F0 DE CE 46 F2 B5 74 75 C3 BD F3 DF 58 A2 3E 79 DC 6F AF BA E1 DE 87 14 1D EE 78 3A 
9D 36 97 54 D9 BE 28 17 6F CD B1 77 30 3E 4F 23 EB EA 66 7A C7 6B EC 03 EA 07 7B C2 7A C2 1B E8 
5D CB 81 C3 1D 35 F2 4B 9C BD D7 6C 0E F6 CD B6 DB FA 9F 8A F2 37 5A 75 36 93 2B EA DA 9F E5 C7 
72 7A 89 C9 E1 30 39 1C 26 87 C3 E4 70 98 1C 0E 93 C3 61 72 38 4C 0E 87 C9 E1 30 39 1C 26 87 C3 
E4 70 9A BF 92 76 BB DD 5A DF 04 1D C2 E4 22 FC FC FC 74 BB 2D 0E 76 38 4C 0E 87 C9 E1 30 39 1C 
26 87 C3 E4 70 98 1C 8E F1 E4 D7 EB 75 73 09 D4 C9 9F 5E B2 79 1C BB E7 EB C6 6F 46 04 7F FF 8A 
F1 55 4E 31 B3 C9 C3 52 0E EF 3C 8D 2F C1 64 36 39 ED 31 9B 7C FD 68 1D 16 F7 CB AF A2 31 9B 9C 
F6 30 39 1C E3 C9 F7 4E 1E 84 CC 78 72 8A D5 F9 00 4C 21 27 82 F2 C0 F7 B4 BC 65 73 95 BF 7D DA 
CD E7 E5 06 21 47 4D DB 4E E3 43 67 07 0E DF 1C CE 47 26 64 A2 1E 3D 71 33 14 B3 AB 3C FF C4 CD 
1A E5 9F B1 35 66 36 B9 B3 5E FD 63 DB 89 6D E3 0C 7F 01 27 7C CC F2 2A F7 2C AD F5 30 CF 4B 3E 
55 DC 7E 72 67 AB 7A 39 88 E4 8E D5 57 50 92 3B CD D5 AB CC F3 00 28 B9 D3 5C BD A2 F7 3B C6 73 
76 B6 8B DA C7 9E E6 B7 E1 CF E7 F3 E8 3B 32 8C 8E 4E B1 C2 95 9A 3E FC 4D DA 33 BA F8 99 F3 A1 
17 BD B6 FF B7 F8 FE A4 B4 3B 03 7E E2 3A 73 3E 34 EB F7 F7 57 5A D7 6E DE 27 4F EC 9C C9 D9 C7 
2E EA 73 0F C2 87 6E 20 3F 9C F7 D8 7C 13 F2 2B E6 87 AC 78 9D B6 D8 87 57 67 EF A0 DF 93 B4 81 
D5 D9 7B AD EB F3 F2 21 D5 D9 7B A3 F7 AE 98 CE D5 D9 3B 36 60 EF 5B B7 EA EC FD D2 98 1D AE 1D 
AA B3 F7 9E 61 FB D8 9B 56 67 EF 84 6D F2 92 83 AA 8E 6A 5A 9D BD F7 0C 7E 25 AD 51 75 F6 4E 18 
FF E2 69 F5 EA 39 BD 91 FF 26 76 0F 77 8C F7 9F 27 14 BE 78 2A 6A 3F BC 79 E3 57 B9 37 7C 8F 2C 
0E 29 C9 1D AB F7 22 EB 23 EB 39 E1 3B 10 77 54 0C AB C7 72 8E 81 89 37 B9 F6 36 B0 04 0D F6 80 
13 BE 29 89 C9 1D AB B7 24 6E B0 07 9C F0 41 DD ED 2A A1 AB DC E3 5A 6F 41 74 72 C7 EA 0D 48 4F 
EE 58 BD 36 05 C9 1D AB 57 A5 23 B9 63 F5 7A D4 24 77 AC 5E 89 A6 E4 8E D5 6B 50 96 DC B1 7A 31 
7D C9 1D AB 97 51 99 DC B1 7A 01 AD C9 1D AB 7F 4A 71 72 C7 EA 1F 91 FB B2 4A 26 1B D5 E3 8F F0 
6B C7 C8 4B 55 B7 DB 6D F4 5D 28 D5 ED 47 30 92 9C F2 E9 7E 2C A7 0F 30 39 1C 26 87 C3 E4 70 98 
1C 0E 93 C3 F9 0B A3 E6 FA B6 AE 73 B7 5C 00 00 00 00 49 45 4E 44 AE 42 60 82 
EndData
$EndBitmap
Wire Wire Line
	7500 5800 7400 5800
Wire Wire Line
	7400 5800 7400 5900
$Comp
L power:GND #PWR010
U 1 1 620D16D3
P 7400 5900
F 0 "#PWR010" H 7400 5700 50  0001 C CNN
F 1 "GND" H 7400 5790 50  0001 C CNN
F 2 "" H 7400 5900 40  0001 C CNN
F 3 "" H 7400 5900 40  0001 C CNN
	1    7400 5900
	1    0    0    -1  
$EndComp
$Comp
L standard:R R16
U 1 1 620D256E
P 7400 5300
F 0 "R16" V 7200 5300 50  0000 L CNN
F 1 "2k2" H 7350 5300 50  0000 L CNN
F 2 "Resistor:R_0805" H 7400 5300 60  0001 C CNN
F 3 "" H 7400 5300 60  0000 C CNN
	1    7400 5300
	0    1    1    0   
$EndComp
$Comp
L standard:R R15
U 1 1 620D39C5
P 7200 5300
F 0 "R15" V 7000 5300 50  0000 L CNN
F 1 "100k" H 7100 5300 50  0000 L CNN
F 2 "Resistor:R_0805" H 7200 5300 60  0001 C CNN
F 3 "" H 7200 5300 60  0000 C CNN
	1    7200 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	7400 5100 7400 4850
Wire Wire Line
	7500 5700 7200 5700
Wire Wire Line
	7200 5700 7200 5500
Wire Wire Line
	7200 5700 6700 5700
Connection ~ 7200 5700
Text Label 6700 5700 0    40   ~ 0
LBSense
Text Label 7400 4850 3    40   ~ 0
LBOn
Wire Wire Line
	7500 5600 7400 5600
Wire Wire Line
	7400 5600 7400 5500
$Comp
L power:+5V #PWR09
U 1 1 620DE0CE
P 7200 5000
F 0 "#PWR09" H 7200 5200 50  0001 C CNN
F 1 "+5V" H 7196 5143 50  0000 C CNN
F 2 "" H 7200 5000 40  0001 C CNN
F 3 "" H 7200 5000 40  0001 C CNN
	1    7200 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 5000 7200 5100
Wire Wire Line
	4550 2850 4800 2850
Text Label 4600 2850 0    40   ~ 0
LBOn
Text Label 2900 2850 0    40   ~ 0
LBSense
$Comp
L connectors:CONN_5X2 J4
U 1 1 62201AFB
P 7800 1600
F 0 "J4" H 7800 1975 50  0000 C CNN
F 1 "CONN_5X2" H 7800 1903 25  0000 C CNN
F 2 "Connector_Header:HEADER_5x2" H 7800 1600 60  0001 C CNN
F 3 "" H 7800 1600 60  0000 C CNN
	1    7800 1600
	1    0    0    -1  
$EndComp
Text Label 7200 1600 0    40   ~ 0
A
Text Label 7200 1700 0    40   ~ 0
B
Text Label 7200 1400 0    40   ~ 0
24V_IN
Text Label 7200 1500 0    40   ~ 0
24V_IN
Text Label 6500 1800 0    40   ~ 0
Contact
Wire Wire Line
	7600 1400 7200 1400
Wire Wire Line
	7200 1500 7600 1500
Wire Wire Line
	7200 1700 7600 1700
Wire Wire Line
	7200 1600 7600 1600
Wire Wire Line
	8000 1400 8100 1400
Wire Wire Line
	8100 1400 8100 1500
Wire Wire Line
	8000 1800 8100 1800
Connection ~ 8100 1800
Wire Wire Line
	8100 1800 8100 1900
Wire Wire Line
	8000 1700 8100 1700
Connection ~ 8100 1700
Wire Wire Line
	8100 1700 8100 1800
Wire Wire Line
	8000 1600 8100 1600
Connection ~ 8100 1600
Wire Wire Line
	8100 1600 8100 1700
Wire Wire Line
	8000 1500 8100 1500
Connection ~ 8100 1500
Wire Wire Line
	8100 1500 8100 1600
$Comp
L power:GND #PWR0101
U 1 1 62222A9D
P 8100 1900
F 0 "#PWR0101" H 8100 1700 50  0001 C CNN
F 1 "GND" H 8100 1790 50  0001 C CNN
F 2 "" H 8100 1900 40  0001 C CNN
F 3 "" H 8100 1900 40  0001 C CNN
	1    8100 1900
	1    0    0    -1  
$EndComp
$Comp
L standard:R R17
U 1 1 622266DB
P 6950 1800
F 0 "R17" H 6950 1900 50  0000 C CNN
F 1 "4k7" H 6950 1800 50  0000 C CNN
F 2 "Resistor:R_0805" H 6950 1800 60  0001 C CNN
F 3 "" H 6950 1800 60  0000 C CNN
	1    6950 1800
	1    0    0    -1  
$EndComp
$Comp
L diodes:BAT54S D1
U 1 1 6222D040
P 6550 1350
F 0 "D1" H 6630 1396 50  0000 L CNN
F 1 "BAT54S" H 6630 1305 50  0000 L CNN
F 2 "Diode_SMD:D_SOD323" H 6550 1350 60  0001 C CNN
F 3 "" H 6550 1350 60  0000 C CNN
	1    6550 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 6222E968
P 6550 1600
F 0 "#PWR0102" H 6550 1400 50  0001 C CNN
F 1 "GND" H 6550 1490 50  0001 C CNN
F 2 "" H 6550 1600 40  0001 C CNN
F 3 "" H 6550 1600 40  0001 C CNN
	1    6550 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 1350 6350 1350
Wire Wire Line
	6350 1350 6350 1800
Wire Wire Line
	6350 1800 6750 1800
Wire Wire Line
	6550 1550 6550 1600
Wire Wire Line
	6550 1150 6550 1100
$Comp
L power:VCC #PWR0103
U 1 1 6223AE03
P 6550 1100
F 0 "#PWR0103" H 6550 1275 50  0001 C CNN
F 1 "VCC" H 6553 1243 50  0000 C CNN
F 2 "" H 6550 1100 40  0001 C CNN
F 3 "" H 6550 1100 40  0001 C CNN
	1    6550 1100
	1    0    0    -1  
$EndComp
Text Notes 7450 1100 0    40   ~ 0
Connector connecting to bus wall outlet
Wire Wire Line
	7150 1800 7600 1800
Wire Wire Line
	4550 2650 4650 2650
$Comp
L standard:L L1
U 1 1 6226FB0E
P 4850 2500
F 0 "L1" V 4804 2578 50  0000 L CNN
F 1 "10µH" V 4895 2578 50  0000 L CNN
F 2 "Inductor:L_0805" H 4850 2500 60  0001 C CNN
F 3 "" H 4850 2500 60  0000 C CNN
	1    4850 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 2750 4850 2700
Wire Wire Line
	4550 2750 4850 2750
Wire Wire Line
	4850 2300 4850 2250
Wire Wire Line
	4850 2250 4750 2250
Wire Wire Line
	4650 2250 4650 2650
Wire Wire Line
	4750 2200 4750 2250
Connection ~ 4750 2250
Wire Wire Line
	4750 2250 4650 2250
$Comp
L standard:C C3
U 1 1 6227E78B
P 3400 3650
F 0 "C3" H 3050 3700 50  0000 C CNN
F 1 "100nF" H 3250 3700 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 3400 3650 60  0001 C CNN
F 3 "" H 3400 3650 60  0000 C CNN
	1    3400 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 6227F5AC
P 3200 3750
F 0 "#PWR0104" H 3200 3550 50  0001 C CNN
F 1 "GND" H 3200 3640 50  0001 C CNN
F 2 "" H 3200 3750 40  0001 C CNN
F 3 "" H 3200 3750 40  0001 C CNN
	1    3200 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 3650 3500 3650
Wire Wire Line
	3300 3650 3200 3650
Wire Wire Line
	3200 3650 3200 3750
Text Label 2900 3950 0    40   ~ 0
LBSense
Text Label 2900 4050 0    40   ~ 0
MotCurrent
Wire Wire Line
	4550 2950 5850 2950
Wire Wire Line
	5850 2950 5850 1800
Wire Wire Line
	5850 1800 6350 1800
Connection ~ 6350 1800
$Comp
L connectors:JUMPER J5
U 1 1 6212ADC7
P 3350 2850
F 0 "J5" H 3350 3025 50  0000 C CNN
F 1 "JUMPER" H 3350 2953 25  0000 C CNN
F 2 "Resistor:R_0805" H 3350 2850 60  0001 C CNN
F 3 "" H 3350 2850 60  0000 C CNN
	1    3350 2850
	1    0    0    -1  
$EndComp
$Comp
L connectors:JUMPER J6
U 1 1 62139A12
P 3350 3950
F 0 "J6" H 3350 4125 50  0000 C CNN
F 1 "JUMPER" H 3350 4053 25  0000 C CNN
F 2 "Resistor:R_0805" H 3350 3950 60  0001 C CNN
F 3 "" H 3350 3950 60  0000 C CNN
	1    3350 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3950 2900 3950
Wire Wire Line
	2900 4050 3600 4050
Wire Wire Line
	3500 3950 3600 3950
Wire Wire Line
	2800 3500 3600 3500
Wire Wire Line
	2750 3400 3600 3400
Wire Wire Line
	2700 3300 3600 3300
Wire Wire Line
	2650 3200 3600 3200
Wire Wire Line
	2650 3100 3600 3100
Wire Wire Line
	2900 2850 3200 2850
Wire Wire Line
	3500 2850 3600 2850
Wire Wire Line
	2900 2950 3600 2950
Wire Wire Line
	6700 2600 6700 2700
Connection ~ 6700 2700
Wire Wire Line
	6700 2700 7200 2700
$Comp
L standard:R R7
U 1 1 6205C5EB
P 6800 4350
F 0 "R7" H 6800 4450 50  0000 C CNN
F 1 "30" H 6800 4350 50  0000 C CNN
F 2 "Resistor:R_1206" H 6800 4350 60  0001 C CNN
F 3 "" H 6800 4350 60  0000 C CNN
	1    6800 4350
	0    1    1    0   
$EndComp
$Comp
L standard:R R8
U 1 1 621371F6
P 7000 4350
F 0 "R8" H 6950 4450 50  0000 L CNN
F 1 "30" H 7000 4350 50  0000 C CNN
F 2 "Resistor:R_1206" H 7000 4350 60  0001 C CNN
F 3 "" H 7000 4350 60  0000 C CNN
	1    7000 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 4000 6400 4000
Wire Wire Line
	6400 4000 6400 4050
Connection ~ 6400 4000
Wire Wire Line
	6400 4000 6600 4000
Wire Wire Line
	6600 4050 6600 4000
Connection ~ 6600 4000
Wire Wire Line
	6400 4500 6400 4450
Wire Wire Line
	6600 4450 6600 4500
Wire Wire Line
	6600 4500 6400 4500
Text Notes 7150 4400 0    40   ~ 0
30Ohm, max 250mA => max 2W (when Motor blocks)\n30Ohm, 40ma (Motor turning without valve)\n0,5W each.\n
Wire Wire Line
	6600 4000 7200 4000
Wire Wire Line
	6800 4150 6800 4100
Wire Wire Line
	6800 4100 7000 4100
Wire Wire Line
	7000 4100 7000 4150
Wire Wire Line
	6800 4100 6700 4100
Wire Wire Line
	6700 4100 6700 4500
Wire Wire Line
	6700 4500 6600 4500
Connection ~ 6800 4100
Connection ~ 6600 4500
Wire Wire Line
	6800 4550 6800 4600
Wire Wire Line
	6800 4600 6900 4600
Wire Wire Line
	7000 4600 7000 4550
Wire Wire Line
	6900 4650 6900 4600
Connection ~ 6900 4600
Wire Wire Line
	6900 4600 7000 4600
$Comp
L standard:ZD D2
U 1 1 62146ED5
P 6100 1350
F 0 "D2" V 6146 1272 50  0000 R CNN
F 1 "5V6" V 6055 1272 50  0000 R CNN
F 2 "Diode_SMD:D_MINIMELF" H 6100 1350 60  0001 C CNN
F 3 "" H 6100 1350 60  0000 C CNN
	1    6100 1350
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR011
U 1 1 621495D6
P 6100 1100
F 0 "#PWR011" H 6100 1275 50  0001 C CNN
F 1 "VCC" H 6103 1243 50  0000 C CNN
F 2 "" H 6100 1100 40  0001 C CNN
F 3 "" H 6100 1100 40  0001 C CNN
	1    6100 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 62149E85
P 6100 1600
F 0 "#PWR012" H 6100 1400 50  0001 C CNN
F 1 "GND" H 6100 1490 50  0001 C CNN
F 2 "" H 6100 1600 40  0001 C CNN
F 3 "" H 6100 1600 40  0001 C CNN
	1    6100 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1100 6100 1150
Wire Wire Line
	6100 1550 6100 1600
$Comp
L transistors:FMMT720 Q1
U 1 1 621E16C8
P 6100 3050
F 0 "Q1" H 6290 3004 50  0000 L CNN
F 1 "FMMT720" H 6290 3095 50  0000 L CNN
F 2 "Housings_SOT:SOT23_21-3" H 6291 3141 29  0001 L CNN
F 3 "" H 6100 3050 60  0000 C CNN
	1    6100 3050
	1    0    0    1   
$EndComp
$Comp
L transistors:FMMT720 Q3
U 1 1 621ED987
P 7100 3050
F 0 "Q3" H 7290 3004 50  0000 L CNN
F 1 "FMMT720" H 7290 3095 50  0000 L CNN
F 2 "Housings_SOT:SOT23_21-3" H 7291 3141 29  0001 L CNN
F 3 "" H 7100 3050 60  0001 C CNN
	1    7100 3050
	1    0    0    1   
$EndComp
Wire Bus Line
	4850 3600 4850 5700
Wire Bus Line
	2850 5700 4850 5700
$Comp
L transistors:FMMT619 Q2
U 1 1 621FAE8D
P 6100 3600
F 0 "Q2" H 6290 3646 50  0000 L CNN
F 1 "FMMT619" H 6290 3555 50  0000 L CNN
F 2 "Housings_SOT:SOT23_21-3" H 6000 3702 29  0001 C CNN
F 3 "" H 6100 3600 60  0000 C CNN
	1    6100 3600
	1    0    0    -1  
$EndComp
$Comp
L transistors:FMMT619 Q4
U 1 1 621FEB0E
P 7100 3600
F 0 "Q4" H 7290 3646 50  0000 L CNN
F 1 "FMMT619" H 7290 3555 50  0000 L CNN
F 2 "Housings_SOT:SOT23_21-3" H 7000 3702 29  0001 C CNN
F 3 "" H 7100 3600 60  0000 C CNN
	1    7100 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC