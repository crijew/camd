#include <camd.h>

int main(void)
{
    int32_t ap[] = {0, 1, 2};
    int32_t ai[] = {0, 1};
    int32_t p[2];
    int32_t constraints[] = {0, 0};
    return camd_order(2, ap, ai, p, NULL, NULL, constraints) == CAMD_OK ? 0 : 1;
}
