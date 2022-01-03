#include "../include/classes.hpp"

sf::Texture Rock::tx;
sf::Texture Rock::txg;
//test
//! Rock
Rock::Rock() : sf::ConvexShape()
{
    // punktow bedzie pointsY
    int points = rand()%5 + 3;
    this->heal = ( ! (rand()%20) ) ? true : false;
    this->setPointCount(points);
    this->speed = rand()%7 + 1; // 1-7
    float angle = 360.f / points;
    // teksturka statyczna +
    if (! this->heal)
        this->setTexture(&Rock::tx);
    else
        this->setTexture(&Rock::txg);
    // generuje pozycje
    this->setPosition(sf::Vector2f(SCREENX + 150,rand()%(SCREENY-50) + 25));
    // generuje punkty figury (zblizone do kola, ale nie kolo)
    sf::Transform temp = sf::Transform::Identity; // transformacja do obrotu
    float cx = this->getPosition().x; // punkt wg. ktorego obracam (center x , center y)
    float cy = this->getPosition().y;
    temp.rotate(angle,cx,cy);
    this->setPoint(0,sf::Vector2f(cx,cy+100/(1+this->speed) ) );
    this->vertex.push_back (this->getPoint(0));
    for (int nr = 1; nr < points ; nr++)
    {
        this->setPoint(nr,temp.transformPoint(this->getPoint(nr-1)));
        this->vertex.push_back (this->getPoint(nr));
    }
    this->setOrigin(cx,cy);
    this->rotate(rand()%180);
    this->rot = rand()%5 - 2;
}

void Rock::update()
{
    // test na œsieræ
    if (this->hit)
    {
        if ( this->getPointCount()>3)
        {
            this->setPointCount(this->getPointCount()-1);
            this->vertex.pop_back();
            // odsuwa sie, zwalnia
            this->move(50,0);
            this->speed = this->speed * 1.1;
            this->direct = rand()%7-3;
        }
        else
            this->kill=true;
        this->hit=false;
    }
    // ruch
    this->setPosition(this->getPosition().x - this->speed, this->getPosition().y + this->direct); // ruch
    this->rotate(this->rot);
}

Rock::~Rock() {}
