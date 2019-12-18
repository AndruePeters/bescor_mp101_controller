EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A0
U 1 1 5DF7ED8F
P 5950 3350
F 0 "A0" H 5950 2261 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 5950 2170 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 6100 2400 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5950 2350 50  0001 C CNN
	1    5950 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_RCGB D0_Status
U 1 1 5DF80815
P 1750 1950
F 0 "D0_Status" H 1750 2447 50  0000 C CNN
F 1 "LED_RCGB" H 1750 2356 50  0000 C CNN
F 2 "" H 1750 1900 50  0001 C CNN
F 3 "~" H 1750 1900 50  0001 C CNN
	1    1750 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector:DIN-7 J?
U 1 1 5DF81442
P 1800 4400
F 0 "J?" H 1800 4125 50  0000 C CNN
F 1 "DIN-7" H 1800 4034 50  0000 C CNN
F 2 "" H 1800 4400 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 1800 4400 50  0001 C CNN
	1    1800 4400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW0
U 1 1 5DF8236A
P 7800 1650
F 0 "SW0" H 7800 1935 50  0000 C CNN
F 1 "SW_Push" H 7800 1844 50  0000 C CNN
F 2 "" H 7800 1850 50  0001 C CNN
F 3 "~" H 7800 1850 50  0001 C CNN
	1    7800 1650
	1    0    0    -1  
$EndComp
$Comp
L RF:NRF24L01_Breakout U0
U 1 1 5DF8328B
P 9050 3050
F 0 "U0" H 9430 3096 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 9430 3005 50  0000 L CNN
F 2 "RF_Module:nRF24L01_Breakout" H 9200 3650 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 9050 2950 50  0001 C CNN
	1    9050 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DF8416A
P 1550 1950
F 0 "#PWR?" H 1550 1700 50  0001 C CNN
F 1 "GND" V 1555 1822 50  0000 R CNN
F 2 "" H 1550 1950 50  0001 C CNN
F 3 "" H 1550 1950 50  0001 C CNN
	1    1550 1950
	0    1    1    0   
$EndComp
Text Label 5250 3250 2    50   ~ 0
D5_RLED
Wire Wire Line
	5250 3250 5450 3250
Text Label 5250 3650 2    50   ~ 0
D9_GLED
Wire Wire Line
	5250 3650 5450 3650
Text Label 5250 3750 2    50   ~ 0
D10_BLED
Wire Wire Line
	5250 3750 5450 3750
$Comp
L Device:R_Small_US R_RLED
U 1 1 5DF88E36
P 2150 1750
F 0 "R_RLED" V 2200 1700 50  0000 C CNN
F 1 "220" V 2250 1750 50  0000 C CNN
F 2 "" H 2150 1750 50  0001 C CNN
F 3 "~" H 2150 1750 50  0001 C CNN
	1    2150 1750
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R_GLED
U 1 1 5DF8A0BF
P 2150 1950
F 0 "R_GLED" V 2200 1900 50  0000 C CNN
F 1 "220" V 2250 1950 50  0000 C CNN
F 2 "" H 2150 1950 50  0001 C CNN
F 3 "~" H 2150 1950 50  0001 C CNN
	1    2150 1950
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R_BLED
U 1 1 5DF8A41A
P 2150 2150
F 0 "R_BLED" V 2200 2150 50  0000 C CNN
F 1 "220" V 2300 2150 50  0000 C CNN
F 2 "" H 2150 2150 50  0001 C CNN
F 3 "~" H 2150 2150 50  0001 C CNN
	1    2150 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	1950 1750 2050 1750
Wire Wire Line
	2050 1950 1950 1950
Wire Wire Line
	1950 2150 2050 2150
Text Label 2450 1750 0    50   ~ 0
D5_RLED
Text Label 2450 1950 0    50   ~ 0
D9_GLED
Text Label 2450 2150 0    50   ~ 0
D10_BLED
Wire Wire Line
	2250 2150 2450 2150
Wire Wire Line
	2250 1950 2450 1950
Wire Wire Line
	2250 1750 2450 1750
Text Label 5250 3350 2    50   ~ 0
D6_MTR_Speed
Wire Wire Line
	5250 3350 5450 3350
Text Label 6750 3550 0    50   ~ 0
A2_MTR_Right
Wire Wire Line
	6750 3550 6450 3550
