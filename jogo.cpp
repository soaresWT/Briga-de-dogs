#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>


void setSize(sf::Sprite& sprite, int width, int height) {
    auto dim = sprite.getLocalBounds();
    sprite.setScale((float) width / dim.width, (float) height / dim.height);
}

struct Entity {
    int x {0}; 
    int y {0};
    int step {0};
    sf::Sprite sprite;

    Entity(int x, int y, int step, sf::Texture& texture):
        x {x}, y {y}, step {step}, sprite(texture) {
    }

    void draw(sf::RenderWindow& window) {
        this->sprite.setPosition(x * step, y * step);
        setSize(this->sprite, step, step);
        window.draw(this->sprite);
    }
};
int colisor(Entity& oi, Entity& ola){
    int quantidade = 0;
    if(oi.x == ola.x && oi.y == ola.y){
       std::cout << "colidiu" << std::endl;
       quantidade++;
       
    }
    
    return quantidade;
}


struct Board {
    int nc {0};
    int nl {0};
    int step {0};
    sf::Sprite sprite;
    sf::RectangleShape rect;

        
    
    Board(int nc, int nl, int step, sf::Texture& texture) {
        this->nc = nc;
        this->nl = nl;
        this->step = step;
        this->sprite.setTexture(texture);
        setSize(this->sprite, nc * step, nl * step);
        this->sprite.setPosition(0, 0);

        this->rect.setSize(sf::Vector2f(step, step));
        this->rect.setFillColor(sf::Color::Transparent);
        this->rect.setOutlineColor(sf::Color::Black);
        this->rect.setOutlineThickness(2);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(this->sprite);
        for (int i = 0; i < nc; i++) {
            for (int j = 0; j < nl; j++) {
                rect.setPosition(i * step, j * step);
                window.draw(rect);
            }
        }
    }
};

void moveEntity(sf::Keyboard::Key key, Entity& entity, std::vector<sf::Keyboard::Key> move_keys) {
    if (key == move_keys[0] && entity.x > 0) 
        entity.x--;
    else if (key == move_keys[1] && entity.y > 0)
        entity.y--;
    else if (key == move_keys[2] && entity.x < 6)
        entity.x++;
        
    else if (key == move_keys[3] && entity.y < 4)
        entity.y++;
        
}

sf::Texture loadTexture(std::string path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cout << "Error loading texture" << std::endl;
        exit(1);
    }
    return texture;
}
sf::Keyboard::Key movecrazy(){
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int random = rand() % 4;
    if(random == 0)
        return sf::Keyboard::W;
    else if(random == 1)
        return sf::Keyboard::S;
    else if(random == 2)
        return sf::Keyboard::A;
    else
        return sf::Keyboard::D;
}

int main() {

    sf::Texture tom_tex { loadTexture("assets/tom.png") };
    sf::Texture jerry_tex { loadTexture("assets/jerry.png") };
    sf::Texture fundo_tex { loadTexture("assets/fundo.jpg") };

    sf:: Font fonte;
    fonte.loadFromFile("assets/maagkramp.ttf");
    sf::Text texto;
    texto.setFont(fonte);
    
    texto.setCharacterSize(50);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(200, 20);
    
    

    const int STEP {100};

    Entity tom(1, 1, STEP, tom_tex);
    Entity jerry(3, 3, STEP, jerry_tex);
    Board board(7, 5, STEP, fundo_tex);

    sf::RenderWindow window(sf::VideoMode(board.nc * STEP, board.nl * STEP), "batalha lendaria de dogs");
   
int colidiram = 0;
    while (window.isOpen()) {
        sf::Event event;
        window.setFramerateLimit(60);
        moveEntity(movecrazy(), jerry, {sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::D, sf::Keyboard::S});
        while (window.pollEvent(event)) {
            texto.setString("COLISOES: " + std::to_string(colidiram));
            if(colidiram == 5){
                texto.setString("VOCE VENCEU");
                //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                //window.close();
            }
            
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                moveEntity(event.key.code, tom, {sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Right, sf::Keyboard::Down});
               
                if(colisor(tom, jerry) == 1){
                    colidiram++;
                }
                
                
            }
        }
       
        window.clear();
        board.draw(window);
        window.draw(texto);
        tom.draw(window);
        jerry.draw(window);
        window.display(); 
    }

    return 0;
}
/*
g++ -c jogo.cpp && g++ jogo.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system && ./sfml-app
*/