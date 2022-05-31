#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lbr/jrb.h"
#include "lbr/dllist.h"
#define INFINITIVE 1200
#define TRUE 1
#define FALSE 0
#define MAX_CHOSE 4
typedef struct _graphics
{
    JRB edges;
    JRB vertices;
    JRB id;
} * Graph;
int dem = -1;
Graph createGraph();
int addVertex(Graph, int, char *);
char *getVertex(Graph, int);
void addEdge(Graph, int, int, char *);
int hasEdge(Graph, int, int);
int indegree(Graph, int, int *);
int outdegree(Graph, int, int *);
int DAG(Graph);
void dropGraph(Graph);
void addVertexFromFile(Graph g, char *filename);
void addEdge_str(Graph, char *, char *, char *);
int getId(Graph g, char *name);
void print(JRB root, Graph g)
{
    JRB root1, root2, root11, root22;
    root11 = (JRB)jval_v(root->val);
    jrb_traverse(root1, root11)
    {
        root22 = (JRB)jval_v(root1->val);
        printf("\nCanh: %s-%s:\n", getVertex(g, jval_i(root->key)), getVertex(g, jval_i(root1->key)));
        jrb_traverse(root2, root22)
        {
            printf("%s\n", jval_s(root2->key));
        }
    }
}
int BFS(Graph g, char *s, char *f, int *rank);
int getPath(Graph g, int end, int rank[], int *paths[]);
int toiUu(Graph g, int path[], int size);
JRB getWeight(Graph g, int v1, int v2);
int getValue(JRB tree, char *key);
int getMin(JRB tree);
void printPath(Graph, int *, int);
char *getBusNameMin(JRB, char *);
int hasVertex(Graph g, char *name);
void printBus(char *filename, char *bus);
void printBusOfV(Graph, int);
void menu(void)
{
    printf("\t\t\t|======================================================|\n");
    printf("\t\t\t|%25sMENU%25s|\n", " ", " ");
    printf("\t\t\t|------------------------------------------------------|\n");
    printf("\t\t\t|  1  |   %-45s|\n", "Tim duong di");
    printf("\t\t\t|  2  |   %-45s|\n", "In ra cac tuyen buyt qua mot ben");
    printf("\t\t\t|  3  |   %-45s|\n", "Tim tuyen buyt");
    printf("\t\t\t|  4  |   %-45s|\n", "Ket thuc chuong trinh");
    printf("\t\t\t|======================================================|\n");
}
int main()
{
    int chose;
    int rank[INFINITIVE], end;
    int i, total, output[20];
    int *paths[100], sum, *toiuu, min;
    char str[50], str2[50], str1[50];
    Graph graph = createGraph();
    JRB root;
    do
    {
        dem = -1;
        addVertexFromFile(graph, "station.text"); //
        menu();
        printf("Hay nhap lua chon cua ban: ");
        while (!scanf("%d", &chose))
        {
            printf("Nhap sai: Khong duoc nhap chu!\nChi duoc nhap so tu %d - %d\n", 1, MAX_CHOSE);
            menu();
            printf("Hay nhap lua chon cua ban: ");
            __fpurge(stdin);
        }
        __fpurge(stdin);
        switch (chose)
        {
        case 1:
            printf("Nhap ten cua diem dau: ");
            fgets(str1, 50, stdin);
            str1[strlen(str1) - 1] = 0;
            if (!hasVertex(graph, str1))
            {
                printf("Khong ton tai diem buyt %s\n", str1);
                goto ext;
            }
            printf("Nhap ten cua diem den: ");
            fgets(str2, 50, stdin);
            str2[strlen(str2) - 1] = 0;
            if (!hasVertex(graph, str2))
            {
                printf("Khong ton tai diem buyt %s\n", str2);
                goto ext;
            }
            end = BFS(graph, str1, str2, rank);
            if (rank[end] == INFINITIVE)
            {
                printf("Khong ton tai duong di\n");
                goto ext;
            }
            // printf("ID: %d\nName: %s\nRank: %d\n", end, getVertex(graph, end), rank[end]);
            for (i = 0; i < 100; i++)
            {
                paths[i] = (int *)malloc(sizeof(int) * (rank[end] + 1));
                for (int j = 0; j < rank[end]; j++)
                {
                    paths[i][j] = -1;
                }
                paths[i][rank[end]] = end;
            }
            sum = getPath(graph, end, rank, paths);
            toiuu = (int *)malloc(sizeof(int) * (sum + 1));
            for (i = 0; i <= sum; i++)
            {
                for (int j = 0; j <= rank[end]; j++)
                {
                    printf("%s%c", getVertex(graph, paths[i][j]), j == rank[end] ? '\n' : '-');
                }
            }
            for (i = 0; i <= sum; i++)
            {
                toiuu[i] = toiUu(graph, paths[i], rank[end]);
                // printf("Lan doi: %d\n", toiuu[i]);
                if (!i)
                    min = i;
                else if (toiuu[i] < toiuu[min])
                    min = i;
            }
            toiUu(graph, paths[min], rank[end]);
            printPath(graph, paths[min], rank[end]);
            for (i = 0; i < 100; i++)
                free(paths[i]);
            free(toiuu);
        ext:
            break;
        case 2:
            printf("Hay nhap vao ten ben ma ban muon tim tuyen: ");
            fgets(str, sizeof(str), stdin);
            str[strlen(str) - 1] = 0;
            if (!hasVertex(graph, str))
            {
                printf("Khong ton tai diem buyt nay\n");
                break;
            }
            printBusOfV(graph, getId(graph, str));
            break;
        case 3:
            printf("Hay nhap vao ten tuyen buyt ban can tim: ");
            fgets(str, 50, stdin);
            str[strlen(str) - 1] = 0;
            printBus("station.text", str);
            break;
        case 4:
            printf("\tKet thuc chuong trinh\n");
            dropGraph(graph);
            chose = 0;
            break;
        default:
            printf("Chi duoc chon cac so tu %d - %d!!!\n", 1, MAX_CHOSE);
            chose = 1;
            break;
        }
    } while (chose);
    return 0;
}
void printBusOfV(Graph g, int id)
{
    int total, index = 0, j;
    JRB bus, root;
    int output[100];
    total = outdegree(g, id, output);
    char tuyen[100][100] = {0};
    for (int i = 0; i < total; i++)
    {
        bus = getWeight(g, id, output[i]);
        jrb_traverse(root, bus)
        {
            for (j = 0; j <= index; i++)
            {
                if (strcmp(tuyen[j], jval_s(root->key)) == 0)
                    break;
            }
            if (j == index + 1)
            {
                strcpy(tuyen[j], jval_s(root->key));
                index++;
            }
        }
    }
    for (int i = 0; i < index; i++)
    {
        printf("%s\n", tuyen[i]);
    }
}
void printBus(char *filename, char *bus)
{
    char str[1024];
    char str1[1024] = "Not Found";
    char str2[1024] = "Not Found";
    char tenbus[20];
    FILE *f;
    f = fopen(filename, "r");
    while (!feof(f) && fgets(str, sizeof(str), f))
    {
        if (strstr(str, "TUYEN") == str)
        {
            sscanf(str + 5, "%s", tenbus);
            if (strcmp(bus, tenbus) == 0)
            {
                // printf("%s\n", tenbus);
                fgets(str1, sizeof(str1), f);
                str1[17] = '\n';
                for (int i = 0; i < strlen(str1); i++)
                {
                    if (str1[i] == '-')
                    {
                        str1[i] = '\n';
                    }
                }
                // printf("%s\n", str1);
                fgets(str2, sizeof(str2), f);
                str2[17] = '\n';
                for (int i = 0; i < strlen(str2); i++)
                {
                    if (str2[i] == '-')
                    {
                        str2[i] = '\n';
                    }
                }
                // printf("%s\n", str2);
                goto doit;
            }
        }
    }
    printf("Khong tim thay tuyen buyt nay\n");
    fclose(f);
    return;
doit:
    printf("Danh sach cac ben trong tuyen %s la:\n\n%s\n%s", bus, str1, str2);
    fclose(f);
}
int hasVertex(Graph g, char *name)
{
    int find = getId(g, name);
    if (find != INFINITIVE)
    {
        return 1;
    }
    return 0;
}
char *getBusNameMin(JRB tree, char *key)
{
    int min = 1000, i;
    char *rt;
    JRB root;
    jrb_traverse(root, tree)
    {
        if (strcmp(jval_s(root->key), key) == 0)
        {
            rt = key;
            goto trave;
        }
        i = jval_i(root->val);
        if (min > i)
        {
            min = i;
            rt = jval_s(root->key);
        }
    }
trave:
    return rt;
}
void printPath(Graph g, int path[], int size)
{
    JRB node;
    char name[100] = "";
    for (int i = 0; i < size; i++)
    {
        node = getWeight(g, path[i], path[i + 1]);
        strcpy(name, getBusNameMin(node, name));
        printf("Ben: \t\t%-25sTuyen buyt:   %s\n", getVertex(g, path[i]), name);
    }
    printf("Ben cuoi: \t%-30s\n", getVertex(g, path[size]));
}
JRB getWeight(Graph g, int v1, int v2)
{
    JRB node = (JRB)(jval_v(jrb_find_int(g->edges, v1)->val));
    return (JRB)(jval_v(jrb_find_int(node, v2)->val));
}
int getValue(JRB tree, char *key)
{
    JRB find = jrb_find_str(tree, key);
    if (!find)
        return 0;
    return jval_i(find->val);
}
int getMin(JRB tree)
{
    JRB root;
    int min = 100;
    int n;
    jrb_traverse(root, tree)
    {
        n = jval_i(root->val);
        if (min > n)
            min = n;
    }
    return min;
}
int toiUu(Graph g, int path[], int size)
{
    JRB root, node, next;
    int val;
    for (int i = size - 2; i >= 0; i--)
    {
        node = getWeight(g, path[i], path[i + 1]);
        next = getWeight(g, path[i + 1], path[i + 2]);
        jrb_traverse(root, node)
        {
            val = getValue(next, jval_s(root->key));
            if (val)
                (root->val).i = val;
            else
                (root->val).i = getMin(next) + 1;
        }
    }
    return getMin(node);
}
int getPath(Graph g, int end, int rank[], int *paths[])
{
    int input[100];
    int i;
    if (!rank[end])
    {
        paths[dem + 1][rank[end]] = end;
        return ++dem;
    }
    int total = indegree(g, end, input);
    for (int j = 0; j < total; j++)
    {
        if (rank[input[j]] == rank[end] - 1)
        {
            i = getPath(g, input[j], rank, paths);
            for (int k = i; k >= 0; k--)
            {
                if (paths[k][rank[input[j]]] >= 0)
                    break;
                paths[k][rank[input[j]]] = input[j];
            }
        }
    }
    return dem;
}
int BFS(Graph g, char *s, char *f, int *rank)
{
    Dllist list;
    list = new_dllist();
    int b = getId(g, s);
    int e = getId(g, f);
    int r, total, n;
    char visited[INFINITIVE];
    int output[100];
    memset(visited, 0, sizeof(visited));
    rank[b] = 0;
    rank[e] = INFINITIVE;
    dll_append(list, new_jval_i(b));
    visited[b] = TRUE;
    while (!dll_empty(list))
    {
        r = jval_i(dll_first(list)->val);
        dll_delete_node(dll_first(list));
        if (rank[r] > rank[e])
        {
            goto here;
        }
        total = outdegree(g, r, output);
        for (int i = 0; i < total; i++)
        {
            n = output[i];
            if (visited[n])
                continue;
            dll_append(list, new_jval_i(n));
            rank[n] = rank[r] + 1;
            visited[output[i]] = TRUE;
        }
    }
here:
    return e;
}
int getId(Graph g, char *name)
{
    JRB node = jrb_find_str(g->id, name);
    if (node)
    {
        return jval_i(node->val);
    }
    else
        return INFINITIVE;
}
void addEdge_str(Graph g, char *s1, char *s2, char *tuyen)
{
    int id1 = getId(g, s1);
    int id2 = getId(g, s2);
    addEdge(g, id1, id2, tuyen);
}
void addVertexFromFile(Graph g, char *filename)
{
    char str[1024];
    char so[20], so1[20], so2[20];
    char *ten;
    FILE *f;
    int i = 0, j, z = 10;
    JRB root;
    f = fopen(filename, "r");
    char ten1[1024];
    while (!feof(f) && fgets(str, 1024, f))
    {
        sscanf(str + 5, "%s", so);
        strcpy(so1, so);
        strcat(so1, " Chieu di");
        strcpy(so2, so);
        strcat(so2, " Chieu ve");
        fgets(str, 1024, f);
        ten = strtok(str, "\t");
        ten = strtok(NULL, "-");
        strcpy(ten1, ten);
        i += addVertex(g, i, strdup(ten));
        while (1)
        {
            ten = strtok(NULL, "-\n");
            if (!ten)
                break;
            i += addVertex(g, i, strdup(ten));
            addEdge_str(g, ten1, ten, strdup(so1));
            strcpy(ten1, ten);
        }
        fgets(str, 1024, f);
        ten = strtok(str, "\t");
        ten = strtok(NULL, "-");
        strcpy(ten1, ten);
        i += addVertex(g, i, strdup(ten));
        while (1)
        {
            ten = strtok(NULL, "-\n");
            if (!ten)
                break;
            i += addVertex(g, i, strdup(ten));
            addEdge_str(g, ten1, ten, strdup(so2));
            strcpy(ten1, ten);
        }
    }
    free(ten);
    fclose(f);
    // printf("Co %d diem buyt\n", i);
}
Graph createGraph()
{
    Graph graph;
    graph = (Graph)malloc(sizeof(struct _graphics));
    graph->edges = make_jrb();
    graph->vertices = make_jrb();
    graph->id = make_jrb();
    return graph;
}
int addVertex(Graph graph, int id, char *name)
{
    if (!jrb_find_str(graph->id, name))
    {
        JRB node = make_jrb();
        jrb_insert_str(graph->id, name, new_jval_i(id));
        jrb_insert_int(graph->vertices, id, new_jval_s(name));
        jrb_insert_int(graph->edges, id, new_jval_v(node));
        return 1;
    }
    return 0;
}
char *getVertex(Graph graph, int id)
{
    if (id == -1)
    {
        return NULL;
    }
    return (char *)jval_v(jrb_find_int(graph->vertices, id)->val);
}
void addEdge(Graph graph, int v1, int v2, char *tuyen)
{
    JRB node = (JRB)jval_v(jrb_find_int(graph->edges, v1)->val);
    JRB node1 = jrb_find_int(node, v2);
    if (node1)
    {
        if (jrb_find_str((JRB)jval_v(node1->val), tuyen))
            return;
        jrb_insert_str((JRB)jval_v(node1->val), tuyen, new_jval_i(1));
    }
    else
    {
        JRB node2 = make_jrb();
        jrb_insert_str(node2, tuyen, new_jval_i(1));
        jrb_insert_int(node, v2, new_jval_v(node2));
    }
}
int hasEdge(Graph graph, int v1, int v2)
{
    JRB node = jrb_find_int(graph->edges, v1);
    if (!node)
    {
        return 0;
    }
    else
    {
        JRB tree = jrb_find_int((JRB)jval_v(node->val), v2);
        if (!tree)
        {
            return 0;
        }
        return 1;
    }
}
int indegree(Graph graph, int v, int *output)
{
    JRB root;
    JRB find;
    int total = 0;
    jrb_traverse(root, graph->edges)
    {
        find = jrb_find_int((JRB)jval_v(root->val), v);
        if (find)
            *(output + total++) = jval_i(root->key);
    }
    return total;
}
int outdegree(Graph graph, int v, int *output)
{
    JRB root, find;
    int total = 0;
    find = jrb_find_int(graph->edges, v);
    if (find)
    {
        JRB tree = (JRB)jval_v(find->val);
        jrb_traverse(root, tree)
        {
            *(output + total++) = jval_i(root->key);
        }
    }
    return total;
}
int DAG(Graph graph)
{
    int visited[1000];
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, graph->vertices)
    {
        memset(visited, 0, sizeof(visited));

        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack, new_jval_i(start));

        while (!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);
            if (!visited[u])
            {
                visited[u] = 1;
                n = outdegree(graph, u, output);
                for (i = 0; i < n; i++)
                {
                    v = output[i];
                    if (v == start)
                        return 0;
                    if (!visited[v])
                        dll_append(stack, new_jval_i(v));
                }
            }
        }
    }
    return 1;
}
void dropGraph(Graph graph)
{
    JRB root, root1, node1;
    JRB node = graph->edges;
    jrb_traverse(root, node)
    {
        node1 = (JRB)jval_v(root->val);
        jrb_traverse(root1, node1)
            jrb_free_tree((JRB)jval_v(root1->val));
    }
    jrb_free_tree(node);
    node = graph->vertices;
    jrb_traverse(root, node)
    {
        free(jval_s(root->val));
    }
    jrb_free_tree(graph->vertices);
    jrb_free_tree(graph->id);
}