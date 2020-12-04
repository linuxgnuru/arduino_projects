typedef enum {
  LED_L_4  =  3, // 3
  LED_L_3,       // 4
  LED_L_2,       // 5
  LED_L_1,       // 6
  LED_L_0,       // 7
  LED_R_0  = 11, // 11
  LED_R_1,       // 12
  LED_R_2,       // 13
  LED_R_3,       // 14
  LED_R_4,       // 15
  WARN_0   =  8, // top left
  WARN_1   =  0, // bottom left
  WARN_2   =  9, // center right
  ERR_0    =  2, // top right
  ERR_1    =  1  // bottom right
} my_enum;

typedef enum {
  LED_ON,       // turn specific led on
  LED_OFF,      // turn specific led off
  LED_TOGGLE,   // toggle led
  LED_LE_A_ON,  // left side bar graph
  LED_RI_A_ON,  // right side bar graph
  LED_LE_A_OFF, // left side bar graph
  LED_RI_A_OFF, // right side bar graph
  LED_A_ON,     // turn every led on
  LED_A_OFF     // turn every led off
} cmd_enum;

typedef enum {
  DATA,
  LATCH,
  CLOCK
} spi_enum;

typedef enum {
  BOARD_0,  // left
  BOARD_1,  // right
  BOARD_B,  // both
  BOARD_SPI // use SPI
} board_enum;

typedef enum {
  LEFT,
  RIGHT
} l_r_enum;

