#ifndef MONO_APP_H_INCLUDED
#define MONO_APP_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "product.h"
#include "catalog.h"
#include "catalogio.h"

void mono_app(struct Catalog* catalog) {
    int exit = 0;
    while(exit != 1) {
        printf("Online shopping application. Select an action \n");
        printf(" 1 - Add Product \n 2 - Search Catalog \n 3 - Display catalog \n 4 - Catalog size \n 5 - Purchase Item (make payment) \n 6 - Exit \n 7 - Persist To File \n 8 - Load From File \n");
        int action = -1;
        scanf("%d", &action);

        switch(action) {
        case 1: ;
        {
            struct Product* product = newProduct("", 0, "");
            printf("Enter the product's name: ");
            scanf(" %[^\n]", product->name);
            printf("\n");
            printf("Enter the product's price: ");
            scanf("%d", &product->price);
            printf("\n");
            int stock = 0;
            printf("Enter the product's stock: ");
            scanf("%d", &stock);
            printf("\n");
            struct CatalogItem* item = newCatalogItem(product, stock);
            addToCatalog(item, catalog);
            break;
        }
        case 2:
            {
                printf("Enter product name: \n");
                char name[20];
                scanf("%s", name);
                struct Product* p = newProduct(name, 0, "");
                struct CatalogItem* item = searchCatalog(*p, *catalog);
                if(item->stock == 0) printf("Product does not exist in the catalog \n");
                else {
                    printf("Name: %s \t Price: %d \t Stock: %d \n", item->product->name, item->product->price, item->stock);
                }
                break;
            }
        case 3:
            for(int i = 0; i<catalog->catalogSize; i++) {
                struct CatalogItem* item = catalog->items[i];
                printf(" Catalog Item: %d\n", i+1);
                printf(" Item name: %s \n Item price: %d \n Stock: %d \n\n", item->product->name, item->product->price, item->stock);
            }
            break;
        case 4:
            printf("Catalog size: %d \n", catalog->catalogSize);
            break;
        case 5: ;
        {
            struct Product* product = newProduct("", 0, "");
            printf("Enter the product's name: ");
            scanf("%s", product->name);
            printf("\n");
            int paymentAmount = 0;
            printf("Enter payment amount: ");
            scanf("%d", &paymentAmount);
            struct Payment payment = *newPayment(paymentAmount);

            struct Payment change = purchaseItem(*product, payment, catalog);
            if(change.amount == payment.amount) {
                printf("Could not complete purchase. Check that the item exists in the catalog AND that the payment amount is more than the product's price \n");
            } else {
                printf("Change amount: %d \n", change.amount);
            }

            break;
        }
        case 6:
            exit = 1;
            break;
        case 7: ; {
            printf("Enter file path: ");
            char filepath[50];
            scanf("%s", filepath);
            persistCatalog(*catalog, filepath);
            printf("Catalog persisted successfully.\n");
            break;
        }
        case 8: ; {
            printf("Enter file path: ");
            char filepath[50];
            scanf("%s", filepath);
            hydrateCatalog(filepath, catalog);
            printf("Catalog hydrated successfully.\n");
            break;
        }
        default:
            printf("Invalid selection");
            break;
        }
        printf("\n");
    }
}

#endif // MONO_APP_H_INCLUDED
