#include <iostream>
#include <queue>

int parent[2000];
std::queue<std::pair<int, int>> close;

int dsu_find(int v) {
    if (parent[v] == v)
        return v;
    parent[v] = dsu_find(parent[v]);
    return parent[v];
}
bool dsu_unite(int a, int b) {
    a = dsu_find(a);
    b = dsu_find(b);
    if (a == b) return false;
    if (rand() & 1) parent[a] = parent[b];
    else parent[b] = parent[a];
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    for (int i = 1; i <= n; i++)
        parent[i] = i;

    int ans = 0;
    for (int i = 1; i < n; i++) {
        int a, b;
        std::cin >> a >> b;
        if (!dsu_unite(a, b)){
            close.push({a, b});
            ans++;
        }
    }
    std::cout << ans << "\n";
    for (int i = 2;i <= n; i++){
        if (dsu_unite(1, i)) {
            std::cout << close.front().first
                << " " << close.front().second
                << " " << 1 << " " << i << "\n";
            close.pop();
        }
    }
    return 0;
}