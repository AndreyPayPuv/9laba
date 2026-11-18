#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>        
#ifdef _WIN32
#include <windows.h>  
#endif

typedef struct Node {
    int v;
    struct Node* next;
} Node;


int** gen_matrix(int n, int percent)
{
    int** g = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        g[i] = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (rand() % 100 < percent)
                g[i][j] = g[j][i] = 1;
    return g;
}

void free_matrix(int** g, int n)
{
    for (int i = 0; i < n; i++) free(g[i]);
    free(g);
}

void print_matrix(int** g, int n)
{
    printf("    ");
    for (int j = 0; j < n; j++) printf("%2d ", j + 1);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%2d: ", i + 1);
        for (int j = 0; j < n; j++) printf("%2d ", g[i][j]);
        printf("\n");
    }
}

Node** matrix_to_lists(int** g, int n)
{
    Node** adj = (Node**)calloc(n, sizeof(Node*));
    for (int i = 0; i < n; i++)
        for (int j = n - 1; j >= 0; j--)
            if (g[i][j]) {
                Node* p = (Node*)malloc(sizeof(Node));
                p->v = j;
                p->next = adj[i];
                adj[i] = p;
            }
    return adj;
}

void free_lists(Node** adj, int n)
{
    for (int i = 0; i < n; i++) {
        Node* p = adj[i];
        while (p) { Node* t = p; p = p->next; free(t); }
    }
    free(adj);
}

void print_lists(Node** adj, int n)
{
    for (int i = 0; i < n; i++) {
        printf("%2d: ", i + 1);
        for (Node* p = adj[i]; p; p = p->next) printf("%d ", p->v + 1);
        printf("\n");
    }
}

void print_dist(const int* dist, int n, int start)
{
    printf("Расстояния от вершины %d:\n", start + 1);
    for (int i = 0; i < n; i++)
        printf("  DIST[%d] = %d\n", i + 1, dist[i]);
}

void bfsd_matrix(int** g, int n, int v, int* dist, int verbose)
{
    std::queue<int> Q;
    Q.push(v);
    dist[v] = 0;
    if (verbose) printf("Порядок обхода: ");
    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        if (verbose) printf("%d ", v + 1);
        for (int i = 0; i < n; i++)
            if (g[v][i] == 1 && dist[i] == -1) {
                Q.push(i);
                dist[i] = dist[v] + 1;
            }
    }
    if (verbose) printf("\n");
}

void bfsd_lists(Node** adj, int n, int v, int* dist, int verbose)
{
    (void)n;
    std::queue<int> Q;
    Q.push(v);
    dist[v] = 0;
    if (verbose) printf("Порядок обхода: ");
    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        if (verbose) printf("%d ", v + 1);
        for (Node* p = adj[v]; p; p = p->next)
            if (dist[p->v] == -1) {
                Q.push(p->v);
                dist[p->v] = dist[v] + 1;
            }
    }
    if (verbose) printf("\n");
}

void dfsd_matrix(int** g, int n, int v, int* dist)
{
    for (int i = 0; i < n; i++)
        if (g[v][i] == 1 && (dist[i] == -1 || dist[i] > dist[v] + 1)) {
            dist[i] = dist[v] + 1;
            dfsd_matrix(g, n, i, dist);
        }
}

void dfsd_lists(Node** adj, int v, int* dist)
{
    for (Node* p = adj[v]; p; p = p->next)
        if (dist[p->v] == -1 || dist[p->v] > dist[v] + 1) {
            dist[p->v] = dist[v] + 1;
            dfsd_lists(adj, p->v, dist);
        }
}

int* new_dist(int n)
{
    int* d = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) d[i] = -1;
    return d;
}

int same(const int* a, const int* b, int n)
{
    for (int i = 0; i < n; i++) if (a[i] != b[i]) return 0;
    return 1;
}

double now_ms(void)
{
    return (double)clock() * 1000.0 / CLOCKS_PER_SEC;
}














void lalala(int** g, int n)
{
    int** D = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        D[i] = new_dist(n);
        bfsd_matrix(g, n, i, D[i], 0);
    }
    printf("Матрица кратцайших расстояний\n  ");

    for (int j = 0; j < n; j++)
        printf("%4d", j + 1);
    printf("\n");


    for (int i = 0; i < n; i++) {
        printf("%4d ", i + 1);

        for (int j = 0; j < n; j++)
            printf("%4d", D[i][j]);

        printf("\n");

    }



    for (int i = 0; i < n; i++)
        free(D[i]);

    free(D);
}





