//
// Created by LIO on 05/09/2017.
//

#ifndef SFML_TEMPLATE_TEST_H
#define SFML_TEMPLATE_TEST_H

namespace mvd {
  class Test {
    Test(int a_x, int a_y) : m_x(a_x), m_y(a_y) {}
  public:
    //--
    int compute() const { return m_x + m_y; }
    int weird();

  private:
    //--- MEMBERS
    int m_x;
    int m_y;
  };

}//eo-namespace mvd
#endif //SFML_TEMPLATE_TEST_H
