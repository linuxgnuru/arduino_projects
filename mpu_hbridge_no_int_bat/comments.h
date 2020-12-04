  /*
    tmp_dir = dir;
    if (tmp_dir == BACKWARD)
    tmp_dir = LEFT;
    if (dir == LEFT || dir == RIGHT || dir == BACKWARD)
    {
    while (dir != FORWARD)
    {
      dir = getData();
      moveRobot((tmp_dir == LEFT ? FORWARD_LEFT : FORWARD_RIGHT), TEST_DELAY);
    }
    }
    else
    moveRobot(dir, (dir == FORWARD ? TEST_DELAY * 8 : TEST_DELAY));
  */
  /*
    #ifdef TEST_DELAY
      delay(dl);
    #else
      if (currentMillis - lastMillisBackupR >= BACKUP_DELAY)
      {
        lastMillisBackupR = currentMillis;
        // while / wait / delay
      }
    #endif
  */

    /*
      lcd.setCursor(0, 0);
      lcd.print(ypr[0] * 180 / M_PI); // -00.00
      lcd.setCursor(7, 0);          // 0123456789012345
      lcd.print(ypr[1] * 180 / M_PI); // -00.00 -00.00
      lcd.setCursor(0, 1);
      lcd.print(ypr[2] * 180 / M_PI); // -00.00 -00.00
    */

