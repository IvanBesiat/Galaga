#include <Gamebuino-Meta.h>

int game_state = 0;

#pragma region declare variables
int score;
int lives;

int ennemies[15][4];
int ennemi_w;
int ennemi_h;
int ennemi_x_move;
bool all_dead;

//vaisseau du joueur

//corp du vaisseau
int joueur_ship_X_bl;
int joueur_ship_Y_bl;
int joueur_ship_X_br;
int joueur_ship_Y_br;
int joueur_ship_X_tm;
int joueur_ship_Y_tm;
//aile gauche du vaisseau
int left_wing_X_bl;
int left_wing_Y_bl;
int left_wing_X_tl;
int left_wing_Y_tl;
int left_wing_X_bm;
int left_wing_Y_bm;
//aile droite du vaisseau
int right_wing_X_bm;
int right_wing_Y_bm;
int right_wing_X_br;
int right_wing_Y_br;
int right_wing_X_tr;
int right_wing_Y_tr;
  
//munition joueur
int bullet_x_pos;
int bullet_y_pos;
int bullet_w;
int bullet_h;
int bullet_exist;
Color bullet_color;

//munition ennemie
int bullet_ennemi_x_pos;
int bullet_ennemi_y_pos;
int bullet_ennemi_w;
int bullet_ennemi_h;
int bullet_ennemi_exist;
Color bullet_ennemi_color;

int ennemi_alive;
int joueur_move_speed;

#pragma endregion

void setup() {
  gb.begin();
}
void loop() {
  while(!gb.update());
  gb.display.clear();

  switch(game_state)
  {
    case 0:
      showTitle();
      break;
    case 1:
      setStartValues();
      startgame();
      break;
    case 2:
      startgame();
      break;
    case 3:
      showEndScreen();
      break;
  }
}

void showTitle(){
  gb.display.setColor(WHITE);
  gb.display.cursorX = 0;
  gb.display.cursorY = 0;
  gb.display.print("  Last score : ");
  gb.display.print(score);

  gb.display.setColor(RED);
  gb.display.cursorX = 30;
  gb.display.cursorY = 28;
  gb.display.print("GALAGA");

  gb.display.setColor(WHITE);
  gb.display.cursorX = 0;
  gb.display.cursorY = 56;
  gb.display.print(" A:PLAY   B:QUIT");

  if(gb.buttons.pressed(BUTTON_A)){
    game_state = 1;
  }
  if(gb.buttons.pressed(BUTTON_B)){
    //kill the process
  }
}

void setStartValues(){
  score = 0;
  lives = 3;

  //vaisseaux ennemis
  int x = 17;
  int y = 5;
  int life = 1;
  int pts = 30;

  // ligne du haut
  for(int i = 0; i < 5; i++){
    ennemies[i][0] = x;
    ennemies[i][1] = y;
    ennemies[i][2] = life;
    ennemies[i][3] = pts;
    
    x = x + 10;
  }

  //ligne du milieu
  x = 17;
  y = 13;
  pts = 20;
  for(int i = 5; i < 10; i++){
    ennemies[i][0] = x;
    ennemies[i][1] = y;
    ennemies[i][2] = life;
    ennemies[i][3] = pts;

    x = x + 10;
  }

  //ligne du bas
  x = 17;
  y = 21;
  pts = 10;
  for(int i = 10; i < 15; i++){
    ennemies[i][0] = x;
    ennemies[i][1] = y;
    ennemies[i][2] = life;
    ennemies[i][3] = pts;

    x = x + 10;
  }
  
  ennemi_w = 6;
  ennemi_h = 4;
  ennemi_x_move = 1;
  all_dead = false;

  //vaisseau du joueur

  //corp du vaisseau
  joueur_ship_X_bl = gb.display.width()/2-1;
  joueur_ship_Y_bl = gb.display.height()-2;
  joueur_ship_X_br = joueur_ship_X_bl+4;
  joueur_ship_Y_br = joueur_ship_Y_bl;
  joueur_ship_X_tm = joueur_ship_X_bl+2;
  joueur_ship_Y_tm = joueur_ship_Y_bl-4;
  //aile gauche du vaisseau
  left_wing_X_bl = joueur_ship_X_bl-2;
  left_wing_Y_bl = joueur_ship_Y_bl;
  left_wing_X_tl = left_wing_X_bl;
  left_wing_Y_tl = joueur_ship_Y_tm;
  left_wing_X_bm = joueur_ship_X_tm;
  left_wing_Y_bm = left_wing_Y_bl;
  //aile droite du vaisseau
  right_wing_X_bm = left_wing_X_bm;
  right_wing_Y_bm = left_wing_Y_bm;
  right_wing_X_br = right_wing_X_bm+4;
  right_wing_Y_br = left_wing_Y_tl;
  right_wing_X_tr = right_wing_X_br;
  right_wing_Y_tr = left_wing_Y_bm;
    
  //munition joueur
  bullet_x_pos = 81;
  bullet_y_pos = 63;
  bullet_w = 1;
  bullet_h = 3;
  bullet_exist = 0;
  bullet_color=BLUE;

  //munition ennemie
  bullet_ennemi_x_pos = 81;
  bullet_ennemi_y_pos = 63;
  bullet_ennemi_w = 1;
  bullet_ennemi_h = 3;
  bullet_ennemi_exist = 0;
  bullet_ennemi_color=RED;

  ennemi_alive = 0;
  joueur_move_speed = 2;
}

