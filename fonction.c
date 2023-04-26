#include "fonction.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "jeu.h"



void initPerso(personnage *p)
{
    int i, w;
    p->animation.spriteSheet[0] = IMG_Load("images/spriteSheet/left.png");
    p->animation.spriteSheet[1] = IMG_Load("images/spriteSheet/right.png");
    p->pos.x = 10;
    p->pos.y = 400;
    p->vie = 3;
    p->score = 0;
    p->vitesse = 5;
    p->acceleration = 0;
    p->animation.frames = 3;
    p->direction = 1;
    p->animation.ClipLoaded = 0;

    p->onGround = 1;
    p->up = 0;

    for (w = 0, i = 0; i < p->animation.frames; i++, w += 220)
    {
        p->animation.Clip[i].x = w;
        p->animation.Clip[i].y = 0;
        p->animation.Clip[i].h = 263;
        p->animation.Clip[i].w = 165;
    }
}

void showText(SDL_Surface *surface, TTF_Font *font, SDL_Color color, char *text, int x, int y)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, color);

    SDL_BlitSurface(textSurface, NULL, surface, &pos);

    SDL_FreeSurface(textSurface);
}

void afficherPersoScore(personnage p, SDL_Surface *screen)
{
    TTF_Font *font = NULL;
    font = TTF_OpenFont("Retro.ttf", 40);

    SDL_Color textColor = {0, 0, 0};

    SDL_Surface *emptyHeart = IMG_Load("emptyHeart.png");
    SDL_Surface *Heart = IMG_Load("heart.png");

    char score[20];
    char acceleration[20];

    sprintf(score, "Score: %d", p.score);

    if (p.acceleration > 0)
        sprintf(acceleration, "Acceleration: on");
    else
        sprintf(acceleration, "Acceleration: off");

    showText(screen, font, textColor, score, 0, 0);
    showText(screen, font, textColor, acceleration, 0, 120);
    showText(screen, font, textColor, "Vie:", 0, 60);

    int i, xChanged = 100;

    SDL_Rect heartPos;
    heartPos.y = 65;
    for (i = 0; i < p.vie; i++)
    {
        heartPos.x = xChanged;
        SDL_BlitSurface(Heart, NULL, screen, &heartPos);
        xChanged += 50;
    }
    for (i = 0; i < 3 - p.vie; i++)
    {
        heartPos.x = xChanged;
        SDL_BlitSurface(emptyHeart, NULL, screen, &heartPos);
        xChanged += 50;
    }
    TTF_CloseFont(font);
    SDL_FreeSurface(emptyHeart);
    SDL_FreeSurface(Heart);
}

void afficherPerso(personnage p, SDL_Surface *screen)
{
    TTF_Font *font = NULL;
    font = TTF_OpenFont("Retro.ttf", 24);

    SDL_Color textColor = {255, 255, 255};

    SDL_BlitSurface(p.animation.spriteSheet[p.direction], &p.animation.Clip[p.animation.ClipLoaded], screen, &p.pos);

    showText(screen, font, textColor, "PLayer 1", p.pos.x , p.pos.y);

    TTF_CloseFont(font);
}

void animerPerso(personnage *p, int *dt)
{
    int delay = 100;

    *dt += 10;

    if (*dt >= delay)
    {
        *dt = 0;

        if (p->animation.ClipLoaded >= p->animation.frames - 1)
            p->animation.ClipLoaded = 0;
        else
            p->animation.ClipLoaded++;
    }
}

void deplacerPerso(personnage *p, int *dt)
{
    if (p->direction == 1) 
    {
        p->pos.x += p->vitesse + p->acceleration;
    }
    else if (p->direction == 0) 
    {
        p->pos.x -= p->vitesse + p->acceleration;
    }

    animerPerso(p, dt);
}

void saut(personnage *p)
{
    if (p->up && p->onGround)
    {
        p->pos.y -= 200;
        p->onGround = 0;
    }

    if (!p->onGround)
    {
        p->pos.y += 10;
        p->up = 0;
    }

    if (p->pos.y >= 400)
    {
        p->onGround = 1;
        p->pos.y = 400;
    }
}
//chahd

void initEnnemi(Ennemi *e) {

    e->sprite=IMG_Load("enemie.png");
    e->pos.y=500;
    e->pos.x=800;
 
    e->direction=0;
    e->pos_sp.x=0;
    e->pos_sp.y=0;
    e->pos_sp.h=100;
    e->pos_sp.w=100;
}



void afficherEnnemi(Ennemi e,SDL_Surface *screen){

    SDL_BlitSurface(e.sprite , &e.pos_sp , screen , &e.pos) ;
}  


void deplacer(Ennemi *e){
    int Xmin =500 , Xmax= 800;
    if (e->direction==0)
    {
        e->pos.x +=40;
    }
    else if (e->direction==1)
    {
        e->pos.x -=40;
    }



    if (e->pos.x < Xmin)
    {
        e->direction = 0;
    }
    if (e->pos.x > Xmax)
    {
        e->direction = 1;
    }   
}


void animerEnnemi(Ennemi *e) {
    e->pos_sp.x += 100;
    if (e->pos_sp.x >= e->sprite->w) { 
        e->pos_sp.x = 0;
        e->pos_sp.y += e->pos_sp.h; 
        if (e->pos_sp.y >= e->sprite->h) { 
            e->pos_sp.y = 0;
        }
    } else if (e->pos_sp.x + e->pos_sp.w > e->sprite->w) { 
        e->pos_sp.x = 0;
        e->pos_sp.y += e->pos_sp.h; 
        if (e->pos_sp.y >= e->sprite->h) { 
            e->pos_sp.y = 0;
        }
    }
    SDL_Delay(10);
}


void playCollisionSound()
{
    static int isInitialized = 0;
    Mix_Chunk *collisionSound = NULL;
    int channel;

    
    if (!isInitialized) {
        Mix_Init(MIX_INIT_MP3);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
        isInitialized = 1;
    }

    
    collisionSound = Mix_LoadWAV("collision_sound.wav");

    
    channel = Mix_PlayChannel(-1, collisionSound, 0);

    
    while (Mix_Playing(channel)) {
        SDL_Delay(50);
    }

    
    Mix_FreeChunk(collisionSound);
}
int collision(SDL_Rect e, SDL_Rect p) {
    int collisionDetected = 0;
    if (((p.x + p.w) < e.x) || (p.x > (e.x + e.w)) || ((p.y + p.h) < e.y) || (p.y > (e.y + e.h))) {
       
        return 0;
    } else {
        if (!collisionDetected) {
            playCollisionSound();
            collisionDetected = 1;
        }
        return 1;
    }}


