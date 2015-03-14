//Last updated 3/7/15


int Ping; ////////put pingpin here

boolean checkSide()
{ //returns true if rightside is greater, left is false
  specDistTurn(90, 70, true);
  int rightDist = getPingDist(Ping);
  specDistTurn(180, 70, false);
  int leftDist = getPingDist(Ping);
  //goes back to original side 
  specDistTurn(90, 70, true);
  if(rightDist>leftDist)
    return true;
  if(leftDist>rightDist)
    return false;
}

void obstAvoidP1(int cm) 
{
  //Uses an asssumed value to go around obstacle
  //Assuming radius is 10 cm
 float radius = 10;  
 float bufferZone = 5; //space in between robot and obstacle -- change as needed
 float tpwr = 70; //taget power level (0-127)
 float inDia =  radius + bufferZone; //the inner diamter of the "ratio of circles"
 float outDia = inDia = WB;
 float pwrRatio = inDia/outDia;
 
 setDist(10);
 boolean side = checkSide();
 //does initial turn
 if(side)
   specDistTurn(60, 70, true);
  if(!side)
    specDistTurn(60, 70, false);
    
   //goes foward to avoid seeing the line it's on
  specDist(5, 40);
 
 //while(lightvals[0] > blackval)
 //{ //getlightvals();
 if(side)
   setbothSpeeds(tpwr * pwrRatio, twpr);
  else if(!side)
    setbothSpeeds(tpwr, tpwr *  pwrRatio
 //}
}


