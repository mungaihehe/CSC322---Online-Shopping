#ifndef CATALOG_H_INCLUDED
#define CATALOG_H_INCLUDED
#include "product.h"
#include "payment.h"


struct CatalogItem {
    struct Product* product;
    int stock;
};

struct CatalogItem* newCatalogItem(struct Product* product, int stock) {
    struct CatalogItem* item = malloc(sizeof(struct CatalogItem));

    item->product = product;
    item->stock = stock;
    return item;
};

struct Catalog {
    struct CatalogItem* items[20];
    int catalogSize;
};

struct Catalog* newCatalog() {
    struct Catalog* catalog = malloc(sizeof(struct Catalog));
    catalog->catalogSize = 0;
    return catalog;
};

int addToCatalog(struct CatalogItem* item, struct Catalog* catalog) {
    if(catalog->catalogSize >= 20) return 0;
    else {
        catalog->items[catalog->catalogSize] = item;
        catalog->catalogSize = catalog->catalogSize + 1;
        return 1;
    }
}

struct CatalogItem* searchCatalog(struct Product product, struct Catalog catalog) {
    for(int i = 0; i<catalog.catalogSize; i++) {
        struct CatalogItem* item = catalog.items[i];
        if(strcmp(item->product->name,product.name) == 0) {
            return catalog.items[i];
        }
    }
    return newCatalogItem(&product, 0);
};

//returns change as Payment
struct Payment purchaseItem(struct Product product, struct Payment payment, struct Catalog* catalog) {
    struct CatalogItem* item = searchCatalog(product, *catalog);
    if(item->stock == 0) return payment;
    else if(item->product->price > payment.amount) return payment;
    else {
        item->stock--;
        struct Payment change;
        change.amount = payment.amount - item->product->price;
    }
};



#endif // CATALOG_H_INCLUDED
