#include "server_app.h"

int main()
{
    struct Catalog* catalog = newCatalog();

    return server_app(catalog);
}
