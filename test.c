#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    char str[1024];
    char so[10];
    char *ten;
    FILE *f, *o;
    o = fopen("sts.txt", "w+");
    f = fopen("station.text", "r");
    while (!feof(f) && fgets(str, 1024, f))
    {
        sscanf(str + 5, "%s", so);
        fprintf(o, "#%s\n", so);
        fgets(str, 1024, f);
        ten = strtok(str, "\t");
        ten = strtok(NULL, "-");
        fprintf(o, "@Chieu di:\n");
        while (ten)
        {
            fprintf(o, "%s\n", ten);
            ten = strtok(NULL, "-\n");
        }
        fprintf(o, "@Chieu ve:\n");
        fgets(str, 1024, f);
        ten = strtok(str, "\t");
        ten = strtok(NULL, "-");
        while (ten)
        {
            fprintf(o, "%s\n", ten);
            ten = strtok(NULL, "-\n");
        }
    }
    free(ten);
    fclose(f);
    fclose(o);
    return 0;
}
// Hong Van (Van La, Thuong Tin)
// Linh Dam (Khu do thi Linh Dam)