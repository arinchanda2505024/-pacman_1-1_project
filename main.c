#include "raylib.h"
#include"raymath.h"
#include "collision.h"
#include <stddef.h>
#include "ghost.h"
#include <math.h>

#define gheight 950
#define gwidth 1900
#define wtiles 28
#define htiles 31
#define main_speed 110

//map er array
char map[htiles][wtiles+1]=
    {"cssssssssssssssssssssssssssa",
     "tddddddddddddvvddddddddddddt",
     "tduwwgduwwwgdvvduwwwgduwwgdt",
     "tbveevdveeevdvvdveeevdveevbt",
     "tdpwwrdpwwwrdprdpwwwrdpwwrdt",
     "tddddddddddddddddddddddddddt",
     "tduwwgdugduwwwwwwgdugduwwgdt",
     "tdpwwrdvvdpwwwwwwrdvvdpwwrdt",
     "tddddddvvddddvvddddvvddddddt",
     "lssssadvpwwgevveuwwrvdcssssf",
     "eeeeetdvuwwreprepwwgvdteeeee",
     "eeeeetdvveeeeeeeeeevvdteeeee",
     "eeeeetdvvecssiissaevvdteeeee",
     "sssssfdpreteeeeeeteprdlsssss",
     "eeeeeedeeeteeeeeeteeedeeeeee",
     "sssssadugeteeeeeeteugdcsssss",
     "eeeeetdvvelssssssfevvdteeeee",
     "eeeeetdvveeeeeeeeeevvdteeeee",
     "eeeeetdvveuwwwwwwgevvdteeeee",
     "cssssfdprepwwguwwreprdlssssa",
     "tddddddddddddvvddddddddddddt",
     "tduwwgduwwwgdvvduwwwgduwwgdt",
     "tdpwgvdpwwwrdprdpwwwrdvuwrdt",
     "tbddvvddddddddddddddddvvddbt",
     "twgdvvdugduwwwwwwgdugdvvduwt",
     "twrdprdvvdpwwguwwrdvvdprdpwt",
     "tddddddvvddddvvddddvvddddddt",
     "tduwwwwrpwwgdvvduwwrpwwwwgdt",
     "tdpwwwwwwwwrdprdpwwwwwwwwrdt",
     "tddddddddddddddddddddddddddt",
     "lssssssssssssssssssssssssssf"
    };
     //pacman er first position
    Rectangle pacman= {976, 670,24,24}; 
    
    

