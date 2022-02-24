#include <stdio.h>

int main() {
#if defined(__alpha__)
    printf("ALPHA");
#elif defined(__arm__)
    printf("ARM");
#elif defined(__aarch64__)
    printf("ARM64");
#elif defined(__avr__)
    printf("AVR32");
#elif defined(__bfin__)
    printf("BFIN");
#elif defined(__i386__)
    printf("I386");
#elif defined(__ia64__)
    printf("IA64");
#elif defined(__m68k__)
    printf("M68K");
#elif defined(__mips__)
    printf("MIPS");
#elif defined(__hppa__)
    printf("PARISC");
#elif defined(__powerpc__)
    printf("POWERPC");
#elif defined(__powerpc64__)
    printf("POWERPC64");
#elif defined(__s390__)
    printf("S390");
#elif defined(__sparc__)
    printf("SPARC");
#elif defined(__sh__)
    printf("SH");
#elif defined(__sh4a__)
    printf("SH4A");
#elif defined(__x86_64__)
    printf("X86_64");
#else
    printf("Unable to detect architecture, please update above list\n");
    return 1;
#endif
    return 0;
}
