#include <stdio.h>
#include <@FROMHEADER@>

int main() {
#ifndef @FORDEFINITION@
    return printf("%p\n", &@FORDEFINITION@);
#endif
    return 0;
}
