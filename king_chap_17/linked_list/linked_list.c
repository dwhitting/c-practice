#include <stdlib.h>
#include <stdio.h>

struct node *add_to_list(struct node *list, int n);
struct node *read_numbers(struct node *list);

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
    first = add_to_list(first, 2);
    first = add_to_list(first, 1);

    first = read_numbers(first);

    struct node *lp;
    for (lp = first; lp != NULL; lp = lp->next)
        printf("num: %d\n", lp->value);

    //printf("first: %d, second: %d, third: %d\n", first->value, \
    //    first->next->value, first->next->next->value);

    return 0;
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