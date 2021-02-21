#include "data_structures/dyn_array.h"
#include <check.h>
#include <stdlib.h>

struct test_data
{
    int a;
    int b;
    int c;
};

struct dyn_array* test_array = NULL;
const int c_num_elements = 200;

void setup(void)
{
    test_array = dyn_array_create(sizeof(struct test_data));
    for (int i = 0; i < c_num_elements; ++i)
    {
        struct test_data tmp = {i, i * 2, i + 10};
        dyn_array_push_back(test_array, &tmp);
    }
}

void teardown(void)
{
    dyn_array_destroy(test_array);
}

START_TEST(test_dyn_array_data)
{
    ck_assert_uint_eq(test_array->size, c_num_elements);
    ck_assert_uint_eq(test_array->element_size, sizeof(struct test_data));
}
END_TEST

START_TEST(test_dyn_array_pop_back)
{
    for (int i = 1; i <= 10; ++i)
    {
        dyn_array_pop_back(test_array);
        ck_assert_uint_eq(test_array->size, c_num_elements - i);
    }
}
END_TEST

START_TEST(test_dyn_array_get_set)
{
    for (size_t i = 0; i < test_array->size; ++i)
    {
        struct test_data* tmp = dyn_array_get(test_array, i);
        ck_assert_int_ne(tmp->a, -1);
        ck_assert_int_ne(tmp->b, -1);
        ck_assert_int_ne(tmp->c, -1);
    }

    const struct test_data tmp = {-1, -1, -1};
    for (size_t i = 0; i < test_array->size; ++i)
    {
        dyn_array_set(test_array, i, &tmp);
    }

    for (size_t i = 0; i < test_array->size; ++i)
    {
        struct test_data* tmp = dyn_array_get(test_array, i);
        ck_assert_int_eq(tmp->a, -1);
        ck_assert_int_eq(tmp->b, -1);
        ck_assert_int_eq(tmp->c, -1);
    }
}
END_TEST

Suite* test_suite(void)
{
    Suite* suite = suite_create("dyn_array_suite");
    TCase* test_case = tcase_create("dyn_array_basic");

    tcase_add_checked_fixture(test_case, setup, teardown);
    tcase_add_test(test_case, test_dyn_array_data);
    tcase_add_test(test_case, test_dyn_array_pop_back);
    tcase_add_test(test_case, test_dyn_array_get_set);
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