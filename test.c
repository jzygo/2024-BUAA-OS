#include <blib.h>

char str[] = "324234+sdfsd+-dsfsffsd/sdfsd*/sdfsdfsdf*3224324///";
const char *sarray[] = {
        "324234",
        "sdfsd",
        "",
        "dsfsffsd",
        "sdfsd",
        "",
        "sdfsdfsdf",
        "3224324",
        "",
        "",
        ""
    };

int main() {
    putstr("begin check string\n");

    panic_on((strlen(str) != 50), "check failed at");
    panic_on((strlen(sarray[2]) != 0), "check failed at");

    char destination[20] = "Hello, ";
    const char *source = "World!";
    char *s = strcat(destination,source);
    panic_on((strcmp(s,"Hello, World!") != 0),"check failed at");
    panic_on((strcmp(destination,"Hello, World!") != 0),"check failed at");

    char destination2[20] = "Hello, ";
    const char *source2 = "World!World!World!";
    s = strncat(destination2,source2,6);
    panic_on((strcmp(s,"Hello, World!") != 0),"check failed at");
    panic_on((strcmp(destination2,"Hello, World!") != 0),"check failed at");

    char destination3[20] = "Hello, ";
    const char *source3 = "World!";
    s = strncat(destination3,source3,200);
    panic_on((strcmp(s,"Hello, World!") != 0),"check failed at");
    panic_on((strcmp(destination3,"Hello, World!") != 0),"check failed at");

    char destination4[20] = "Hello, ";
    const char *source4 = "World!";
    s = strncat(destination4,source4,0);
    panic_on((strcmp(s,"Hello, ") != 0),"check failed at");
    panic_on((strcmp(destination4,"Hello, ") != 0),"check failed at");

    char destination5[20] = "";
    const char *source5 = "Hello World!";
    s = strncat(destination5,source5,12);
    panic_on((strcmp(s,"Hello World!") != 0),"check failed at");
    panic_on((strcmp(destination5,"Hello World!") != 0),"check failed at");


    char *token;
    char *rest = str;
    const char *delimiters = "+-*/";
    int index = 0;
    while ((token = strsep(&rest, delimiters)) != NULL) {
        panic_on((strcmp(sarray[index++],token) != 0), "check failed at");
    }
    panic_on(index != 11, "check failed at");
    putstr("string test pass!\n");
    return 0;
}
