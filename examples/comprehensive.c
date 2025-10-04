int fibonacci(int n) {
    if (n < 2) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int sum_range(int start, int end) {
    int total;
    int i;
    
    total = 0;
    
    for (i = start; i < end; i = i + 1) {
        if (i == 5) {
            continue;
        }
        if (i > 10) {
            break;
        }
        total = total + i;
    }
    
    return total;
}

int main() {
    int fib_result;
    int sum_result;
    
    fib_result = fibonacci(7);
    sum_result = sum_range(1, 20);
    
    return fib_result + sum_result;
}
