#include <iostream>
#include <string>
// TRIE IMPLEMENTATION
using namespace std;

struct Node
{
    Node *children[26] = {};
    int pass = 0, ends = 0;

    bool containsKey(char c)
    {
        return this->children[c - 'a'] != nullptr;
    }
    ~Node()
    {
        for (Node *child : children)
            delete child;
    }
};

class Trie
{
    Node *root;

public:
    Trie()
    {
        this->root = new Node();
    }

    ~Trie()
    {
        delete root;
    }

    void insert(const string &s)
    {
        ++root->pass;
        Node *node = root;
        for (char c : s)
        {
            if (!node->containsKey(c))
            {
                node->children[c - 'a'] = new Node();
            }
            node = node->children[c - 'a'];
            ++node->pass;
        }
        ++node->ends;
    }

    bool search(const string &s)
    {
        Node *node = root;
        for (char c : s)
        {
            if (node->containsKey(c))
            {
                node = node->children[c - 'a'];
            }
            else
                return false;
        }
        return node->ends;
    }

    int countSearch(const string &s)
    {
        Node *node = root;
        for (char c : s)
        {
            if (node->containsKey(c))
                node = node->children[c - 'a'];
            else
                return 0;
        }
        return node->ends;
    }

    bool startsWith(const string &prefix)
    {
        Node *node = root;
        for (char c : prefix)
        {
            if (node->containsKey(c))
                node = node->children[c - 'a'];
            else
                return false;
        }
        return node->pass;
    }

    int startsWithCount(const string &prefix)
    {
        Node *node = root;
        for (char c : prefix)
        {
            if (node->containsKey(c))
                node = node->children[c - 'a'];
            else
                return 0;
        }
        return node->pass;
    }

    void erase(const string &s)
    {

        if (!search(s))
        {
            cout << "Word " << s << " does not exist.\n";
            return;
        }
        Node *node = root;

        for (char c : s)
        {
            --node->pass;
            node = node->children[c - 'a'];
        }
        --node->pass;
        --node->ends;
    }

    // prints the trie like the `tree` command, '*' marks the end of a word
    // nodes with pass == 0 are left over from erased words, so they are skipped
    void printTree(Node *node, const string &prefix)
    {
        int last = -1;
        for (int i = 0; i < 26; i++)
            if (node->children[i] != nullptr && node->children[i]->pass > 0)
                last = i;

        for (int i = 0; i <= last; i++)
        {
            Node *child = node->children[i];
            if (child == nullptr || child->pass == 0)
                continue;
            bool isLast = (i == last);
            cout << prefix << (isLast ? "└── " : "├── ") << (char)('a' + i) << (child->ends ? " *" : "") << endl;
            printTree(child, prefix + (isLast ? "    " : "│   "));
        }
    }

    void print()
    {
        cout << "(root)" << endl;
        printTree(root, "");
    }
};

// int main(int argc, char **argv)
// {
//     Trie trie = Trie();
//     trie.insert("apple");
//     trie.insert("apps");
//     trie.insert("fox");
//     trie.print();
// }