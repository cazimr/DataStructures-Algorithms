#include <iostream>

using namespace std;

int fib2_0(int n, int prosli=1, int pretprosli=0){
    if(n<=1) return n;
    if(n==2) return prosli+pretprosli;
    return fib2_0(n-1, pretprosli+prosli, prosli);
}

int nzd(int x, int y){
    if(x<0) x*=-1;
    if(y==0) return x;
    return nzd(y,x%y);
}
int main() {
    cout<<fib2_0(3)<<endl;
    cout<<nzd(-24,-9)<<endl;
    return 0;
}
