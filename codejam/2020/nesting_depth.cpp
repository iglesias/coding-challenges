#include <bits/stdc++.h>

/* sample input at the bottom */

using namespace std;

struct node
{
    node(char _c) : org(_c), c(_c) {}

    char org; // original digit value
    char c;
};

list<node> init_list(const string& S)
{
    list<node> l;
    for (char c : S)
    {
        l.push_back(node(c));
    }

    return l;
}

string list_to_string(const list<node>& l)
{
    string str;
    for (const node& n : l)
    {
        if (n.c == '(' || n.c == ')')
            str.push_back(n.c);
        else
            str.push_back(n.org);
    }

    return str;
}

bool is_nonzero_digit(char c)
{
    if (c == '(' || c == ')') return false;
    assert(c >= '0' && c <= '9');
    return c != '0';
}

bool is_all_zeroes(const list<node>& l)
{
  for (const node& n : l)
    if (is_nonzero_digit(n.c)) return false;

  return true;
}

void add_parentheses(list<node>& l)
{
    auto left = l.begin();
    while (true)
    {
        // Find next non-zero.
        //while (left != l.end() && left->c == '0')
        while (left != l.end() && !is_nonzero_digit(left->c))
            advance(left, 1);
        if (left == l.end())
        {
          if (is_all_zeroes(l)) break;
          left = l.begin();
          continue;
        }

        // From left, find last consecutive non-zero.
        auto right = left;
        //while (right != l.end() && right->c != '0')
        while (right != l.end() && is_nonzero_digit(right->c))
            advance(right, 1);

        for (auto it = left; it != right; advance(it, 1))
        {
            assert(is_nonzero_digit(it->c));
            int digit = it->c - '0';
            assert(digit > 0 && digit <= 9);
            int new_digit = digit-1;
            it->c = '0' + new_digit;
        }

        l.insert(left, '(');
        l.insert(right, ')');

        left = right;
    }
}

int main()
{
    int T;
    cin >> T;

    for (int t=0; t<T; t++)
    {
        string S;
        cin >> S;

        list<node> l = init_list(S);

        add_parentheses(l);

        string ans = list_to_string(l);
        cout << "Case #" << t+1 << ": " << ans << '\n';
    }
}

/*

8
0000
101
111000
1
021
312
4
221

*/
