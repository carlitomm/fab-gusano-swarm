#ifndef _HTML_H_
#define _HTML_H_

String HTMLheader(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name =\"veiwport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>MEAT Control</title>\n";

  ptr += "<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #9c4444; margin: 50px auto 30px; } h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #b43333;margin: 50px;} h2 {color: #444444; padding: 30px 0px; margin: 50px; background-color: #5c8297c0;}\n";
  ptr += ".button {display: block; width: 80px; background-color: #3498db; border: none; color: white; padding: 13px 30px; text-decoration: none; font-size: 35px; margin: 0px auto 30px; cursor: pointer; border-radius: 4px}\n";

  ptr += ".button-on {background-color: #65e70f; width: 100px; height: 45px; font-size: 45px;}\n";
  ptr += ".button-on:active {background-color: #65e70f; width: 100px;}\n";
  ptr += ".button-off {background-color: #ee0808; width: 120px; height: 60px; font-size: 45px;}\n";
  ptr += ".button-off:active {background-color: #ee0808; width: 100px;}\n";

  ptr += ".button-spd {background-color: #3498db; margin: auto}\n";
  ptr += ".button-mov {background-color: #3498db; width: 170px; text-align: center;}\n";

  ptr += "p {font-size: 35px;}";
  ptr += "</style>\n";

  ptr += "</head>\n";
  return ptr;
}

String SendHTML(uint8_t gusano_state, int movment, int Tdelay) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name =\"veiwport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>MEAT Control</title>\n";

  ptr += "<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #9c4444; margin: 50px auto 30px; } h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #b43333;margin: 50px;} h2 {color: #444444; padding: 30px 0px; margin: 50px; background-color: #5c8297c0;}\n";
  ptr += ".button {display: block; width: 80px; background-color: #3498db; border: none; color: white; padding: 13px 30px; text-decoration: none; font-size: 35px; margin: 0px auto 30px; cursor: pointer; border-radius: 4px}\n";

  ptr += ".button-on {background-color: #65e70f; width: 100px; height: 45px; font-size: 45px;}\n";
  ptr += ".button-on:active {background-color: #65e70f; width: 100px;}\n";
  ptr += ".button-off {background-color: #ee0808; width: 120px; height: 60px; font-size: 45px;}\n";
  ptr += ".button-off:active {background-color: #ee0808; width: 100px;}\n";

  ptr += ".button-spd {background-color: #3498db; margin: auto}\n";
  ptr += ".button-mov {background-color: #3498db; width: 170px; text-align: center;}\n";

  ptr += "p {font-size: 35px;}";
  ptr += "</style>\n";

  ptr += "</head>\n";

  ptr += "<body>\n";
  ptr += "<h1>MEAT WEB SERVER</h1>\n";

  if (gusano_state) {
    ptr += "<p>Status:Turn ON</p> <a class=\"button button-off\" href = \"/On-Off\"> OFF </a> \n";
  } else {
    ptr += "<p>Status:Turn OFF</p><a class=\"button button-on\" href = \"/On-Off\"> ON </a>\n";
    if (Tdelay == 15)
    {
      ptr += "<h2>Speed: NORMAL</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\"> --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\" > +++ </a> \n";
      ptr += "</div>\n";
    }
    if (Tdelay == 10)
    {
      ptr += "<h2>Speed: FAST</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\"> --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\"> +++ </a> \n";
      ptr += "</div>\n";
    }
    if (Tdelay == 20)
    {
      ptr += "<h2>Speed: SLOW</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\"> --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\" > +++ </a> \n";
      ptr += "</div>\n";
    }
    if (Tdelay == 25)
    {
      ptr += "<h2>Speed: SUPER SLOW</h2>\n";
      ptr += "<div style=\"display:flex\">\n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_less\" > --- </a> \n";
      ptr += "<a class=\"button button-spd\" href = \"/speed_mod_more\" > +++ </a> \n";
      ptr += "</div>\n";
    }

    if (movment == 1)
    {
      ptr += "<h2>Movement Type: COMBINED</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";

      /*if (first == 5) {
        ptr += "<h2>Movement Range: SHORT</h2>";
        }
        if (first == 10) {
        ptr += "<h2>Movement Range: NORMAL</h2>";
        }
        if (first == 15) {
        ptr += "<h2>Movement Range: LARGE</h2>";
        }
        if (first == 20) {
        ptr += "<h2>Movement Range: LARGER</h2>";
        }
        ptr += "<div style=\"display:flex\">\n";
        ptr += "<a class=\"button button-spd\" href = \"/range_mod_less\" > --- </a> \n";
        ptr += "<a class=\"button button-spd\" href = \"/range_mod_more\" > +++ </a> \n";
        ptr += "</div>\n";*/
    }
    if (movment == 2)
    {
      ptr += "<h2>Movement Type: LATERAL RIGHT Rotating</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (movment == 3)
    {
      ptr += "<h2>Movement Type: LINEAR BACK</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      // ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      // ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (movment == 4)
    {
      ptr += "<h2>Movement Type: LATERAL lEFT</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LINEAR FRONT</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      //ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (movment == 5)
    {
      ptr += "<h2>Movement Type: LINEAR FRONT</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      // ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
      // ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (movment == 6)
    {
      ptr += "<h2>Movement Type: 6 ROTATION</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_7\" >7 New Lateral</a> \n";
    }
    if (movment == 7)
    {
      ptr += "<h2>Movement Type: 7 Lateral Left Rotating</h2>";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_1\" >COMBINED</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_2\" >LATERAL RIGHT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_3\" >LINEAR BACK</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_4\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_5\" >LATERAL LEFT</a> \n";
      ptr += "<a class=\"button button-mov\" href = \"/change_mov_6\" >6 ROTATION</a> \n";
    }
  }

  ptr += "<a class=\"button button-mov\" href = \"/swarm_behavior\" >swarm_behavior</a> \n";
  ptr += "<a class=\"button button-mov\" href = \"/swarm_role\" >swarm_role</a> \n";
  ptr += " </body>\n";
  ptr += " </html>\n";
  return ptr;
}

String SendSwarmBehavior(){
  String ptr = HTMLheader();
  ptr += "<a class=\"button button-mov\" href = \"/coleography\" >Coleography</a> \n";
  ptr += "<a class=\"button button-mov\" href = \"/following\" >Following</a> \n";
  ptr += "<a class=\"button button-mov\" href = \"/mixed\" >Mixed</a> \n";
  return ptr;
}

String SendSwarmRole(){
  String ptr = HTMLheader();
  ptr += "<a class=\"button button-mov\" href = \"/leader\" >Leader</a> \n";
  ptr += "<a class=\"button button-mov\" href = \"/follower\" >Follower</a> \n";
  return ptr;
}

#endif