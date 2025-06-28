# RegisterOverview of BlindControl5

last updated: 2025-06-28
<br>

| dec | hex  | Name                   | Description                                                                              | Size   | pv |
|-----|------|------------------------|------------------------------------------------------------------------------------------|--------|----|
| 000 | 0x00 | unused                 | n/a                                                                                      |        |    |
| 001 | 0x01 | ModuleID               | Module ID as it appears on the bus                                                       | uint16 | p  |
| 002 | 0x02 | BldFlag                | Storage for bootloader flags                                                             | uint8  | p  |
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
| 016 | 0x10 | PositionCurrent_B0     | Current position of blind 0. 100 = completely closed. 0 = completely open                | uint8  | vro |
| 017 | 0x11 | PositionSetPoint_B0    | Wanted position of blind. 100 = completely closed. 0 = completely open                   | uint8  | v   |
| 018 | 0x12 | PositionDifference_B0  | Difference between wanted and current position                                           | int8   | vro |
| 019 | 0x13 | ReactionDelay_B0       | Delay time in timer ticks (1/100sec) until movement starts after applying power          | uint8  | p   |
| 020 | 0x14 | DurationOpen_B0        | Duration in timer ticks (1/100sec) to fully open the blind                               | uint16 | p   |
| 021 | 0x15 | DurationClose_B0       | Duration in timer ticks (1/100sec) to fully close the blind                              | uint16 | p   |
| 022 | 0x16 | Mode_B0                |                                                                                          | uint8  | p   |
| 023 | 0x17 | unused                 |                                                                                          |        |     |
| 024 | 0x18 | unused                 |                                                                                          |        |     |
| 025 | 0x19 | unused                 |                                                                                          |        |     |
| 026 | 0x1A | PositionCurrent_B1     | Current position of blind 0. 100 = completely closed. 0 = completely open                | uint8  | vro |
| 027 | 0x1B | PositionSetPoint_B1    | Wanted position of blind. 100 = completely closed. 0 = completely open                   | uint8  | v   |
| 028 | 0x1C | PositionDifference_B1  | Difference between wanted and current position                                           | int8   | vro |
| 029 | 0x1D | ReactionDelay_B1       | Delay time in timer ticks (1/100sec) until movement starts after applying power          | uint8  | p   |
| 030 | 0x1E | DurationOpen_B1        | Duration in timer ticks (1/100sec) to fully open the blind                               | uint16 | p   |
| 031 | 0x1F | DurationClose_B1       | Duration in timer ticks (1/100sec) to fully close the blind                              | uint16 | p   |
| 032 | 0x20 | Mode_B1                |                                                                                          | uint8  | p   |
| 033 | 0x21 | unused                 |                                                                                          |        |     |
| 034 | 0x22 | unused                 |                                                                                          |        |     |
| 035 | 0x23 | unused                 |                                                                                          |        |     |
| 036 | 0x24 | PositionCurrent_B2     | Current position of blind 0. 100 = completely closed. 0 = completely open                | uint8  | vro |
| 037 | 0x25 | PositionSetPoint_B2    | Wanted position of blind. 100 = completely closed. 0 = completely open                   | uint8  | v   |
| 038 | 0x26 | PositionDifference_B2  | Difference between wanted and current position                                           | int8   | vro |
| 039 | 0x27 | ReactionDelay_B2       | Delay time in timer ticks (1/100sec) until movement starts after applying power          | uint8  | p   |
| 040 | 0x28 | DurationOpen_B2        | Duration in timer ticks (1/100sec) to fully open the blind                               | uint16 | p   |
| 041 | 0x29 | DurationClose_B2       | Duration in timer ticks (1/100sec) to fully close the blind                              | uint16 | p   |
| 042 | 0x2A | Mode_B2                |                                                                                          | uint8  | p   |
| 043 | 0x2B | unused                 |                                                                                          |        |     |
| 044 | 0x2C | unused                 |                                                                                          |        |     |
| 045 | 0x2D | unused                 |                                                                                          |        |     |
| 046 | 0x2E | PositionCurrent_B3     | Current position of blind 0. 100 = completely closed. 0 = completely open                | uint8  | vro |
| 047 | 0x2F | PositionSetPoint_B3    | Wanted position of blind. 100 = completely closed. 0 = completely open                   | uint8  | v   |
| 048 | 0x30 | PositionDifference_B3  | Difference between wanted and current position                                           | int8   | vro |
| 049 | 0x31 | ReactionDelay_B3       | Delay time in timer ticks (1/100sec) until movement starts after applying power          | uint8  | p   |
| 050 | 0x32 | DurationOpen_B3        | Duration in timer ticks (1/100sec) to fully open the blind                               | uint16 | p   |
| 051 | 0x33 | DurationClose_B3       | Duration in timer ticks (1/100sec) to fully close the blind                              | uint16 | p   |
| 052 | 0x34 | Mode_B3                |                                                                                          | uint8  | p   |
| 053 | 0x35 | unused                 |                                                                                          |        |     |
| 054 | 0x36 | unused                 |                                                                                          |        |     |
| 055 | 0x37 | unused                 |                                                                                          |        |     |
| 056 | 0x38 | PositionCurrent_B4     | Current position of blind 0. 100 = completely closed. 0 = completely open                | uint8  | vro |
| 057 | 0x39 | PositionSetPoint_B4    | Wanted position of blind. 100 = completely closed. 0 = completely open                   | uint8  | v   |
| 058 | 0x3A | PositionDifference_B4  | Difference between wanted and current position                                           | int8   | vro |
| 059 | 0x3B | ReactionDelay_B4       | Delay time in timer ticks (1/100sec) until movement starts after applying power          | uint8  | p   |
| 060 | 0x3C | DurationOpen_B4        | Duration in timer ticks (1/100sec) to fully open the blind                               | uint16 | p   |
| 061 | 0x3D | DurationClose_B4       | Duration in timer ticks (1/100sec) to fully close the blind                              | uint16 | p   |
| 062 | 0x3E | Mode_B4                |                                                                                          | uint8  | p   |
| 063 | 0x3F | unused                 |                                                                                          |        |     |
| 064 | 0x40 | unused                 |                                                                                          |        |     |
| 065 | 0x41 | unused                 |                                                                                          |        |     |
| 066 | 0x42 | TimeUp1_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 067 | 0x43 | TimeUp1_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 068 | 0x44 | TimeUp1_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 069 | 0x45 | unused                 |                                                                                          |        |     |
| 070 | 0x46 | unused                 |                                                                                          |        |     |
| 071 | 0x47 | TimeDown1_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 072 | 0x48 | TimeDown1_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 073 | 0x49 | TimeDown1_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 074 | 0x4A | unused                 |                                                                                          |        |     |
| 075 | 0x4B | unused                 |                                                                                          |        |     |
| 076 | 0x4C | TimeUp2_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 077 | 0x4D | TimeUp2_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 078 | 0x4E | TimeUp2_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 079 | 0x4F | unused                 |                                                                                          |        |     |
| 080 | 0x50 | unused                 |                                                                                          |        |     |
| 081 | 0x51 | TimeDown2_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 082 | 0x52 | TimeDown2_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 083 | 0x53 | TimeDown2_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 084 | 0x54 | unused                 |                                                                                          |        |     |
| 085 | 0x55 | unused                 |                                                                                          |        |     |
| 086 | 0x56 | TimeUp1_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 087 | 0x57 | TimeUp1_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 088 | 0x58 | TimeUp1_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 089 | 0x59 | unused                 |                                                                                          |        |     |
| 090 | 0x5A | unused                 |                                                                                          |        |     |
| 091 | 0x5B | TimeDown1_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 092 | 0x5C | TimeDown1_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 093 | 0x5D | TimeDown1_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 094 | 0x5E | unused                 |                                                                                          |        |     |
| 095 | 0x5F | unused                 |                                                                                          |        |     |
| 096 | 0x60 | TimeUp2_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 097 | 0x61 | TimeUp2_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 098 | 0x62 | TimeUp2_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 099 | 0x63 | unused                 |                                                                                          |        |     |
| 100 | 0x64 | unused                 |                                                                                          |        |     |
| 101 | 0x65 | TimeDown2_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 102 | 0x66 | TimeDown2_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 103 | 0x67 | TimeDown2_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 104 | 0x68 | unused                 |                                                                                          |        |     |
| 105 | 0x69 | unused                 |                                                                                          |        |     |
| 106 | 0x6A | TimeUp1_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 107 | 0x6B | TimeUp1_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 108 | 0x6C | TimeUp1_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 109 | 0x6D | unused                 |                                                                                          |        |     |
| 110 | 0x6E | unused                 |                                                                                          |        |     |
| 111 | 0x6F | TimeDown1_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 112 | 0x70 | TimeDown1_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 113 | 0x71 | TimeDown1_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 114 | 0x72 | unused                 |                                                                                          |        |     |
| 115 | 0x73 | unused                 |                                                                                          |        |     |
| 116 | 0x74 | TimeUp2_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 117 | 0x75 | TimeUp2_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 118 | 0x76 | TimeUp2_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 119 | 0x77 | unused                 |                                                                                          |        |     |
| 120 | 0x78 | unused                 |                                                                                          |        |     |
| 121 | 0x79 | TimeDown2_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 122 | 0x7A | TimeDown2_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 123 | 0x7B | TimeDown2_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 124 | 0x7C | unused                 |                                                                                          |        |     |
| 125 | 0x7D | unused                 |                                                                                          |        |     |
| 126 | 0x7E | TimeUp1_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 127 | 0x7F | TimeUp1_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 128 | 0x80 | TimeUp1_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 129 | 0x81 | unused                 |                                                                                          |        |     |
| 130 | 0x82 | unused                 |                                                                                          |        |     |
| 131 | 0x83 | TimeDown1_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 132 | 0x84 | TimeDown1_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 133 | 0x85 | TimeDown1_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 134 | 0x86 | unused                 |                                                                                          |        |     |
| 135 | 0x87 | unused                 |                                                                                          |        |     |
| 136 | 0x88 | TimeUp2_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 137 | 0x89 | TimeUp2_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 138 | 0x8A | TimeUp2_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 139 | 0x8B | unused                 |                                                                                          |        |     |
| 140 | 0x8C | unused                 |                                                                                          |        |     |
| 141 | 0x8D | TimeDown2_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 142 | 0x8E | TimeDown2_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 143 | 0x8F | TimeDown2_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 144 | 0x90 | unused                 |                                                                                          |        |     |
| 145 | 0x91 | unused                 |                                                                                          |        |     |
| 146 | 0x92 | TimeUp1_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 147 | 0x93 | TimeUp1_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 148 | 0x94 | TimeUp1_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 149 | 0x95 | unused                 |                                                                                          |        |     |
| 140 | 0x96 | unused                 |                                                                                          |        |     |
| 141 | 0x97 | TimeDown1_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 142 | 0x98 | TimeDown1_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 143 | 0x99 | TimeDown1_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 144 | 0x9A | unused                 |                                                                                          |        |     |
| 145 | 0x9B | unused                 |                                                                                          |        |     |
| 146 | 0x9C | TimeUp2_Weekday        | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 147 | 0x9D | TimeUp2_Hour           | Hour when this timer applies.                                                            | uint8  | p   |
| 148 | 0x9E | TimeUp2_Minute         | Minute when this timer applies.                                                          | uint8  | p   |
| 149 | 0x9F | unused                 |                                                                                          |        |     |
| 160 | 0xA0 | unused                 |                                                                                          |        |     |
| 161 | 0xA1 | TimeDown2_Weekday      | Flags on which weekday this timer applies. Index flags with day-of-week                  | uint8  | p   |
| 162 | 0xA2 | TimeDown2_Hour         | Hour when this timer applies.                                                            | uint8  | p   |
| 163 | 0xA3 | TimeDown2_Minute       | Minute when this timer applies.                                                          | uint8  | p   |
| 164 | 0xA4 | unused                 |                                                                                          |        |     |
| 165 | 0xA5 | unused                 |                                                                                          |        |     |
| 166 | 0xA6 | Chn0SwitchCurrent      | Output current value 0...255                                                             | uint8  | vro |
| 167 | 0xA7 | Chn0SwitchSetPoint     | Output target value 0...255                                                              | uint8  | v   |
| 168 | 0xA8 | Chn0ThresholdOff       | If value goes below, channel switches off                                                | uint8  | p   |
| 169 | 0xA9 | Chn0ThresholdOn        | If value goes above, channel switches on                                                 | uint8  | p   |
| 170 | 0xAA | Chn0Mode               |                                                                                          | uint8  | p   |
| 171 | 0xAB | Chn1SwitchCurrent      | Output current value 0...255                                                             | uint8  | vro |
| 172 | 0xAC | Chn1SwitchSetPoint     | Output target value 0...255                                                              | uint8  | v   |
| 173 | 0xAD | Chn1ThresholdOff       | If value goes below, channel switches off                                                | uint8  | p   |
| 174 | 0xAE | Chn1ThresholdOn        | If value goes above, channel switches on                                                 | uint8  | p   |
| 175 | 0xAF | Chn1Mode               |                                                                                          | uint8  | p   |
| 176 | 0xB0 | Chn2SwitchCurrent      | Output current value 0...255                                                             | uint8  | vro |
| 177 | 0xB1 | Chn2SwitchSetPoint     | Output target value 0...255                                                              | uint8  | v   |
| 178 | 0xB2 | Chn2ThresholdOff       | If value goes below, channel switches off                                                | uint8  | p   |
| 179 | 0xB3 | Chn2ThresholdOn        | If value goes above, channel switches on                                                 | uint8  | p   |
| 180 | 0xB4 | Chn2Mode               |                                                                                          | uint8  | p   |
| 181 | 0xB5 | Chn3SwitchCurrent      | Output current value 0...255                                                             | uint8  | vro |
| 182 | 0xB6 | Chn3SwitchSetPoint     | Output target value 0...255                                                              | uint8  | v   |
| 183 | 0xB7 | Chn3ThresholdOff       | If value goes below, channel switches off                                                | uint8  | p   |
| 184 | 0xB8 | Chn3ThresholdOn        | If value goes above, channel switches on                                                 | uint8  | p   |
| 185 | 0xB9 | Chn3Mode               |                                                                                          | uint8  | p   |
| 186 | 0xBA | Chn4SwitchCurrent      | Output current value 0...255                                                             | uint8  | vro |
| 187 | 0xBB | Chn4SwitchSetPoint     | Output target value 0...255                                                              | uint8  | v   |
| 188 | 0xBC | Chn4ThresholdOff       | If value goes below, channel switches off                                                | uint8  | p   |
| 189 | 0xBD | Chn4ThresholdOn        | If value goes above, channel switches on                                                 | uint8  | p   |
| 190 | 0xBE | Chn4Mode               |                                                                                          | uint8  | p   |
| 191 | 0xBF | Chn5SwitchCurrent      | Output current value 0...255                                                             | uint8  | vro |
| 192 | 0xC0 | Chn5SwitchSetPoint     | Output target value 0...255                                                              | uint8  | v   |
| 193 | 0xC1 | Chn5ThresholdOff       | If value goes below, channel switches off                                                | uint8  | p   |
| 194 | 0xC2 | Chn5ThresholdOn        | If value goes above, channel switches on                                                 | uint8  | p   |
| 195 | 0xC3 | Chn5Mode               |                                                                                          | uint8  | p   |
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
| 223 | 0xDF | Year                   | Current date and time.                                                                    | uint16 | v   |
| 224 | 0xE0 | Month                  | 1=January ... 12=December                                                                 | uint8  | v   |
| 225 | 0xE1 | Day                    | 1...31                                                                                    | uint8  | v   |
| 226 | 0xE2 | DayOfWeek              | DayOfWeek: 0=Monday, 6=Sunday                                                             | uint8  | v   |
| 227 | 0xE3 | Hour                   | 0...23                                                                                    | uint8  | v   |
| 228 | 0xE4 | Minute                 | 0-59                                                                                      | uint8  | v   |
| 229 | 0xE5 | Second                 | 0-59                                                                                      | uint8  | v   |
| 230 | 0xE6 | DeviceSignature0       | 1st byte of device signature                                                              | uint8  | pro |
| 231 | 0xE7 | DeviceSignature1       | 2nd byte of device signature                                                              | uint8  | pro |
| 232 | 0xE8 | DeviceSignature2       | 3rd byte of device signature                                                              | uint8  | pro |
| 233 | 0xE9 | DeviceSignature3       | 4th byte of device signature                                                              | uint8  | pro |
| 234 | 0xEA | BoardID                | Needed Board ID                                                                           | uint16 | pro |
| 235 | 0xEB | BoardRev               | Needed Board revision (at least)                                                          | uint8  | pro |
| 236 | 0xEC | AppID                  | Application ID                                                                            | uint16 | pro |
| 237 | 0xED | AppVersionMajor        | Application version major                                                                 | uint8  | pro |
| 238 | 0xEE | AppVersionMinor        | Application version minor                                                                 | uint8  | pro |
| 239 | 0xEF | AppVersionBugfix       | Application version bug-fix                                                               | uint8  | pro |
| 240 | 0xF0 | AppVersionHash         | Application versions git hash                                                             | uint32 | pro |
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
