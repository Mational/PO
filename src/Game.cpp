#include "../include/classes.hpp"

vector <Rock> Game::Rocks;  //konterner na skaly
vector <FirstAid> Game::FirstAids;
vector <InfiniteAmmo> Game::InfiniteAmmos;
sf::Clock Game::clock;  //zegar
sf::Clock Game::clock_inf;
sf::Vertex Game::va[1000];  //pylek
sf::Font Game::Font;    //czcionka

Game::Game(){}

Game::Game ( sf::String title ) : sf::RenderWindow(sf::VideoMode(SCREENX,SCREENY,CBITS),title)
{
    this->ending = false;
    this->setActive(true);   //ustawia okno jako aktywne

    this->setFramerateLimit( 75 );   //ustawia limit klatek na 75
    this->setVerticalSyncEnabled(true); //przytnie do monitora + moze byc przydatne, gdy sa artefakty z grafiki

    this->setPosition(sf::Vector2i(POSX,POSY));    //ustawia pozycje okna od lewego gornego rogu
    this->setKeyRepeatEnabled(false);     //ustawia autopowtarzanie eventu KeyPressed

    this->gg.setFont(Game::Font);   //czcionka napisu koncowego
    this->gg.setPosition(SCREENX/2.8,SCREENY/3);  //pozycja napisu koncowego
    this->gg.setCharacterSize(30); //rozmiar czcionki napisu koncowego
    this->gg.setFillColor(sf::Color(0xffffffff));   //kolor napisu koncowego
    this->gg.setStyle(sf::Text::Bold);  //styl napisu koncowego

    this->strT.setFont(Font);   //ustawia czcionke wyniku
    this->strT.setPosition(SCREENX*0.2,22); //pozycja wyniku
    this->strT.setCharacterSize(20);    //rozmiar czcionki wyniku
    this->strT.setFillColor(sf::Color(0xffffffff)); //kolor wyniku

    this->t = sf::seconds(10);  //czas gry ustawiony na 90 sekund
    this->inf = sf::seconds(0);

    srand(time(NULL));  //losowanie 1 z 3 backgroundow
    int los=rand()%3;
    if(los==0)
        this->bgtx.loadFromFile("images/kosmo1.png");
    else if(los==1)
        this->bgtx.loadFromFile("images/kosmo2.png");
    else
        this->bgtx.loadFromFile("images/kosmo3.png");

    this->bg.setTexture(&(this->bgtx)); //ustawia teksture tla
    this->bg.setPosition(0,0);  //pozycja tla
    this->bg.setSize(sf::Vector2f(SCREENX,SCREENY));    //rozmiar tla

    for (int i=0; i<1000; i++)  //[TEORETYCZNIE] tworzy 1000 gwiazdek na planszy
    {
        Game::va[i].position = sf::Vector2f(rand()%SCREENX,rand()%SCREENY);
        Game::va[i].color = sf::Color(100+rand()%155, 100+rand()%155, 100+rand()%155, 255);
    }
}

