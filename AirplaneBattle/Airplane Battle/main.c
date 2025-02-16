#include<stdio.h>
#include<raylib.h>

void DrawBG(Texture2D);
void InitTextures(Texture2D*, Texture2D*, Texture2D*, float*);
void init_planes_position(int);

#define ScreenWidth 1200
#define ScreenHeight 700
#define num_bullets 100
#define num_enemies 7 // 0 to 6

struct bullets { //structure for bullets
    Rectangle pos;
};

struct player { //player data
    Texture2D sprite;
    Rectangle pos;

    float health;
    float speed;
    float size;
    int score;
    bool died;
    
    struct bullets goli[num_bullets];
} player;

struct enemy { //enemy data
    Texture2D sprite;
    Rectangle pos;

    float health;
    float speed;
    float size;
    
    struct bullets goli[num_bullets];
} enemy[num_enemies];

struct health { //healthup data
    Texture2D sprite;
    Rectangle pos;
    float size;
    bool to_show;
} health;

int main()
{
    int framecounter = 0; //to count frames
    float bullet_speed; //speed of bullets
    Texture2D grass, player_bullet, enemy_bullet; //Textures to store the images

    InitWindow(ScreenWidth, ScreenHeight, "Airplane Battle");
    InitTextures(&grass, &player_bullet, &enemy_bullet, &bullet_speed); //Initializing Textures and default values
    SetTargetFPS(60); //FPS 

    while(!WindowShouldClose())
    {
        if(!player.died)
        {
            //Calculation Section
            framecounter++;
            if(framecounter%5 == 0) //Number of Gaps of Frame before spawning player bullet
            {
                //spawning bullet in a negative element of array
                for(int i=0; i<num_bullets; ++i)
                {
                    if (player.goli[i].pos.y < 0)
                    {
                        player.goli[i].pos.x = player.pos.x+10;
                        player.goli[i].pos.y = player.pos.y + 30;

                        player.goli[i+1].pos.x = player.pos.x+90;
                        player.goli[i+1].pos.y = player.pos.y + 30;

                        break;
                    }

                }

                if(framecounter%20 == 0) //number of gaps of frame before spawing enemy bullet
                {
                    for(int i=0; i<num_enemies; ++i)
                    {
                        for(int j=0; j<num_bullets; ++j)
                        {
                            if( (enemy[i].pos.y + (enemy[i].sprite.height * enemy[i].size)) > 0.0f)
                            {
                                if(enemy[i].goli[j].pos.y > ScreenHeight)
                                {
                                    enemy[i].goli[j].pos.x = enemy[i].pos.x + 10;
                                    enemy[i].goli[j].pos.y = (enemy[i].pos.y + (enemy[i].sprite.height * enemy[i].size)) - 20;

                                    enemy[i].goli[j+1].pos.x = enemy[i].pos.x + 60;
                                    enemy[i].goli[j+1].pos.y = (enemy[i].pos.y + (enemy[i].sprite.height * enemy[i].size)) - 20;

                                    break;
                                }
                            }

                        }
                    }
                    framecounter = 0;
                    if(GetRandomValue(0, 30) == 4)
                    {
                        health.pos.x = GetRandomValue(300, 900);
                        health.pos.y = GetRandomValue(200, 500);
                        health.to_show = true;
                    }
                }
            }
            //player
                    //player movement and restricting to go beyond the screen
                if(IsKeyDown(KEY_RIGHT)) player.pos.x += player.speed;
                    if(player.pos.x > (ScreenWidth - ( player.sprite.width * player.size))) player.pos.x = (ScreenWidth - ( player.sprite.width * player.size));
                if(IsKeyDown(KEY_LEFT)) player.pos.x -= player.speed;
                    if(player.pos.x < 0) player.pos.x = 0;
                if(IsKeyDown(KEY_UP)) player.pos.y -= player.speed;
                    if(player.pos.y < 0) player.pos.y = 0;
                if(IsKeyDown(KEY_DOWN)) player.pos.y += player.speed;
                    if(player.pos.y > (ScreenHeight - ( player.sprite.height * player.size))) player.pos.y = (ScreenHeight - ( player.sprite.height * player.size));

                    //making player bullets go up , there position will be in negative once they cross the screen
                for(int i=0; i<num_bullets; ++i)
                {   
                    if(player.goli[i].pos.y >= 0)
                        player.goli[i].pos.y -= bullet_speed;
                }

                    //player dieing
                if(player.health <= 0)
                {
                    player.died = true;
                }

            //enemy
                //moving enemy planes and their bullets
                for(int i=0; i<num_enemies; ++i)
                {
                    for(int j=0; j<num_bullets; ++j)
                    {
                        if(enemy[i].goli[j].pos.y >= 0 && enemy[i].goli[j].pos.y < ScreenHeight)
                            enemy[i].goli[j].pos.y += bullet_speed / 2.0f;

                    }
                    enemy[i].pos.y += (1.0f / (i + 1)); //Planes Moving Forward
                    if(enemy[i].pos.y > 0 && enemy[i].pos.y < ScreenHeight)
                    {
                        if(player.pos.x > enemy[i].pos.x)
                            enemy[i].pos.x += enemy[i].speed;
                        else
                            enemy[i].pos.x -= enemy[i].speed;
                    } //Planes moving toward player

                    if(enemy[i].health <= 0)
                    {
                        player.score += 100;
                        init_planes_position(i);
                    }
                    
                    if(enemy[i].pos.y > ScreenHeight) //spawn planes in the start if they die or reach the end
                    {
                        init_planes_position(i);
                    }
                }

            //Checking Collisions
                for(int i=0; i<num_enemies; ++i)
                {
                    for(int j=0; j<num_bullets; ++j)
                    {
                        if(CheckCollisionRecs(player.goli[j].pos, enemy[i].pos)) //player bullet with enemy
                        {
                            player.goli[j].pos.x = -1;
                            player.goli[j].pos.y = -1;
                            enemy[i].health -= 5;
                        }
    
                        if(CheckCollisionRecs(player.pos, enemy[i].goli[j].pos)) //enemy bullet with player
                        {
                            enemy[i].goli[j].pos.x = (ScreenHeight + 69);
                            enemy[i].goli[j].pos.y = (ScreenHeight + 69);
                            player.health -= 0.5f;
                        }
    
                        if(CheckCollisionRecs(player.goli[j].pos, enemy[i].goli[j].pos)) //enemy bullet with player bullet
                        {
                            player.goli[j].pos.x = -1;
                            player.goli[j].pos.y = -1;
                            enemy[i].goli[j].pos.y = (ScreenHeight + 69);
                            enemy[i].goli[j].pos.y = (ScreenHeight + 69);
    
                        }
    
                        if(CheckCollisionRecs(player.pos, health.pos) && health.to_show) //player with healthUP
                        {
                            player.score -= 50;
                            health.to_show = false;
                            player.health += 25;
                            if(player.health > 100) player.health = 100;
                        }
                    }
                }

            //Draw Section
            BeginDrawing();
            ClearBackground(WHITE);
            DrawBG(grass); //Drawing Grass Background
            DrawTextureEx(player.sprite, (Vector2){player.pos.x, player.pos.y}, 0.0f, player.size, WHITE); //player rendering
            DrawRectangle(player.pos.x + 30, player.pos.y + ( player.sprite.height * player.size) - 20, (int)player.health / 2.0f, 10,SKYBLUE); //health bar for the player
            DrawRectangleLines(player.pos.x + 30, player.pos.y + (player.sprite.height * player.size) - 20, (int)100.0f / 2.0f, 10, BLACK);
            for(int i=0; i < num_enemies; ++i) //enemies and their health bar
            {
                DrawTextureEx(enemy[i].sprite, (Vector2){enemy[i].pos.x, enemy[i].pos.y}, 0.0f, enemy[i].size, WHITE);
                DrawRectangle(enemy[i].pos.x + 18, enemy[i].pos.y - 15, (int)(enemy[i].health / 2.0f),10, RED);
                DrawRectangleLines(enemy[i].pos.x + 18, enemy[i].pos.y - 15, (int)100.0f / 2.0f, 10, BLACK);
            }
            if(health.to_show)
                DrawTextureEx(health.sprite, (Vector2){health.pos.x, health.pos.y}, 0.0f, health.size, WHITE);
            //Bullets of Player with changing gradients
            for(int i=0; i<num_bullets; ++i) {
                if(player.goli[i].pos.y >= 0) {
                    switch(GetRandomValue(1,3)) {
                        case 1 : DrawTextureEx(player_bullet, (Vector2){player.goli[i].pos.x, player.goli[i].pos.y}, 0.0f, 1.0f, RED);
                                 break;
                        case 2 : DrawTextureEx(player_bullet, (Vector2){player.goli[i].pos.x, player.goli[i].pos.y}, 0.0f, 1.0f, ORANGE);
                                 break;
                        case 3 : DrawTextureEx(player_bullet, (Vector2){player.goli[i].pos.x, player.goli[i].pos.y}, 0.0f, 1.0f, YELLOW);
                                 break;
                    }
                }
            }
            //enemy bullet with changing gradient 
            for(int i=0; i<num_enemies; ++i) {
                for(int j=0; j<num_bullets; ++j) {
                    if(enemy[i].goli[j].pos.y > 0) {
                        switch(GetRandomValue(1,3)) {
                            case 1 : DrawTextureEx(enemy_bullet, (Vector2){enemy[i].goli[j].pos.x, enemy[i].goli[j].pos.y}, 0.0f, 1.0f, PINK);
                                     break;
                            case 2 : DrawTextureEx(enemy_bullet, (Vector2){enemy[i].goli[j].pos.x, enemy[i].goli[j].pos.y}, 0.0f, 1.0f, PURPLE);
                                     break;
                            case 3 : DrawTextureEx(enemy_bullet, (Vector2){enemy[i].goli[j].pos.x, enemy[i].goli[j].pos.y}, 0.0f, 1.0f, VIOLET);
                                     break;
                        }
                    }
                }
            }
            DrawText(TextFormat("SCORE : %d",player.score), 10, 10, 20, WHITE);
        }
        else
        {
            if(framecounter <= 300)
            {
                int x, y;
                for(int i=0; i < 10000; ++i)
                {
                    x = GetRandomValue(0, ScreenWidth);
                    y = GetRandomValue(0, ScreenHeight);
                    DrawPixel(x, y, BLACK);
                }
                framecounter++;
            }
            if(framecounter <= 600 && framecounter >= 300)
            {
                ClearBackground(BLACK);
                DrawText(TextFormat("Game Over - %d",player.score), 200, 300, 100, WHITE);
                framecounter++;
            }
            if(framecounter > 600)
                break;
            
        }
        EndDrawing();
         
    }
    CloseWindow();
    return 0;
}

