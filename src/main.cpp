#include <iostream>

#include "Map.h"
#include "SfmlRenderer.h"

int main() {
    Map m = Map(90);
    m.initialize();
    //SfmlRenderer renderer = SfmlRenderer(30);

    std::cout << std::endl;
    for (int i = 0; i < m.getDim() + 1; i++) {
        for (int j = 0; j < m.getDim() + 1; j++) {
            switch (m.getGrid()[j][i]->getType()) {
                case Point_type::Empty:
                    std::cout << "- ";
                    break;
                case Point_type::Boundary:
                    std::cout << "h ";
                    break;
                case Point_type::Road:
                    std::cout << "r ";
                    break;
            }
        }
        std::cout << std::endl;
    }

    /*while (true) {
        renderer.draw(m);
    }
    */
    return 0;
}
