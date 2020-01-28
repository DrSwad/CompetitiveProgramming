/*
	Author: Nayeemul Islam Swad

	Idea:
		- Editorial(2nd Solution idea): https://community.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm388
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int TOTAL_ADJACENT = 7 * 16; // adjancency means when two numbers are not
								   // more than one hex digit apart
int clear_hex_dig[TOTAL_ADJACENT], set_hex_dig[TOTAL_ADJACENT];
int last_5b_vis[1 << 24];

class TelephoneNumbers {
public:
	string kthNumber(int separation, int k) {
		for (int dig_pos = 0, loop_counter = 0; dig_pos < 7; dig_pos++) {
			for (int dig_val = 0; dig_val < 16; dig_val++, loop_counter++) {
				clear_hex_dig[loop_counter] = ~(15 << (dig_pos * 4));
				set_hex_dig[loop_counter] = dig_val << (dig_pos * 4);
			}
		}

		int at_number = 0;

		while (true) {
			if (separation >= 2) { // Check if at_number is at distance 1 from
								   // some already selected number
				int exclude_last_5b = at_number >> 5;
				if ((last_5b_vis[exclude_last_5b] & (1 << (31 & at_number))) != 0) {
					// move to the next number by skipping the last (separation - 2)
					// hex digits
					at_number |= (1 << ((separation - 2) * 4)) - 1;
					at_number++;
					continue;
				}
			}

			if (separation == 3) { // Check all the adjacent numbers of at_number
								   // to see if they are adjacent to some already
								   // selected number
				bool flag = true;
				for (int i = 0; i < TOTAL_ADJACENT; i++) {
					int adjacent_number = (at_number & clear_hex_dig[i]) | set_hex_dig[i];
					int exclude_last_5b = adjacent_number >> 5;
					if ((last_5b_vis[exclude_last_5b] & (1 << (31 & adjacent_number))) != 0) {
						// move to the next number
						at_number++;
						flag = false;
						break;
					}
				}
				if (!flag) continue;
			}

			if (--k == 0) {
				string ret = "";
				while (at_number != 0) {
					int dig = at_number % 16;
					ret = (char)(dig < 10 ? '0' + dig : 'a' + dig - 10) + ret;
					at_number /= 16;
				}
				while (ret.length() < 7) ret = "0" + ret;
				return ret;
			}

			if (separation >= 2) { // Mark all adjacent numbers of at_numbers as visible
				for (int i = 0; i < TOTAL_ADJACENT; i++) {
					int adjacent_number = (at_number & clear_hex_dig[i]) | set_hex_dig[i];
					int exclude_last_5b = adjacent_number >> 5;
					last_5b_vis[exclude_last_5b] |= (1 << (31 & adjacent_number));
				}
			}

			// move to the next number by skipping the last (separation - 1)
			// hex digits
			at_number |= (1 << ((separation - 1) * 4)) - 1;
			at_number++;
		}
	}
};
