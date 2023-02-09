#include <iostream>  
#include <vector>

std::vector<int> sieve(int max) {
        
    std::vector<int> primes;
    std::vector<bool> nums(max, true);

    for (int i = 2; i*i <= max; i++) { 
        
        if (nums[i]) {

            for (int p = i*i; p <= max; p += i) {

                nums[p] = false;

            } 
        
        }

    }

    for (int i = 2; i <= max; i++) {
       
        if (nums[i]) {
            
            primes.push_back(i); 

        }

    }
    return primes;

}

int main() { 
    
    int inut;
    std::string pref;
    std::cout << "Range: 1, ";
    std::cin >> inut;

    if (input <= 2000000000) {
        
        std::vector<int> primes = sieve(input); 

        if (input <= 1000000) {

            std::cout << "Would you like to see the list of primes? (Y/n) ";
            std::cin >> pref;

            if (pref == "Y") {

                for (int i = 0; i < primes.size(); i++) {
                
                    std::cout << primes[i] << "\n"; 

                }
            
            }
        
        }
        std::cout << "There are " << primes.size() << " prime numbers between 1 and " << input;
    
    } else {

        std::cout << "Input must be <= 2,000,000,000";

    }

}
