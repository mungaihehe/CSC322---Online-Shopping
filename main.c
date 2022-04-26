#include "mono_app.h"

int main()
{
    struct Catalog* catalog = newCatalog();

    mono_app(catalog);
    return 0;
}
