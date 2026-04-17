#include <stdlib.h>
#include <stdio.h>

struct node *add_to_list(struct node *list, int n);
struct node *read_numbers(struct node *list);
struct node *search_list(struct node *list, int n);
int compare_nodes(const void *p, const void *q);

struct node {
    int value;
    struct node *next;
};

int main(void)
{
    struct node *first = NULL;

    struct node *new_node;
    new_node = malloc(sizeof(struct node));
    //(*new_node).value = 10;  //one way to do it, or use ->
    new_node->value = 3;
    new_node->next = first;
    first = new_node;

    for (int i = 10; i > 0; i--)
        first = add_to_list(first, i+5);

    //first = read_numbers(first);

    struct node *lp;
    for (lp = first; lp != NULL; lp = lp->next) 
        printf("num: %d\n", lp->value);
        

    printf("\n");
    struct node *found = search_list(first, 10);
    if (found == NULL)
        printf("Num not in list");
    else
        printf("found match: %d\n", found->value);

    return 0;
}

int compare_nodes(const void *p, const void *q)  //would be if using qsort on an array
{
    const struct node *pn1 = p;
    const struct node *qn1 = q;

    return pn1->value - qn1->value;
}

struct node *add_to_list(struct node *list, int n)
{
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Error: malloc failed to add_to_list\n");
        exit(EXIT_FAILURE);
    }
    new_node->value = n;
    new_node->next = list;
    return new_node;
}

struct node *read_numbers(struct node *list)
{
    struct node *new_node = NULL;
    int n;
    for (;;) {
        scanf("%d", &n);
            if (n == 0)
                break;
        list = add_to_list(list, n);
    }
    return list;
}

struct node *search_list(struct node *list, int n)
{
    struct node *p;
    for (p = list; p != NULL; p = p->next)
        if (p->value == n)
            return p;
    return NULL;
}  