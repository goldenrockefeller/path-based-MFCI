#ifndef PATH_HPP
#define PATH_HPP


#include "frame.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <chrono>

struct Path {
  Path() {}

  Path(Pixel sourceIn, Pixel interIn, Pixel destIn) :
    source(sourceIn), inter(interIn), dest(destIn) {}

  void blend() {
    double x = inter.x;
    double y = inter.y;
    inter = (source + dest) / 2;
    inter.x = x;
    inter.y = y;
  }

  Pixel source;
  Pixel inter;
  Pixel dest;
  int s_ind, d_ind;
};

struct PathsMaker {
  virtual std::vector<Path> operator () (Frame& source, Frame& dest) = 0;
};

struct FullRandomPathsMaker : PathsMaker {
  std::vector<Path> operator () (Frame& source, Frame& dest) {
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    int pixel_count = source.image.getSize().x * source.image.getSize().y;
    std::vector<Path> paths;
    std::vector<int> source_pixel_inds(pixel_count);
    std::vector<int> dest_pixel_inds(pixel_count);

    std::iota(source_pixel_inds.begin(), source_pixel_inds.end(), 0);
    std::iota(dest_pixel_inds.begin(), dest_pixel_inds.end(), 0);

    paths.resize(pixel_count);

    std::ranlux24 g(seed);

    std::shuffle(source_pixel_inds.begin(), source_pixel_inds.end(), g);
    std::shuffle(dest_pixel_inds.begin(), dest_pixel_inds.end(), g);

    int s_ind;
    int d_ind;
    int i_ind;
    for (int x = 0; x < int(source.image.getSize().x); x ++) {
      for (int y = 0; y < int(source.image.getSize().y); y ++) {
        i_ind = y * int(source.image.getSize().x) + x;
        paths[i_ind].inter.x = x;
        paths[i_ind].inter.y = y;
      }
    }

    for (int ind = 0; ind < pixel_count; ind ++) {
      s_ind = source_pixel_inds.back();
      source_pixel_inds.pop_back();
      paths[ind].s_ind = s_ind;
      paths[ind].source = source.pixels[s_ind];
      source.pixel_infos[s_ind].assignment = 1;


      d_ind = dest_pixel_inds.back();
      dest_pixel_inds.pop_back();
      paths[ind].d_ind = d_ind;
      paths[ind].dest = dest.pixels[d_ind];
      dest.pixel_infos[d_ind].assignment = 1;

      paths[ind].blend();
    }

    return paths;
  }

};

struct FullPathsMaker : PathsMaker {
  std::vector<Path> operator () (Frame& source, Frame& dest) {\
    int pixel_count = source.image.getSize().x * source.image.getSize().y;
    std::vector<Path> paths;
    std::vector<int> source_pixel_inds(pixel_count);
    std::vector<int> dest_pixel_inds(pixel_count);

    std::iota(source_pixel_inds.begin(), source_pixel_inds.end(), 0);
    std::iota(dest_pixel_inds.begin(), dest_pixel_inds.end(), 0);

    paths.resize(pixel_count);

    int s_ind;
    int d_ind;
    int i_ind;
    for (int x = 0; x < int(source.image.getSize().x); x ++) {
      for (int y = 0; y < int(source.image.getSize().y); y ++) {
        i_ind = y * int(source.image.getSize().x) + x;
        paths[i_ind].inter.x = x;
        paths[i_ind].inter.y = y;
      }
    }

    for (int ind = 0; ind < pixel_count; ind ++) {
      s_ind = ind;
      paths[ind].s_ind = s_ind;
      paths[ind].source = source.pixels[s_ind];
      source.pixel_infos[s_ind].assignment = 1;


      d_ind = ind;
      paths[ind].d_ind = d_ind;
      paths[ind].dest = dest.pixels[d_ind];
      dest.pixel_infos[d_ind].assignment = 1;

      paths[ind].blend();
    }

    return paths;
  }

};


#endif // PATH_HPP

