#ifndef DISK_HPP
#define DISK_HPP

#include <vector>
#include <iostream>

#include "entities.hpp"

class Disk {
  vector<Page> pages;

 public:
  Disk();

  void insert_page(Page page);
  Page get_page(int page_address);
};

#endif