#ifndef CATALOGIO_H_INCLUDED
#define CATALOGIO_H_INCLUDED
#include "catalog.h"
#include "product.h"

void persistCatalog(struct Catalog catalog, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("The file: %s could not be opened", filename);
        return;
    }
    for(int i = 0; i<catalog.catalogSize; i++){
        struct CatalogItem* item = catalog.items[i];
        fprintf(fp, "%s\n%d\n%d\n", item->product->name, item->product->price, item->stock);
    }
    fclose(fp);
}

void hydrateCatalog(const char* filename, struct Catalog* catalog) {
    FILE* fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("The file: %s could not be opened", filename);
        return;
    }
    char name[20];
    char price[20];
    char stock[20];
    while(fscanf(fp, "%20[^\n]\n%20[^\n]\n%20[^\n]\n", name, price, stock) != EOF) {
        struct Product *product = newProduct(name, atoi(price), "");
        struct CatalogItem *item = newCatalogItem(product, atoi(stock));
        addToCatalog(item, catalog);
    }
    return catalog;
};

#endif // CATALOGIO_H_INCLUDED