void Game::run()
{
    Game::Font.loadFromFile("fonts/OpenSans-Light.ttf");    //ladowanie czcionki
    Bullet::tx.loadFromFile("images/bullet2.png");  //ladowanie grafiki pociskow
    Rock::tx.loadFromFile("images/rock.png");   //ladowanie grafki skal zwyklych
    Rock::txg.loadFromFile("images/rockg.png"); //ladowanie grafiki skal leczacych
    FirstAid::tx.loadFromFile("images/first_aid.png"); //ladowanie grafiki apteczki
    InfiniteAmmo::tx.loadFromFile("images/interceptor.png"); //ladowanie grafiki nieskonczonej amunicji
    setSFMLWindow(this->getSystemHandle()); //nie wiem
    sf::Time start = Game::clock.getElapsedTime();  //czas ktory uplynal od startu albo restartu
    sf::Time tick = sf::microseconds(0);
    sf::Event e;

    Interceptor itor;   //tworzymy statek
    sf::Time tpause;    // zmienna pomocnicza do pauzy
    sf::Time infpause;
    unsigned steps=0;
    while(this->isOpen())
    {
        if(itor.hp>0 && this->t.asSeconds()<= 0)   //premia punktow (3*zdrowie) po wytrzymaniu calego czasu
        {
            itor.pt += itor.hp * 3; //dodanie punktow
            itor.hp = 0;    //ustawienie zycia statku na 0
            itor.destroy = false;   //ustawienie zmiennej destroy na false
        }

        while(this->pollEvent(e))  //jezeli pojawi sie zdarzenie
        {
            if (e.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ))
                this->close();  //jezeli zamkniemy lub nacisniemy ESC aplikacja sie zamknie
            if (ending==false && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P)
                this->pause = (this->pause) ? false : true; //p przelacza nam tryb pauzy
            if (!this->pause)
                itor.catchEvents(e, *this);    //jezeli nie ma pauzy pauza wylaczona statek zbiera eventy
        }

        if (this->pause)    //jezeli jest pauza
        {
            this->clear(sf::Color(0xffffffff));
            this->draw(this->bg); //tlo
            this->ss.str("");
            this->strT.setCharacterSize(25);    //rozmiar czcionki wyniku/menu
            this->strT.setString("[P] - Start/Pauza\n[W] - Ruch w gore\n[S] - Ruch w dol\n[SPACE] - Strzal\n[A] - Wolne pociski\n[D] - Szybkie pociski");
            this->draw(this->strT); //rysuj wynik/menu
            this->strT.setPosition(SCREENX/3,SCREENY/3);    //ustaw pozycje wyniku
            this->display();    //wyswietl klatke
            Game::clock.restart();  //restart zegara
            tpause = this->t;
            infpause = sf::seconds(6);
            continue;
        }

        // Losujemy kamien
        if (itor.hp > 0 &&  !(rand()%20) )
            Game::Rocks.push_back(Rock());  //dodajemy do kontenera skal skale

        if(itor.hp >0 && !(rand()%600))
            if(rand()%2)
                Game::FirstAids.push_back(FirstAid()); //dodajemy apteczke do kontenera apteczek
            else
                Game::InfiniteAmmos.push_back(InfiniteAmmo()); //dodajemy inf_amm do kontenera inf_amm

        this->clear(sf::Color(0xffffffff));
        this->draw(this->bg);   //rysujemy tlo

        // pyl gwiezdny
        this->draw(Game::va,1000,sf::PrimitiveType::Points);
        for (int i=0;i<1000;i++)
        {
            Game::va[i].position.x -= (i%3+1);
            if (Game::va[i].position.x <= 0)
                Game::va[i].position = sf::Vector2f(SCREENX,Game::va[i].position.y);
        }
        // leca glazy
        if (itor.hp>0)
        {
            Game::updateRocks(*this);   //aktualizuj skaly
            Game::updateFirstAids(*this);   //aktualizacja apteczki
            Game::updateInfiniteAmmo(*this);    //aktualizacja inf_amm
        }
        else
            this->gogg(&itor);  //wywolaj koniec gry

        //statek
        itor.paint(*this);  //rysuj statek

        // czas i liczba pociskow
        this->ss.str("");
        if (itor.hp > 0)
        {
            this->ss << "Czas do konca: " << (int) this->t.asSeconds() << " sek.\n";
            if(this->inf.asSeconds()>1)
                this->ss << "Nieskonczona amunicja: " << (int) this->inf.asSeconds() << " sek.\n";
            else if(itor.bullets.size()>itor.maxBullets)
                this->ss << "0/" << itor.maxBullets << " pociskow";
            else
                this->ss << itor.maxBullets-itor.bullets.size() << "/" << itor.maxBullets << " pociskow";
        }
        this->strT.setCharacterSize(20);
        this->strT.setStyle(sf::Text::Bold);
        this->strT.setString(this->ss.str());
        this->strT.setPosition(22,22);
        this->ss.str("");
        this->draw(this->strT);

        sf::sleep(sf::milliseconds(10));
        this->display();

        if(!this->pause)
        {
            this->t = tpause - Game::clock.getElapsedTime();
            if(this->inf.asSeconds() > 0)
            {
                this->inf = infpause - Game::clock_inf.getElapsedTime();
            }
        }

        tick = Game::clock.getElapsedTime() - start;
        if (tick.asMilliseconds() >= 1000)
        {
            start = Game::clock.getElapsedTime();
            steps=0;
        }
        steps++;
    }
}

void Game::startConsole()
{
    std::stringstream ss;
    ss << "konsola" << time(0);
    std::string s = ss.str();
    std::wstring stemp = std::wstring(s.begin(), s.end());
    LPCWSTR sw = stemp.c_str();
    HWND cw = GetConsoleWindow(); // okno konsoli przypisuje do cw dzieki bibliotece windows.h
    SetWindowText(cw, sw); // zmieniam tytul okna
    SetWindowPos(cw,HWND_TOPMOST,0,0,940,1080,SWP_NOMOVE); // always on top
}

void Game::setSFMLWindow(HWND gw)
{
    RECT rect;
    GetWindowRect(gw, &rect);
    SetWindowPos(gw,HWND_TOPMOST,rect.left,rect.top,rect.right,rect.bottom,SWP_NOMOVE);
    SetWindowLong(gw, GWL_EXSTYLE, GetWindowLong(gw, GWL_EXSTYLE) | WS_EX_LAYERED );
    SetLayeredWindowAttributes(gw,0x000000, 0xff, LWA_ALPHA);
}

