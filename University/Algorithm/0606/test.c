#include <stdio.h>
#include <stdlib.h>

#define N 100



typedef struct _node
{
    int data;
    struct _node *left;
    struct _node *center;
    struct _node *right;
} NODE;



NODE *root;



NODE *create_node(int d)
{
    NODE *p;
    p = (NODE*)malloc(sizeof(NODE));
    p -> data = d;
    p -> left = NULL;
    p -> center = NULL;
    p -> right = NULL;
    return p;
}



int height(NODE *p)
{
    int l, r, c;

    if(p == NULL)
        return -1;

    l = height(p -> left);
    r = height(p -> right);
    c = height(p -> center);

    if(l >= r && l >= c)
        return l + 1;
    else if(r >= l && r >= c)
        return r + 1;
    else
        return c + 1;
}


// 深さ d の節点 p を根とする木の深さの合計
int total_depth(NODE *p, int d)
{
    int count = d;

    if(p -> left != NULL)
    {
        count += total_depth(p -> left, d + 1);
    }

    if(p -> right != NULL)
    {
        count += total_depth(p -> right, d + 1);
    }

    if(p -> center != NULL)
    {
        count += total_depth(p -> center, d + 1);
    }

    return count;
}



// pを根とする木のノード数
int total_nodes(NODE *p)
{
    int count = 1;

    if(p -> left != NULL)
    {
        count += total_nodes(p -> left);
    }

    if(p -> right != NULL)
    {
        count += total_nodes(p -> right);
    }

    if(p -> center != NULL)
    {
        count += total_nodes(p -> center);
    }

    return count;
}



double average_depth(NODE *p)
{
    return total_depth(p, (height(root) - height(p))) / total_nodes(p);
}



void insert_data(int d, double t)
{
    if(root == NULL)
    {
        root = create_node(d);
        return;
    }

    NODE *p;
    p = root;
    double r;

    srand(100);

    while(1)
    {
        r = (double)rand() / ((double)RAND_MAX + 1.0) * 1.0;

        if(r < t)   // left
        {
            if(p -> left == NULL)
            {
                p -> left = create_node(d);
                return;
            }
            p = p -> left;
        }
        else if(r > (1 - t))   // right
        {
            if(p -> right == NULL)
            {
                p -> right = create_node(d);
                return;
            }
            p = p -> right;
        }
        else   // center
        {
            if(p -> center == NULL)
            {
                p -> center = create_node(d);
                return;
            }
            p = p -> center;
        }
    }
}



int main(void)
{
    int i;
    double t;

    printf("N = %d\n\n", N);
    printf("t\theight\tave_depth\n");

    for(t = 0.05; t <= 0.45; t += 0.05)
    {
        root = NULL;
        for(i = 0; i < N; i++)
        {
            insert_data(i, t);   // ノードの中に入れる整数は i とした
        }

        printf("%lf\t", t);
        printf("%d\t", height(root));
        printf("%lf\n", average_depth(root));



    }   
}