Text Label 5250 3150 2    50   ~ 0
D4_MTR_Left
Wire Wire Line
	5250 3150 5450 3150
Text Label 6750 3450 0    50   ~ 0
A1_MTR_Up
Wire Wire Line
	6750 3450 6450 3450
Text Label 5250 2950 2    50   ~ 0
D2_MTR_Down
Wire Wire Line
	5250 2950 5450 2950
Text Label 6750 3650 0    50   ~ 0
A3_ID_Btn
Wire Wire Line
	6750 3650 6450 3650
$Comp
L power:GND #PWR?
U 1 1 5DF941B9
P 9050 3700
F 0 "#PWR?" H 9050 3450 50  0001 C CNN
F 1 "GND" V 9055 3572 50  0000 R CNN
F 2 "" H 9050 3700 50  0001 C CNN
F 3 "" H 9050 3700 50  0001 C CNN
	1    9050 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 3650 9050 3700
Text Label 6050 1950 0    50   ~ 0
3V3_ARDUINO_OUT
Wire Wire Line
	6050 1950 6050 2350
Text Label 9050 1900 0    50   ~ 0
3V3_ARDUINO_OUT
$Comp
L Device:CP1_Small C3v3_1
U 1 1 5DF97525
P 9150 2100
F 0 "C3v3_1" V 9250 1450 50  0000 C CNN
F 1 "10uF" V 9250 1800 50  0000 C CNN
F 2 "" H 9150 2100 50  0001 C CNN
F 3 "~" H 9150 2100 50  0001 C CNN
	1    9150 2100
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C?
U 1 1 5DF98D39
P 9150 2300
F 0 "C?" V 9250 1650 50  0000 C CNN
F 1 "0.1uF" V 9250 2000 50  0000 C CNN
F 2 "" H 9150 2300 50  0001 C CNN
F 3 "~" H 9150 2300 50  0001 C CNN
	1    9150 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9050 1900 9050 2100
Wire Wire Line
	9050 2100 9050 2300
Connection ~ 9050 2100
Wire Wire Line
	9050 2300 9050 2450
Connection ~ 9050 2300
$Comp
L power:GND #PWR?
U 1 1 5DF9B24F
P 9300 2100
F 0 "#PWR?" H 9300 1850 50  0001 C CNN
F 1 "GND" V 9305 1972 50  0000 R CNN
F 2 "" H 9300 2100 50  0001 C CNN
F 3 "" H 9300 2100 50  0001 C CNN
	1    9300 2100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9250 2100 9300 2100
$Comp
L power:GND #PWR?
U 1 1 5DF9B48B
P 9300 2300
F 0 "#PWR?" H 9300 2050 50  0001 C CNN
F 1 "GND" V 9305 2172 50  0000 R CNN
F 2 "" H 9300 2300 50  0001 C CNN
F 3 "" H 9300 2300 50  0001 C CNN
	1    9300 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9250 2300 9300 2300
Text Label 5250 3450 2    50   ~ 0
D7_CE
Wire Wire Line
	5250 3450 5450 3450
Text Label 5250 3550 2    50   ~ 0
D8_CS
Wire Wire Line
	5250 3550 5450 3550
Text Label 5250 3950 2    50   ~ 0
D12_MISO
Wire Wire Line
	5250 3950 5450 3950
Text Label 8200 2850 2    50   ~ 0
D12_MISO
Wire Wire Line
	8200 2850 8550 2850
Text Label 5250 4050 2    50   ~ 0
D13_SCK
Wire Wire Line
	5250 4050 5450 4050
Text Label 8200 2950 2    50   ~ 0
D13_SCK
Wire Wire Line
	8200 2950 8550 2950
Text Label 8200 2750 2    50   ~ 0
D11_MOSI
Wire Wire Line
	8200 2750 8550 2750
Text Label 5250 3850 2    50   ~ 0
D11_MOSI
Wire Wire Line
	5250 3850 5450 3850
Text Label 8200 3250 2    50   ~ 0
D7_CE
Wire Wire Line
	8200 3250 8550 3250
Text Label 8200 3050 2    50   ~ 0
D8_CS
Wire Wire Line
	8200 3050 8550 3050
$EndSCHEMATC
