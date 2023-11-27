#include <iostream>
#include "trie.h"
#include <SFML/Graphics.hpp>

main(){

    std::cout << "pretend that this is the project" << std::endl;
    std::cout << "sounds good" << std::endl;
    std::cout << "peepeepoopoo" << std::endl;

    std::cout << "testing trie object without loading in csv" << std::endl;
    trie test;
    product airpods;
    airpods.name = "apple airpods";
    airpods.price = 99.99;
    airpods.rating = 4.8;
    airpods.bought = 1500;
    airpods.imageURL = "https://m.media-amazon.com/images/I/61sxnqv5FdL._AC_UL320_.jpg";
    airpods.productURL = "https://www.amazon.com/dp/B0B4NQBZ4Q";
    test.insert("airpod", airpods);
    std::vector<product> p = test.getKeyword("airpod");
    product p1 = p[0];
    std::cout << p1.name << " " << p1.price << std::endl;
    std::cout << p1.rating << " " << p1.bought << std::endl;
    std::cout << p1.productURL << std::endl;
    std::cout << p1.imageURL << std::endl;

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    int i = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
        i++;
    }
    return 0;
}
