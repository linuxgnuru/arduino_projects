int maximum = 0; //declare and initialize maximum as zero
int minimum = 1023; //declare and initialize minimum as 1023
int track = 0; //variable to keep track

void setup()
{
  Serial.begin(9600); //set baud rate
}

void loop()
{
  //record the highest value recieved on A5
  if (analogRead(5) > maximum)
    maximum = analogRead(A5);
  //record the lowest value recieved on A5
  if (analogRead(5) < minimum)
    minimum = analogRead(A5);

  //increase track by 1 after every iteration
  track++;

  /*
  display both the maximum and minimum value after 5 second
  track is used to determine the time interval it takes for
  the program to display the maximum and minimum values
  e.g. here i use 1000 so as to display the min and max values
  after every 5 second
  */
  if (track == 5000)
  {
    Serial.print("Maximum:\t");
    Serial.println(maximum);
    Serial.print("Minimum:\t");
    Serial.println(minimum);
    track = 0; //set back track to zero
  }
}

