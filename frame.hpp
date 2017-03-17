#ifndef FRAME_HPP
#define FRAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "pixel.hpp"
#include <string>

struct MandatoryMaker {
  virtual std::vector<bool> operator () (sf::Image image) = 0;
};

struct CheckerBoardMandatoryMaker : MandatoryMaker {
  virtual std::vector<bool> operator () (sf::Image image) {
    std::vector<bool> mandatory;
    mandatory.resize(image.getSize().x * image.getSize().y, false);
    for (int col_ind = 0; col_ind < int(image.getSize().y); col_ind++) {
      for (int row_ind = 0; row_ind < int(image.getSize().x); row_ind++) {
        if ((col_ind + row_ind) % 2 == 0)
        {
          mandatory[col_ind * image.getSize().x + row_ind] = true;
        }
      }
    }

    return mandatory;
  }
};

struct NoMandatoryMaker : MandatoryMaker {
  NoMandatoryMaker() {};
  virtual std::vector<bool> operator () (sf::Image image) {
    std::vector<bool> mandatory;
    mandatory.resize(image.getSize().x * image.getSize().y, false);
    return mandatory;
  }
};

struct Frame {
  Frame(const std::string& filename, MandatoryMaker& makeMandatory) {
    if (!image.loadFromFile(filename)) {
      std::cout << "Failure to load " << filename << "\n";
    }
    else {
      sf::Vector2u image_size = image.getSize();
      int pixel_count = image_size.x * image_size.y;
      std::vector<bool> mandatory = makeMandatory(image);
      pixels.resize(pixel_count);
      pixel_infos.resize(pixel_count);
      sf::Color color;

      int ind;
      for (int x = 0; x < int(image_size.x); x ++) {
        for (int y = 0; y < int(image_size.y); y ++) {
          color = image.getPixel(x, y);
          ind = y * image_size.x + x;
          pixels[ind].r = double(color.r);
          pixels[ind].g = double(color.g);
          pixels[ind].b = double(color.b);
          pixels[ind].x = x;
          pixels[ind].y = y;
          pixel_infos[ind].mandatory = mandatory[ind];
        }
      }
    }
  }

  sf::Image image;
  std::vector<Pixel> pixels;
  std::vector<PixelInfo> pixel_infos;
};



#endif // FRAME_HPP

