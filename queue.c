#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

void q_merge_two(struct list_head *first,
                 struct list_head *second,
                 bool descend);


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return head;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    // struct list_head *temp = head;
    element_t *tmp, *safe;
    if (!head)
        return;
    list_for_each_entry_safe (tmp, safe, head, list)
        q_release_element(tmp);
    free(head);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    element->value = malloc(sizeof(char) + strlen(s) + 1);
    element->value = strncpy(element->value, s, strlen(s) + 1);
    if (!element->value) {
        free(element);
        return false;
    }

    list_add(&element->list, head);
    return true;
    /*if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    char *copy = strdup(s);
    element->value = copy;
    list_add(&element->list, head);
    return true;*/
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    element->value = malloc(sizeof(char) + strlen(s) + 1);
    element->value = strncpy(element->value, s, strlen(s) + 1);
    if (!element->value) {
        free(element);
        return false;
    }
    list_add_tail(&element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;
    element_t *tmp = list_first_entry(head, element_t, list);
    strncpy(sp, tmp->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    list_del_init(&tmp->list);
    return tmp;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;
    element_t *tmp = list_last_entry(head, element_t, list);
    strncpy(sp, tmp->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    list_del_init(&tmp->list);
    return tmp;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;
    struct list_head *slow, *fast;
    slow = fast = head->next;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;

    element_t *entry = list_entry(slow, element_t, list);
    list_del(&entry->list);
    q_release_element(entry);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    element_t *entry, *safe;
    element_t *entry2, *safe2;
    if (!head || list_empty(head))
        return false;

    list_for_each_entry_safe (entry, safe, head, list) {
        // element_t *tmp = list_entry(element, element_t, list);
        list_for_each_entry_safe (entry2, safe2, head, list) {
            if (entry != entry2 && strcmp(entry->value, entry2->value) == 0) {
                list_del(&entry2->list);

                free(entry2->value);
                // free(entry2);
            }
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;
    struct list_head *first = head->next;
    struct list_head *second = first->next;
    struct list_head *tmp;
    struct list_head *temp;
    for (; second->next != head || second->next->next != head;
         first = first->next->next, second = second->next->next) {
        tmp = first->prev;
        temp = second->next;
        first->prev = second;
        second->prev = tmp;
        tmp->prev = first;
        tmp->next = second;
        second->next = first;
        first->next = temp;
    }
    // if (!head || list_empty(head))
    //     return;
    // struct list_head *prev = head;
    // while (prev->next != head && prev->next->next != head) {
    //     struct list_head *node = prev->next;
    //     list_move_tail(node, node->next->next);
    //     prev = node;
    // }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *now;

    struct list_head *tmp = head->prev;
    head->prev = head->next;
    head->next = tmp;

    list_for_each (now, head) {
        /*swap*/
        tmp = now->prev;
        now->prev = now->next;
        now->next = tmp;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;
    struct list_head *split_node = head->next;
    int count = 0;
    while (count < k) {
        split_node = split_node->next;
        count += 1;
    }
    struct list_head *temp = split_node->prev;
    struct list_head *tail = head->prev;
    temp->next = head;
    split_node->prev = head->prev;
    tail->next = split_node;
    head->prev = temp;

    struct list_head *now;
    struct list_head *tmp = head->prev;
    head->prev = head->next;
    head->next = tmp;

    list_for_each (now, head) {
        /*swap*/
        tmp = now->prev;
        now->prev = now->next;
        now->next = tmp;
    }
    list_splice_tail(split_node, head);
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head) {
        return 0;
    }
    if (list_empty(head)) {
        return 0;
    }
    element_t *entry, *safe, *entry2, *safe2;
    list_for_each_entry_safe (entry, safe, head, list) {
        list_for_each_entry_safe (entry2, safe2, head, list) {
            if (entry->value > entry2->value) {
                list_del(&entry->list);
            }
        }
    }
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head) {
        return 0;
    }
    if (list_empty(head)) {
        return 0;
    }
    element_t *entry, *safe, *entry2, *safe2;
    list_for_each_entry_safe (entry, safe, head, list) {
        list_for_each_entry_safe (entry2, safe2, head, list) {
            if (entry->value < entry2->value) {
                list_del(&entry->list);
            }
        }
    }
    return 0;
}

/*struct list_head *merge_two_list(struct list_head *left,
                                 struct list_head *right)
{
    struct list_head *head = malloc(sizeof(struct list_head));
    struct list_head *ptr = head;
    while (left && right) {
        if (list_entry(left, element_t, list)->value <
            list_entry(right, element_t, list)->value) {
            ptr->next = left;
            left = left->next;
        } else {
            ptr->next = right;
            right = right->next;
        }
        ptr = ptr->next;
    }
    ptr->next = left ? left : right;
    return head->next;
}*/
void merge_two_list(struct list_head *head,
                    struct list_head *left,
                    struct list_head *right,
                    bool descend)
{
    LIST_HEAD(tmp);

    while (!list_empty(left) && !list_empty(right)) {
        element_t *left_entry = list_entry(left->next, element_t, list);
        element_t *right_entry = list_entry(right->next, element_t, list);
        if (descend ? strcmp(left_entry->value, right_entry->value) > 0
                    : strcmp(left_entry->value, right_entry->value) < 0) {
            list_move_tail(left->next, &tmp);
        } else {
            list_move_tail(right->next, &tmp);
        }
    }
    if (list_empty(left)) {
        list_splice_tail_init(right, &tmp);
    }
    if (list_empty(right)) {
        list_splice_tail_init(left, &tmp);
    }
    // list_splice_init(&tmp, head);
}

void merge_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *mid = head;
    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next)
        mid = mid->next;

    LIST_HEAD(left);
    LIST_HEAD(right);

    list_cut_position(&left, head, mid);
    list_splice_init(head, &right);

    merge_sort(&left, descend);
    merge_sort(&right, descend);

    merge_two_list(head, &left, &right, descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return;
    merge_sort(head, descend);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
/*int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    struct list_head *node, *safe;
    queue_contex_t *first_q = list_entry(head->next, queue_contex_t, chain);
    list_for_each_safe (node, safe, head->next->next) {
        if (node == head->next) {
            continue;
        }
        queue_contex_t *tmp_q = list_entry(node, queue_contex_t, chain);
        merge_two_list(first_q->q, tmp_q->q);
    }
    q_sort(first_q->q, descend);
    return q_size(first_q->q);
}*/
int q_merge(struct list_head *head, bool descend)
{
    queue_contex_t *qhead = list_first_entry(head, queue_contex_t, chain);
    list_del_init(&qhead->chain);
    queue_contex_t *cur = NULL;

    list_for_each_entry (cur, head, chain) {
        list_splice_init(cur->q, qhead->q);
        qhead->size += cur->size;
    }

    list_add(&qhead->chain, head);
    q_sort(qhead->q, descend);
    return qhead->size;
}


// struct list_head *node, safe;
/*
if (!head || list_empty(head)) {

queue_contex_t *first_q = list_entry(head->next, queue_contex_t, chain);
struct list_head *node, *safe;
list_for_each_safe (node, safe, head) {
    if (node == head->next)
        continue;
    queue_contex_t *cur_q = list_entry(node, queue_contex_t, chain);
    list_splice_init(cur_q->q, first_q->q);
}
q_sort(first_q->q, descend);

return q_size(first_q->q);*/

// int main(void)
// {
//     q_new();
//     return 0;
// }