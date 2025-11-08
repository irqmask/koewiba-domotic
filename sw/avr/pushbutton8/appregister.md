# RegisterOverview of Pushbutton8

last updated: 2025-11-08

### AppID: 0x09



|         |            |
|---------|------------|
| **p**:  | persistent |
| **v**:  | volatile   |
| **ro**: | read-only  |

## Register

| dec | hex  | Name                         | Description                                                                              | Size   | pv |
|-----|------|------------------------------|------------------------------------------------------------------------------------------|--------|----|
| 000 | 0x00 | unused                       | n/a                                                                                      |        |    |
| 001 | 0x01 | ModuleID                     | Module ID as it appears on the bus                                                       | uint16 | p  |
| 002 | 0x02 | BldFlag                      | Storage for bootloader flags                                                             | uint8  | p  |
| 003 | 0x03 |
| 004 | 0x04 |
| 005 | 0x05 |
| 006 | 0x06 |
| 007 | 0x07 |
| 008 | 0x08 |
| 009 | 0x09 |
| 010 | 0x0A |
| 011 | 0x0B |
| 012 | 0x0C |
| 013 | 0x0D |
| 014 | 0x0E |
| 015 | 0x0F |
| 016 | 0x10 | APP_eReg_K0_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 017 | 0x11 | APP_eReg_K0_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 018 | 0x12 | APP_eReg_K0_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 019 | 0x13 | APP_eReg_K0_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 020 | 0x14 | APP_eReg_K0_Reserved3        |                                                                                          | uint8  | p   |
| 021 | 0x15 | APP_eReg_K0_Reserved1        |                                                                                          | uint8  | p   |
| 022 | 0x16 | APP_eReg_K0_Reserved2        |                                                                                          | uint8  | p   |
| 023 | 0x17 | APP_eReg_K0_Reserved0        |                                                                                          | uint8  | p   |
| 024 | 0x18 | APP_eReg_K1_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 025 | 0x19 | APP_eReg_K1_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 026 | 0x1A | APP_eReg_K1_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 027 | 0x1B | APP_eReg_K1_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 028 | 0x1C | APP_eReg_K1_Reserved3        |                                                                                          | uint8  | p   |
| 029 | 0x1D | APP_eReg_K1_Reserved1        |                                                                                          | uint8  | p   |
| 030 | 0x1E | APP_eReg_K1_Reserved2        |                                                                                          | uint8  | p   |
| 031 | 0x1F | APP_eReg_K1_Reserved0        |                                                                                          | uint8  | p   |
| 032 | 0x20 | APP_eReg_K2_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 033 | 0x21 | APP_eReg_K2_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 034 | 0x22 | APP_eReg_K2_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 035 | 0x23 | APP_eReg_K2_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 036 | 0x24 | APP_eReg_K2_Reserved3        |                                                                                          | uint8  | p   |
| 037 | 0x25 | APP_eReg_K2_Reserved1        |                                                                                          | uint8  | p   |
| 038 | 0x26 | APP_eReg_K2_Reserved2        |                                                                                          | uint8  | p   |
| 039 | 0x27 | APP_eReg_K2_Reserved0        |                                                                                          | uint8  | p   |
| 040 | 0x28 | APP_eReg_K3_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 041 | 0x29 | APP_eReg_K3_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 042 | 0x2A | APP_eReg_K3_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 043 | 0x2B | APP_eReg_K3_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 044 | 0x2C | APP_eReg_K3_Reserved3        |                                                                                          | uint8  | p   |
| 045 | 0x2D | APP_eReg_K3_Reserved1        |                                                                                          | uint8  | p   |
| 046 | 0x2E | APP_eReg_K3_Reserved2        |                                                                                          | uint8  | p   |
| 047 | 0x2F | APP_eReg_K3_Reserved0        |                                                                                          | uint8  | p   |
| 048 | 0x30 | APP_eReg_K4_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 049 | 0x31 | APP_eReg_K4_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 050 | 0x32 | APP_eReg_K4_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 051 | 0x33 | APP_eReg_K4_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 052 | 0x34 | APP_eReg_K4_Reserved3        |                                                                                          | uint8  | p   |
| 053 | 0x35 | APP_eReg_K4_Reserved1        |                                                                                          | uint8  | p   |
| 054 | 0x36 | APP_eReg_K4_Reserved2        |                                                                                          | uint8  | p   |
| 055 | 0x37 | APP_eReg_K4_Reserved0        |                                                                                          | uint8  | p   |
| 056 | 0x38 | APP_eReg_K5_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 057 | 0x39 | APP_eReg_K5_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 058 | 0x3A | APP_eReg_K5_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 059 | 0x3B | APP_eReg_K5_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 060 | 0x3C | APP_eReg_K5_Reserved3        |                                                                                          | uint8  | p   |
| 061 | 0x3D | APP_eReg_K5_Reserved1        |                                                                                          | uint8  | p   |
| 062 | 0x3E | APP_eReg_K5_Reserved2        |                                                                                          | uint8  | p   |
| 063 | 0x3F | APP_eReg_K5_Reserved0        |                                                                                          | uint8  | p   |
| 064 | 0x40 | APP_eReg_K6_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 065 | 0x41 | APP_eReg_K6_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 066 | 0x42 | APP_eReg_K6_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 067 | 0x43 | APP_eReg_K6_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 068 | 0x44 | APP_eReg_K6_Reserved3        |                                                                                          | uint8  | p   |
| 069 | 0x45 | APP_eReg_K6_Reserved1        |                                                                                          | uint8  | p   |
| 070 | 0x46 | APP_eReg_K6_Reserved2        |                                                                                          | uint8  | p   |
| 071 | 0x47 | APP_eReg_K6_Reserved0        |                                                                                          | uint8  | p   |
| 072 | 0x48 | APP_eReg_K7_Mode,            | Key mode function (0=deaktivated 1=On/Off 2=Toggle)                                      | uint8  | p   |
| 073 | 0x49 | APP_eReg_K7_RemoteAddr       | Receiver's address                                                                       | uint16 | p   |
| 074 | 0x4A | APP_eReg_K7_RemoteReg,       | Receiver's register to set                                                               | uint8  | p   |
| 075 | 0x4B | APP_eReg_K7_Value            | Value to set (not used in all modes)                                                     | uint8  | p   |
| 076 | 0x4C | APP_eReg_K7_Reserved3        |                                                                                          | uint8  | p   |
| 077 | 0x4D | APP_eReg_K7_Reserved1        |                                                                                          | uint8  | p   |
| 078 | 0x4E | APP_eReg_K7_Reserved2        |                                                                                          | uint8  | p   |
| 079 | 0x4F | APP_eReg_K7_Reserved0        |                                                                                          | uint8  | p   |
| 080 | 0x50 | APP_eReg_RecMsg0_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 081 | 0x51 | APP_eReg_RecMsg0_Command     | Message command                                                                          | uint8  | p   |
| 082 | 0x52 | APP_eReg_RecMsg0_Register    | Messages Register number                                                                 | uint8  | p   |
| 083 | 0x53 | APP_eReg_RecMsg0_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 084 | 0x54 | APP_eReg_RecMsg0_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 085 | 0x55 | APP_eReg_RecMsg1_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 086 | 0x56 | APP_eReg_RecMsg1_Command     | Message command                                                                          | uint8  | p   |
| 087 | 0x57 | APP_eReg_RecMsg1_Register    | Messages Register number                                                                 | uint8  | p   |
| 088 | 0x58 | APP_eReg_RecMsg1_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 089 | 0x59 | APP_eReg_RecMsg1_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 090 | 0x5A | APP_eReg_RecMsg2_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 091 | 0x5B | APP_eReg_RecMsg2_Command     | Message command                                                                          | uint8  | p   |
| 092 | 0x5C | APP_eReg_RecMsg2_Register    | Messages Register number                                                                 | uint8  | p   |
| 093 | 0x5D | APP_eReg_RecMsg2_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 094 | 0x5E | APP_eReg_RecMsg2_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 095 | 0x5F | APP_eReg_RecMsg3_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 096 | 0x60 | APP_eReg_RecMsg3_Command     | Message command                                                                          | uint8  | p   |
| 097 | 0x61 | APP_eReg_RecMsg3_Register    | Messages Register number                                                                 | uint8  | p   |
| 098 | 0x62 | APP_eReg_RecMsg3_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 099 | 0x63 | APP_eReg_RecMsg3_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 100 | 0x64 | APP_eReg_RecMsg4_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 101 | 0x65 | APP_eReg_RecMsg4_Command     | Message command                                                                          | uint8  | p   |
| 102 | 0x66 | APP_eReg_RecMsg4_Register    | Messages Register number                                                                 | uint8  | p   |
| 103 | 0x67 | APP_eReg_RecMsg4_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 104 | 0x68 | APP_eReg_RecMsg4_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 105 | 0x69 | APP_eReg_RecMsg5_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 106 | 0x6A | APP_eReg_RecMsg5_Command     | Message command                                                                          | uint8  | p   |
| 107 | 0x6B | APP_eReg_RecMsg5_Register    | Messages Register number                                                                 | uint8  | p   |
| 108 | 0x6C | APP_eReg_RecMsg5_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 109 | 0x6D | APP_eReg_RecMsg5_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 110 | 0x6E | APP_eReg_RecMsg6_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 111 | 0x6F | APP_eReg_RecMsg6_Command     | Message command                                                                          | uint8  | p   |
| 112 | 0x70 | APP_eReg_RecMsg6_Register    | Messages Register number                                                                 | uint8  | p   |
| 113 | 0x71 | APP_eReg_RecMsg6_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 114 | 0x72 | APP_eReg_RecMsg6_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 115 | 0x73 | APP_eReg_RecMsg7_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 116 | 0x74 | APP_eReg_RecMsg7_Command     | Message command                                                                          | uint8  | p   |
| 117 | 0x75 | APP_eReg_RecMsg7_Register    | Messages Register number                                                                 | uint8  | p   |
| 118 | 0x76 | APP_eReg_RecMsg7_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 119 | 0x77 | APP_eReg_RecMsg7_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 120 | 0x78 | APP_eReg_RecMsg8_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 121 | 0x79 | APP_eReg_RecMsg8_Command     | Message command                                                                          | uint8  | p   |
| 122 | 0x7A | APP_eReg_RecMsg8_Register    | Messages Register number                                                                 | uint8  | p   |
| 123 | 0x7B | APP_eReg_RecMsg8_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 124 | 0x7C | APP_eReg_RecMsg8_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 125 | 0x7D | APP_eReg_RecMsg9_RemoteAddr  | Sender's address                                                                         | uint16 | p   |
| 126 | 0x7E | APP_eReg_RecMsg9_Command     | Message command                                                                          | uint8  | p   |
| 127 | 0x7F | APP_eReg_RecMsg9_Register    | Messages Register number                                                                 | uint8  | p   |
| 128 | 0x80 | APP_eReg_RecMsg9_LEDFunc     | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 129 | 0x81 | APP_eReg_RecMsg9_AddInfo     | Bit -> LED                                                                               | uint8  | p   |
| 130 | 0x82 | APP_eReg_RecMsg10_RemoteAddr | Sender's address                                                                         | uint16 | p   |
| 131 | 0x83 | APP_eReg_RecMsg10_Command    | Message command                                                                          | uint8  | p   |
| 132 | 0x84 | APP_eReg_RecMsg10_Register   | Messages Register number                                                                 | uint8  | p   |
| 133 | 0x85 | APP_eReg_RecMsg10_LEDFunc    | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 134 | 0x86 | APP_eReg_RecMsg10_AddInfo    | Bit -> LED                                                                               | uint8  | p   |
| 135 | 0x87 | APP_eReg_RecMsg11_RemoteAddr | Sender's address                                                                         | uint16 | p   |
| 136 | 0x88 | APP_eReg_RecMsg11_Command    | Message command                                                                          | uint8  | p   |
| 137 | 0x89 | APP_eReg_RecMsg11_Register   | Messages Register number                                                                 | uint8  | p   |
| 138 | 0x8A | APP_eReg_RecMsg11_LEDFunc    | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 139 | 0x8B | APP_eReg_RecMsg11_AddInfo    | Bit -> LED                                                                               | uint8  | p   |
| 140 | 0x8C | APP_eReg_RecMsg12_RemoteAddr | Sender's address                                                                         | uint16 | p   |
| 141 | 0x8D | APP_eReg_RecMsg12_Command    | Message command                                                                          | uint8  | p   |
| 142 | 0x8E | APP_eReg_RecMsg12_Register   | Messages Register number                                                                 | uint8  | p   |
| 143 | 0x8F | APP_eReg_RecMsg12_LEDFunc    | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 144 | 0x90 | APP_eReg_RecMsg12_AddInfo    | Bit -> LED                                                                               | uint8  | p   |
| 145 | 0x91 | APP_eReg_RecMsg13_RemoteAddr | Sender's address                                                                         | uint16 | p   |
| 146 | 0x92 | APP_eReg_RecMsg13_Command    | Message command                                                                          | uint8  | p   |
| 147 | 0x93 | APP_eReg_RecMsg13_Register   | Messages Register number                                                                 | uint8  | p   |
| 148 | 0x94 | APP_eReg_RecMsg13_LEDFunc    | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 149 | 0x95 | APP_eReg_RecMsg13_AddInfo    | Bit -> LED                                                                               | uint8  | p   |
| 150 | 0x96 | APP_eReg_RecMsg14_RemoteAddr | Sender's address                                                                         | uint16 | p   |
| 151 | 0x97 | APP_eReg_RecMsg14_Command    | Message command                                                                          | uint8  | p   |
| 152 | 0x98 | APP_eReg_RecMsg14_Register   | Messages Register number                                                                 | uint8  | p   |
| 153 | 0x99 | APP_eReg_RecMsg14_LEDFunc    | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 154 | 0x9A | APP_eReg_RecMsg14_AddInfo    | Bit -> LED                                                                               | uint8  | p   |
| 155 | 0x9B | APP_eReg_RecMsg15_RemoteAddr | Sender's address                                                                         | uint16 | p   |
| 156 | 0x9C | APP_eReg_RecMsg15_Command    | Message command                                                                          | uint8  | p   |
| 157 | 0x9D | APP_eReg_RecMsg15_Register   | Messages Register number                                                                 | uint8  | p   |
| 158 | 0x9E | APP_eReg_RecMsg15_LEDFunc    | LED function 2: [0:3]: Bitposition | [4:7] anzusteuernde LED                             | uint8  | p   |
| 159 | 0x9F | APP_eReg_RecMsg15_AddInfo    | Bit -> LED                                                                               | uint8  | p   |
| 160 | 0xA0 | APP_eReg_0_LEDState          |   *[0..3] Intensity:    0x0=off ... 0xF=bright*                                          | uint8  | p   |
| 161 | 0xA1 | APP_eReg_1_LEDState          |   *[4..7] Blink freq.: 0x0=const., 0x10=slow ... 0xF0=fast*                             | uint8  | p   |
| 162 | 0xA2 | APP_eReg_2_LEDState          |   *Examples:*                                                                            | uint8  | p   |
| 163 | 0xA3 | APP_eReg_3_LEDState          |   *0x0F = LED on*                                                                        | uint8  | p   |
| 164 | 0xA4 | APP_eReg_4_LEDState          |   *0x?0 = LED off (higher nibble doesn't matter)*                                        | uint8  | p   |
| 165 | 0xA5 | APP_eReg_5_LEDState          |   *0xFF = LED very fast blinking*                                                        | uint8  | p   |
| 166 | 0xA6 | APP_eReg_6_LEDState          |   *0x88 = LED half inensity, blinking with medium freq.                                  | uint8  | p   |
| 167 | 0xA7 | APP_eReg_7_LEDState          |                                                                                          | uint8  | p   |
| 168 | 0xA8 |
| 169 | 0xA9 |
| 170 | 0xAA |
| 171 | 0xAB |
| 172 | 0xAC |
| 173 | 0xAD |
| 174 | 0xAE |
| 175 | 0xAF |
| 176 | 0xB0 |
| 177 | 0xB1 |
| 178 | 0xB2 |
| 179 | 0xB3 |
| 180 | 0xB4 |
| 181 | 0xB5 |
| 182 | 0xB6 |
| 183 | 0xB7 |
| 184 | 0xB8 |
| 185 | 0xB9 |
| 186 | 0xBA |
| 187 | 0xBB |
| 188 | 0xBC |
| 189 | 0xBD |
| 190 | 0xBE |
| 191 | 0xBF |
| 192 | 0xC0 |
| 193 | 0xC1 |
| 194 | 0xC2 |
| 195 | 0xC3 |
| 196 | 0xC4 |
| 197 | 0xC5 |
| 198 | 0xC6 |
| 199 | 0xC7 |
| 200 | 0xC8 |
| 201 | 0xC9 |
| 202 | 0xCA |
| 203 | 0xCB |
| 204 | 0xCC |
| 205 | 0xCD |
| 206 | 0xCE |
| 207 | 0xCF |
| 208 | 0xD0 |
| 209 | 0xD1 |
| 210 | 0xD2 |
| 211 | 0xD3 |
| 212 | 0xD4 |
| 213 | 0xD5 |
| 214 | 0xD6 |
| 215 | 0xD7 |
| 216 | 0xD8 |
| 217 | 0xD9 |
| 218 | 0xDA |
| 219 | 0xDB |
| 220 | 0xDC |
| 221 | 0xDD |
| 222 | 0xDE |
| 223 | 0xDF |
| 224 | 0xE0 |
| 225 | 0xE1 |
| 226 | 0xE2 |
| 227 | 0xE3 |
| 228 | 0xE4 |
| 229 | 0xE5 |
| 230 | 0xE6 | DeviceSignature0             | 1st byte of device signature                                                              | uint8  | pro |
| 231 | 0xE7 | DeviceSignature1             | 2nd byte of device signature                                                              | uint8  | pro |
| 232 | 0xE8 | DeviceSignature2             | 3rd byte of device signature                                                              | uint8  | pro |
| 233 | 0xE9 | DeviceSignature3             | 4th byte of device signature                                                              | uint8  | pro |
| 234 | 0xEA | BoardID                      | Needed Board ID                                                                           | uint16 | pro |
| 235 | 0xEB | BoardRev                     | Needed Board revision (at least)                                                          | uint8  | pro |
| 236 | 0xEC | AppID                        | Application ID                                                                            | uint16 | pro |
| 237 | 0xED | AppVersionMajor              | Application version major                                                                 | uint8  | pro |
| 238 | 0xEE | AppVersionMinor              | Application version minor                                                                 | uint8  | pro |
| 239 | 0xEF | AppVersionBugfix             | Application version bug-fix                                                               | uint8  | pro |
| 240 | 0xF0 | AppVersionHash               | Application versions git hash                                                             | uint32 | pro |
| 241 | 0xF1 |
| 242 | 0xF2 |
| 243 | 0xF3 |
| 244 | 0xF4 |
| 245 | 0xF5 |
| 246 | 0xF6 |
| 247 | 0xF7 |
| 248 | 0xF8 |
| 249 | 0xF9 |
| 250 | 0xFA |
| 251 | 0xFB |
| 252 | 0xFC |
| 253 | 0xFD |
| 254 | 0xFE |
| 255 | 0xFF |
