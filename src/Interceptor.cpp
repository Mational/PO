#include "../include/classes.hpp"

// INTERCEPTOR
Interceptor::Interceptor() : sf::Sprite()
{
    //tekstura
    this->skin.loadFromFile("images/ufo.png");  //laduje grafike do zmiennej skin typu sf::Image
    for(unsigned int x=0; x<this->skin.getSize().x ; x++)   //przejscie przez wszystkie kolumny grafiki
    {
        for (unsigned int y=0; y < this->skin.getSize().y ; y++)    //przejscie przez wszystkie wiersze danej kolumny w grafice
        {
            if (this->skin.getPixel(x,y) == sf::Color(255,0,0)) //jezeli dany piksel jest idealnie czerwony
            {
                this->localCollisionPoints.push_back(sf::Vector2i(x,y));    //dodaj go do kontenera kolizji
                this->skin.setPixel(x,y,sf::Color(0xffffff00)); //usun czerwony piksel
            }
        }
    }
    this->skinTx.loadFromImage(this->skin,sf::IntRect(0,0,this->skin.getSize().x,this->skin.getSize().y));  //jako teksture ladujemy caly zmieniony obraz
    this->setTexture(this->skinTx); //laduje testure
    this->setOrigin(this->skin.getSize().x/2,this->skin.getSize().y/2); //ustawia srodek ciezkosci statku
    this->setPosition(this->skin.getSize().x*2,SCREENY/2);  //ustawia pozycje poczatkowa statku (2*dlugosc, polowa ekranu)
    this->setTextureRect(sf::IntRect(0,0,this->skin.getSize().x,this->skin.getSize().y));   //ustawia TextureRect jako cala grafika

    //pasek zdrowia
    this->hpRect.setSize(sf::Vector2f(this->hpRectX * this->hp / this->hpmax * 1.f,3.f));   //ustawia rozmiar paska zdrowia (x,y)
    this->hpRect.setOrigin(this->skin.getSize().x/2,2.5f);  //ustawia srodek ciezkosci paska zdrowia
    this->hpRect.setFillColor(sf::Color(0xff0000ff));   //ustawia kolor paska zdrowia na czerwony
    this->hpRect.setPosition(this->getPosition().x, this->getPosition().y-this->skin.getSize().y/2);  //ustawia pasek zdrowia nad statkiem
    this->hpRect.setOutlineColor(sf::Color(0x000000ff));    //ustawia czarna obramowke
    this->hpRect.setOutlineThickness(1);    //rozmiar obramowki = 1px

    //wynik
    this->ss << this->pt << " punktow";
    this->strPt.setFont(Game::Font);    //font wyniku
    this->strPt.setCharacterSize(16);   //rozmiar czcionki
    this->strPt.setPosition(SCREENX-200,SCREENY-20);    //ustawiamy wynik w prawym dolnym rogu ekranu
    this->strPt.setString(ss.str());    //podstawiamy ss za wynik
    this->strPt.setFillColor(sf::Color(0xffff00ff));    //ustawiamy kolor czcionki wyniku
    ss.str("");

    //zycie
    this->ss << this->hp << " hp";
    this->strHp.setFont(Game::Font);    //font zycia
    this->strHp.setCharacterSize(20);   //rozmiar czcionki
    this->strHp.setPosition(SCREENX-200,SCREENY-42);    //ustawiamy zycie nad wynikiem
    this->strHp.setString(ss.str());    //podstawiamy ss za zycie
    this->strHp.setFillColor(sf::Color(0xff0000ff));    //kolor czcionki zycia
    this->strHp.setStyle(sf::Text::Bold);   //pogrubienie zycia
    ss.str("");
}

