#include <cstdio>
#include <cctype>

using namespace std;

struct Node
{
    Node* left;
    Node* right;
    Node(int v) : left(nullptr), right(nullptr), val(v) {}
    void addNode(Node* const n)
    {
        if (n->val < this->val) {
            if (left == nullptr) left = n;
            else left->addNode(n);
        }
        else if (n->val > this->val) {
            if (right == nullptr) right = n;
            else right->addNode(n);
        }
    }
    const int getVal() const noexcept {return val;}
private:
    int val;
};

struct Tree
{
    Tree() : root(nullptr) {}
    ~Tree() {if(root != nullptr) delete root;}
    void addVal(const int val)
    {
        Node* temp = new Node(val);
        if (root == nullptr)
            root = temp;
        else {
            root->addNode(temp);
        }
    }
    void symTraverse(Node* const  n) const  // symmetrical travers
    {
        if (n->left != nullptr) symTraverse(n->left);
        printf("%d\n", n->getVal());
        if (n->right != nullptr) symTraverse(n->right);

    }
    void fwdTraverse(Node* const n) const   // forward travers
    {
        printf("%d\n", n->getVal());
        if (n->left != nullptr) fwdTraverse(n->left);
        if (n->right != nullptr) fwdTraverse(n->right);

    }
    void bwdTraverse(Node* const n) const   // backward travers
    {
        if (n->left != nullptr) bwdTraverse(n->left);
        if (n->right != nullptr) bwdTraverse(n->right);
        printf("%d\n", n->getVal());

    }
    void showTreeVals(char ch) const
    {
        if (root == nullptr) return;
        switch (ch){
            case 's' :
            symTraverse(root);
            break;
            case 'f' :
            fwdTraverse(root);
            break;
            case 'b' :
            bwdTraverse(root);
        }
    }

    void searchTree(Node* const n, const int& val, bool& b)
    {
        if (n->getVal() == val) b = true;
        if (val < n->getVal() && n->left != nullptr)
            searchTree(n->left, val, b);
        if (val > n->getVal() && n->right != nullptr)
            searchTree(n->right, val, b);
    }

    void findVal(int val)
    {
        bool isFound = false;
        searchTree(root, val, isFound);
        if (isFound)
            puts("This value is in the tree.");
        else
            puts("This value is NOT in the tree.");
    }

    Node* root;
};

void showTreeContents(Tree& t);
void showValExist(Tree& t);

int main()
{
    Tree tree;
    tree.addVal(5);
    tree.addVal(3);
    tree.addVal(9);
    tree.addVal(6);
    tree.addVal(1);
    tree.addVal(4);

    showValExist(tree);
    showTreeContents(tree);

    return 0;
}

void showTreeContents(Tree& t)
{
    puts ("Select 's', 'f' or 'b' for the type of tree traversing.");
    char ch;
        do {
        ch = getchar();
        ch = tolower(ch);
        if (ch == 's' || ch == 'f' || ch == 'b'){
            t.showTreeVals(ch);
            break;
        }
    } while(true);
}

void showValExist(Tree& t)
{
    puts("Input a number.");
    int num;
    scanf("%d", &num);
    //printf("%d\n", num);
    t.findVal(num);
}
