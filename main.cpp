#include "screen.h"
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_video.h>
#include <cmath>
#include <cstdio>
#include <valarray>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480
SDL_Surface* scr;



class ColorRGB8bit {
    public:
        Uint8 r;
        Uint8 g;
        Uint8 b;
};

class ColorRGB {
    public:
        Uint8 red;
        Uint8 green;
        Uint8 blue;

        ColorRGB(Uint8 r, Uint8 g, Uint8 b);
        ColorRGB(const ColorRGB8bit& color);
        ColorRGB();

        friend ColorRGB operator/(const ColorRGB& color, float value);
};

ColorRGB::ColorRGB(Uint8 r, Uint8 g, Uint8 b) {
    this->red = r;
    this->green = g;
    this->blue = b;
};

ColorRGB::ColorRGB() {
    red = 0;
    green = 0;
    blue = 0;
}

ColorRGB operator/(const ColorRGB& color, float value) {
    Uint8 r = static_cast<Uint8>(color.red / value);
    Uint8 g = static_cast<Uint8>(color.green / value);
    Uint8 b = static_cast<Uint8>(color.blue / value);
    return ColorRGB(r, g, b);
}

const ColorRGB RGB_Red(255, 0, 0);
const ColorRGB RGB_Green(0, 255, 0);
const ColorRGB RGB_Blue(0, 0, 255);
const ColorRGB RGB_White(255, 255, 255);
const ColorRGB RGB_Yellow(255, 255, 0);

bool keyDown(int key) {
    if (!inkeys) return false;
    return (inkeys[key] != 0);
}

bool verLine(int x, int y1, int y2, const ColorRGB& color) {
    if (y2 < y1) {
        y1 += y2;
        y2 = y1 - y2;
        y1 -= y2;
    }

    if ( y2 < 0 || y1 >= screenHeight || x < 0 || x >= screenWidth) 
        return false;

    if (y1 < 0)
        y1 = 0;

    if (y2 >= screenWidth)
        y2 = screenHeight -1;

    Uint32 colorSDL = SDL_MapRGB(scr->format, color.red, color.green, color.blue);
    Uint32* bufp = (Uint32*)scr->pixels + y1 * scr->pitch / 4 + x;
    unsigned add = scr->pitch / 4;

    for (int y = y1; y <= y2; y++) {
        *bufp = colorSDL;
        bufp += add;
    }
    return true;
}

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main(int /*argc*/, char */*argv*/[])
{
  double posX = 22, posY = 12;  //x and y start position
  double dirX = -1, dirY = 0; //initial direction vector
  double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

  scr = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0, 0, 0, 0);

  double time = 0; //time of current frame
  double oldTime = 0; //time of previous frame 

  screen(screenWidth, screenHeight, 0, "Raycaster");

  while (!done()) {

      for (int x = 0; x < screenWidth; x++) {

          double cameraX = 2 * x / double(screenWidth) - 1;

          double rayDirX = dirX + planeX * cameraX;
          double rayDirY = dirY + planeY * cameraX;

          int mapX = static_cast<int>(posX);
          int mapY = static_cast<int>(posY);

          double sideDistX;
          double sideDistY;

          double deltaDistX = std::abs(1.0 / rayDirX);
          double deltaDistY = std::abs(1.0 / rayDirY);
          double perpWallDist;

          int stepX;
          int stepY;

          int hit = 0;
          int side;

          if (rayDirX < 0) {
              stepX = -1;
              sideDistX = (posX - mapX) * deltaDistX;
            }
          else {
              stepX = 1;
              sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
          if (rayDirY < 0) {
              stepY = -1;
              sideDistY = (posY - mapY) * deltaDistY;
          }
          else {
              stepY = 1;
              sideDistY = (mapY + 1.0 - posY) * deltaDistY;
          }

          while (hit == 0) {
              if (sideDistX < sideDistY) {
                  sideDistX += sideDistY;
                  mapX += stepX;
                  side = 0;
              }
              else {
                  sideDistY += deltaDistY;
                  mapY += stepY;
                  side = 1;
              }
              if (worldMap[mapX][mapY] > 0) hit = 1;
          
          }
          if (side == 0) perpWallDist = (sideDistX - deltaDistX);
          else           perpWallDist = (sideDistY - deltaDistY);

          int lineHeight = (int)(screenHeight / perpWallDist);

          int drawStart = -lineHeight / 2 + screenHeight / 2;
          if(drawStart < 0)drawStart = 0;
          int drawEnd = lineHeight / 2 + screenHeight / 2;
          if(drawEnd >= screenHeight)drawEnd = screenHeight -1;

          ColorRGB color;
          switch (worldMap[mapX][mapY]) {
              case 1: color = RGB_Red; break;
              case 2: color = RGB_Green; break;
              case 3: color = RGB_Blue; break;
              case 4: color = RGB_White; break;
              default: color = RGB_Yellow; break;
          }
            
          if (side == 1) {color = color / 2.0f;}

          verLine(x, drawStart, drawEnd, color);

          }

      oldTime = time;
      time = SDL_GetTicks();
      double frameTime = (time - oldTime) / 1000.0;
      SDL_UpdateRect(scr, 0, 0, 0, 0);
      SDL_FillRect(scr, NULL, 0);

      double moveSpeed = frameTime * 5.0;
      double rotSpeed = frameTime * 3.0;

      readKeys();
      if (keyDown(SDLK_UP)) {
          if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
          if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
        }
      if (keyDown(SDLK_DOWN)) {
          if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
          if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
        }

      if (keyDown(SDLK_RIGHT)) {
          double oldDirX = dirX;
          dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
          dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
          double oldPlaneX = planeX;
          planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
          planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }

      if(keyDown(SDLK_LEFT)) {
          double oldDirX = dirX;
          dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
          dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
          double oldPlaneX = planeX;
          planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
          planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
    } 
}
