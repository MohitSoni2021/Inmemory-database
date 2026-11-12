#include <iostream>
#include "core.hpp"

using namespace std;

int main() {

    cout << "Enter the key to store : " << endl;
    string key;
    cin >> key;
    cout << "Enter the value to store : " << endl;
    string value;
    cin >> value;

    Redis redis;

    redis.set(key, value);

    cout << "Enter the key to retrieve : " << endl;
    vector<string> keys = redis.keys();

    // listing counter;
    int cnt = 0;
    for (const auto& k : keys) {
        cout << cnt << " : " << k << endl;
        cnt++;
    };
    
    cout << "Enter the index of the key to retrieve : " << endl;
    int userChoise;
    cin >> userChoise;

    if (userChoise < 0 || userChoise >= keys.size()) {
        cout << "Invalid index. Exiting..." << endl;
        return 1;
    }else {
        string selectedKey = keys[userChoise];
        cout << "Retrieving value for key: " << selectedKey << endl;
        JsonResponse response = redis.get(selectedKey);
        if (response.found) {
            cout << "Value: " << response.value << endl;
        } else {
            cout << "Key not found." << endl;
        }
    }

    return 0;
}