// Czy punkt zawiera sie wewnatrz bloku wypuklych punktów ?
bool Game::isPointInsidePolygon(vector <sf::Vector2f> &points, const sf::Vector2f &vec)
{
    int is= -1;
    int n = points.size();
    float xi, yi, xj, yj, d;
    for (int i=0; i<n; i++)
    {
        xi = points[i].x;
        yi = points[i].y;
        xj = (i+1==n) ? points[0].x :  points[i+1].x;
        yj = (i+1==n) ? points[0].y :  points[i+1].y;
        d = (vec.x - xi)*(yj-yi) - (vec.y-yi)*(xj - xi);
        if (d==0)
            continue;
        else
        {
            if (is == -1)
                is = d > 0.0;
            else if ( (d > 0.0) != is )
                return false;
        }
    }
    return true;
}

// Czy punkt zawiera sie wewnatrz bloku wypuklych punktow ?
bool Game::isPointInsidePolygon(const sf::Transform &t, vector <sf::Vector2f> &points, const sf::Vector2f &vec)
{
    int is= -1;
    int n = points.size();
    float xi, yi, xj, yj, d;
    for (int i=0; i<n; i++)
    {
        sf::Vector2f point(t.transformPoint(points[i]));
        xi = point.x;
        yi = point.y;
        if (i+1 ==n)
        {
            sf::Vector2f point2(t.transformPoint(points[0]));
            xj = point2.x;
            yj = point2.y;
        }
        else
        {
            sf::Vector2f point2(t.transformPoint(points[i+1]));
            xj = point2.x;
            yj = point2.y;
        }

        d = (vec.x - xi)*(yj-yi) - (vec.y-yi)*(xj - xi);
        if (d==0)
            continue;
        else
        {
            if (is == -1)
                is = d > 0.0;
            else if ( (d > 0.0) != is )
                return false;
        }
    }
    return true;
}

void Game::updateRocks(Game &g)
{
    Rock *r;
    for (auto i = Game::Rocks.begin(); i!=Game::Rocks.end(); i++)
    {
        r = &(*i);
        if (r->kill || r->getPosition().x < 0 || r->getPosition().y < -50 || r->getPosition().y > SCREENY + 50) //znniszczona albo wyleciala poza ekran
        {
            Game::Rocks.erase(i);   //usun
            i--;
            continue;
        }
        r->update();    //jezeli nie zniszczona aktualizuj pozycje
        g.draw(*r); //rysuj skale
    }
}

void Game::updateFirstAids(Game &g)
{
    FirstAid *f;
    for(auto i = Game::FirstAids.begin(); i!=Game::FirstAids.end(); i++)
    {
        f = &(*i);
        if(f->kill || f->getPosition().x < 0) //znniszczona albo wyleciala poza ekran
        {
            Game::FirstAids.erase(i);   //usun
            i--;
            continue;
        }
        f->update();    //jezeli nie zniszczona aktualizuj pozycje
        g.draw(*f); //rysuj apteczke
    }
}

void Game::updateInfiniteAmmo(Game &g)
{
    InfiniteAmmo *f;
    for(auto i=Game::InfiniteAmmos.begin(); i!=Game::InfiniteAmmos.end(); i++)
    {
        f=&(*i);
        if(f->kill || f->getPosition().x<0)
        {
            Game::InfiniteAmmos.erase(i);
            i--;
            continue;
        }
        f->update();
        g.draw(*f);
    }
}

void Game::gogg(Interceptor *i)
{
    this->ending = true;
    this->ss << "Dziekujemy za gre!!!\nTwoj wynik: " << i->pt << " pkt.\n\n[R] - Graj Ponownie";
    this->gg.setString(this->ss.str());
    this->ss << "";
    this->strT.setFillColor(sf::Color(0x00000000));
    this->draw(this->gg);
    while(this->Rocks.size()>0) //kasowanie kamieni z planszy
        this->Rocks.pop_back();
}

void Game::restart(Game &g, Interceptor *i)
{
        i->hp = 100;    //restart hp statku
        i->maxBullets = 12; //restart maksymalnej liczby amunicji
        i->pt = 0;  //restart punktow

        while(i->bullets.size()>0)  //kasowanie pociskow z planszy
            i->bullets.pop_back();

        while(g.FirstAids.size()>0)
            g.FirstAids.pop_back();

        while(g.InfiniteAmmos.size()>0)
            g.InfiniteAmmos.pop_back();

        g.strT.setFillColor(sf::Color(0xffffffff)); //pokazanie wyniku
        g.clock.restart();  //restart zegara gry

        i->setPosition(i->skin.getSize().x*2,SCREENY/2);  //restart pozycji statku
        i->rotate(-(i->getRotation())); //restartowanie rotacji statku
}

Game::~Game(){}