void startgame(){

  game_state = 2;
  int ennemi_alive = 0;

  if(all_dead || lives == 0)
  {
      game_state = 3;
  }
  else
  {
    //score de la partie
    gb.display.setCursor(0,0);
    gb.display.print("Score: ");
    gb.display.print(score);
    gb.display.print("  live: ");
    gb.display.print(lives);

    createJoueur();
    
    //création ennemis
    for(int i=0; i < 15; i++){
      if(i < 5){gb.display.setColor(RED);}
      else if(i >= 5 && i < 10){gb.display.setColor(ORANGE);}
      else {gb.display.setColor(YELLOW);}
      
      //hit du projectil sur un mob
      if(gb.collideRectRect(bullet_x_pos, bullet_y_pos, bullet_w, bullet_h, ennemies[i][0], ennemies[i][1], ennemi_w, ennemi_h)
        && ennemies[i][2] > 0
        && bullet_exist == 1){
          ennemies[i][2]--;
          score = score + ennemies[i][3];
          bullet_exist = 0;
      }

      //hit du projectil ennemi sur le joueur
      if(gb.collideRectRect(bullet_ennemi_x_pos, bullet_ennemi_y_pos, bullet_ennemi_w, bullet_ennemi_h, left_wing_X_tl, left_wing_Y_tl, (right_wing_X_br - left_wing_X_bl), (left_wing_Y_bl - left_wing_X_tl))
        && lives > 0
        && bullet_ennemi_exist == 1){
          lives--;
          bullet_ennemi_exist = 0;
      }

      if(ennemies[i][2] > 0){
        all_dead = false;
        ennemi_alive++;
        gb.display.fillRect(ennemies[i][0], ennemies[i][1], ennemi_w, ennemi_h);
      }
      
      if(((ennemies[i][0] + ennemi_w) >= gb.display.width() && ennemies[i][2] > 0) 
        || (ennemies[i][0] <= 0 && ennemies[i][2] > 0)){
        ennemi_x_move = -ennemi_x_move;
        break;
      }
    }

      //selection de l'ennemi qui tir 
    if(bullet_ennemi_exist == 0)
    {
      int shooter_ennemi = random(ennemi_alive);
      for(int i = 0; i < 15; i++){
        if(ennemies[i][2] > 0 && shooter_ennemi == 0)
        {
          bullet_ennemi_x_pos = ennemies[i][0]+ennemi_w/2;
          bullet_ennemi_y_pos = ennemies[i][1]+ennemi_h;
          bullet_ennemi_exist = 1;
          break;
        }
        else if(ennemies[i][2] > 0 && shooter_ennemi != 0)
        {
          shooter_ennemi--;
        }
      }
    }

    all_dead = true;
    for(int i=0; i < 15; i++){
      if(ennemies[i][2] > 0){
        all_dead = false;
      }
      ennemies[i][0] = ennemies[i][0] + ennemi_x_move;
    }

    //Deplacement joueur
    if(gb.buttons.repeat(BUTTON_RIGHT, 0)){
      if((right_wing_X_br + joueur_move_speed) < gb.display.width()){
        joueur_ship_X_bl = joueur_ship_X_bl + joueur_move_speed;
      }
    }
    if(gb.buttons.repeat(BUTTON_LEFT, 0)){
      if(left_wing_X_bl > 0){
        joueur_ship_X_bl = joueur_ship_X_bl - joueur_move_speed;
      }
    }

    //mouvement de la munition joueur
    if(bullet_y_pos != gb.display.height() && bullet_y_pos != 0 && bullet_exist == 1){
      bullet_y_pos = bullet_y_pos - 2;
      gb.display.setColor(bullet_color);
      gb.display.fillRect(bullet_x_pos, bullet_y_pos-2, bullet_w,bullet_h);
    }
    
    if(bullet_y_pos <= 0){
      bullet_exist = 0;
    }

    //mouvement de la munition ennemi 
    if(bullet_ennemi_y_pos != gb.display.height() && bullet_ennemi_y_pos != 0 && bullet_ennemi_exist == 1){
      bullet_ennemi_y_pos = bullet_ennemi_y_pos + 2;
      gb.display.setColor(bullet_ennemi_color);
      gb.display.fillRect(bullet_ennemi_x_pos, bullet_ennemi_y_pos-2, bullet_ennemi_w,bullet_ennemi_h);
    }
    
    if(bullet_ennemi_y_pos >= gb.display.height()){
      bullet_ennemi_exist = 0;
    }

    //Action de tir du joueur
    if(gb.buttons.pressed(BUTTON_A) && bullet_exist == 0){
        bullet_x_pos = joueur_ship_X_tm;
        bullet_y_pos = joueur_ship_Y_tm;
        bullet_exist = 1;
        gb.display.setColor(bullet_color);
        gb.display.fillRect(bullet_x_pos, bullet_y_pos, bullet_w,bullet_h);
    }
  }
}

