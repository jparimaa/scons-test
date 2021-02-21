#include "data_structures/hashmap.h"
#include <check.h>
#include <stdlib.h>

struct test_data
{
    int a;
    int b;
    int c;
};

struct hashmap* test_map = NULL;
const int c_num_elements = 200;

void setup(void)
{
    test_map = hashmap_create(sizeof(int), sizeof(struct test_data));
    for (int i = 0; i < c_num_elements; ++i)
    {
        struct test_data tmp = {i, i * 2, i + 10};
        hashmap_set(test_map, &i, &tmp);
    }
}

void teardown(void)
{
    hashmap_destroy(test_map);
}

START_TEST(test_hashmap_get)
{
    for (int i = 0; i < c_num_elements; ++i)
    {
        struct test_data* from_hashmap = hashmap_get(test_map, &i);
        struct test_data tmp = {i, i * 2, i + 10};
        ck_assert_int_eq(tmp.a, from_hashmap->a);
        ck_assert_int_eq(tmp.b, from_hashmap->b);
        ck_assert_int_eq(tmp.c, from_hashmap->c);
    }
}
END_TEST

START_TEST(test_hashmap_remove)
{
    for (int i = 0; i < c_num_elements; i += 2)
    {
        bool removed = hashmap_remove(test_map, &i);
        ck_assert(removed);
    }

    for (int i = 0; i < c_num_elements; ++i)
    {
        if (i % 2 == 0)
        {
            void* tmp = hashmap_get(test_map, &i);
            ck_assert_ptr_eq(tmp, NULL);
        }
        else
        {
            struct test_data* from_hashmap = hashmap_get(test_map, &i);
            struct test_data tmp = {i, i * 2, i + 10};
            ck_assert_int_eq(tmp.a, from_hashmap->a);
            ck_assert_int_eq(tmp.b, from_hashmap->b);
            ck_assert_int_eq(tmp.c, from_hashmap->c);
        }
    }
}
END_TEST

Suite* test_suite(void)
{
    Suite* suite = suite_create("hashmap_suite");
    TCase* test_case = tcase_create("hashmap_basic");

    tcase_add_checked_fixture(test_case, setup, teardown);
    tcase_add_test(test_case, test_hashmap_get);
    tcase_add_test(test_case, test_hashmap_remove);
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