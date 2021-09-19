// C++ program to implement persistent segment
// tree.
#include "bits/stdc++.h"

using namespace std;

#define MAXN 100

struct node {
    // stores sum of the elements in node
    int val;

    node* left, * right;

    node() {}

    node(node* l, node* r, int v) {
        left = l;
        right = r;
        val = v;
    }
};

// input array
//std::vector<int> arr;
int arr[MAXN];

// root pointers for all versions
node* version[MAXN];

void build(node* n, int low, int high) {
    if (low == high) {
        n->val = arr[low];
        return;
    }
    int mid = (low + high) / 2;
    n->left = new node(nullptr, nullptr, 0);
    n->right = new node(nullptr, nullptr, 0);
    build(n->left, low, mid);
    build(n->right, mid + 1, high);
    n->val = n->left->val + n->right->val;
}

/**
* Upgrades to new Version
* @param prev : points to node of previous version
* @param cur : points to node of current version
* Time Complexity : O(logn)
* Space Complexity : O(logn) */
void upgrade(node* prev, node* cur, int low, int high,
             int idx, int value) {
    if (idx > high or idx < low or low > high)
        return;

    if (low == high) {
        // modification in new version
        cur->val = value;
        return;
    }
    int mid = (low + high) / 2;
    if (idx <= mid) {
        // link to right child of previous version
        cur->right = prev->right;

        // create new node in current version
        cur->left = new node(nullptr, nullptr, 0);

        upgrade(prev->left, cur->left, low, mid, idx, value);
    } else {
        // link to left child of previous version
        cur->left = prev->left;

        // create new node for current version
        cur->right = new node(nullptr, nullptr, 0);

        upgrade(prev->right, cur->right, mid + 1, high, idx, value);
    }

    // calculating data for current version
    // by combining previous version and current
    // modification
    cur->val = cur->left->val + cur->right->val;
}

int query(node* n, int low, int high, int l, int r) {
    if (l > high or r < low or low > high)
        return 0;
    if (l <= low and high <= r)
        return n->val;
    int mid = (low + high) / 2;
    int p1 = query(n->left, low, mid, l, r);
    int p2 = query(n->right, mid + 1, high, l, r);
    return p1 + p2;
}

int main() {
    std::vector<int> A = {1, 2, 3, 4, 5};
    int n = A.size();
//    arr.resize(n);
    for (int i = 0; i < n; i++)
        arr[i] = A[i];

    node* root = new node(nullptr, nullptr, 0);
    build(root, 0, n - 1);

    // storing root node for version-0
    version[0] = root;

    // upgrading to version-1
    version[1] = new node(nullptr, nullptr, 0);
    upgrade(version[0], version[1], 0, n - 1, 4, 1);

    // upgrading to version-2
    version[2] = new node(nullptr, nullptr, 0);
    upgrade(version[1], version[2], 0, n - 1, 2, 10);

    cout << "In version 1 , query(0, 4) : ";
    cout << query(version[1], 0, n - 1, 0, 4) << endl;

    cout << "In version 2 , query(3,4) : ";
    cout << query(version[2], 0, n - 1, 3, 4) << endl;

    cout << "In version 0 , query(0,3) : ";
    cout << query(version[0], 0, n - 1, 0, 3) << endl;
    return 0;
}
