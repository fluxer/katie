#include <stdio.h>

int main() {
#if defined(__aarch64__)
    printf("ARM64");
#elif defined(__arm__)
    printf("ARM");
#elif defined(__x86_64__)
    printf("X86_64");
#elif defined(__i386__)
    printf("I386");
#elif defined(__mips64__)
    printf("MIPS64");
#elif defined(__mips__)
    printf("MIPS");
#elif defined(__powerpc64__)
    printf("POWERPC64");
#elif defined(__powerpc__)
    printf("POWERPC");
#elif defined(__s390__)
    printf("S390");
#else
    printf("The architecture is not supported\n");
    return 1;
#endif
    return 0;
}
