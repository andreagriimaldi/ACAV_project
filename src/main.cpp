#include <iostream>

#include "Map.h"
#include "../SfmlRenderer.h"

int main() {
    Map m = Map(300);
    m.initialize();
    SfmlRenderer renderer = SfmlRenderer(300);
    while (true) {
        renderer.draw(m);
    }
    return 0;
}
