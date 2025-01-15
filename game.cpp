#include "game.h"
#include <cmath>
#include <ctime>

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

Animation::Animation() {}

Animation::Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
{
    Frame = 0;
    speed = Speed;

    for (int i = 0; i < count; i++)
        frames.push_back(IntRect(x + i * w, y, w, h));

    sprite.setTexture(t);
    sprite.setOrigin(w / 2, h / 2);
    sprite.setTextureRect(frames[0]);
}

void Animation::update()
{
    Frame += speed;
    int n = frames.size();
    if (Frame >= n)
        Frame -= n;
    if (n > 0)
        sprite.setTextureRect(frames[int(Frame)]);
}

bool Animation::isEnd()
{
    return Frame + speed >= frames.size();
}

Entity::Entity()
{
    life = 1;
}

void Entity::settings(Animation &a, int X, int Y, float Angle, int radius)
{
    anim = a;
    x = X;
    y = Y;
    angle = Angle;
    R = radius;
}

void Entity::update() {}

void Entity::draw(RenderWindow &app)
{
    anim.sprite.setPosition(x, y);
    anim.sprite.setRotation(angle + 90);
    app.draw(anim.sprite);
}

Entity::~Entity() {}

asteroid::asteroid()
{
    dx = rand() % 8 - 4;
    dy = rand() % 8 - 4;
    name = "asteroid";
}

void asteroid::update()
{
    x += dx;
    y += dy;

    if (x > W)
        x = 0;
    if (x < 0)
        x = W;
    if (y > H)
        y = 0;
    if (y < 0)
        y = H;
}

bullet::bullet()
{
    name = "bullet";
}

void bullet::update()
{
    dx = cos(angle * DEGTORAD) * 6;
    dy = sin(angle * DEGTORAD) * 6;

    x += dx;
    y += dy;

    if (x > W || x < 0 || y > H || y < 0)
        life = 0;
}

player::player()
{
    name = "player";
}

void player::update()
{
    if (thrust)
    {
        dx += cos(angle * DEGTORAD) * 0.2;
        dy += sin(angle * DEGTORAD) * 0.2;
    }
    else
    {
        dx *= 0.99;
        dy *= 0.99;
    }

    int maxSpeed = 15;
    float speed = sqrt(dx * dx + dy * dy);
    if (speed > maxSpeed)
    {
        dx *= maxSpeed / speed;
        dy *= maxSpeed / speed;
    }

    x += dx;
    y += dy;

    if (x > W)
        x = 0;
    if (x < 0)
        x = W;
    if (y > H)
        y = 0;
    if (y < 0)
        y = H;
}

bool isCollide(Entity *a, Entity *b)
{
    return (b->x - a->x) * (b->x - a->x) +
               (b->y - a->y) * (b->y - a->y) <
           (a->R + b->R) * (a->R + b->R);
}

void resetGame(std::list<Entity *> &entities, player *&p, int &score, bool &isGameOver, Animation &sRock, Animation &sPlayer)
{
    entities.clear();
    score = 0;
    isGameOver = false;

    for (int i = 0; i < 15; i++)
    {
        asteroid *a = new asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }

    p = new player();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);
}