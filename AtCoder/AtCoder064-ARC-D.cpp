/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Since we can't remove the first and last characters, they also need to
		be the first and last characters in the final state before one player loses.

		- The losing state of a game must be of the form ABABABABABAB.....

		- If the last two characters are same, then the final state must be of the
		form A(BABABAB...)A which is odd in length.
		  And if the last two characters are different, then the final state must
		be of the form A(BABABA...)B which is even in length.

		- Finally, if we know the parities of the lengths of the final state and the
		initial string, then we can figure out the parity of the number of moves
		played, and thus, the winner of the game.
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 1e5 + 10;

string s;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> s;
	int len = s.length();
	if (s[0] == s[len - 1] && len % 2 == 0) cout << "First\n";
	else if (s[0] != s[len - 1] && len % 2 == 1) cout << "First\n";
	else cout << "Second\n";

	return 0;
}
