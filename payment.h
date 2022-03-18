#ifndef PAYMENT_H_INCLUDED
#define PAYMENT_H_INCLUDED

struct Payment {
    int amount;
};


struct Payment* newPayment(int amount) {
    struct Payment* p = malloc(sizeof(struct Payment));
    p->amount = amount;
    return p;
};


#endif // PAYMENT_H_INCLUDED
