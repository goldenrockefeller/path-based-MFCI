#include <SFML/Graphics.hpp>
#include "frame.hpp"
#include "pixel.hpp"
#include "path.hpp"
#include "interframe.hpp"
#include <iostream>
#include <vector>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define FRAME_DRAW_WIDTH 100
#define FRAME_DRAW_HEIGHT 100

#define DRAW_Y          WINDOW_HEIGHT / 2 - FRAME_DRAW_HEIGHT / 2
#define SOURCE_DRAW_X   WINDOW_WIDTH / 4 - FRAME_DRAW_WIDTH / 2
#define INTER_DRAW_X    WINDOW_WIDTH / 2 - FRAME_DRAW_WIDTH / 2
#define DEST_DRAW_X     3 * WINDOW_WIDTH / 4 - FRAME_DRAW_WIDTH / 2

#define SOURCE_FILENAME "C:/Users/Erricane/Desktop/path-based-MFCI/bin/Debug/blank.png"
#define DEST_FILENAME "C:/Users/Erricane/Desktop/path-based-MFCI/bin/Debug/blank2.png"
#define MANDATORY_MAKER NoMandatoryMaker

#define PATHS_MAKER FullPathsMaker


int main()
{
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PBMCFI");

  MANDATORY_MAKER mandatory_maker;
  PATHS_MAKER paths_maker;

  // Set up resources for source frame
  sf::Texture source_texture;
  if (!source_texture.loadFromFile(SOURCE_FILENAME)) {
    return -1;
  }
  sf::RectangleShape source_shape(sf::Vector2f(FRAME_DRAW_WIDTH, FRAME_DRAW_HEIGHT));
  source_shape.setPosition(SOURCE_DRAW_X, DRAW_Y);
  Frame source(SOURCE_FILENAME, mandatory_maker);
  source_shape.setTexture(&source_texture, true);

  // Set up resources for destination frame
  sf::Texture dest_texture;
  if (!dest_texture.loadFromFile(DEST_FILENAME)) {
    return -1;
  }
  sf::RectangleShape dest_shape(sf::Vector2f(FRAME_DRAW_WIDTH, FRAME_DRAW_HEIGHT));
  dest_shape.setPosition(DEST_DRAW_X, DRAW_Y);
  Frame dest(DEST_FILENAME, mandatory_maker);
  dest_shape.setTexture(&dest_texture, true);

  // Set up resources for intermediate frame
  std::vector<Path> paths = paths_maker(source, dest);
  sf::Texture inter_texture;
  InterFrame inter(paths, &inter_texture, source, dest);
  sf::RectangleShape inter_shape(sf::Vector2f(FRAME_DRAW_WIDTH, FRAME_DRAW_HEIGHT));
  inter_shape.setPosition(INTER_DRAW_X, DRAW_Y);
  inter_shape.setTexture(&inter_texture, true);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) {
        int assignments = 0;
        for (int ind = 0; ind < int(source.image.getSize().x) * int(source.image.getSize().y); ind ++) {
          assignments += source.pixel_infos[ind].assignment;
        }
        std::cout << "Frame Updated: " << assignments << "\n";
        inter.update();


      }

    }

    window.clear();
    window.draw(source_shape);
    window.draw(inter_shape);
    window.draw(dest_shape);
    window.display();
  }

  return 0;
}
