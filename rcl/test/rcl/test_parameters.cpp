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

    this->parameter_service = new rcl_parameter_service_t;
    *this->parameter_service = rcl_get_zero_initialized_parameter_service();
    rcl_parameter_service_options_t ps_options = rcl_parameter_service_get_default_options();
    ret = rcl_parameter_service_init(this->parameter_service, this->node_ptr, &ps_options)

    this->parameter_client = new rcl_parameter_client_t;
    *this->parameter_client = rcl_get_zero_initialized_parameter_client();
    rcl_parameter_client_options_t ps_options = rcl_parameter_client_get_default_options();
    ret = rcl_parameter_client_init(this->parameter_client, this->node_ptr, &ps_options);;

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

    rcl_ret_t ret = rcl_parameter_service_fini(this->parameter_service);
    delete this->parameter_service;

    rcl_ret_t ret = rcl_parameter_client_fini(this->parameter_client);
    delete this->parameter_client;
    EXPECT_EQ(RCL_RET_OK, ret) << rcl_get_error_string_safe();
    ret = rcl_shutdown();
    EXPECT_EQ(RCL_RET_OK, ret) << rcl_get_error_string_safe();
  }
};

TEST_P(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_parameters_nominal)
{
  stop_memory_checking();

  rcl_wait_set_t wait_set = get_zero_initialized_wait_set();
  rcl_wait_set_init(&wait_set, 0, 0, 0, 0, 0, rcl_get_default_allocator());

  // These implicitly add the parameter clients, services and subscriptions to the waitset
  rcl_wait_set_add_parameter_client(&wait_set, this->parameter_client);
  rcl_wait_set_add_parameter_service(&wait_set, this->parameter_service);

  // how to show parameter events?
  // parameters:
  // bool_param = true
  // int_param = 123
  // float_param = 45.67
  // string_param = "hello world"
  // bytes_param = '\1\2\3\4'

  start_memory_checking();

  // parameter_client could provide convenience functions
  rcl_interfaces__srv__SetParameters_Request set_param_req;
  rcl_interfaces__srv__SetParameters_Request__init(&set_param_req);
  rcl_interfaces__srv__Set(set_param_req.parameters);

  // Set parameters
  //rcl_parameter_client_();

  // This will send a request for 
  rcl_parameter_client_send_set_parameters_request();

  // wait until the parameters were set, may have to set up waiting/taking for parameter services
  rcl_wait(&wait_set, -1);
  // Service take/send?
  // rcl_parameter_service_take_set_parameters_request();
  rcl_parameters_take_set_parameters_response();

  // Check

  rcl_wait_set_fini(&wait_set);
}
