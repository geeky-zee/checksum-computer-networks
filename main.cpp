#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <bitset>

using namespace std;

// ---------- Compute Checksum ----------
unsigned short computeChecksum(vector<unsigned char> data) {
    unsigned int totalSum = 0;

    if (data.size() % 2 != 0) {
        data.push_back(0x00); // Padding
    }

    for (size_t i = 0; i < data.size(); i += 2) {
        unsigned short word = (data[i] << 8) | data[i + 1];
        totalSum += word;

        if (totalSum > 0xFFFF) {
            totalSum = (totalSum & 0xFFFF) + 1; // Wrap around
        }
    }

    return ~(totalSum & 0xFFFF) & 0xFFFF; // One's complement
}

// ---------- Verify Checksum ----------
unsigned short verifyChecksum(vector<unsigned char> data) {
    unsigned int totalSum = 0;

    if (data.size() % 2 != 0) {
        data.push_back(0x00); // Padding
    }

    for (size_t i = 0; i < data.size(); i += 2) {
        unsigned short word = (data[i] << 8) | data[i + 1];
        totalSum += word;

        if (totalSum > 0xFFFF) {
            totalSum = (totalSum & 0xFFFF) + 1; // Wrap around
        }
    }

    return totalSum & 0xFFFF; // Should be 0xFFFF if valid
}

// ---------- Convert Binary String to Bytes ----------
vector<unsigned char> binaryStringToBytes(const string& binary) {
    vector<unsigned char> bytesList;

    for (size_t i = 0; i < binary.length(); i += 8) {
        string byteString = binary.substr(i, 8);
        if (byteString.length() < 8) {
            byteString.append(8 - byteString.length(), '0'); // Padding
        }

        unsigned char byte = static_cast<unsigned char>(bitset<8>(byteString).to_ulong());
        bytesList.push_back(byte);
    }

    return bytesList;
}

// ---------- Introduce Errors ----------
void introduceErrors(vector<unsigned char>& data, int numErrors) {
    for (int i = 0; i < numErrors; ++i) {
        int byteIndex = rand() % data.size();
        int bitIndex = rand() % 8;
        data[byteIndex] ^= (1 << bitIndex); // Flip bit
    }
}

// ---------- Simulate Binary Transmission ----------
void simulateBinaryTransmission(const string& binaryInput) {
    cout << "\n=== Transmission Simulation Steps ===\n";

    // Step 1: Original Input
    cout << "\nStep 1: Original Binary Input\n-----------------------------\n";
    cout << binaryInput << endl;

    vector<unsigned char> data = binaryStringToBytes(binaryInput);

    // Step 2: Binary to Bytes
    cout << "\nStep 2: Binary Data Converted to Bytes\n-------------------------------------\n";
    for (auto byte : data) {
        cout << bitset<8>(byte) << " ";
    }
    cout << endl;

    // Step 3: Compute Checksum
    unsigned short checksum = computeChecksum(data);
    cout << "\nStep 3: Checksum Computation\n---------------------------\n";
    cout << "Sender Checksum (Hex):    0x" << hex << uppercase << setw(4) << setfill('0') << checksum << endl;
    cout << "Sender Checksum (Binary): " << bitset<16>(checksum) << endl;

    // Step 4: Append Checksum
    cout << "\nStep 4: Data with Checksum Appended (16-bit words)\n--------------------------------------------------\n";
    data.push_back((checksum >> 8) & 0xFF);
    data.push_back(checksum & 0xFF);
    for (size_t i = 0; i < data.size(); i += 2) {
        unsigned short word = (data[i] << 8) | data[i + 1];
        cout << bitset<16>(word) << endl;
    }

    // Step 5: Error Simulation
    int errorBits = 0;
    int randomChance = rand() % 100;
    if (randomChance < 10) {
        errorBits = rand() % 3 + 1;
        introduceErrors(data, errorBits);
        cout << "\n⚠  Noise simulated: " << errorBits << " bit error(s) introduced.\n";
    } else {
        cout << "\n✅ Clean transmission (no errors introduced).\n";
    }

    // Step 6: Verification
    cout << "\nStep 5: Receiver's Verification\n------------------------------\n";
    unsigned short receiverSum = verifyChecksum(data);
    cout << "Receiver Sum (Hex):    0x" << hex << uppercase << setw(4) << setfill('0') << receiverSum << endl;
    cout << "Receiver Sum (Binary): " << bitset<16>(receiverSum) << endl;

    cout << "\nStep 6: Verification Result\n--------------------------\n";
    if (receiverSum == 0xFFFF) {
        cout << "✅ Checksum Validation (Binary): 1111111111111111\n";
        cout << "✅ No error detected. Data is valid.\n";
    } else {
        cout << "❌ Checksum Validation (Binary): " << bitset<16>(receiverSum) << endl;
        cout << "❌ Error detected! Data is corrupted.\n";
    }
}

// ---------- Main Function ----------
int main() {
    srand(time(0));
    string choice;

    while (true) {
        cout << "\n==================================================\n";
        cout << "🌐 Internet Checksum (Binary Input) Simulator 🌐\n";
        cout << "==================================================\n";
        cout << "📜 Course: Computer Networks\n";
        cout << "📜 Submitted by:\n";
        cout << "   - Muhammad Zeshan \n";
        cout << "==================================================\n";
        cout << "\n📌 Menu Options:\n";
        cout << "  1. 🔢 Enter binary data and calculate checksum\n";
        cout << "  2. 🚪 Exit\n";
        cout << "==================================================\n";
        cout << "Choose an option (1 or 2): ";
        getline(cin, choice);

        if (choice == "1") {
            string binaryInput;
            cout << "\nEnter binary string (only 0s and 1s, e.g., 0100100001100101):\n";
            getline(cin, binaryInput);

            bool valid = true;
            for (char c : binaryInput) {
                if (c != '0' && c != '1') {
                    valid = false;
                    break;
                }
            }

            if (!valid) {
                cout << "❌ Invalid binary input. Only 0s and 1s are allowed.\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                continue;
            }

            simulateBinaryTransmission(binaryInput);
            cout << "\nPress Enter to return to the main menu...";
            cin.ignore();
        } else if (choice == "2") {
            cout << "\n👋 Thank you for using the Internet Checksum Simulator. Goodbye!\n";
            break;
        } else {
            cout << "\n❌ Invalid choice. Please select a valid option.\n";
        }
    }

    return 0;
}