// rysowanie
void Interceptor::paint (Game &game)
{
    this->update(game); //wywolujemy update
    game.draw(*this);   //rysuje statek
    if (this->hp>0) //jezeli zycie>0
    {
        if(this->hp>50)
            this->hpRect.setFillColor(sf::Color(0x00ff00ff));
        else if(this->hp>25 && this->hp <=50)
            this->hpRect.setFillColor(sf::Color(255,153,51,255));
        else if(this->hp<=25)
            this->hpRect.setFillColor(sf::Color(0xff0000ff));
        game.draw(this->hpRect);    //rysujemy prostokat zycia
    }

    for (auto i=this->bullets.begin(); i!=this->bullets.end(); i++) //dla wszystkich wystrzelonych pociskow
        game.draw(*i);  //rysuj pocisk

    game.draw(this->strPt); //rysuj wynik
    game.draw(this->strHp); //rysuj zycie
    if(this->speed>0.1f || this->speed<-0.1f)   //jezeli statek sie porusza
        game.draw(this->smoke,100,sf::PrimitiveType::Points);   //rysuj dym
}

// event
void Interceptor::catchEvents(sf::Event &e, Game &g)
{
    if (this->hp<=0)    //jezeli zginelismy
    {
        if(e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::R)
            g.restart(g, this);
    }
    if (e.type==sf::Event::KeyPressed && (e.key.code==sf::Keyboard::Up || e.key.code==sf::Keyboard::W)) //jezeli strzalka w gore lub W
        this->speed =  - this->boostSpeed;  //zmieniamy predkosc na -4
    if (e.type==sf::Event::KeyPressed && (e.key.code==sf::Keyboard::Down || e.key.code==sf::Keyboard::S))   //jezeli strzalka w dol lub S
        this->speed =  this->boostSpeed;    //zmieniamy predkosc na 4
    if (e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::Space)   //jezeli spacja
        this->shoot(g);  //strzal
}

