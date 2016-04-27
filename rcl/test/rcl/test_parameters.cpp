// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include "rosidl_generator_c/string_functions.h"
#include "rcl_interfaces/msg/parameter__functions.h"
#include "rcl_interfaces/msg/set_parameters_result__functions.h"

#include "rcl/error_handling.h"
#include "rcl/node.h"
#include "rcl/parameter_service.h"
#include "rcl/parameter_client.h"

#include "rcl/rcl.h"

#include "../memory_tools/memory_tools.hpp"
#include "../scope_exit.hpp"

#ifdef RMW_IMPLEMENTATION
# define CLASSNAME_(NAME, SUFFIX) NAME ## __ ## SUFFIX
# define CLASSNAME(NAME, SUFFIX) CLASSNAME_(NAME, SUFFIX)
#else
# define CLASSNAME(NAME, SUFFIX) NAME
#endif


class CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION) : public ::testing::Test
{
public:
  rcl_node_t * node_ptr;
  rcl_wait_set_t * wait_set;
  rcl_parameter_service_t * parameter_service;
  rcl_parameter_client_t * parameter_client;

  void SetUp()
  {
    stop_memory_checking();
    rcl_ret_t ret;
    ret = rcl_init(0, nullptr, rcl_get_default_allocator());
    ASSERT_EQ(RCL_RET_OK, ret) << rcl_get_error_string_safe();
    this->node_ptr = new rcl_node_t;
    *this->node_ptr = rcl_get_zero_initialized_node();
    const char * name = "parameter_node";
    rcl_node_options_t node_options = rcl_node_get_default_options();
    ret = rcl_node_init(this->node_ptr, name, &node_options);

    ret = rcl_wait_set_init(this->wait_set, 0, 0, 0, 0, 0, rcl_get_default_allocator());

    this->parameter_service = new rcl_parameter_service_t;
    *this->parameter_service = rcl_get_zero_initialized_parameter_service();
    rcl_parameter_service_options_t ps_options = rcl_parameter_service_get_default_options();
    ret = rcl_parameter_service_init(this->parameter_service, this->node_ptr, &ps_options);

    this->parameter_client = new rcl_parameter_client_t;
    *this->parameter_client = rcl_get_zero_initialized_parameter_client();
    rcl_parameter_client_options_t cs_options = rcl_parameter_client_get_default_options();
    ret = rcl_parameter_client_init(this->parameter_client, this->node_ptr, &cs_options);

    // These implicitly add the parameter clients, services and subscriptions to the waitset
    // Can we share the impl struct of parameter_client and parameter_service to only wait.c
    // (kind of like a friend class in c++)?
    ret = rcl_wait_set_add_parameter_client(wait_set, this->parameter_client);
    ret = rcl_wait_set_add_parameter_service(wait_set, this->parameter_service);

    ASSERT_EQ(RCL_RET_OK, ret) << rcl_get_error_string_safe();
    set_on_unexpected_malloc_callback([]() {ASSERT_FALSE(true) << "UNEXPECTED MALLOC";});
    set_on_unexpected_realloc_callback([]() {ASSERT_FALSE(true) << "UNEXPECTED REALLOC";});
    set_on_unexpected_free_callback([]() {ASSERT_FALSE(true) << "UNEXPECTED FREE";});
    start_memory_checking();
  }

  void TearDown()
  {
    assert_no_malloc_end();
    assert_no_realloc_end();
    assert_no_free_end();
    stop_memory_checking();
    set_on_unexpected_malloc_callback(nullptr);
    set_on_unexpected_realloc_callback(nullptr);
    set_on_unexpected_free_callback(nullptr);
    rcl_ret_t ret = rcl_node_fini(this->node_ptr);
    delete this->node_ptr;


    rcl_wait_set_fini(this->wait_set);
    delete this->wait_set;

    ret = rcl_parameter_service_fini(this->parameter_service);
    delete this->parameter_service;

    ret = rcl_parameter_client_fini(this->parameter_client);
    delete this->parameter_client;
    EXPECT_EQ(RCL_RET_OK, ret) << rcl_get_error_string_safe();
    ret = rcl_shutdown();
    EXPECT_EQ(RCL_RET_OK, ret) << rcl_get_error_string_safe();
  }
};

TEST_F(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_set_parameters)
{
  stop_memory_checking();

  rcl_ret_t ret;

  // TODO parameter events?

  start_memory_checking();
  const size_t num_params = 5;

  rcl_interfaces__msg__Parameter__Array parameters;
  rcl_interfaces__msg__Parameter__Array__init(&parameters, num_params);
  size_t parameters_idx = 0;
  ret = rcl_parameter_set_bool(&parameters.data[parameters_idx++], "bool_param", true);
  ret = rcl_parameter_set_int(&parameters.data[parameters_idx++], "int_param", 123);
  ret = rcl_parameter_set_float(&parameters.data[parameters_idx++], "float_param", 45.67);

  ret = rcl_parameter_set_string(&parameters.data[parameters_idx++], "string_param", "hello world");
  ret = rcl_parameter_set_bytes(&parameters.data[parameters_idx++], "bytes_param", "\1\2\3\4");

  ret = rcl_parameter_client_send_set_request(this->parameter_client, &parameters);

  // wait until the parameters were set, may have to set up waiting/taking for parameter services
  ret = rcl_wait(this->wait_set, -1);
  rcl_interfaces__msg__Parameter__Array * parameters_req = nullptr;
  ret = rcl_parameter_service_take_set_request(this->parameter_service, parameters_req);
  // In a real client library, need to access the request, fill in the response based on
  // node's storage, debugging tools that set the reason, etc.
  // TODO validate that parameters_req matches parameters

  // For now we'll just set them all to be successful
  rcl_interfaces__msg__SetParametersResult__Array results;
  rcl_interfaces__msg__SetParametersResult__Array__init(&results, num_params);

  // Maybe helper functions can help associate the index?
  for (size_t i = 0; i < num_params; ++i) {
    results.data[i].successful = true;
    rosidl_generator_c__String__assign(&results.data[i].reason, "Because reasons");
  }
  ret = rcl_parameter_service_send_set_response(this->parameter_service, &results);

  ret = rcl_wait(this->wait_set, -1);
  // Service take
  rcl_interfaces__msg__SetParametersResult__Array * results_response = nullptr;
  ret = rcl_parameter_client_take_set_response(this->parameter_client, results_response);
  // TODO validate results and results_response
}

