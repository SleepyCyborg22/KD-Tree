#include <bits/stdc++.h>

using namespace std;

struct kd_tree
{
    int k;
    int depth;
    vector<int> coord;
    kd_tree *left;
    kd_tree *right;
};

int c_sort(vector<vector<int>> &a, int l, int r, int d)
{
    if ((r - l) <= 1)
        return 0;
    // for (int i = l; i < r; i++)
    // {
    //     for (int j = i + 1; j < r; j++)
    //     {
    //         if (a[j][d] < a[i][d])
    //         {
    //             vector<int> temp = a[i];
    //             a[i] = a[j];
    //             a[j] = temp;
    //         }
    //     }
    // }

    int m = (l + r) / 2;
    c_sort(a, l, m, d);
    c_sort(a, m, r, d);
    vector<vector<int>> k;
    int i = l, j = m;
    while (i < m && j < r)
    {
        if (a[i][d] <= a[j][d])
        {
            k.push_back(a[i]);
            i++;
        }
        else
        {
            k.push_back(a[j]);
            j++;
        }
    }
    while (i < m)
    {
        k.push_back(a[i]);
        i++;
    }
    while (j < r)
    {
        k.push_back(a[j]);
        j++;
    }

    for(int i=l;i<r;i++)a[i]=k[i-l];

    return 0;
}

kd_tree *build_kd_tree(vector<vector<int>> &inp, int l, int r, int d, int k)
{
    int d1 = d % k;
    c_sort(inp, l, r, d1);
    int m = (l + r) / 2;
    kd_tree *node = new kd_tree;
    while ((m + 1) < r)
    {
        if (inp[m][d1] == inp[m + 1][d1])
            m++;
        else
            break;
    }
    node->k = k;
    node->depth = d;
    node->coord = inp[m];
    if (l != m)
        node->left = build_kd_tree(inp, l, m, d + 1, k);
    else
        node->left = NULL;
    if ((r - m) > 1)
        node->right = build_kd_tree(inp, m + 1, r, d + 1, k);
    else
        node->right = NULL;
    return node;
}

void node_insert(kd_tree *node, vector<int> a)
{
    if (node->coord[node->depth % (node->k)] < a[node->depth % (node->k)])
    {
        if (node->right)
            node_insert(node->right, a);
        else
        {
            kd_tree *nnode = new kd_tree;
            nnode->coord = a;
            nnode->depth = node->depth + 1;
            nnode->k = node->k;
            nnode->left = NULL;
            nnode->right = NULL;
            node->right = nnode;
        }
    }
    else
    {
        if (node->left)
            node_insert(node->left, a);
        else
        {
            kd_tree *nnode = new kd_tree;
            nnode->coord = a;
            nnode->depth = node->depth + 1;
            nnode->k = node->k;
            nnode->left = NULL;
            nnode->right = NULL;
            node->left = nnode;
        }
    }
}

void kd_tree_insert(kd_tree *root, int k)
{
    vector<int> node(k);
    for (int i = 0; i < k; i++)
        cin >> node[i];
    node_insert(root, node);
}

void trav(kd_tree *node)
{
    for (int i = 0; i < node->k; i++)
    {
        cout << node->coord[i] << " ";
    }
    cout << "\n";
    if (node->left)
        trav(node->left);
    else
        cout << "NULL\n";

    if (node->right)
        trav(node->right);
    else
        cout << "NULL\n";
}

vector<vector<int>> tuple_input(int n, int k)
{
    vector<vector<int>> inp;
    for (int i = 0; i < n; i++)
    {
        vector<int> p(k);
        for (int j = 0; j < k; j++)
            cin >> p[j];
        inp.push_back(p);
    }
    return inp;
}

double coord_distance(vector<int> cord, vector<int> point)
{
    int k = cord.size();
    double distance = 0;
    for (int i = 0; i < k; i++)
        distance += (cord[i] - point[i]) * (cord[i] - point[i]);
    distance = pow(distance, 0.5);
    return distance;
}

void nn_ap(kd_tree *node, vector<int> &nn, double &distance, vector<int> cord)
{
    int d1 = (node->depth) % (node->k);
    double di = coord_distance(cord, node->coord);
    if (di < distance)
    {
        distance = di;
        nn = node->coord;
    }
    if (cord[d1] <= node->coord[d1])
    {
        if (node->left)
            nn_ap(node->left, nn, distance, cord);
    }
    else
    {
        if (node->right)
            nn_ap(node->right, nn, distance, cord);
    }
}

void nearest_neighbor_approx(kd_tree *root, int k)
{
    vector<int> cord(k);
    for (int i = 0; i < k; i++)
        cin>>cord[i];
    vector<int> nn = root->coord;
    double distance = coord_distance(cord, root->coord);
    nn_ap(root, nn, distance, cord);
    cout << "Approximate Nearest Neighbour Distance is " << distance << endl;
    cout << "It's coordinates are ";
    for (int i = 0; i < k; i++)
        cout << nn[i] << " ";
    cout << endl;
}

int main()
{
    int k;
    cout << "Enter the number of dimensions\n";
    cin >> k;
    int n;
    cout << "Enter the inital number of points\n";
    cin >> n;
    cout << "Enter the " << k << "-dimensional points\n";
    vector<vector<int>> inp_points = tuple_input(n, k);

    kd_tree *root = build_kd_tree(inp_points, 0, n, 0, k);
    cout << "Traversal is:\n";
    trav(root);

    char c = 'Q';

    do
    {
        cout << "Enter I to insert, T for traversal, N for nearest neighbor search, Q to quit" << endl;
        cin >> c;
        if (c == 'I')
            kd_tree_insert(root, k);
        else if (c == 'T')
        {
            cout << "Traversal is:\n";
            trav(root);
        }
        else if (c == 'N')
            nearest_neighbor_approx(root, k);
    } while (c != 'Q');

    return 0;
}