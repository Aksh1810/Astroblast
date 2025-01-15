#include <SFML/Graphics.hpp>
#include "game.h"

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4, t5, t6, t7;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);

    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);

    std::list<Entity *> entities;

    player *p = nullptr;
    int score = 0;
    bool isGameOver = false;

    resetGame(entities, p, score, isGameOver, sRock, sPlayer);

    Font font;
    font.loadFromFile("arial.ttf");

    Text scoreText, gameOverText, restartText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(W / 2 - 150, H / 2 - 100);

    restartText.setFont(font);
    restartText.setCharacterSize(30);
    restartText.setFillColor(Color::White);
    restartText.setString("Press 'R' to Restart");
    restartText.setPosition(W / 2 - 150, H / 2 + 20);

    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (isGameOver && event.type == Event::KeyPressed && event.key.code == Keyboard::R)
            {
                resetGame(entities, p, score, isGameOver, sRock, sPlayer);
            }

            if (!isGameOver && event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                bullet *b = new bullet();
                b->settings(sBullet, p->x, p->y, p->angle, 10);
                entities.push_back(b);
            }
        }

        if (!isGameOver)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
                p->angle += 3;
            if (Keyboard::isKeyPressed(Keyboard::Left))
                p->angle -= 3;
            if (Keyboard::isKeyPressed(Keyboard::Up))
                p->thrust = true;
            else
                p->thrust = false;

            for (auto a : entities)
                for (auto b : entities)
                {
                    if (a->name == "asteroid" && b->name == "bullet")
                        if (isCollide(a, b))
                        {
                            a->life = false;
                            b->life = false;

                            score += (a->R > 15) ? 10 : 5;

                            Entity *e = new Entity();
                            e->settings(sExplosion, a->x, a->y);
                            e->name = "explosion";
                            entities.push_back(e);

                            for (int i = 0; i < 2; i++)
                            {
                                if (a->R == 15)
                                    continue;
                                Entity *e = new asteroid();
                                e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                                entities.push_back(e);
                            }
                        }

                    if (a->name == "player" && b->name == "asteroid")
                        if (isCollide(a, b))
                        {
                            b->life = false;

                            Entity *e = new Entity();
                            e->settings(sExplosion_ship, a->x, a->y);
                            e->name = "explosion";
                            entities.push_back(e);

                            isGameOver = true;
                        }
                }

            if (p->thrust)
                p->anim = sPlayer_go;
            else
                p->anim = sPlayer;

            for (auto e : entities)
                if (e->name == "explosion")
                    if (e->anim.isEnd())
                        e->life = 0;

            if (rand() % 150 == 0)
            {
                asteroid *a = new asteroid();
                a->settings(sRock, 0, rand() % H, rand() % 360, 25);
                entities.push_back(a);
            }

            for (auto i = entities.begin(); i != entities.end();)
            {
                Entity *e = *i;

                e->update();
                e->anim.update();

                if (e->life == false)
                {
                    i = entities.erase(i);
                    delete e;
                }
                else
                    i++;
            }
        }

        app.clear();

        // Draw background
        app.draw(background);

        // Draw entities
        for (auto i : entities)
            i->draw(app);

        // Display score
        scoreText.setString("Score: " + std::to_string(score));
        app.draw(scoreText);

        // Display game over text
        if (isGameOver)
        {
            app.draw(gameOverText);
            app.draw(restartText);
        }

        app.display();
    }

    return 0;
}