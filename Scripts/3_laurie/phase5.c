#include <stdio.h>

int main() {
    const char* index = "isrveawhobpnutfg";
    const char* target = "giants";

    for (char c = 'a'; c <= 'z'; c++) {
        char mapped = index[c & 0xf]; // c & 0xf keeps the lower 4 bits

        // Check if mapped character is in "giants"
        for (int j = 0; target[j] != '\0'; j++) {
            if (mapped == target[j]) {
                printf("[%c] = %c\n", mapped, c);
                break;
            }
        }
    }

    return 0;
}
