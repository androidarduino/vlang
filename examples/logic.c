int main() {
    int x;
    int y;
    int result;
    
    x = 5;
    y = 10;
    
    if (x < 10 && y > 5) {
        result = 1;
    }
    
    if (x > 20 || y > 5) {
        result = result + 2;
    }
    
    return result;
}
