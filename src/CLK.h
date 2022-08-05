#ifndef CLK_H
#define CLK_H

#include "vertex.h"
#include <list>

class CLK {
  public:
    CLK();
    ~CLK();

  private:
    std::list<Vertex> vertices;
};

#endif  // CLK_H
