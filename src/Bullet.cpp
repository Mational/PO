#include "../include/classes.hpp"

// BULLET
unsigned int Bullet::xy=10; //srednica pocisku
sf::Texture Bullet::tx;

Bullet::Bullet(const sf::Vector2f &vec,Interceptor *inter) : sf::CircleShape()
{
    this->setRadius(Bullet::xy);    //ustawienie promienia pocisku
    this->setOrigin(Bullet::xy,Bullet::xy); //ustawia origin pocisku
    this->setTexture(&Bullet::tx);  //ustawia teksture pocisku
    this->setTextureRect(sf::IntRect(0,0,Bullet::xy,Bullet::xy)); //ustawienie prostokata tekstury (x, y, width, height)
    this->frames = Bullet::tx.getSize().y / Bullet::xy ;    //ustawienie liczby klatek (180/10) - rozne kolory pocisku 180 = dlugosc obrazka bullet2
    this->setPosition(vec.x,vec.y); //poczatkowa pozycja pocisku
    this->interceptor = inter;  //statek wlasciciel
    this->setScale(0.5f,0.5f);  //przeskalowanie pocisku o 50% w dol
}

void Bullet::update(Game &theGame)
{
    int tempSpeed = this->speed ;  // przesuwam pocisk, ale nie od razu o caly SPEED, ale po troche!
    if (theGame.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::D))   //jezeli aktywny ekran i nacisnieto D
    {
        for (int i=1; i<=tempSpeed * 2; i++) //przesuwam pocisk co piksel aby nie ominac malego Rock'a; predkosc 2 razy wieksza
        {
            this->move(1, 0);   //przesuwanie pocisku o 1 piksel
            if (this->collTest())   //sprawdzanie czy nie ma kolizji
                break; //jak wszedl w kolizje szybciej, nie przechodze do konca
        }
    }
    else if (theGame.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //spowolnienie i sinusoidalny ruch pociskow
    {
        this->move(this->speed-2, rand()%5-2 ); //predkosc 1 po osi X i losowa wartosc po osi Y
        this->collTest();   //test kolizji
    }
    else
    {
        this->move(this->speed,0.f);    //zwykly strzal - 3 po osi X i 0 po osi Y
        this->collTest();   //test kolizji
    }

    //animacja
    int top = (this->getTextureRect().top + Bullet::xy)%(this->frames * Bullet::xy);    //ustawienie kolejnej klatki animacji (obecny Y + srednica pocisku)%180 - zeby sie zapetlalo
    this->setTextureRect(sf::IntRect( 0, top, Bullet::xy,Bullet::xy )); //ustawienie nowej tekstury pocisku
}

// test kolizji pocisku z wszystkimi glazami
bool Bullet::collTest()
{
    for (auto i = Game::Rocks.begin() ; i!= Game::Rocks.end(); i++) //sprawdzenie kazdej skaly po kolei
    {
        Rock *r = &(*i);    //tworzenie wskaznika na skale
        // kolizja o ile w ogóle zazebiaja sie prostokaty
        if (this->getGlobalBounds().intersects(r->getGlobalBounds()) &&
            Game::isPointInsidePolygon(r->getTransform(), r->vertex, this->getPosition() )) //jezeli pocisk jest w skale albo wypadl za granice okna
        {
            r->hit = true;  //ustaw hit na true
            this->kill = true; //srodek pocisku wewnatrz glazu ten glaz i ten pocisk do usuniecia
            if(this->interceptor->hp>0)
                this->interceptor->pt += (r->speed>3) ? 2 : 1; //punkty do gory; liczba punktow zalezy od predkosci glazu
            return true;    //zwroc, ze byla kolizja
        }
    }
    return false;   //zwor, ze nie bylo kolizji
}

Bullet::~Bullet(){}
