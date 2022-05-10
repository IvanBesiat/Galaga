#include <Gamebuino-Meta.h>

int ennemies[10][2] = {
  {17,20,1},{27,20,1},{37,20,1},{47,20},{57,20},
  {17,28},{27,28},{37,28},{47,28},{57,28}};
int ennemi_h = 4;
int ennemi_w = 6;

int joueur_ship_X_pos1 = gb.display.width()/2-4;
int joueur_ship_Y_pos1 = gb.display.height()-2;

int bullet_x_pos = 81;
int bullet_y_pos = 63;
int bullet_h = 3;
int bullet_w = 1;

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

  //création joueur
  gb.display.setColor(WHITE);
  gb.display.fillTriangle(
    joueur_ship_X_pos1, joueur_ship_Y_pos1,
    joueur_ship_X_pos2, joueur_ship_Y_pos2,
    joueur_ship_X_pos3, joueur_ship_Y_pos3);

  //création ennemis
  for(int i=0; i < 10; i++){
    gb.display.setColor(RED);
    //if(ennemies[i][0] == bullet_x_pos-3){
      gb.display.fillRect(ennemies[i][0], ennemies[i][1], ennemi_w, ennemi_h);
    //}
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
  if(bullet_x_pos != gb.display.height()+1 && bullet_x_pos + bullet_h != 0){
    bullet_y_pos = bullet_y_pos - 2;
    gb.display.setColor(RED);
    gb.display.fillRect(bullet_x_pos, bullet_y_pos-2, bullet_w,bullet_h);
  }
  
  if(gb.buttons.pressed(BUTTON_A)){
      bullet_x_pos = joueur_ship_X_pos3;
      bullet_y_pos = joueur_ship_Y_pos3;
      gb.display.setColor(RED);
      gb.display.fillRect(bullet_x_pos, bullet_y_pos, bullet_w,bullet_h);
  }


}