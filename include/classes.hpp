#define FRAME_LIMIT 60
#define POSX 0
#define POSY 0
#define SCREENX 1120
#define SCREENY 630
#define CBITS 32
#define PI 3.14159265358979323846
#define MAX_SOUNDS 40
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Network.hpp>

using namespace std;

class Rock;
class Interceptor;
class Game;
class Bullet;

class Game : public sf::RenderWindow
{
    public:
        //! zegar gry (moze sie przyda)
        static sf::Clock clock;
        //! Rock kontener na przeszkody
        static vector <Rock> Rocks;
        //! gwiazdeczki
        static sf::Vertex va[1000];
        //! Czcionka gry
        static sf::Font Font;
        //! napis koncowy
        sf::Text gg;
        //! uplyw czasu
        sf::Text strT;
        //! Tlo
        sf::Texture bgtx;
        sf::RectangleShape bg;
        //! pauza
        bool pause=true;
        //! bufor
        stringstream ss;
        //! maksymalny czas na gre
        sf::Time t;
        //! ustawia wlasciwosci glownego okna z gra
        Game();
        //! konstruktor \param tytul okna
        Game(sf::String);
        //! uruchamia cala gre
        void run();
        //! odpala tryb Game Over
        void gogg(Interceptor *);
        //! obsluguje konsole - statyczna
        static void startConsole();
        //! pobiera uchwyt do okna i pozwala obsluzyc okno z wykorzystaniem biblioteki window.h od Microsoft
        static void setSFMLWindow(HWND gw);
        static bool isPointInsidePolygon(vector <sf::Vector2f> &, const sf::Vector2f&);
        static bool isPointInsidePolygon(const sf::Transform&, vector <sf::Vector2f> &, const sf::Vector2f&);
        static void updateRocks(Game &);
        virtual ~Game();
};

//! BULLET
class Bullet : public sf::CircleShape
{
    public:
        //! statyczna tekstura
        static sf::Texture tx;
        //! rozmiary klatki animacji
        static unsigned int xy;
        //! klatek animacji
        unsigned int frames=6*3;
        //! predkosc standardowa
        float speed = 3;
        //! statek wlasciciel
        Interceptor *interceptor;
        //! czy usuwac pocisk
        bool kill = false;

        Bullet(const sf::Vector2f &,Interceptor*);

        //! nowa pozycja, zmiana animacji
        void update(Game &);
        //! test kolizji dla obecnej pozycji
        bool collTest();

        virtual ~Bullet();
};

//! Interceptor
class Interceptor : public sf::Sprite
{
    public:
        //! predkosc (po osi Y)
        float speed = 0;
        //! o ile podnosze predkosc podczas "skoku"
        float boostSpeed=4;
        //! stopien zwalniania przyspieszenia  (wiecej, wolniej zwalnia)
        float speedCooldown = 50;
        //! obrazek do tekstury
        sf::Image skin;
        //! tekstura
        sf::Texture skinTx;
        //! punkty (piksele) ktore wchodza w kolizje
        std::vector <sf::Vector2i>  localCollisionPoints;
        //! HP
        int hp=30;
        int hpmax = 30;
        sf::RectangleShape hpRect;
        //! dlugisc paska zdrowia
        float hpRectX = 50;
        //! kontener z pociskami
        vector <Bullet> bullets;
        //! maksymalnie mozesz wystrzelic
        unsigned int maxBullets = 12;
        //! czcionka z punktami
        sf::Text strPt;
        //! czcioneczka z HP
        sf::Text strHp;
        //! punkty
        int pt = 0;
        //! stringstream do przerobek
        stringstream ss;
        //! zniszczony przed koncem gry
        bool destroy = true;
        //! piksele do animacji dymu silnika
        sf::Vertex smoke[100];

        //! Konstruktor, dziedziczy po sf::Sprite
        Interceptor();
        //! rysujemy Interceptor i jego podelementy \param Game &
        void paint (Game&);
        //! przechwyc i obsluz zdarzenia \param sf::Event &
        void catchEvents(sf::Event&);
        //! update pozycji/animacji
        void update(Game&);
        //! shoot
        void shoot();

        virtual ~Interceptor();
};

//! ROCK
class Rock : public sf::ConvexShape
{
    public:
        //! statyczna tekstura
        static sf::Texture tx;
        static sf::Texture txg;

        //! predkosc standardowa
        float speed;
        //! skosy w ruchu
        float direct=0;
        //! wierzcholeczki w konteneru
        vector <sf::Vector2f> vertex;
        //! trafienie
        bool hit=false;
        //! usmiercenie
        bool kill=false;
        //! rotacja
        int rot;
        //! czy leczy
        bool heal=false;

        Rock();

        //! nowa pozycja, zmiana animacji
        void update();

        virtual ~Rock();
};
