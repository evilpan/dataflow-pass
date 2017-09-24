int foo() {
    int a = 1;
    int b = 2;
    return a+b;
}
int bar() {
    int a = 3;
    int b = 4;
    int c;
    for(int i = 0; i < 500; i++)
        c = a + b + c;
    return c;
}
int main(int argc, char **argv)
{
    int a = 1;
    int b = 2;
    int c = a+b;
    if (argc == 1) foo();
    else bar();
    return c;
}
