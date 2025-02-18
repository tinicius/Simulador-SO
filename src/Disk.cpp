#include "Disk.hpp"

Disk::Disk() {}

void Disk::insert_page(Page page) { pages.push_back(page); }

Page Disk::get_page(int page_address) {
  if (page_address < 0 && page_address >= (int)pages.size()) {
    cout << "Invalid page address" << endl;
    exit(1);
  }

  return pages[page_address];
}