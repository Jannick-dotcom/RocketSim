void setup()
{
     size(500,500);
     noStroke();
     frameRate(60);
     loop();
}

float vehHeight = 0;
float vehSpeed = 0;
int val = 1;
void draw()
{
    vehHeight += vehSpeed / 20.0;
    clear();
    rect(250,height - vehHeight - 20,20,-100);
    triangle(250,height - vehHeight - 120,270,height - vehHeight-120,260,height - vehHeight - 140);
    triangle(260, height - vehHeight - 20 , 255, height - vehHeight , 265,height - vehHeight);
    vehSpeed += 1 * val;
    if(vehHeight > 500)
    {
      val = -1;
    }
    else if( vehHeight < 0)
    {
      val = 1;
    }
}
