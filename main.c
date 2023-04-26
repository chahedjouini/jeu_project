
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>
#include "jeu.h"
#include "fonction.h"

int main()
{
    SDL_Surface *screen = NULL;
    SDL_Surface *background;
    
    Ennemi e;
    personnage p;
    int t_curr, dt = 0;
    TTF_Init();
    
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(1300, 800, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    initPerso(&p);
    initEnnemi(&e);
    background = IMG_Load("images/background.png");

    int prev_time = SDL_GetTicks() / 1000;
    int quit = 1;
    int is_shift_pressed = 0;
    
    while (quit)
    {
        p.score = SDL_GetTicks() / 1000 - prev_time;

        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                p.up = 1;
                break;
            case SDLK_RIGHT:
                p.direction = 1;
                deplacerPerso(&p, &dt);
                break;
            case SDLK_LEFT:
                p.direction = 0;
                deplacerPerso(&p, &dt);
                break;
            case SDLK_KP_MINUS:
                p.vie = 1;
                break;
            case SDLK_KP_PLUS:
                p.vie = 3;
                break;
            case SDLK_LSHIFT:
                is_shift_pressed = 1;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_LSHIFT:
                is_shift_pressed = 0;
                break;
            }
            break;
        }

        saut(&p);

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(background, NULL, screen, NULL);
        afficherPerso(p, screen);
        afficherPersoScore(p, screen);
        afficherEnnemi(e, screen);
        deplacer(&e);
        animerEnnemi(&e);
        
        if (collision(e.pos, p.pos))
        {
            playCollisionSound();
            p.vie-=1;
            p.score-=1;
              
 
        }
        if(p.vie==0 || p.score<0)
            {quit =0;}
           SDL_Flip(screen);
           SDL_Delay(50);
        
    }

    SDL_FreeSurface(background);
    SDL_FreeSurface(p.animation.spriteSheet[1]);
    SDL_FreeSurface(p.animation.spriteSheet[0]);
    SDL_FreeSurface(screen);

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
