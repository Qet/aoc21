#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


char getFirst(uint16_t charPair)
{
	return static_cast<char>(charPair);
}

char getSecond(uint16_t charPair)
{
	return static_cast<char>(charPair >> 8);
}

uint16_t getCharPair(char A, char B)
{
	return (((uint16_t)A) << 8) + ((uint16_t)B);
}

class processing
{
	const map<uint16_t, char>& rules;
	string templ;
	map<char, uint64_t> counts;

public:

	processing(const map<uint16_t, char>& rules, string templ) : rules(rules), templ(templ)
	{
		for (char c = 'A'; c <= 'Z'; c++)
		{
			counts[c] = 0;
		}

		//Each char is counted in the initial map. 
		for (int i = 0; i < templ.length(); i++)
		{
			char curItem = templ.substr(i, 1)[0];
			counts[curItem]++;
		}
	}
	
	void go()
	{
		//expand each initial pair in the template.
		for (int i = 0; i < templ.length() - 1; i++)
		{
			string curItem = templ.substr(i, 2);
			cout << "Starting initial pair: " << curItem << endl;
			expand(curItem[0], curItem[1], 0);
		}
	}

	void expand(char A, char B, int depth)
	{
		static uint64_t count = 0;
		if (depth < 40)
		{
			auto r = rules.find(getCharPair(A, B));
			if (r != rules.end())
			{
				char insertingChar = r->second;
				counts[insertingChar]++;

				expand(A, insertingChar, depth + 1);
				expand(insertingChar, B, depth + 1);
			}
		}
		count++;
		if (count % 100000000 == 0)
		{
			cout << count/1000000 << "M" <<  endl;
		}
	}

	void printResults()
	{
		uint64_t largest = 0, smallest = UINT64_MAX;
		for (pair<char, uint64_t> item : counts)
		{
			cout << "Letter " << item.first << " count: " << item.second << endl;
			if (item.second < smallest && item.second > 0)  //have to exclude the zeroed-out alphabet as some letters are never included.
				smallest = item.second;
			if (item.second > largest)
				largest = item.second;
		}
		cout << "Largest minus smallest = " << (largest - smallest) << endl;
	}

};



int main()
{
	string templ;
	cin >> templ;

	map<uint16_t, char> rules;

	string ruleL, ruleR, spacer;

	while (cin >> ruleL && cin >> spacer && cin >> ruleR)
	{
		rules.insert({ getCharPair(ruleL[0], ruleL[1]) , ruleR[0] });
	}

	processing p(rules, templ);
	p.go();
	p.printResults();

}
