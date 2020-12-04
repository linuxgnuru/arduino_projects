#if 0
    /*
      num = getNum(x, y);
      printGrid(num);
    */
    // move up
    if ((jx >= 500 && jx <= 1023) || (jx <= 500 && jx >= 0) || (jy >= 500 && jy <= 1023) || (jy <= 500 && jy >= 0))
    {
      // x up, y stays same
      if ((jx >= 500 && jx <= 1023) && (jy <= 500 && jy >= 500))
      {
        if (++x > 5)
          x = 0;
      }
      // x down, y stays same
      if ((jx <= 500 && jx >= 0) && (jy <= 500 && jy >= 500))
      {
        if (--x < 0)
          x = 5;
      }

      // y left/right, x stays same
      if ((jy >= 500 && jy <= 1023) && (jx <= 500 && jx >= 500))
      {
        if (++y > 2)
          y = 0;
      }
      // y right/left, x stays same
      if ((jy <= 500 && jy >= 0) && (jx <= 500 && jx >= 500))
      {
        if (--y < 0)
          y = 2;
      }
      // x up, y left/right
      if ((jx >= 500 && jx <= 1023) && (jy >= 500 && jy <= 1023))
      {
        if (++x > 5)
          x = 0;
        if (++y > 2)
          y = 0;
      }
      // x down, y right/left
      if ((jx <= 500 && jx >= 0) && (jy <= 500 && jy >= 0))
      {
        if (--x < 0)
          x = 5;
        if (--y < 0)
          y = 2;
      }
    }
    num_j = getNum(x, y);
    //debugPrintAll();
    printGrid(num_j);
    if (curMillis - bMillis >= 500)
    {
      bMillis = curMillis;
      //debugPrintAll();
    }
    // no movement
    if ((jx >= 510 || jx <= 450) || (jy >= 510 || jy <= 450))
    {
      if (jx >= 510 || jx <= 450)
      {
        if (lastX != jx)
        {
          //x = map(jx, 0, 1023, 0, 5);
          if (jx >= 510 && ++x > 6)
            x = 0;
          if (jx <= 450 && --x <= -1)
            x = 5;
        }
      }
      if (jy >= 510 || jy <= 450)
      {
        //y = map(jy, 0, 1023, 0, 2);
        if (jy >= 510 && ++y > 2)
          y = 0;
        if (jy <= 450 && --y <= -1)
          y = 2;
      }
      num_j = getNum(x, y);
      //debugPrintAll();
      printGrid(num);
    }
#endif

