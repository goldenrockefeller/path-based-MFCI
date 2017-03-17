#ifndef INTERFRAME_HPP
#define INTERFRAME_HPP


#include <SFML/Graphics.hpp>
#include "path.hpp"
#include "frame.hpp"
#include <vector>

class InterFrame {
  std::vector<Path>& paths;
  sf::Texture* texture;
  Frame& source;
  Frame& dest;
  sf::Image image;
  void updateTexture();

public:
  InterFrame(std::vector<Path>& pathsIn,
             sf::Texture* textureIn,
             Frame& sourceIn,
             Frame& destIn);
  void update();


};

#endif // INTERFRAME_HPP