int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    srand((unsigned)time(NULL));
    int n, start;

    printf("Введите число вершин графа (для вывода): ");
    if (scanf("%d", &n) != 1 || n < 1) return 1;
    printf("Введите номер исходной вершины (1..%d): ", n);
    if (scanf("%d", &start) != 1 || start < 1 || start > n) return 1;
    start--;

    printf("\nЗадание 1.1. Матрица смежности графа\n");
    int** g = gen_matrix(n, 35);
    print_matrix(g, n);

    printf("\nЗадание 1.2. Поиск расстояний в ширину (матрица смежности)\n");
    int* d_bfs = new_dist(n);
    bfsd_matrix(g, n, start, d_bfs, 1);
    print_dist(d_bfs, n, start);

    printf("\nЗадание 1.3*. Поиск расстояний в ширину (списки смежности)\n");
    Node** adj = matrix_to_lists(g, n);
    printf("Списки смежности:\n");
    print_lists(adj, n);
    int* d_bfs_l = new_dist(n);
    bfsd_lists(adj, n, start, d_bfs_l, 1);
    print_dist(d_bfs_l, n, start);
    printf("Совпадает с результатом задания 1.2: %s\n", same(d_bfs, d_bfs_l, n) ? "да" : "нет");




    printf("\nДополнительное задание\n");
    lalala(g, n);







    printf("\nЗадание 2.1*. Поиск расстояний в глубину (матрица смежности)\n");
    int* d_dfs = new_dist(n);
    d_dfs[start] = 0;
    dfsd_matrix(g, n, start, d_dfs);
    print_dist(d_dfs, n, start);
    printf("Совпадает с результатом задания 1.2: %s\n", same(d_bfs, d_dfs, n) ? "да" : "нет");

    printf("\nЗадание 2.2*. Поиск расстояний в глубину (списки смежности)\n");
    int* d_dfs_l = new_dist(n);
    d_dfs_l[start] = 0;
    dfsd_lists(adj, start, d_dfs_l);
    print_dist(d_dfs_l, n, start);
    printf("Совпадает с результатом задания 1.2: %s\n", same(d_bfs, d_dfs_l, n) ? "да" : "нет");










    free(d_bfs); free(d_bfs_l); free(d_dfs); free(d_dfs_l);
    free_lists(adj, n);
    free_matrix(g, n);

    printf("\nЗадание 2.3*. Время работы алгоритмов (мс), плотность 30%% \n");
    printf("%8s  %10s %10s  %10s %10s\n", "n",
        "BFS матр.", "BFS спис.", "DFS матр.", "DFS спис.");

    int sizes[] = { 50, 100, 200, 400, 800 };
    for (int s = 0; s < 5; s++) {
        int m = sizes[s];
        int reps = 5;
        int** G = gen_matrix(m, 30);
        Node** A = matrix_to_lists(G, m);
        int* d = (int*)malloc(m * sizeof(int));
        double t[4] = { 0, 0, 0, 0 }, t0;

        for (int r = 0; r < reps; r++) {
            for (int i = 0; i < m; i++) d[i] = -1;
            t0 = now_ms(); bfsd_matrix(G, m, 0, d, 0); t[0] += now_ms() - t0;

            for (int i = 0; i < m; i++) d[i] = -1;
            t0 = now_ms(); bfsd_lists(A, m, 0, d, 0);  t[1] += now_ms() - t0;

            for (int i = 0; i < m; i++) d[i] = -1;
            d[0] = 0;
            t0 = now_ms(); dfsd_matrix(G, m, 0, d);    t[2] += now_ms() - t0;

            for (int i = 0; i < m; i++) d[i] = -1;
            d[0] = 0;
            t0 = now_ms(); dfsd_lists(A, 0, d);        t[3] += now_ms() - t0;
        }
        printf("%8d  %10.3f %10.3f  %10.3f %10.3f\n", m,
            t[0] / reps, t[1] / reps, t[2] / reps, t[3] / reps);
        free(d); free_lists(A, m); free_matrix(G, m);
    }
    return 0;
}