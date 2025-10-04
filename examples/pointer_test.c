// 测试指针基本操作
int main()
{
    int x = 42;
    int *p = &x;
    int y = *p;

    return y; // 应该返回 42
}
