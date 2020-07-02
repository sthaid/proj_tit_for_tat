#include <stdio.h>

int main()
{
    char s[100];
    int cnt=0;

    setlinebuf(stdout);

    while (1) {
        fgets(s,sizeof(s),stdin);
        fputs(s, stdout);
        if (cnt++ == 5) return 0;
    }

    return 0;
}
