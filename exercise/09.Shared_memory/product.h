#ifndef PRODUCT_H
#define PRODUCT_H
#define Name_path   20
#define SIZE 1024

typedef struct{
    int ID;
    char name[Name_path];
    int price;

} Product;

const char *filename = "shared.txt";


#endif