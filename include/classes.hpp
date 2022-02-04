#define FRAME_LIMIT 60
#define POSX 0
#define POSY 0
#define SCREENX 1120
#define SCREENY 630
#define CBITS 32
#define PI 3.14159265358979323846
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;

class Rock;
class Interceptor;
class Game;
class Bullet;
class FirstAid;
class InfiniteAmmo;

class Game : public sf::RenderWindow
{
    public:
        static sf::Clock clock;                                 // zegar gry
        static sf::Clock clock_inf;
        static vector <Rock> Rocks;                             // Rock kontener na przeszkody
        static vector<FirstAid> FirstAids;                      //FirstAid kontener na apteczki
        static vector<InfiniteAmmo> InfiniteAmmos;             //InfiniteAmmo kontener na apteczki
        static sf::Vertex va[1000];                             // gwiazdeczki
        static sf::Font Font;                                   // Czcionka gry
        sf::Text gg;                                            // napis koncowy
        sf::Text strT;                                          // uplyw czasu
        sf::Texture bgtx;                                       // Tlo
        sf::RectangleShape bg;
        bool ending=false;
        bool pause=true;                                        // pauza
        stringstream ss;                                        // bufor
        sf::Time t;                                             // maksymalny czas na gre
        sf::Time inf;

        Game();                                                 // ustawia wlasciwosci glownego okna z gra
        Game(sf::String);                                       // konstruktor param tytul okna
        void run();                                             // uruchamia cala gre
        void gogg(Interceptor *);                               // odpala tryb Game Over
        static void startConsole();                             // obsluguje konsole - statyczna
        static void setSFMLWindow(HWND gw);                     // pobiera uchwyt do okna i pozwala obsluzyc okno z wykorzystaniem biblioteki window.h od Microsoft
        static bool isPointInsidePolygon(vector <sf::Vector2f> &, const sf::Vector2f&);
        static bool isPointInsidePolygon(const sf::Transform&, vector <sf::Vector2f> &, const sf::Vector2f&);
        static void updateRocks(Game &);
        static void updateFirstAids(Game &);
        static void updateInfiniteAmmo(Game &);
        static void restart(Game &, Interceptor *);
        virtual ~Game();
};

class Bullet : public sf::CircleShape
{
    public:
        static sf::Texture tx;                                  //statyczna tekstura
        static unsigned int xy;                                 //rozmiary klatki animacji
        unsigned int frames=6*3;                                //klatek animacji
        float speed = 3;                                        //predkosc standardowa
        Interceptor *interceptor;                               //statek wlasciciel
        bool kill = false;                                      //czy usuwac pocisk

        Bullet(const sf::Vector2f &,Interceptor*);
        void update(Game &);                                    //nowa pozycja, zmiana animacji
        bool collTest();                                        //test kolizji dla obecnej pozycji
        virtual ~Bullet();
};

// Interceptor
class Interceptor : public sf::Sprite
{
    public:
        float speed = 0;                                        //predkosc (po osi Y)
        float boostSpeed=4;                                     //o ile podnosze predkosc podczas "skoku"
        float speedCooldown = 50;                               //stopien zwalniania przyspieszenia  (wiecej, wolniej zwalnia)
        sf::Image skin;                                         //obrazek do tekstury
        sf::Texture skinTx;                                     //tekstura
        std::vector <sf::Vector2i>  localCollisionPoints;       //punkty (piksele) ktore wchodza w kolizje
        int hp=100;                                             //HP
        int hpmax = 100;                                        //maks HP
        sf::RectangleShape hpRect;                              //pasek zdrowia
        float hpRectX = 100;                                    //dlugisc paska zdrowia
        vector <Bullet> bullets;                                //kontener z pociskami
        unsigned int maxBullets = 12;                                    //maksymalnie mozesz wystrzelic
        sf::Text strPt;                                         //czcionka z punktami
        sf::Text strHp;                                         //czcioneczka z HP
        int pt = 0;                                             //punkty
        stringstream ss;                                        //stringstream do przerobek
        bool destroy = true;                                    //zniszczony przed koncem gry
        sf::Vertex smoke[100];                                  //piksele do animacji dymu silnika

        Interceptor();                                          //Konstruktor, dziedziczy po sf::Sprite
        void paint (Game&);                                     //rysujemy Interceptor i jego podelementy param Game &
        void catchEvents(sf::Event&, Game&);                           //przechwyc i obsluz zdarzenia param sf::Event &
        void update(Game&);                                     //update pozycji/animacji
        void shoot(Game&);                                           //shoot
        virtual ~Interceptor();
};

class Rock : public sf::ConvexShape
{
    public:
        static sf::Texture tx;                                  //statyczna tekstura
        static sf::Texture txg;                                 //tekstura skaly leczacej
        float speed;                                            //predkosc standardowa
        float direct=0;                                         //skosy w ruchu
        vector <sf::Vector2f> vertex;                           //wierzcholeczki w kontenerze
        bool hit=false;                                         //trafienie
        bool kill=false;                                        //usmiercenie
        int rot;                                                //rotacja
        bool heal=false;                                        //czy leczy

        Rock();
        void update();                                          //nowa pozycja, zmiana animacji
        virtual ~Rock();
};

class FirstAid : public sf::RectangleShape
{
    public:
        static sf::Texture tx;
        float speed;
        bool kill=false;
        int rot;

        FirstAid();
        void update();
        virtual ~FirstAid();
};

class InfiniteAmmo : public sf::RectangleShape
{
    public:
        static sf::Texture tx;
        float speed;
        bool kill=false;
        int rot;

        InfiniteAmmo();
        void update();
        virtual ~InfiniteAmmo();
};
