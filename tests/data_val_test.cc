#include <gtest/gtest.h>

#include "neu_data_expr.h"
#include "neu_types.h"

TEST(DataValueTest, neu_dvalue_set_get_int8)
{
    neu_data_val_t *val = neu_dvalue_new(NEU_DTYPE_INT8);
    int             rc  = neu_dvalue_set_int8(val, 16);
    EXPECT_EQ(0, rc);
    int8_t ret;
    rc = neu_dvalue_get_int8(val, &ret);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(16, ret);
    neu_dvalue_free(val);
}

TEST(DataValueTest, neu_dvalue_set_get_int64)
{
    neu_data_val_t *val = neu_dvalue_new(NEU_DTYPE_INT64);
    int             rc  = neu_dvalue_set_int64(val, 100010111010);
    EXPECT_EQ(0, rc);
    int64_t ret;
    rc = neu_dvalue_get_int64(val, &ret);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(100010111010, ret);
    neu_dvalue_free(val);
}

TEST(DataValueTest, neu_dvalue_set_get_double)
{
    neu_data_val_t *val = neu_dvalue_new(NEU_DTYPE_DOUBLE);
    int             rc  = neu_dvalue_set_double(val, 100010111010.010603);
    EXPECT_EQ(0, rc);
    double ret;
    rc = neu_dvalue_get_double(val, &ret);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(100010111010.010603, ret);
    neu_dvalue_free(val);
}

#define INT_VAL_TEST_STR "hello int-val"
#define STRING_KEY_TEST_STR "string-key"
#define STRING_VAL_TEST_STR "hello string-val"
TEST(DataValueTest, neu_dvalue_set_get_keyvalue)
{
    /* teset set/get int_val */
    neu_int_val_t int_val;
    int_val.key = 21;
    int_val.val = neu_dvalue_new(NEU_DTYPE_CSTR);
    neu_dvalue_set_cstr(int_val.val, (char*)INT_VAL_TEST_STR);

    neu_data_val_t *val;
    int             rc;
    val = neu_dvalue_new(NEU_DTYPE_INT_VAL);
    rc = neu_dvalue_set_int_val(val, int_val);
    EXPECT_EQ(0, rc);
    
    neu_int_val_t int_val_get;
    char* cstr;
    rc        = neu_dvalue_get_int_val(val, &int_val_get);
    EXPECT_EQ(0, rc);
    rc        = neu_dvalue_get_ref_cstr(int_val_get.val, &cstr);
    EXPECT_EQ(0, rc);
    EXPECT_STREQ((char*)INT_VAL_TEST_STR, cstr);
    neu_int_val_uninit(&int_val_get);
    neu_dvalue_free(val);

    /* teset set/get string_val */
    neu_string_t* test_string;
    neu_string_val_t string_val;
    string_val.key = neu_string_from_cstr((char*)STRING_KEY_TEST_STR);
    test_string = neu_string_from_cstr((char*)STRING_VAL_TEST_STR);
    string_val.val = neu_dvalue_new(NEU_DTYPE_STRING);
    neu_dvalue_set_move_string(string_val.val, test_string);

    val = neu_dvalue_new(NEU_DTYPE_STRING_VAL);
    rc = neu_dvalue_set_string_val(val, string_val);
    EXPECT_EQ(0, rc);
    
    neu_string_val_t string_val_get;
    char* key_cstr;
    char* val_cstr;
    neu_string_t* string_get;
    rc        = neu_dvalue_get_string_val(val, &string_val_get);
    EXPECT_EQ(0, rc);
    key_cstr  = neu_string_get_ref_cstr(string_val_get.key);
    EXPECT_STREQ((char*)STRING_KEY_TEST_STR, key_cstr);
    rc        = neu_dvalue_get_ref_string(string_val_get.val, &string_get);
    EXPECT_EQ(0, rc);
    EXPECT_STREQ((char*)STRING_VAL_TEST_STR, neu_string_get_ref_cstr(string_get));
    neu_string_val_uninit(&string_val_get);
    neu_dvalue_free(val);
}

TEST(DataValueTest, neu_dvalue_set_get_cstr)
{
    neu_data_val_t *val = neu_dvalue_new(NEU_DTYPE_CSTR);
    int             rc  = neu_dvalue_set_cstr(val, (char *) "Hello, Neuron!");
    EXPECT_EQ(0, rc);
    char *ret = NULL;
    rc        = neu_dvalue_get_cstr(val, &ret);
    EXPECT_EQ(0, rc);
    EXPECT_STREQ("Hello, Neuron!", ret);
    free(ret);
    neu_dvalue_free(val);
}

