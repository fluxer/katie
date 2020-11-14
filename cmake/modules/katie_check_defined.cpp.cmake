#include <stdio.h>
#include <@FROMHEADER@>

int main() {
    return 0;
#ifndef @FORDEFINITION@
    return printf("%p\n", &@FORDEFINITION@);
#endif
}
