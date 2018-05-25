#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

void straightCounter(vector<int> cards, int index, int &counter) {
	if(cards.at(index) + 1 == cards.at(index + 1)) ++counter;
	else if(counter < 5 && cards.at(index) != cards.at(index + 1)) counter = 1;
} 

int main() {
	vector<int> cards(52);
	srand((unsigned)time(NULL));
	
	//------------------------------------------------------
	//此為如何隨機把1~13、101~113、201~213、301~313的方法：
	//當放入第n項的值的時候，並另外隨機產生0~51的值，
	//如果產隨機生的值不等於n，則第n項與該隨機值取得的值對調 
	//------------------------------------------------------ 
	for(int i = 0; i <= 3; i++) {
		for(int j = 1; j <= 13; j++) {
			int index = i * 13 + j - 1;
			int randomIndex = rand() % 52;
			int indexValue = i * 100 + j;
			
			//做「項的值是否為0的檢查」是因為之前在對調之後可能已在較高的項產生了值，
			//也有可能是隨機項的項數小於當前項的項數的緣故(畢竟前面的項都已經有了值)， 
			//而採取不同的對調方法。 
			if(randomIndex != index) {
				if(cards[randomIndex] == 0) {
					if(cards[index] == 0) {
						cards[randomIndex] = indexValue;
					}
					else {
						cards[randomIndex] = cards.at(index);
					}
					cards[index] = (randomIndex / 13) * 100 + (randomIndex % 13) + 1;
				}
				else {
					if(cards[index] == 0) {
						cards[index] = cards.at(randomIndex);
						cards[randomIndex] = indexValue;
					}
					else {
						int tmp = cards.at(index);
						cards[index] = cards.at(randomIndex);
						cards[randomIndex] = tmp;
					}
				}
			}
			else {
				if(cards[index] == 0) cards[index] = indexValue;
			}
		}
	}
	
	//------------------------------------------------------
	//順子(Straight)：後項加1等於前項連續出現5次。 
	//葫蘆(Full house)：由於可能會在陣列最底時才出現先2對再3對或先3對再2對的狀況，
	//所以有兩個使用不同順序的葫蘆探測器。 
	//------------------------------------------------------ 
	for(int i = 0; i <= 51; i++) {
		cards[i] %= 100;
	}
	for(int i = 1; i <= 4; i++) {
		int begin = (i - 1) * 13;
		int end = i * 13 - 1;
		
		sort(cards.begin() + begin, cards.end() + 13 * (i - 4));
		
		int straight = 1;
		bool fullhouse_2_3 = false; 
		bool fullhouse_2_3_flag = false;
		bool fullhouse_3_2 = false;
		bool fullhouse_3_2_flag = false;
		
		for(int j = begin; j <= end; j++) {
			//cout << cards.at(j) << "\t"; //Debug
			if(j > begin && cards.at(j) != cards.at(j - 1)) {
				if(j < end - 3) {
					if(cards.at(j) == cards.at(j + 1)) {
						if(fullhouse_3_2) fullhouse_3_2_flag = true;
						if(cards.at(j) == cards.at(j + 2)) {
							if(fullhouse_2_3) fullhouse_2_3_flag = true;
							fullhouse_3_2 = true;
						}
						fullhouse_2_3 = true;
					}
					else {
						straightCounter(cards, j, straight);
					}
				}
				else if(j < end - 1) {
					if(cards.at(j) == cards.at(j + 1)) {
						if(cards.at(j) == cards.at(j + 2)) {
							if(fullhouse_2_3) fullhouse_2_3_flag = true;
						}
						if(fullhouse_3_2) fullhouse_3_2_flag = true;
					}
					else {
						straightCounter(cards, j, straight);
					}
				}
				else if(j < end) {
					if(cards.at(j) == cards.at(j + 1) && fullhouse_3_2) fullhouse_3_2_flag = true;
					straightCounter(cards, j, straight);
				}
			}
			else {
				if(j < end - 3) {
					if(cards.at(j) == cards.at(j + 1)) {
						fullhouse_2_3 = true;
						if(cards.at(j) == cards.at(j + 2)) fullhouse_3_2 = true;
					}
					else {
						straightCounter(cards, j, straight);
					}
				}
				else if(j < end) {
					straightCounter(cards, j, straight);
				}
			}
			
			
			/*
			//展開前的程式碼 
			if(j < end) {
				if(cards.at(j) + 1 == cards.at(j + 1)) ++straight;
				else if(straight < 5 && cards.at(j) != cards.at(j + 1)) straight = 1;
			}
			if(j > begin && cards.at(j) != cards.at(j - 1)) {
				if(fullhouse_2_3 && j < end - 1 && cards.at(j) == cards.at(j + 1) && cards.at(j) == cards.at(j + 2)) fullhouse_2_3_flag = true;
				if(fullhouse_3_2 && j < end && cards.at(j) == cards.at(j + 1)) fullhouse_3_2_flag = true;
			}
			if(j < end - 3) {
				if(cards.at(j) == cards.at(j + 1)) {
					fullhouse_2_3 = true;
					if(cards.at(j) == cards.at(j + 2)) fullhouse_3_2 = true;
				}
			}
			*/
		}
		
		cout << "第" << i << "家有：" ;
		if(straight >= 5) cout << "順子 ";
		if(fullhouse_2_3_flag || fullhouse_3_2_flag) cout << "葫蘆";
		cout << endl;
	}
	return 0;
}
