#include "data_structures/linked_list.h"
#include <check.h>
#include <stdlib.h>

struct test_data
{
    int a;
    int b;
    int c;
};

struct linked_list* test_list = NULL;
const int c_num_elements = 200;

void setup(void)
{
    test_list = linked_list_create(sizeof(struct test_data));
    for (int i = 0; i < c_num_elements; ++i)
    {
        struct test_data tmp = {i, i * 2, i + 10};
        linked_list_push_back(test_list, &tmp);
    }
}

void teardown(void)
{
    linked_list_destroy(test_list);
}

START_TEST(test_linked_list_data)
{
    ck_assert_uint_eq(test_list->size, c_num_elements);
    ck_assert_uint_eq(test_list->element_size, sizeof(struct test_data));
}
END_TEST

START_TEST(test_linked_list_remove)
{
    struct linked_list_node* current = test_list->begin->next;
    struct linked_list_node* first = test_list->begin;
    struct linked_list_node* second = test_list->begin->next;
    for (int i = 1; i <= 10; ++i)
    {
        current = current->next;
        linked_list_remove(test_list, current->previous);
        ck_assert_uint_eq(test_list->size, c_num_elements - i);
    }
    ck_assert(test_list->begin == first);
    ck_assert(test_list->begin->next != second);

    struct test_data* t = test_list->begin->data;
    ck_assert_int_eq(t->a, 0);
    ck_assert_int_eq(t->b, 0);
    ck_assert_int_eq(t->c, 10);
    t = test_list->begin->next->data;
    ck_assert_int_eq(t->a, 11);
    ck_assert_int_eq(t->b, 22);
    ck_assert_int_eq(t->c, 21);
    t = test_list->end->data;
    ck_assert_int_eq(t->a, 199);
    ck_assert_int_eq(t->b, 398);
    ck_assert_int_eq(t->c, 209);

    current = test_list->begin;
    for (int i = 0; i < 100; ++i)
    {
        ck_assert(current != NULL);
        current = current->next;
    }
}
END_TEST

Suite* test_suite(void)
{
    Suite* suite = suite_create("linked_list_suite");
    TCase* test_case = tcase_create("linked_list_basic");

    tcase_add_checked_fixture(test_case, setup, teardown);
    tcase_add_test(test_case, test_linked_list_data);
    tcase_add_test(test_case, test_linked_list_remove);
    suite_add_tcase(suite, test_case);

    return suite;
}

int main(void)
{
    Suite* suite = test_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}