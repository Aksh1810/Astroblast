#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <string>

using namespace sf;

extern const int W;
extern const int H;

extern float DEGTORAD;

class Animation
{
public:
    float Frame, speed;
    Sprite sprite;
    std::vector<IntRect> frames;

    Animation();
    Animation(Texture &t, int x, int y, int w, int h, int count, float Speed);

    void update();
    bool isEnd();
};

class Entity
{
public:
    float x, y, dx, dy, R, angle;
    bool life;
    std::string name;
    Animation anim;

    Entity();
    void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1);
    virtual void update();
    void draw(RenderWindow &app);
    virtual ~Entity();
};

class asteroid : public Entity
{
public:
    asteroid();
    void update();
};

class bullet : public Entity
{
public:
    bullet();
    void update();
};

class player : public Entity
{
public:
    bool thrust;

    player();
    void update();
};

bool isCollide(Entity *a, Entity *b);
void resetGame(std::list<Entity *> &entities, player *&p, int &score, bool &isGameOver, Animation &sRock, Animation &sPlayer);

#endif