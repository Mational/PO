#include "../include/classes.hpp"

sf::Texture InfiniteAmmo::tx;

InfiniteAmmo::InfiniteAmmo() : sf::RectangleShape()
{
    this->setTexture(&InfiniteAmmo::tx);
    this->setSize(sf::Vector2f(30.f,30.f));
    this->setOrigin(this->getSize().x/2,this->getSize().y/2); //ustawia srodek ciezkosci apteczki
    this->setPosition(sf::Vector2f(SCREENX + 150,rand()%(SCREENY-50) + 25));    //generuje pozycje poczatkowa apteczki
    this->setTextureRect(sf::IntRect(0,0, this->tx.getSize().x, this->tx.getSize().y));

    this->speed = rand()%7 + 1; //losowanie 1 z 7 poziomow predkosci apteczki
    this->rot=rand()%5-2;   //losowanie wartosci rotacji apteczki podczas lotu
}

void InfiniteAmmo::update()
{
    //ruch
    this->setPosition(this->getPosition().x - this->speed, this->getPosition().y); //przemieszcza apteczke o wartosc predkosci i kierunku
    this->rotate(this->rot);    //rotuje apteczke o dany kat
}

InfiniteAmmo::~InfiniteAmmo() {}
