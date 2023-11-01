#ifndef CUSTOMER_H
#define CUSTOMER_H

struct CUSTOMER {
    const char name[30];
    unsigned char id[8];
    unsigned char itemprefix[2];
};

extern struct CUSTOMER __ALL_CUSTOMERS[103];

#define __CURRENT_CUSTOMER (__ALL_CUSTOMERS[5])
#define __CUSTOMER_COUNT 103

#endif // CUSTOMER_H
