#ifndef PRODUCT_H_INCLUDED
#define PRODUCT_H_INCLUDED

struct Product {
    char name[20];
    int price;
    char description[50];
};

struct Product* newProduct(char name[20], int price, char description[50]) {
    struct Product* p = malloc(sizeof(struct Product));
    strcpy(p->name, name);
    p->price = price;
    strcpy(p->description, description);
    return p;
};


#endif // PRODUCT_H_INCLUDED
