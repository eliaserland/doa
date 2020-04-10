#include <stdio.h>
#include "int_array_1d.h"

void zero_histogram(int_array_1d *a)
{
        for (int i = int_array_1d_low(a); i <= int_array_1d_high(a); i++){
                int_array_1d_set_value(a, 0, i);
        }

}

void make_histogram(const char *msg, int_array_1d *a)
{
        for (int i = 0; msg[i] != '\0'; i++){
                if (msg[i] >= int_array_1d_low(a) && msg[i] <= int_array_1d_high(a)){
                        int_array_1d_set_value(a, int_array_1d_inspect_value(a, msg[i]) + 1, msg[i]);
                }
        }
}

void print_histogram(const int_array_1d *a)
{
        for (int i = int_array_1d_low(a); i <= int_array_1d_high(a); i++){
                if (int_array_1d_inspect_value(a, i) > 0){
                        printf("%c: %d\n", i, int_array_1d_inspect_value(a, i));
                }
        }
}

int main(int argc, char const *argv[])
{
        const char *msg = "090-786 68 32";
        int_array_1d *a = int_array_1d_create('0', '9');

        zero_histogram(a);
        make_histogram(msg, a);
        print_histogram(a);

        int_array_1d_print(a);

        int_array_1d_kill(a);
        return 0;
}