int main(){
    
    InitWindow(gwidth, gheight, "Pacman");
    SetTargetFPS(60);
    
    int score=0;
    int wall_position_x[28];
    int wall_position_y[31];
    int life=3;

    here:
    Vector2 position={976, 670};
    Vector2 speed = {0,0};
    Vector2 nextSpeed = {0, 0};
    float scattered_time=0.0f;
    float chase_time=0.0f;
    float frightened_time=0.0f;
    float eaten_time=0.0f;
    int time=5;
    

    
    
    //w=horizontal single wall,v=vertical single wall, s=horizontal double wall, t=vertical double wall,d=dot,e=empty space
    //g = top right single corner, u = top left single corner
    //r= bottom right single corner, p = bottom left single corner
    //a = top right double corner, c = top left double corner
    //f= bottom right double corner, l= bottom left double corner
    
    //wall sprite er jonno
    Texture tex_c = LoadTexture("sprite\\wall\\WALL_DOUBLE_CORNER_TL.png");
    Texture tex_s = LoadTexture("sprite\\wall\\WALL_DOUBLE_H.png");
    Texture tex_a = LoadTexture("sprite\\wall\\WALL_DOUBLE_CORNER_TR.png");
    Texture tex_t = LoadTexture("sprite\\wall\\WALL_DOUBLE_V.png");
    Texture tex_v = LoadTexture("sprite\\wall\\WALL_SINGLE_V.png");
    Texture tex_u = LoadTexture("sprite\\wall\\WALL_SINGLE_CORNER_TL.png");
    Texture tex_w = LoadTexture("sprite\\wall\\WALL_SINGLE_H.png");
    Texture tex_g = LoadTexture("sprite\\wall\\WALL_SINGLE_CORNER_TR.png");
    Texture tex_r = LoadTexture("sprite\\wall\\WALL_SINGLE_CORNER_BR.png");
    Texture tex_f = LoadTexture("sprite\\wall\\WALL_DOUBLE_CORNER_BR.png");
    Texture tex_l = LoadTexture("sprite\\wall\\WALL_DOUBLE_CORNER_BL.png");
    Texture tex_p = LoadTexture("sprite\\wall\\WALL_SINGLE_CORNER_BL.png");
    
    
    
    ghost blinky_ghost;
    ghost pinky_ghost;
    ghost inky_ghost;
    ghost clyde_ghost;
    
    init_phase(&blinky_ghost, blinky);
    init_phase(&pinky_ghost, pinky);
    init_phase(&inky_ghost, inky);
    init_phase(&clyde_ghost, clyde);

    ghost_mode phase=scattered;
    bool over=false;
    
    while(!WindowShouldClose()){
        float dt= GetFrameTime();
        //pacman er movement:
        if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
            nextSpeed = (Vector2){-main_speed, 0};
        }
        else if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
            nextSpeed = (Vector2){main_speed, 0};
        }
        else if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
            nextSpeed = (Vector2){0, main_speed};
        }
        else if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
            nextSpeed = (Vector2){0, -main_speed};
        }

        //pacman er movement smooth er jonno
        /*ekhane jkhn key press kora hoy tkhn next speed update hoy r speed mainly ager tai thake r then next speed turnbox e check hoy,
        jodi turnbox kono wall na pay taile speed er vitor nexspeed assign kore dibe r oita intersection e gele turn korbe r jodi turnbox wall pay
        taile speed ager tai thakbe r as usual cholte thakbe*/
        float shrink = 2;

        Rectangle turnBox = {
            position.x + nextSpeed.x * dt + shrink / 2,
            position.y + nextSpeed.y * dt + shrink / 2,
            26 - shrink,
            26 - shrink
        };

        if(!collision(turnBox, map)){
            speed = nextSpeed;
        }

        Rectangle collisionBox = {
            position.x + speed.x * dt + shrink / 2,
            position.y + speed.y * dt + shrink / 2,
            26 - shrink,
            26 - shrink
        };
        //pacman er position update
        
        
        if(!collision(collisionBox, map)){
            
            
            position.x += speed.x*dt;
            position.y += speed.y*dt;
            
        }
        //pacman = (Rectangle){position.x + speed.x*dt, position.y + speed.y*dt, 24, 24};
        
        pacman= (Rectangle){position.x,position.y,24,24};
        //dot collection
            
        //pacman er center khuje tiles number ber kora
        int tile_j = (int)((position.x + 12 - 586) / 26);
        int tile_i = (int)((position.y + 12 - 72) / 26);

        if (tile_i >= 0 && tile_i < htiles && tile_j >= 0 && tile_j < wtiles) {
            if (map[tile_i][tile_j] == 'd') {
                map[tile_i][tile_j] = 'e'; 
                score += 10;
            } 
            else if (map[tile_i][tile_j] == 'b') {
                map[tile_i][tile_j] = 'e'; 
                score += 50;

                
                phase=frightened;
                flip_dir(&blinky_ghost);
                flip_dir(&pinky_ghost);
            }
        }

        /*if(position.x<612){
            position.x=612;
            
        }
        else if(position.x>1262){
            position.x=1262;
        }
        else if(position.y<98){
            position.y=98;
        }
        else if(position.y>826){
            position.y=826;
        }*/
       if(tile_i == 14){
            if(position.x < 564){
                position.x = 1288;
            }
            else if(position.x > 1288){
                position.x = 564;
            }
        }
        else{
            if(position.x < 612){
                position.x = 612;
            }
            else if(position.x > 1262){
                position.x=1262;
            }
        }

        if(position.y < 98){
            position.y = 98;
        }
        else if(position.y > 826){
            position.y = 826;
        }
        
        Rectangle ghost_rec_blinky=g_rec(&blinky_ghost,blinky);
        Rectangle ghost_rec_pinky=g_rec(&pinky_ghost,pinky);
        Rectangle ghost_rec_inky=g_rec(&inky_ghost,inky);
        Rectangle ghost_rec_clyde=g_rec(&clyde_ghost,clyde);
        
        
        
        if(!over){
            if(phase == scattered){
                if(scattered_time<10.0f){
                    
                    tile ghost_tile_blinky = tiles_no(blinky_ghost.position);
                    tile ghost_tile_pinky = tiles_no(pinky_ghost.position);
                    tile ghost_tile_inky = tiles_no(inky_ghost.position);
                    tile ghost_tile_clyde = tiles_no(clyde_ghost.position);

                    Vector2 blinky_tile_position =pixel(ghost_tile_blinky);
                    Vector2 pinky_tile_position =pixel(ghost_tile_pinky);
                    Vector2 inky_tile_position =pixel(ghost_tile_inky);
                    Vector2 clyde_tile_position =pixel(ghost_tile_clyde);

                    if (fabsf(blinky_ghost.position.x - blinky_tile_position.x) < 1.0f && fabsf(blinky_ghost.position.y - blinky_tile_position.y) < 1.0f) 
                    {
                        blinky_scatter_alg(&blinky_ghost);
                    }
                    if (fabsf(pinky_ghost.position.x - pinky_tile_position.x) < 1.0f && fabsf(pinky_ghost.position.y - pinky_tile_position.y) < 1.0f)  
                    {
                        pinky_scatter_alg(&pinky_ghost);
                    }

                    if (fabsf(inky_ghost.position.x - inky_tile_position.x) < 1.0f && fabsf(inky_ghost.position.y - inky_tile_position.y) < 1.0f)  
                    {
                        inky_scatter_alg(&inky_ghost);
                    }
                    if (fabsf(clyde_ghost.position.x - clyde_tile_position.x) < 1.0f && fabsf(clyde_ghost.position.y - clyde_tile_position.y) < 1.0f)  
                    {
                        clyde_scatter_alg(&clyde_ghost);
                    }
                        
                    
                    scattered_time+=dt;
                }
                else{
                    phase = chase;
                    scattered_time = 0.0f;
                    chase_time = 0.0f;
                }
                movement(&blinky_ghost,ghost_normal_speed);
                movement(&pinky_ghost,ghost_normal_speed);
                movement(&inky_ghost,ghost_normal_speed);
                movement(&clyde_ghost,ghost_normal_speed);
            }
        
        
            Vector2 pacpos={position.x,position.y};
        

        
            if(phase == chase){
                if(chase_time<20.0f){
                    
                    tile ghost_tile_blinky = tiles_no(blinky_ghost.position);
                    tile ghost_tile_pinky = tiles_no(pinky_ghost.position);
                    tile ghost_tile_inky = tiles_no(inky_ghost.position);
                    tile ghost_tile_clyde = tiles_no(clyde_ghost.position);

                    Vector2 blinky_tile_position =pixel(ghost_tile_blinky);
                    Vector2 pinky_tile_position =pixel(ghost_tile_pinky);
                    Vector2 inky_tile_position =pixel(ghost_tile_inky);
                    Vector2 clyde_tile_position =pixel(ghost_tile_clyde);

                    if (fabsf(blinky_ghost.position.x - blinky_tile_position.x) < 1.0f && fabsf(blinky_ghost.position.y - blinky_tile_position.y) < 1.0f)
                    {
                        
                        
                        blinky_chase_alg(&blinky_ghost, pacpos);
                        
                    }
                    if (fabsf(pinky_ghost.position.x - pinky_tile_position.x) < 1.0f && fabsf(pinky_ghost.position.y - pinky_tile_position.y) < 1.0f)
                    {
                        
                        
                        pinky_chase_alg(&pinky_ghost, pacpos, speed);
                        
                    }

                    if (fabsf(inky_ghost.position.x - inky_tile_position.x) < 1.0f && fabsf(inky_ghost.position.y - inky_tile_position.y) < 1.0f)
                    {
                        
                        
                        inky_chase_alg(&inky_ghost, &blinky_ghost, pacpos, speed);
                        
                    }
                    if (fabsf(clyde_ghost.position.x - clyde_tile_position.x) < 1.0f && fabsf(clyde_ghost.position.y - clyde_tile_position.y) < 1.0f)
                    {
                        
                        
                        clyde_chase_alg(&clyde_ghost, pacpos);
                        
                    }
                        
                    
                    chase_time += dt;
                }
                else{
                    phase = scattered;
                    scattered_time = 0.0f;
                    chase_time = 0.0f;
                }
                movement(&blinky_ghost, ghost_normal_speed);
                movement(&pinky_ghost, ghost_normal_speed);
                movement(&inky_ghost, ghost_normal_speed);
                movement(&clyde_ghost, ghost_normal_speed);
            }
        /*if(phase == frightened){
            if(frightened_time<6.0f){
                frightened_time+=dt;
                
                ghost_frightened(&g);
                movement(&g,ghost_frightended_speed);


            }
            
        */
        
            if (phase == frightened){
                if(CheckCollisionRecs(pacman, ghost_rec_blinky) ||  CheckCollisionRecs(pacman, ghost_rec_pinky) ||  CheckCollisionRecs(pacman, ghost_rec_inky) || CheckCollisionRecs(pacman, ghost_rec_clyde)){
                    phase = eaten; 
                    score+=200;
                    continue; 
                }
                
                frightened_time += dt;

                tile ghost_tile_blinky = tiles_no(blinky_ghost.position);
                tile ghost_tile_pinky = tiles_no(pinky_ghost.position);
                tile ghost_tile_inky = tiles_no(inky_ghost.position);
                tile ghost_tile_clyde = tiles_no(clyde_ghost.position);

                Vector2 blinky_tile_position =pixel(ghost_tile_blinky);
                Vector2 pinky_tile_position =pixel(ghost_tile_pinky);
                Vector2 inky_tile_position =pixel(ghost_tile_inky);
                Vector2 clyde_tile_position =pixel(ghost_tile_clyde);

                bool at_tile_blinky =fabsf(blinky_ghost.position.x - blinky_tile_position.x) < 1.0f && fabsf(blinky_ghost.position.y - blinky_tile_position.y) < 1.0f;
                    
                bool at_tile_pinky = fabsf(pinky_ghost.position.x - pinky_tile_position.x) < 1.0f && fabsf(pinky_ghost.position.y - pinky_tile_position.y) < 1.0f;

                bool at_tile_inky = fabsf(inky_ghost.position.x - inky_tile_position.x) < 1.0f && fabsf(inky_ghost.position.y - inky_tile_position.y) < 1.0f;

                bool at_tile_clyde = fabsf(clyde_ghost.position.x - clyde_tile_position.x) < 1.0f && fabsf(clyde_ghost.position.y - clyde_tile_position.y) < 1.0f;
                    
                    

                if (at_tile_blinky) {
                    ghost_frightened(&blinky_ghost);
                    blinky_ghost.eaten=true;
                    
                }
                else if (at_tile_pinky) {
                    ghost_frightened(&pinky_ghost);
                    pinky_ghost.eaten=true;
                    
                }
                else if (at_tile_inky) {
                    ghost_frightened(&inky_ghost);
                    inky_ghost.eaten=true;
                    
                }
                else if (at_tile_clyde) {
                    ghost_frightened(&clyde_ghost);
                    clyde_ghost.eaten=true;
                    
                }


                if (frightened_time >6.0f) {
                    phase = chase;
                    frightened_time = 0.0f;
                    chase_time = 0.0f;
                }
                movement(&blinky_ghost,ghost_frightended_speed);
                movement(&pinky_ghost,ghost_frightended_speed);
                movement(&inky_ghost,ghost_frightended_speed);
                movement(&clyde_ghost,ghost_frightended_speed);
            }
            if (phase == eaten) {
                tile ghost_tile_blinky = tiles_no(blinky_ghost.position);
                tile ghost_tile_pinky = tiles_no(pinky_ghost.position);
                tile ghost_tile_inky = tiles_no(inky_ghost.position);
                tile ghost_tile_clyde = tiles_no(clyde_ghost.position);

                Vector2 blinky_tile_position =pixel(ghost_tile_blinky);
                Vector2 pinky_tile_position =pixel(ghost_tile_pinky);
                Vector2 inky_tile_position =pixel(ghost_tile_inky);
                Vector2 clyde_tile_position =pixel(ghost_tile_clyde);
                
                bool at_tile_blinky = fabsf(blinky_ghost.position.x - blinky_tile_position.x) < 1.0f && fabsf(blinky_ghost.position.y - blinky_tile_position.y) < 1.0f;

                bool at_tile_pinky = fabsf(pinky_ghost.position.x - pinky_tile_position.x) < 1.0f && fabsf(pinky_ghost.position.y - pinky_tile_position.y) < 1.0f;

                bool at_tile_inky = fabsf(inky_ghost.position.x - inky_tile_position.x) < 1.0f && fabsf(inky_ghost.position.y - inky_tile_position.y) < 1.0f;

                bool at_tile_clyde = fabsf(clyde_ghost.position.x - clyde_tile_position.x) < 1.0f && fabsf(clyde_ghost.position.y - clyde_tile_position.y) < 1.0f;
                    

                if (at_tile_blinky) {
                    eaten_phase_blinky(&blinky_ghost); 
                }

                if (at_tile_pinky) {
                    eaten_phase_pinky(&pinky_ghost); 
                }

                if (at_tile_inky) {
                    eaten_phase_inky(&inky_ghost); 
                }
                if (at_tile_clyde) {
                    eaten_phase_clyde(&clyde_ghost); 
                }
                
                movement(&blinky_ghost, ghost_eaten_speed);
                movement(&pinky_ghost, ghost_eaten_speed);
                movement(&inky_ghost, ghost_eaten_speed);
                movement(&clyde_ghost, ghost_eaten_speed);

                
                if (ghost_tile_blinky.row == 11 && ghost_tile_blinky.col == 13 ) {
                    phase = chase;      
                    chase_time = 0.0f;  
                    scattered_time = 0.0f; 
                }
                if (ghost_tile_pinky.row == 11 && ghost_tile_pinky.col == 12) {
                    phase = chase;      
                    chase_time = 0.0f;  
                    scattered_time = 0.0f; 
                }
                if (ghost_tile_inky.row == 11 && ghost_tile_inky.col == 15) {
                    phase = chase;      
                    chase_time = 0.0f;  
                    scattered_time = 0.0f; 
                }
                if (ghost_tile_clyde.row == 11 && ghost_tile_clyde.col == 14) {
                    phase = chase;      
                    chase_time = 0.0f;  
                    scattered_time = 0.0f; 
                }
                
            }
        }
        
        

        BeginDrawing();
        ClearBackground(BLACK);

        
        
        //position = Vector2Add(position, Vector2Scale(speed,dt)); //s=s+vt vector2add((x,y),(a,b)) (x+a,y+b) 3(2i+3j)=6i+9j
        for(int i = 0; i<htiles; i++){
            for(int j = 0; j<wtiles; j++){
                Texture *wall_map = NULL;
                switch(map[i][j]){
                    case 'c':
                    wall_map=&tex_c;
                    break;
                    
                    case 's':
                    wall_map=&tex_s;
                    break;

                    case 'a':
                    wall_map=&tex_a;
                    break;

                    case 't':
                    wall_map=&tex_t;
                    break;

                    case 'v':
                    wall_map=&tex_v;
                    break;

                    case 'u':
                    wall_map=&tex_u;
                    break;

                    case 'w':
                    wall_map=&tex_w;
                    break;

                    case 'g':
                    wall_map=&tex_g;
                    break;

                    case 'r':
                    wall_map=&tex_r;
                    break;

                    case 'f':
                    wall_map=&tex_f;
                    break;

                    case 'l':
                    wall_map=&tex_l;
                    break;

                    case 'p':
                    wall_map=&tex_p;

                    default:
                    break;
                }
                
            

                Rectangle dest = {586+j*26, 72+i*26, 26, 26};
                if (wall_map != NULL) {
                    Rectangle source = {1, 1, (float)wall_map->width - 2, (float)wall_map->height - 2};
                    DrawTexturePro(*wall_map, source, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
                
                
            }
        
        
        }
    }

        
        
       
        
        /*if(position.x == 612 && position.y == 462){
            position.x = 586;
            position.x = 1288;
            position.y = 462;
        }*/
        

            
        
        for(int i=0; i<htiles;i++){
            for(int j=0; j<wtiles; j++){
                
                if(map[i][j]=='d'){
                    DrawRectangle(586+j*26, 72+i*26, 26, 26, BLACK);
                    DrawCircle(586+j*26+13, 72+i*26+13, 3, ORANGE);
                }
                else if(map[i][j]=='e'){
                    DrawRectangle(586+j*26, 72+i*26, 26, 26, BLACK);
                }
                else if(map[i][j]=='b'){
                    DrawRectangle(586+j*26, 72+i*26, 26, 26, BLACK);
                    DrawCircle(586+j*26+13, 72+i*26+13, 8, YELLOW);
                }
                else if(map[i][j]=='i'){
                    DrawRectangle(586+j*26, 72+i*26, 26, 26, PINK);
                    
                }



            }
        }
        
        if(!over && (phase == chase || phase == scattered)){
            if(CheckCollisionRecs(pacman, ghost_rec_blinky) || CheckCollisionRecs(pacman, ghost_rec_pinky) || CheckCollisionRecs(pacman, ghost_rec_inky) || CheckCollisionRecs(pacman, ghost_rec_clyde)){
                
                life--;
                if(life>0){
                    goto here;
                }
                else if(life == 0){
                    over=true;
                }
            }
            
        }

        if(over){
            speed = (Vector2){0, 0};
            nextSpeed = (Vector2){0, 0};
            movement(&blinky_ghost, 0.0f); 
            movement(&pinky_ghost, 0.0f); 
            movement(&inky_ghost, 0.0f);
            movement(&clyde_ghost, 0.0f);
        } 
        
            
        
        if(over){
            DrawText("Game Over", 898, 514,26, RED);
            nextSpeed=( Vector2){0,0};
        }

        
        DrawRectangleRec(pacman, YELLOW);

        DrawText(TextFormat("SCORE: %d", score), 586, 30, 30, WHITE);
        DrawText("LIFE:",600,895,26,WHITE);
        Texture2D life_sprite=LoadTexture("C:\\Users\\USER\\Desktop\\1-1 Project\\raylib_template\\life_sprite.png");
        
        for(int i=0;i<life;i++){
            DrawTexture(life_sprite,700+50*i,895,YELLOW);
        }

        Rectangle draw_g_rec_blinky=g_rec(&blinky_ghost,blinky);
        Rectangle draw_g_rec_pinky = g_rec(&pinky_ghost,pinky);
        Rectangle draw_g_rec_inky = g_rec(&inky_ghost,inky);
        Rectangle draw_g_rec_clyde = g_rec(&clyde_ghost,clyde);

        if(phase==frightened){
            DrawRectangleRec(draw_g_rec_blinky, BLUE);
            DrawRectangleRec(draw_g_rec_pinky, BLUE);
            DrawRectangleRec(draw_g_rec_inky, BLUE);
            DrawRectangleRec(draw_g_rec_clyde, BLUE);
        }
        else if(phase==eaten){
            DrawRectangleRec(draw_g_rec_blinky, GRAY);
            DrawRectangleRec(draw_g_rec_pinky, GRAY);
            DrawRectangleRec(draw_g_rec_inky, GRAY);
            DrawRectangleRec(draw_g_rec_clyde, GRAY);
        }
        else{
            DrawRectangleRec(draw_g_rec_blinky, RED);
            DrawRectangleRec(draw_g_rec_pinky, PINK);
            DrawRectangleRec(draw_g_rec_inky, SKYBLUE);
            DrawRectangleRec(draw_g_rec_clyde, ORANGE);
        }

        
        
        
        EndDrawing();

        


    }
    UnloadTexture(tex_c);
    UnloadTexture(tex_s);
    UnloadTexture(tex_a);
    UnloadTexture(tex_t);
    UnloadTexture(tex_v);
    UnloadTexture(tex_u);
    UnloadTexture(tex_w);
    UnloadTexture(tex_g);
    UnloadTexture(tex_r);
    UnloadTexture(tex_f);
    UnloadTexture(tex_l);
    UnloadTexture(tex_p);
    CloseWindow();
}