void DrawBG(Texture2D grass)
{
    int x , y ;
    x = 0;
    y = 0;

    while(y < ScreenHeight)
    {
        DrawTextureEx(grass, (Vector2){x, y}, 0.0f, 0.5f, GREEN);
        x += grass.width*0.5f;
        if(x > ScreenWidth)
        {
            x = 0;
            y += grass.height*0.5f;
        }
    }
    x = y = 0;
}

void InitTextures(Texture2D *grass, Texture2D *player_bullet, Texture2D *enemy_bullet, float *bullet_speed)
{
    //player
    player.sprite = LoadTexture("res/player1.png");
    player.speed = 3.0f;
    player.health = 100.0f; 
    player.size = 0.45f;
    player.score = 0;
    player.died = false;
    player.pos.y = ScreenHeight - ( player.sprite.height * player.size);

    player.pos.height = (player.sprite.height * player.size);
    player.pos.width = (player.sprite.width * player.size);

    *player_bullet = LoadTexture("res/player_bullet.png");
    for(int i=0; i<num_bullets; ++i)
    {
        player.goli[i].pos.x = -1;
        player.goli[i].pos.y = -1;
        player.goli[i].pos.height = (player_bullet->height);
        player.goli[i].pos.width = (player_bullet->width);
    }

    //misc
    *bullet_speed = 7.0f;
    *grass = LoadTexture("res/grass.png");

    //health_levelup
    health.sprite = LoadTexture("res/health.png");
    health.pos.x = 100;
    health.pos.y = 100;
    health.size = 1.5f;
    health.to_show = false;
    health.pos.height = health.sprite.height * health.size;
    health.pos.width = health.sprite.width * health.size;

    //enemy
    *enemy_bullet = LoadTexture("res/enemy_bullet.png");
    for(int i=0; i<num_enemies; ++i)
    {
        enemy[i].sprite = LoadTexture( TextFormat("res/enemy%d.png",i) );
        enemy[i].speed = i * 0.075f; //all enemy gets different speed;
        enemy[i].size = 0.50f;
        enemy[i].health = 100;

        enemy[i].pos.height = (enemy[i].sprite.height * enemy[i].size);
        enemy[i].pos.width = (enemy[i].sprite.width * enemy[i].size);

        init_planes_position(i);

        for(int j=0; j<num_bullets; ++j)
        {
            enemy[i].goli[j].pos.x = (ScreenHeight + 69); 
            enemy[i].goli[j].pos.y = (ScreenHeight + 69);
            enemy[i].goli[j].pos.height = enemy_bullet->height;
            enemy[i].goli[j].pos.width = enemy_bullet->width;
        }
    }
}

void init_planes_position(int i)
{
    enemy[i].pos.x = GetRandomValue(1, (ScreenWidth-enemy[i].sprite.width));
    enemy[i].pos.y = GetRandomValue(-175, -200);
    enemy[i].health = 100;
}
