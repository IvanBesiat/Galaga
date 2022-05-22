#include <Gamebuino-Meta.h>

int ennemies[15][3] = {
  {17,5,1},{27,5,1},{37,5,1},{47,5,1},{57,5,1},
  {17,13,1},{27,13,1},{37,13,1},{47,13,1},{57,13,1},
  {17,21,1},{27,21,1},{37,21,1},{47,21,1},{57,21,1}};
int ennemi_w = 6;
int ennemi_h = 4;
int ennemi_x_move = 1;
bool all_dead = false;

int joueur_ship_X_pos1 = gb.display.width()/2-4;
int joueur_ship_Y_pos1 = gb.display.height()-2;
int joueur_lives = 2;

int bullet_x_pos = 81;
int bullet_y_pos = 63;
int bullet_w = 1;
int bullet_h = 3;
int bullet_exist = 0;

int bullet_ennemi_x_pos = 81;
int bullet_ennemi_y_pos = 63;
int bullet_ennemi_w = 1;
int bullet_ennemi_h = 3;
int bullet_ennemi_exist = 0;


int joueur_move_speed = 2;

void setup() {
  gb.begin();
}
void loop() {
  while(!gb.update());
  gb.display.clear();

  //propiétés relative du joueur
  int joueur_ship_X_pos2 = joueur_ship_X_pos1+8;
  int joueur_ship_Y_pos2 = joueur_ship_Y_pos1;
  int joueur_ship_X_pos3 = joueur_ship_X_pos1+4;
  int joueur_ship_Y_pos3 = joueur_ship_Y_pos1-6;
  if(all_dead)
  {
      gb.display.setCursor(gb.display.width()/2, gb.display.height()/2);
      gb.display.setColor(GREEN);
      gb.display.print("GAME OVER");
  }
  else
  {
    //création joueur
    gb.display.setColor(WHITE);
    gb.display.fillTriangle(
      joueur_ship_X_pos1, joueur_ship_Y_pos1,
      joueur_ship_X_pos2, joueur_ship_Y_pos2,
      joueur_ship_X_pos3, joueur_ship_Y_pos3);

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
          bullet_exist = 0;
      }

      if(ennemies[i][2] > 0){
        all_dead = false;
        gb.display.fillRect(ennemies[i][0], ennemies[i][1], ennemi_w, ennemi_h);
      }

      if(((ennemies[i][0] + ennemi_w) >= gb.display.width() && ennemies[i][2] > 0) 
        || (ennemies[i][0] <= 0 && ennemies[i][2] > 0)){
        ennemi_x_move = -ennemi_x_move;
        break;
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
      if((joueur_ship_X_pos2 + joueur_move_speed) < gb.display.width()){
        joueur_ship_X_pos1 = joueur_ship_X_pos1 + joueur_move_speed;
      }
    }
    if(gb.buttons.repeat(BUTTON_LEFT, 0)){
      if(joueur_ship_X_pos1 != 0){
        joueur_ship_X_pos1 = joueur_ship_X_pos1 - joueur_move_speed;
      }
    }

    //création de la munition
    if(bullet_y_pos != gb.display.height() && bullet_y_pos != 0 && bullet_exist == 1){
      bullet_y_pos = bullet_y_pos - 2;
      gb.display.setColor(RED);
      gb.display.fillRect(bullet_x_pos, bullet_y_pos-2, bullet_w,bullet_h);
    }
    
    if(bullet_y_pos <= 0){
      bullet_exist = 0;
    }

    if(gb.buttons.pressed(BUTTON_A) && bullet_exist == 0){
        bullet_x_pos = joueur_ship_X_pos3;
        bullet_y_pos = joueur_ship_Y_pos3;
        bullet_exist = 1;
        gb.display.setColor(RED);
        gb.display.fillRect(bullet_x_pos, bullet_y_pos, bullet_w,bullet_h);
    }
  }

}