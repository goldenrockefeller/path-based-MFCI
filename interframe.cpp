#include "interframe.hpp"
#include "pixel.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <chrono>


InterFrame::InterFrame(std::vector<Path>& pathsIn, sf::Texture* textureIn, Frame& sourceIn, Frame& destIn) :
    paths(pathsIn), texture(textureIn), source(sourceIn), dest(destIn) {
  image.create(sourceIn.image.getSize().x, sourceIn.image.getSize().y);
  texture->loadFromImage(image);
  updateTexture();
}

void InterFrame::updateTexture() {
  Pixel current_pixel;
  for (int y = 0; y < int(image.getSize().y); y ++) {
    for (int x = 0; x < int(image.getSize().x); x++) {
      current_pixel = paths[y * image.getSize().x + x].inter;
      image.setPixel(x, y, sf::Color(uint8_t(current_pixel.r),
                                                uint8_t(current_pixel.g),
                                                uint8_t(current_pixel.b)));
    }
  }
  texture->update(image);
}

Pixel mTransform(const Pixel& p) {
  double c = 0.01;
  double l = 1;
  return Pixel{c*p.r, c*p.g, c*p.b, l* p.x, l* p.y};
}

Pixel sTransform(const Pixel& p) {
  double c = 0.01;
  double l = 1;
  return Pixel{c*p.r, c*p.g, c*p.b, l * p.x, l * p.y};
}

double calculateCost(const Path& path, const std::vector<Path>& neighbors) {
  double cost = 0;

  //Motion Cost

  cost += LengthSquared(mTransform(path.source) - mTransform(path.inter));
  cost += LengthSquared(mTransform(path.dest) - mTransform(path.inter));

  double ds;
  double dd;
  //Alignment Cost TODO
  for (Path neighbor : neighbors) {
    ds = Length(sTransform(path.source) - sTransform(neighbor.source)) -
                Length(sTransform(path.inter) - sTransform(neighbor.inter));
    dd = Length(sTransform(path.dest) - sTransform(neighbor.dest)) -
          Length(sTransform(path.inter) - sTransform(neighbor.inter));
    cost += ds * ds + dd * dd;
  }

  return cost;
}



void InterFrame::update() {
  Path path;
  Pixel chosen_pixel;
  int chosen_pixel_ind;
  int initial_ind;
  double initial_cost;
  double chosen_cost;
  double candidate_cost;
  std::vector<Path> prev_paths = paths;

  std::vector<Path> neighbors;
  int image_width = image.getSize().x;
  int image_height = image.getSize().y;
  int pixel_count = image_width * image_height;

  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::ranlux24 g(seed);
  std::uniform_int_distribution<> dis(0, pixel_count - 1);
  std::vector<int> source_inds(pixel_count, 0);
  std::vector<int> dest_inds(pixel_count, 0);

  for (int x = 0; x < image_width; x ++) {
    for (int y = 0; y < image_height; y ++) {
      neighbors.clear();
      path = prev_paths[y * image_width + x];


      // TODO custom neighbors maker
      if (x >= 2) {
        neighbors.push_back(paths[y * image_width + x - 2]);
      }
      else if (x < image_width - 2) {
        neighbors.push_back(paths[y * image_width + x + 2]);
      }
      if (y >= 2) {
        neighbors.push_back(paths[(y - 2) * image_width + x]);
      }
      else if (y < image_height - 2) {
        neighbors.push_back(paths[(y + 2) * image_width + x]);
      }

      // update source pixel
      chosen_pixel = path.source;
      chosen_pixel_ind = path.s_ind;
      initial_ind = path.s_ind;
      chosen_cost = calculateCost(path, neighbors);
      initial_cost = calculateCost(path, neighbors);
      for (int s_ind = 0; s_ind < pixel_count; s_ind ++) { // TODO replace with custom source iterator
        path.source = source.pixels[s_ind];
        path.blend();
        candidate_cost = calculateCost(path, neighbors) + source.pixel_infos[s_ind].transaction;
        if (candidate_cost < chosen_cost) {
            chosen_cost = candidate_cost;
            chosen_pixel = path.source;
            chosen_pixel_ind = s_ind;
        }
      }
      if (source.pixel_infos[initial_ind].mandatory && source.pixel_infos[initial_ind].assignment == 1) {
        //transaction is negative
        source.pixel_infos[initial_ind].transaction = chosen_cost - initial_cost;
      }
      else {
        paths[y * image_width + x].source = chosen_pixel;
        paths[y * image_width + x].s_ind = chosen_pixel_ind;
        source.pixel_infos[initial_ind].assignment--;
        source.pixel_infos[chosen_pixel_ind].assignment++;
        source.pixel_infos[chosen_pixel_ind].transaction = 0;
      }


      paths[y * image_width + x].blend();
      path = prev_paths[y * image_width + x];

      // update dest pixel
      chosen_pixel = path.dest;
      chosen_pixel_ind = path.d_ind;
      initial_ind = path.d_ind;
      chosen_cost = calculateCost(path, neighbors);
      initial_cost = calculateCost(path, neighbors);
      for (int d_ind = 0; d_ind < pixel_count; d_ind ++) { // TODO replace with custom source iterator
        path.dest = dest.pixels[d_ind];
        path.blend();
        candidate_cost = calculateCost(path, neighbors) + dest.pixel_infos[d_ind].transaction;
        if (candidate_cost < chosen_cost) {
            chosen_cost = candidate_cost;
            chosen_pixel = path.dest;
            chosen_pixel_ind = d_ind;
        }
      }
      if (dest.pixel_infos[initial_ind].mandatory && dest.pixel_infos[initial_ind].assignment == 1) {
        //transaction is negative
        dest.pixel_infos[initial_ind].transaction = chosen_cost - initial_cost;
      }
      else {
        paths[y * image_width + x].dest = chosen_pixel;
        paths[y * image_width + x].d_ind = chosen_pixel_ind;
        dest.pixel_infos[initial_ind].assignment--;
        dest.pixel_infos[chosen_pixel_ind].assignment++;
        dest.pixel_infos[chosen_pixel_ind].transaction = 0;
      }

      paths[y * image_width + x].blend();
    }
  }

  updateTexture();
}
