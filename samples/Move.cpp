#include "Log.h"
#include "Timestamp.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace mars;

int main() {

	std::vector<int> list;
	std::vector<int> other_list;

	for (int i = 0; i < 1000000000; i ++) {
		list.push_back(i);
	}

	long long begin = Timestamp::now().macrosecond();
	other_list = list;
	long long end   = Timestamp::now().macrosecond();

	DEBUG << "Spend Time => " << (end - begin);

	std::vector<int> list_a;
	std::vector<int> other_list_a;

	for (int i = 0; i < 1000000000; i ++) {
		list_a.push_back(i);
	}

	long long begin_a = Timestamp::now().macrosecond();
	other_list_a = std::move(list_a);
	long long end_a   = Timestamp::now().macrosecond();

	DEBUG << "Spend Time => " << (end_a - begin_a);
	for (int i = 0 ;i < 100; i ++) {
		DEBUG << "i = > " << other_list_a[i];

	}

	return 0;
}
