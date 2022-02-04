#include "../include/classes.hpp"

sf::Texture Rock::tx;
sf::Texture Rock::txg;

Rock::Rock() : sf::ConvexShape()
{
    int points = rand()%5 + 3;  //losowanie liczby wierzcholkow skaly
    this->heal = ( ! (rand()%20) ) ? true : false;  //1 na 20 leczy
    this->setPointCount(points);    //ustawianie liczby wierzcholkow skaly
    this->speed = rand()%7 + 1; //losowanie 1 z 7 poziomow predkosci skaly
    float angle = 360.f / points;   //obliczanie kata nachylenia skaly
    if (! this->heal)
        this->setTexture(&Rock::tx);    //tekstura zwykla
    else
        this->setTexture(&Rock::txg);   //tekstura leczacej

    //generowanie ksztaltu podobnego do kola, ale nie kola
    this->setPosition(sf::Vector2f(SCREENX + 150,rand()%(SCREENY-50) + 25));    //generuje pozycje poczatkowa skaly
    sf::Transform temp = sf::Transform::Identity;   //transformacja do obrotu
    float cx = this->getPosition().x;   //punkt wg. ktorego obracam (center x)
    float cy = this->getPosition().y;   //punkt wg. ktorego obracam (center y)
    temp.rotate(angle,cx,cy);   //obracamy skale o dany kat wedlug punkty (x,y)
    this->setPoint(0, sf::Vector2f(cx,cy+100/(1+this->speed))); //ustawianie punktu skaly
    this->vertex.push_back(this->getPoint(0));  //uzupelnianie kontenera wierzcholkow pierwszym wierzcholkiem
    for(int nr=1; nr<points; nr++)
    {
        this->setPoint(nr, temp.transformPoint(this->getPoint(nr-1)));  //generuje nowy wierzcholek na podstawie poprzedniego
        this->vertex.push_back(this->getPoint(nr)); //dodaje wierzcholek do kontenera wierzcholkow
    }
    this->setOrigin(cx,cy); //ustawia skale na poczatkowej pozycji
    this->rotate(rand()%180);   //obraca skale o losowy kat z zakreu 0-179
    this->rot=rand()%5-2;   //losowanie wartosci rotacji skaly podczas lotu
}

void Rock::update()
{
    //sprawdzanie strzalu
    if (this->hit)  //jezeli skala zostanie trafiona
    {
        if ( this->getPointCount()>3)   //jezeli skala ma wiecej niz 3 wierzcholki
        {
            this->setPointCount(this->getPointCount()-1);   //zmniejsza liczbe wierzcholkow o 1
            this->vertex.pop_back();    //usuwa wierzcholek z kontenera wierzcholkow
            this->move(50,0);   //skala odsuwa sie o 50 pikseli w osi X i 0 w osi Y
            this->speed = this->speed * 1.1;    //skala przyspiesza o 10%
            this->direct = rand()%7-3;  //ustawia nowy kierunek ruchu (skos)
        }
        else    //jezeli skala ma 3 wierzcholki lub mniej
            this->kill=true;    //ustaw kill na true
        this->hit=false;    //ustaw hit na false
    }
    // ruch
    this->setPosition(this->getPosition().x - this->speed, this->getPosition().y + this->direct); //przemieszcza skale o wartosc predkosci i kierunku
    this->rotate(this->rot);    //rotuje skale o dany kat
}

Rock::~Rock() {}
