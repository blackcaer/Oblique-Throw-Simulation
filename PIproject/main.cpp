// blackcaer
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "FluidSimulator", sf::Style::Default, sf::ContextSettings(32));
    //sf::RectangleShape player(sf::Vector2f(100.0, 100.0));

    //sf::View view(sf::Vector2f(0, 0), sf::Vector2f(512, 512));

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {

            }
            else if (event.type == sf::Event::TextEntered)
            {
                printf("%c", event.text.unicode);
            }
        }

        //view.setCenter(player.getPosition());
        window.clear(sf::Color(100, 100, 100, 255));
        //window.setView(view);
        //window.draw(player);

        window.display();
    }

    return 0;
}