// C++ program to convert a decimal 
// number to binary number 
  
#include <iostream> 
#include <algorithm>
using namespace std; 
  
// function to convert decimal to binary 
void decToBinary(int n) 
{ 
    // array to store binary number 
    int binaryNum[32]; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 
  
    // printing binary array in reverse order 
    for (int j = i - 1; j >= 0; j--) 
        cout << binaryNum[j]; 
} 

string int2bin(int n){
    string x;
    for(int i=0;i<32;i++){
        if(n&1) {x+='1';}
        else {x+='0';}
        n>>=1;
    }
    reverse(x.begin(),x.end());
    return x;
}
  
// Driver program to test above function 
int main() 
{ 
    // decToBinary(64);
    // printf("\n");
    // decToBinary(-64);

    std::string res;

    for (int n=64; n!=0; n/=2)
    {
        res = int2bin(n);
        fprintf(stderr, "n=%d, bin(n)=%s\n",
            n, res.c_str());

        res = int2bin(-n);
        fprintf(stderr, "-n=%d, bin(-n)=%s\n",
            -n, res.c_str());
        
        fprintf(stderr, "\n");
    }

    cout << int2bin(63) << std::endl;
    cout << int2bin(-63) << std::endl;

    return 0; 
} 