//update
void Interceptor::update(Game &theGame)
{
    //aktualizacja wyniku
    this->ss << this->pt << " punktow";
    this->strPt.setString(this->ss.str());
    this->ss.str("");
    //aktualizacja zycia
    this->ss << this->hp << " hp";
    this->strHp.setString(this->ss.str());
    this->ss.str("");

    if (this->hp<=0 && this->destroy ) //jezeli zginelismy
    {
        this->move(rand()%3-1,rand()%3-1); //wiec sie tylko telepie
        this->rotate(rand()%7-2);   //obracamy statkiem
        this->ss << "";
        this->strHp.setString(this->ss.str());  //usuwamy zycie
        this->strPt.setString(this->ss.str());  //usuwamy wynik
    }
    else //jezeli zyjemy
    {
        //po ruchu test kolizji ze scianami
        if(this->getGlobalBounds().top+this->getGlobalBounds().height >= SCREENY)   //dolna sciana
            this->speed *= -1.5;    //odbijamy sie od sciany w przeciwnym kierunku
        if(this->getGlobalBounds().top <= 0)    //gorna sciana
            this->speed *= -1.5;    //odbijamy sie od sciany w przeciwnym kierunku

        //smoke generate
        if(this->speed>0.1f || this->speed<-0.1f)
        {
            for(int i=0; i<100; i++)
            {
                this->smoke[i].position = sf::Vector2f(this->getPosition().x-60-(rand()%21-10)-i, (rand()%(i+1)-i/2) + this->getPosition().y+15); // losujemy pozycje za statkiem
                this->smoke[i].color=sf::Color(100+rand()%155,100+rand()%155,255,255);  //losujemy kolor
            }
        }

        //zmiana predkosci  podczas ruchu
        if (this->speed>0)
            this->speed=std::max(0.f,this->speed-(this->boostSpeed/this->speedCooldown));
        else if(speed<0)
            this->speed=std::min(0.f,this->speed+(this->boostSpeed/this->speedCooldown));

        this->setPosition(this->getPosition().x, this->getPosition().y + this->speed); //zmiana pozycji o predkosc
        this->hpRect.setPosition(this->getPosition().x, this->getPosition().y-this->skin.getSize().y/2);    //ustawianie pozycji paska zdrowia
        this->hpRect.setSize(sf::Vector2f(this->hpRectX * this->hp / this->hpmax * 1.f, 3.f));  //ustawianie rozmiaru paska zdrowia
    }
    //update pociskow
    if(this->bullets.size())
    {
        for(auto i=this->bullets.begin(); i!=this->bullets.end(); i++)  //dla wszystkich wystrzelonych pociskow
            (*i).update(theGame); //update pociskow
    }
    //usuwanie pociskow za ekranem
    Bullet *p;  //wskaznik typu Bullet
    if(this->bullets.size())
    {
        for (auto i=this->bullets.begin(); i!=this->bullets.end(); i++)
        {
            p = &(*i);
            if(p->kill || p->getPosition().x > SCREENX + Bullet::xy*2) //pocisk rozbity lub wyleciala za ekran
                this->bullets.erase(i);      //usuwamy pocisk z kontenera pociskow
            break;
        }
    }
    //testy kolizji dla kazdego punktu "spisanego" z ekranu
    if(this->hp>0)  //jezeli zyjemy
    {
        Rock *r;    //wskaznik typu Rock
        for (auto i=Game::Rocks.begin() ; i!=Game::Rocks.end(); i++)    //dla wszystkich skal na ekranie
        {
            r = &(*i);
            if (r->getGlobalBounds().left <= this->getGlobalBounds().left + this->getGlobalBounds().width &&
                this->getGlobalBounds().intersects(r->getGlobalBounds()))  //jezeli prostokaty sie nachodza sprawdzamy dalej
            {
                bool coll=false;
                for (auto v=this->localCollisionPoints.begin(); v!=this->localCollisionPoints.end(); v++)   //dla wszystkich punktow kolizji na statku
                {
                    sf::Vector2f p(*v); //nie wiem
                    float x = this->skin.getSize().x/2; //sradek w osi X statku
                    float y = this->skin.getSize().y/2; //srodek w osi Y statku
                    if (Game::isPointInsidePolygon(r->getTransform(),r->vertex,
                        sf::Vector2f( this->getPosition().x - x + p.x, this->getPosition().y - y + p.y)))   //jezeli wewnatrz
                    {
                        if (r->heal)  //lecze i zwiekszam ilosc pociskow
                        {
                            r->kill = true; //zniszcz skale
                            this->hp = std::min(this->hpmax, this->hp + (int)(r->getPointCount()*2)); //heal
                            coll = true;    //byla kolizja
                            this->maxBullets += 4;  //zwieksz maksymalna liczbe pociskow
                            break;
                        }
                        else
                        {
                            r->kill=true;   //zniszcz skale
                            this->hp -= r->getPointCount()*2; //damage
                            coll = true;    //byla kolizja
                            if(this->hp<=0)  //jezeli zycie <= 0
                                this->destroy = true;   //ustaw destroy na true
                            break;
                        }
                    }
                }
                if(coll)
                    break;
            }
        }

        FirstAid *f;    //wskaznik typu FirstAid
        for (auto i=Game::FirstAids.begin(); i!=Game::FirstAids.end(); i++)    //dla wszystkich skal na ekranie
        {
            f = &(*i);
            if (f->getGlobalBounds().left <= this->getGlobalBounds().left + this->getGlobalBounds().width &&
                this->getGlobalBounds().intersects(f->getGlobalBounds()))  //jezeli prostokaty sie nachodza sprawdzamy dalej
            {
                f->kill = true; //zniszcz apteczke
                if(this->hp + (int)(this->hpmax*0.3)<100)
                    this->hp=this->hp+(int)(this->hpmax*0.3);
                else
                    this->hp=100;
            }
        }

        InfiniteAmmo *a;
        for(auto i=Game::InfiniteAmmos.begin(); i!=Game::InfiniteAmmos.end(); i++)
        {
            a=&(*i);
            if(a->getGlobalBounds().left <= this->getGlobalBounds().left + this->getGlobalBounds().width &&
               this->getGlobalBounds().intersects(a->getGlobalBounds()))
            {
                a->kill=true;
                theGame.inf = sf::seconds(5);
                theGame.clock_inf.restart();
            }
        }
    }
}

//strzal
void Interceptor::shoot(Game &theGame)
{
    if(this->bullets.size()>=this->maxBullets && theGame.inf.asSeconds() < 1)
        return;
    this->bullets.push_back(Bullet(sf::Vector2f(this->getPosition().x + this->getTexture()->getSize().x/2 - 4 ,
                                                this->getPosition().y + 10.f),this) );  //tworzymy pocisk w odleglosci od centrum statku
}

Interceptor::~Interceptor()
{
}
