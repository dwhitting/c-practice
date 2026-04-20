#include <stdio.h>
#include <stdlib.h>

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
    struct ListNode *prev = NULL, *curr = head, *next = NULL, \
        *left_node = NULL, *right_node = NULL, *rev_list, *rev_head = NULL, \
        *rev_tail = NULL;

    int i = 0;
    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }


    return prev;
}

struct ListNode *add_node(struct ListNode *head, int n) {
    struct ListNode *new_node = (struct ListNode*) malloc(sizeof(struct ListNode));
    new_node->val = n;
    new_node->next = head;
}

int main(void)
{
    struct ListNode *head = (struct ListNode*) malloc(sizeof(struct ListNode));
    head->val = 5;
    head->next = NULL;

    for (int i = 4; i > 0; i--) {
        head = add_node(head, i);
    }

    printf("head val: %d\n", head->val);

    head = reverseBetween(head, 2, 4);


    return 0;
}