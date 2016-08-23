#include "Log.h"
#include "Base64.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace mars;

int main() {
	while (true) {
		string binary_data;
		cin >> binary_data;
		if (binary_data == "hehe") break;
		string ascii_data = Base64::encode(binary_data);
		cout << ascii_data << endl;
		cout << Base64::decode(ascii_data) << endl;
	}
	

	return 0;
}