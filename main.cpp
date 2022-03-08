#include <iostream>
#include <cmath>
#include <vector>

// 12.01.2022 - Program hashujący
// Autor: Mariusz Gworek
// Informatyka II rok III semestr

class Hasher {
	public:
		std::string hash(const std::string &text);
	private:
		const short HASH_LENGTH = 64;
		const short KEY_LENGTH = 16;
		const short SEED_MULTIPLIER = 3;
		// 33 -> !
		const short ASCII_START = 33;
		// 127 -> DEL, the last char we use is 126 -> ~
		const short ASCII_LIMIT = 127;
		long generateHashSeed(const std::string &text);
		std::string generateHashKey(const long &seed, const std::string &text);
		std::vector<int> generatePrimeVector(const int &count);
		bool isPrime(const int &num);
};

std::string Hasher::hash(const std::string &text) {
	std::string hashed = "";
	int index;
	int tempChar;
	std::string hashKey = generateHashKey(generateHashSeed(text), text);
	const int TXT_LEN = text.length();
	const int HASH_KEY_LENGTH = hashKey.length();

	while (hashed.length() < HASH_LENGTH) {
		// This saves the absolute value of addition of the current char from text variable
		// with the current char from the hashKey variable
		// (It always adds index to the hashKey variable to prevent hash repetition)
		// then it limits the char value to constant ASCII_LIMIT
		tempChar = abs(text[index % TXT_LEN] + (hashKey[index % HASH_KEY_LENGTH] + index)) % ASCII_LIMIT;
		// This checks whether the value of a character is less than ASCII_START constant
		// if so, then it adds the mentioned constant to it
		if (tempChar < ASCII_START) tempChar += ASCII_START;
		hashed += tempChar;
		index++;
	}

	return hashed;
}

long Hasher::generateHashSeed(const std::string &text) {
	// This method generated the seed
	// it takes in a reference to text
	long temp;
	long seed = 0;
	// Function length() for variable text used more than once
	// saved into TXT_LEN constans
	const short TXT_LEN = text.length();
	// vector size of TXT_LEN containing prime numbers
	std::vector<int> primeVec = generatePrimeVector(TXT_LEN);

	for (int i = 0; i < TXT_LEN; i++) {
		// This raises the value of current character to the power of constant SEED_MULTIPLIER
		// then it shifts the bits to the right by the current prime number from primeVec vector
		temp = ((long) (pow(text[i], SEED_MULTIPLIER)) >> primeVec.at(i));
		seed += temp;
	}

	return seed;
}

std::string Hasher::generateHashKey(const long &seed, const std::string &text) {
	// This method generates a hash key
	// it takes in references to seed and text
	std::string key = "";
	int index = 0;
	int ascii = 0;

	// This loop repeats until the key isn't the length declared in the constant KEY_LENGTH
	while (key.length() < KEY_LENGTH) {
		// This shifts the bits of seed variable by the value of current ascii character
		ascii += (seed >> text[index]);
		// This prevents the possibility of a negative value occuring in the ascii variable
		// using the abs() function.
		// Also the maximum value of character is limited by ASCII_LIMIT constant
		// and the modulus operator
		ascii = abs(ascii % ASCII_LIMIT);

		// This limits the minimal character value using the ASCII_START constant
		// when the character value is less than this constant
		// it adds this constant to the value
		// ex. if ascii == 10, then ascii += ASCII_START
		if (ascii < ASCII_START) ascii += ASCII_START;
		// This prevents characters from reoccuring after themselves
		// if the previous character has the same value as the current one
		// then we add a square root of constant KEY_LENGTH to it
		if ((int) key[index - 1] == ascii) ascii += (int) sqrt(KEY_LENGTH);

		key += ascii;
		index++;
	}

	return key;
}

std::vector<int> Hasher::generatePrimeVector(const int &count) {
	// This function generates a vector of a set length containing prime numbers
	std::vector<int> primeVec;
	int generated = 0;
	int currentNum = 2;

	while (generated < count) {
		if (isPrime(currentNum)) {
			primeVec.push_back(currentNum);
			generated++;
		}
		currentNum++;
	}

	return primeVec;
}

bool Hasher::isPrime(const int &num) {
	bool PRIME_FLAG = true;
	if (num < 2) return false;
	for (int i = 2; i <= num/2; i++) {
		if (num % i == 0) PRIME_FLAG = false;
	}

	return PRIME_FLAG;
}

int main() {
	std::string str;
    short int mode;

    while (true) {
        mode = 0;
        Hasher *hasher = new Hasher();
        
        system("cls");

        std::cout << "-----------------------------------------\n";
        std::cout << "|                                       |\n";
        std::cout << "|         HASHING ALGORITHM 1.0         |\n";
        std::cout << "|             Mariusz Gworek            |\n";
        std::cout << "|                                       |\n";
        std::cout << "-----------------------------------------\n\n";
        std::cout << "Select an option\n";
        std::cout << "-----------------\n\n";
        std::cout << "1. Hash\n";
        std::cout << "--------\n";
        std::cout << "2. Exit\n\n";
        std::cout << "Choice: ";
        std::cin >> mode;
        if (mode == 2) {
        	delete hasher;
            break;
        }
        std::cin.ignore();

        system("cls");

        std::cout << "Please input a string: ";
        std::getline(std::cin, str);

        std::cout << "\nHashed: " << hasher->hash(str) << "\n";

        std::cout << "\nPress any key to continue...";
        std::getchar();
    }

	return 0;
}
