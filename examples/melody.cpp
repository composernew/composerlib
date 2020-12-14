#include <matplot/matplot.h>
#include <composer/melody.h>
#include <vector>

int main() {
    composer::melody m;
    m.notes({1,4,6,3,8});
    matplot::plot(m.notes());
    matplot::show();
    return 0;
}