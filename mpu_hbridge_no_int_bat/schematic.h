/*
   Pinouts:

   Arduino UNO (with energyShield):


                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |                                     |
         |                           A5/SCL[ ] |   C5
         |                           A4/SDA[ ] |   C4
         |                             AREF[ ] |
         |                              GND[ ] |
         |                           SCK/13[ ] |   B5
         | [ ]IOREF                 MISO/12[ ] |   .
         | [ ]RST                   MOSI/11[ ]~|   .
         | [ ]3V3    +---+               10[ ]~|   .
         | [ ]5v    -| A |-               9[ ]~|   .
         | [ ]GND   -| R |-               8[ ] |   B0
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |   D7
         |          -| I |-               6[ ]~|   .
         | [ ]A0    -| N |-               5[ ]~|   .
         | [ ]A1    -| O |-               4[ ] |   .
         | [ ]A2     +---+           INT1/3[ ]~|   .
         | [ ]A3                     INT0/2[ ] |   .
         | [ ]A4/SDA                   TX>1[ ] |   .
         | [ ]A5/SCL                   RX<0[ ] |   D0
         |                                     |
         |  UNO_R3                 ____________/
          \_______________________/

      http://busyducks.com/ascii-art-arduinos

   L293D H-driver

   A side                B side
          +----   ----+
   1,2EN [| 1  (_) 16 |] Vcc1
      1A [| 2      15 |] 4A   
      1Y [| 3      14 |] 4Y
     GND [| 4      13 |] GND
     GND [| 5      12 |] GND
      2Y [| 6      11 |] 3Y
      2A [| 7      10 |] 3A
    Vcc2 [| 8       9 |] 3,4EN
          +-----------+

    My driver board:

     (-------------)
    (               )
     ----+    +-----
         |    |
      L[]|    |[]R
         |    |
         +----+
            
            /|\
             |
             |
       front of robot
    +----------------------------------+
    |    jp0          [-][+] jp1       |
    |                  7-12v           |
    | led0                        led2 |
    | led1                        led3 |
    |                                  |
    | gndPin                           |
    |                                  |
    | 5vPin                            |
    |                            p4    |
    | p1                         p5    |
    | p2   m0            m1      p6    |
    | p3 [cA][cB]      [cC][cD]  p7    |
    +----------------------------------+
           Arduino
             |
             |
            \|/
  I1  E1  I2
  3   4   5  right motor
  I3  E2  I4
  6   7   8  left motor
   +-------+-------+-------+--------+---------+
   | Board | l293D | l293D | Wire   | Arduino |
   | Pin   | Pin   | Funct | Color  | Pin     |
   +-------+-------+-------+--------+---------+
   |  p1   |   9   |  2E   | green  |    7    |
   |  p2   |  10   |  3A   | orange |    6    |
   |  p3   |  15   |  4A   | yellow |    8    |
   |  p4   |  --   |  --   | purple |   A3    |
   |  p5   |   2   |  1A   | white  |    3    |
   |  p6   |   1   |  1E   | brown  |    4    |
   |  p7   |   7   |  2A   | blue   |    5    |
   +-------+-------+-------+--------+---------+
   |  cA   |  14   |  4Y   | black  |   --    |
   |  cB   |  11   |  3Y   | white  |   --    |
   |  cC   |   3   |  1Y   | red    |   --    |
   |  cD   |   6   |  2Y   | green  |   --    |
   +-------+-------+-------+--------+---------+
   Right:
   E1 (EN1,2) = brown
   I1 (1A)    = white
   I2 (2A)    = blue
   Y1 (1Y)    = red (22g)
   Y2 (2Y)    = green (22g)
   Left:
   E2 (EN3,4) = green
   I3 (3A)    = orange
   I4 (4A)    = yellow
   Y3 (3Y)    = white (22g)
   Y4 (4Y)    = black (22g)

   internal / external battery LED board: () = screw hole
   +----------------------------+
   | ()    YT    0              | 0 vcc
   |             1              | 1 yellow top    - ard pin 9
   |       YB    2       RT     | 2 yellow bottom - ard pin 10
   |             3       RB     | 3 red top       - ard pin 11
   | ()          4              | 4 red bottom    - ard pin 12
   +----------------------------+

*/