void showEndScreen(){
  gb.display.setCursor(gb.display.width()/4, gb.display.height()/4);
  gb.display.setColor(GREEN);
  gb.display.print("GAME OVER");

  gb.display.setCursor(gb.display.width()/4, gb.display.height()/4 + 10);
  gb.display.print("SCORE ");
  gb.display.print(score);

  gb.display.setColor(WHITE);
  gb.display.cursorX = 0;
  gb.display.cursorY = 56;
  gb.display.print("A:REPLAY  B:TITLE");

  if(gb.buttons.pressed(BUTTON_A)){
    game_state = 1;
  }
  if(gb.buttons.pressed(BUTTON_B)){
    game_state = 0;
  }
}

void createJoueur(){

  //valeur relatives du vaisseau du joueur
    
  //corp du vaisseau
  joueur_ship_X_br = joueur_ship_X_bl+4;
  joueur_ship_Y_br = joueur_ship_Y_bl;
  joueur_ship_X_tm = joueur_ship_X_bl+2;
  joueur_ship_Y_tm = joueur_ship_Y_bl-4;
  //aile gauche du vaisseau
  left_wing_X_bl = joueur_ship_X_bl-2;
  left_wing_Y_bl = joueur_ship_Y_bl;
  left_wing_X_tl = left_wing_X_bl;
  left_wing_Y_tl = joueur_ship_Y_tm;
  left_wing_X_bm = joueur_ship_X_tm;
  left_wing_Y_bm = left_wing_Y_bl;
  //aile droite du vaisseau
  right_wing_X_bm = left_wing_X_bm;
  right_wing_Y_bm = left_wing_Y_bm;
  right_wing_X_br = left_wing_X_bm+4;
  right_wing_Y_br = left_wing_Y_tl;
  right_wing_X_tr = right_wing_X_br;
  right_wing_Y_tr = left_wing_Y_bm;
  
  gb.display.setColor(WHITE);
  gb.display.fillTriangle(
    joueur_ship_X_bl, joueur_ship_Y_bl,
    joueur_ship_X_br, joueur_ship_Y_br,
    joueur_ship_X_tm, joueur_ship_Y_tm);
  gb.display.fillTriangle(
    left_wing_X_bl, left_wing_Y_bl,
    left_wing_X_tl, left_wing_Y_tl,
    left_wing_X_bm, left_wing_Y_bm);
  gb.display.fillTriangle(
    right_wing_X_bm, right_wing_Y_bm,
    right_wing_X_br, right_wing_Y_br,
    right_wing_X_tr, right_wing_Y_tr);
}