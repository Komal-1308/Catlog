#include <bits/stdc++.h>
#include "nlohmann/json.hpp"   // include JSON library

using namespace std;
using json = nlohmann::json;

// Function to decode y-value given a base and string representation
long long decodeY(int base, const string &val) {
    long long result = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c))
            digit = c - '0';
        else if (isalpha(c))
            digit = toupper(c) - 'A' + 10; // support bases > 10
        else
            throw invalid_argument("Invalid character in value string.");

        if (digit >= base)
            throw invalid_argument("Digit out of range for the given base.");

        result = result * base + digit;
    }
    return result;
}

// Function to compute secret c = f(0) using Lagrange interpolation
double lagrangeAtZero(const vector<pair<int, long long>>& points, int k) {
    double total = 0.0;
    for (int i = 0; i < k; i++) {
        double xi = points[i].first;
        double yi = points[i].second;

        double num = 1.0, den = 1.0;
        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            double xj = points[j].first;
            num *= (0.0 - xj);
            den *= (xi - xj);
        }
        total += yi * (num / den);
    }
    return total;
}

int main() {
    // Load JSON file
    ifstream file("polynomial_roots_input.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open JSON file.\n";
        return 1;
    }

    json j;
    file >> j;

    // Extract keys
    int n = j["keys"]["n"];
    int k = j["keys"]["k"];
    cout << "n = " << n << ", k = " << k << "\n\n";

    // Store decoded points
    vector<pair<int, long long>> points;

    // Iterate through JSON entries
    for (auto& [key, val] : j.items()) {
        if (key == "keys") continue; // skip meta info

        int x = stoi(key);                          // root key as integer
        int base = stoi(val["base"].get<string>()); // base as integer
        string value = val["value"];                // value as string

        long long y = decodeY(base, value);         // decode y
        points.push_back({x, y});

        cout << "x = " << x 
             << ", base = " << base 
             << ", value = " << value 
             << " => decoded y = " << y << "\n";
    }

    // Sort points by x (important for consistency)
    sort(points.begin(), points.end());

    // Compute secret c using first k points
    vector<pair<int, long long>> subset(points.begin(), points.begin() + k);
    double c = lagrangeAtZero(subset, k);

    cout << "\nComputed secret c = f(0) = " << c << "\n";

    return 0;
}
