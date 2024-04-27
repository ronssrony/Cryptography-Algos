#include <iostream>
#include <random>
#include <cmath>
#include <string>
#include <sstream> // Include the <sstream> header for std::stringstream

using namespace std;

// Function to check if a number is prime
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

// Function to generate a random prime number within a specified range
int generate_prime(int min_val, int max_val) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(min_val, max_val);

    int num = distribution(gen);
    while (!is_prime(num)) {
        num = distribution(gen);
    }
    return num;
}

// Function to compute the greatest common divisor (GCD) using Euclid's algorithm
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Function to compute modular exponentiation (base^exp % mod) using iterative method
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) // If exp is odd, multiply base with result
            result = (result * base) % mod;
        exp = exp >> 1;   // exp = exp / 2
        base = (base * base) % mod; // base = base^2 % mod
    }
    return result;
}

// Function to generate RSA key pair (public key: (e, n), private key: (d, n))
void generate_keypair(int& e, int& d, int& n) {
    // Choose two distinct prime numbers p and q
    int p = generate_prime(50, 100); // Random prime between 50 and 100
    int q = generate_prime(100, 150); // Random prime between 100 and 150

    // Compute n = p * q
    n = p * q;

    // Compute Euler's totient function φ(n) = (p-1) * (q-1)
    int phi_n = (p - 1) * (q - 1);

    // Choose public exponent e such that 1 < e < φ(n) and gcd(e, φ(n)) = 1
    e = 2;
    while (e < phi_n) {
        if (gcd(e, phi_n) == 1)
            break;
        e++;
    }

    // Compute private exponent d such that (d * e) % φ(n) = 1
    d = 1;
    while (true) {
        if ((d * e) % phi_n == 1)
            break;
        d++;
    }
}

// Function to perform RSA encryption
string rsa_encrypt(const string& plaintext, int e, int n) {
    string ciphertext = "";
    for (char ch : plaintext) {
        int pt = static_cast<int>(ch);
        int ct = mod_exp(pt, e, n);
        ciphertext += to_string(ct) + " ";
    }
    return ciphertext;
}

// Function to perform RSA decryption
string rsa_decrypt(const string& ciphertext, int d, int n) {
    string decrypted_text = "";
    stringstream ss(ciphertext); // Use std::stringstream for tokenizing

    string token;
    while (ss >> token) { // Tokenize the input ciphertext string
        int ct = stoi(token);
        int pt = mod_exp(ct, d, n);
        decrypted_text += static_cast<char>(pt);
    }
    return decrypted_text;
}

int main() {
    int e, d, n;
    generate_keypair(e, d, n);

    cout << "RSA Key Generation:" << endl;
    cout << "Public Key (e, n): " << e << ", " << n << endl;
    cout << "Private Key (d, n): " << d << ", " << n << endl;

    string plaintext ;
  getline(cin, plaintext);
    cout << "Original Text: " << plaintext << endl;

    string ciphertext = rsa_encrypt(plaintext, e, n);
    cout << "Encrypted Text: " << ciphertext << endl;

    string decrypted_text = rsa_decrypt(ciphertext, d, n);
    cout << "Decrypted Text: " << decrypted_text << endl;

    return 0;
}
