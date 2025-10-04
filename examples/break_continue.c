int main() {
    int i;
    int sum;
    
    sum = 0;
    
    for (i = 1; i < 20; i = i + 1) {
        if (i > 10) {
            break;
        }
        if (i == 5) {
            continue;
        }
        sum = sum + i;
    }
    
    return sum;
}
