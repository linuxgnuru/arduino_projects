#include <dataflash.h>

int buffer_counter = 0;
int page_counter = 0;

int lastpage = 0;
// last page written to
// total pages that will be used
int pages = 8;
Dataflash dflash;

void setup()
{
  Serial.begin(9600);
  // initialize the memory (pins are defined in dataflash.cpp
  dflash.init();
}

void loop()
{
  int j = 0;
  int i = 0;

  if (lastpage < pages)
  {
    for (buffer_counter = 0; buffer_counter < 528; buffer_counter++)
    {
      // write to buffer 1, 1 byte at a time
      dflash.Buffer_Write_Byte(1, buffer_counter, 33);
    }
    // write buffer 1 to the memory on page: lastpage
    dflash.Buffer_To_Page(1, lastpage);
    lastpage++;
    for (buffer_counter = 0; buffer_counter < 528; buffer_counter++)
    {
      // write to buffer 2, 1 byte at a time
      dflash.Buffer_Write_Byte(2, buffer_counter, 55);
    }
    // write buffer 2 to the memory on page: lastpage
    dflash.Buffer_To_Page(2, lastpage);
    lastpage++;
  }
  else
  {
    for (int i = 0; i <= pages; i++)
    {
      // copy page i to the buffer
      dflash.Page_To_Buffer(i, 1);
      // j depends on the amount of data on the page
      for (int j = 0; j < 528; j++)
      {
        // print the buffer data to the serial port
        Serial.println(dflash.Buffer_Read_Byte(1, j));
      }
    }
  }
}

