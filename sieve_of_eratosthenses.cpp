#include <iostream>  
#include <vector>

std::vector<int> sieve(int max) {
    
    // Creating a vector of bools with true/false on the corresponding index. True if prime and false if composite
    std::vector<bool> nums(max, true);

    for (int p = 2; p*p <= max; p++) { 
        
        // Setting all multiples of prime numbers to false
        if (nums[p]) {

            for (int i = p*p; i <= max; i += p) {

                nums[i] = false;

            } 
        
        }

    }

    // Creating the primes vector by checking if the index of nums is true or false
    std::vector<int> primes;
    for (int i = 2; i <= max; i++) {
       
        if (nums[i]) {
            
            primes.push_back(i); 

        }

    }
    return primes;

}

int main() { 
    
    // Taking user input to see what number they want to find the primes up to, and if they want to see the numbers or how many there are
    int input;
    std::string pref;
    std::cout << "Range: 1, ";
    std::cin >> input;

    // Only running the program if the input is less than or equal to 2,000,000,000
    if (input <= 2000000000) {
        
        std::vector<int> primes = sieve(input); 

        // Only giving the option to print the primes if the input is less than or equal to 1,000,000
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