TEST(DataValueTest, neu_dvalue_set_get_array)
{
    int             rc;
    neu_data_val_t *val =
        neu_dvalue_array_new(NEU_DTYPE_INT8, 4, sizeof(int8_t));
    neu_fixed_array_t *array_set;
    array_set = neu_fixed_array_new(4, sizeof(int8_t));
    rc        = neu_dvalue_set_array(val, array_set);
    neu_fixed_array_t *array_get;
    rc = neu_dvalue_get_array(val, &array_get);

    // TODO: assert two array is equal

    neu_fixed_array_free(array_set);
    neu_fixed_array_free(array_get);
    neu_dvalue_free(val);
}

TEST(DataValueTest, neu_dvalue_set_get_vec)
{
    int             rc;
    neu_data_val_t *val = neu_dvalue_vec_new(NEU_DTYPE_INT8, 4, sizeof(int8_t));
    vector_t *      vec_set;
    vec_set = vector_new(4, sizeof(int8_t));
    rc      = neu_dvalue_set_vec(val, vec_set);
    vector_t *vec_get;
    rc = neu_dvalue_get_vec(val, &vec_get);

    // TODO: assert two vector is equal

    vector_free(vec_set);
    vector_free(vec_get);
    neu_dvalue_free(val);
}

TEST(DataValueTest, neu_dvalue_prim_val_deser)
{
    int      rc;
    uint8_t *buf;

    neu_data_val_t *val = neu_dvalue_new(NEU_DTYPE_INT64);
    neu_dvalue_set_int64(val, 100);
    rc = neu_dvalue_serialize(val, &buf);
    EXPECT_EQ(0, rc);

    neu_data_val_t *val1;
    rc = neu_dvalue_desialize(buf, &val1);
    EXPECT_EQ(0, rc);
    int64_t i64;
    rc = neu_dvalue_get_int64(val1, &i64);
    EXPECT_EQ(0, rc);
    EXPECT_EQ(100, i64);

    free(buf);
    neu_dvalue_free(val);
    neu_dvalue_free(val1);
}

TEST(DataValueTest, neu_dvalue_array_deser)
{
    int      rc;
    uint8_t *buf;

    neu_data_val_t *val =
        neu_dvalue_array_new(NEU_DTYPE_INT8, 4, sizeof(int8_t));
    neu_fixed_array_t *array_set;
    array_set = neu_fixed_array_new(4, sizeof(int8_t));
    rc        = neu_dvalue_set_array(val, array_set);
    rc        = neu_dvalue_serialize(val, &buf);
    EXPECT_EQ(0, rc);

    neu_data_val_t *val1;
    rc = neu_dvalue_desialize(buf, &val1);
    EXPECT_EQ(0, rc);
    neu_fixed_array_t *array_get;
    rc = neu_dvalue_get_array(val, &array_get);
    EXPECT_EQ(0, rc);
    // TODO: assert two array is euqal

    free(buf);
    neu_fixed_array_free(array_set);
    neu_fixed_array_free(array_get);
    neu_dvalue_free(val);
    neu_dvalue_free(val1);
}

TEST(DataValueTest, neu_dvalue_vec_deser)
{
    int      rc;
    uint8_t *buf;

    neu_data_val_t *val = neu_dvalue_vec_new(NEU_DTYPE_INT8, 4, sizeof(int8_t));
    vector_t *      vec_set;
    vec_set = vector_new(4, sizeof(int8_t));
    rc      = neu_dvalue_set_vec(val, vec_set);
    rc      = neu_dvalue_serialize(val, &buf);
    EXPECT_EQ(0, rc);

    neu_data_val_t *val1;
    rc = neu_dvalue_desialize(buf, &val1);
    EXPECT_EQ(0, rc);
    vector_t *vec_get;
    rc = neu_dvalue_get_vec(val, &vec_get);
    EXPECT_EQ(0, rc);
    // TODO: assert two vector is euqal

    free(buf);
    vector_free(vec_set);
    vector_free(vec_get);
    neu_dvalue_free(val);
    neu_dvalue_free(val1);
}