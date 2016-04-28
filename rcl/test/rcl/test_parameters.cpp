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
#include "rosidl_generator_c/primitives_array_functions.h"

#include "rcl_interfaces/msg/list_parameters_result__functions.h"
#include "rcl_interfaces/msg/parameter__functions.h"
#include "rcl_interfaces/msg/parameter_type__struct.h"
#include "rcl_interfaces/msg/parameter_value__functions.h"
#include "rcl_interfaces/msg/set_parameters_result__functions.h"

#include "rcl/error_handling.h"
#include "rcl/node.h"
#include "rcl/parameter.h"
#include "rcl/parameter_client.h"
#include "rcl/parameter_service.h"

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
  const size_t num_params = 5;

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

// Helper function for filling in hardcoded test values
// TODO Fix the return code here
rcl_ret_t fill_parameter_array(rcl_interfaces__msg__Parameter__Array * parameters)
{
  size_t parameters_idx = 0;
  rcl_ret_t ret = rcl_parameter_set_bool(&parameters->data[parameters_idx++], "bool_param", true);
  ret = rcl_parameter_set_int(&parameters->data[parameters_idx++], "int_param", 123);
  ret = rcl_parameter_set_float(&parameters->data[parameters_idx++], "float_param", 45.67);

  ret = rcl_parameter_set_string(&parameters->data[parameters_idx++], "string_param", "hello world");
  ret = rcl_parameter_set_bytes(&parameters->data[parameters_idx++], "bytes_param", "\1\2\3\4");
  return ret;
}

rcl_ret_t fill_parameter_array(rcl_interfaces__msg__ParameterValue__Array * parameters)
{
  size_t parameters_idx = 0;
  rcl_ret_t ret = rcl_parameter_set_value_bool(&parameters->data[parameters_idx++], true);
  ret = rcl_parameter_set_value_int(&parameters->data[parameters_idx++], 123);
  ret = rcl_parameter_set_value_float(&parameters->data[parameters_idx++], 45.67);

  ret = rcl_parameter_set_value_string(&parameters->data[parameters_idx++], "hello world");
  ret = rcl_parameter_set_value_bytes(&parameters->data[parameters_idx++], "\1\2\3\4");
  return ret;
}

// TODO use boolean return codes
bool fill_parameter_names_array(rosidl_generator_c__String__Array * names)
{
  size_t parameters_idx = 0;
  rosidl_generator_c__String__assign(&names->data[parameters_idx++], "bool_param");
  rosidl_generator_c__String__assign(&names->data[parameters_idx++], "int_param");
  rosidl_generator_c__String__assign(&names->data[parameters_idx++], "float_param");
  rosidl_generator_c__String__assign(&names->data[parameters_idx++], "string_param");
  rosidl_generator_c__String__assign(&names->data[parameters_idx++], "bytes_param");
  return true;
}

// TODO Test un-setting parameters using set_parameters?
TEST_F(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_set_parameters)
{
  stop_memory_checking();
  rmw_request_id_t request_header;
  rcl_ret_t ret;
  (void) ret;

  // TODO parameter events?

  // TODO More utilities in rosidl_generator_c Array types can reduce the boilerplate needed for
  // error checking and dynamic resizing here.
  rcl_interfaces__msg__Parameter__Array parameters;
  rcl_interfaces__msg__Parameter__Array__init(&parameters, num_params);

  rcl_interfaces__msg__SetParametersResult__Array results;
  ret = rcl_interfaces__msg__SetParametersResult__Array__init(&results, num_params);

  start_memory_checking();

  ret = fill_parameter_array(&parameters);

  ret = rcl_parameter_client_send_set_request(this->parameter_client, &parameters);

  // wait until the parameters were set, may have to set up waiting/taking for parameter services
  ret = rcl_wait(this->wait_set, -1);
  rcl_interfaces__msg__Parameter__Array * parameters_req = nullptr;
  ret = rcl_parameter_service_take_set_request(this->parameter_service, &request_header, parameters_req);
  // In a real client library, need to access the request, fill in the response based on
  // node's storage, debugging tools that set the reason, etc.
  // TODO validate that parameters_req matches parameters

  // For now we'll just set them all to be successful
  // Should SetParametersResult have a "name" field for the parameter key it describes?
  for (size_t i = 0; i < num_params; ++i) {
    results.data[i].successful = true;
    rosidl_generator_c__String__assign(&results.data[i].reason, "Because reasons");
  }
  ret = rcl_parameter_service_send_set_response(this->parameter_service, &request_header, &results);

  ret = rcl_wait_set_add_parameter_client(this->wait_set, this->parameter_client);
  ret = rcl_wait(this->wait_set, -1);
  rcl_interfaces__msg__SetParametersResult__Array * results_response = nullptr;
  ret = rcl_parameter_client_take_set_response(this->parameter_client, &request_header, results_response);
  // TODO validate results and results_response
}

TEST_F(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_set_parameters_atomically)
{
  stop_memory_checking();

  rmw_request_id_t request_header;
  rcl_ret_t ret;
  (void) ret;

  // TODO parameter events?

  // TODO More utilities in rosidl_generator_c Array types can reduce the boilerplate needed for
  // error checking and dynamic resizing here.
  rcl_interfaces__msg__Parameter__Array parameters;
  rcl_interfaces__msg__Parameter__Array__init(&parameters, num_params);

  rcl_interfaces__msg__SetParametersResult result;
  ret = rcl_interfaces__msg__SetParametersResult__init(&result);

  start_memory_checking();

  ret = fill_parameter_array(&parameters);

  ret = rcl_parameter_client_send_set_atomically_request(this->parameter_client, &parameters);

  // wait until the parameters were set, may have to set up waiting/taking for parameter services
  ret = rcl_wait(this->wait_set, -1);
  rcl_interfaces__msg__Parameter__Array * parameters_req = nullptr;
  ret = rcl_parameter_service_take_set_atomically_request(this->parameter_service, &request_header, parameters_req);
  // In a real client library, need to access the request, fill in the response based on
  // node's storage, debugging tools that set the reason, etc.
  // TODO validate that parameters_req matches parameters

  // For now we'll just set them all to be successful
  // Should SetParametersResult have a "name" field for the parameter key it describes?
  result.successful = true;
  rosidl_generator_c__String__assign(&result.reason, "Because reasons");
  ret = rcl_parameter_service_send_set_atomically_response(this->parameter_service, &request_header, &result);

  ret = rcl_wait_set_add_parameter_service(this->wait_set, this->parameter_service);
  ret = rcl_wait(this->wait_set, -1);
  rcl_interfaces__msg__SetParametersResult * result_response = nullptr;
  ret = rcl_parameter_client_take_set_atomically_response(this->parameter_client, &request_header, result_response);
  // TODO validate results and results_response
}


TEST_F(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_get_parameters)
{
  stop_memory_checking();
  rmw_request_id_t request_header;
  rcl_ret_t ret;
  (void) ret;

  rosidl_generator_c__String__Array parameter_names;
  rosidl_generator_c__String__Array__init(&parameter_names, num_params);

  rcl_interfaces__msg__ParameterValue__Array parameter_values;
  rcl_interfaces__msg__ParameterValue__Array__init(&parameter_values, num_params);

  start_memory_checking();

  fill_parameter_names_array(&parameter_names);

  ret = rcl_parameter_client_send_get_request(this->parameter_client, &parameter_names);
  ret = rcl_wait(this->wait_set, -1);

  rosidl_generator_c__String__Array * request = nullptr;
  ret = rcl_parameter_service_take_get_request(this->parameter_service, &request_header, request);
  // TODO Validate the request

  // Assign some bogus values
  // In a real client library, these would be pulled from the language-specific storage
  ret = fill_parameter_array(&parameter_values);
  ret = rcl_parameter_service_send_get_response(this->parameter_service, &request_header, &parameter_values);

  ret = rcl_wait_set_add_parameter_client(this->wait_set, this->parameter_client);
  rcl_wait(this->wait_set, -1);

  // TODO: Should GetParameters_Response have a Parameter__Array subfield
  // instead of a ParameterValue__Array?
  rcl_interfaces__msg__ParameterValue__Array * response = nullptr;
  ret = rcl_parameter_client_take_get_response(this->parameter_client, &request_header, response);

  // TODO Validate the response values
}


TEST_F(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_get_parameter_types)
{
  stop_memory_checking();
  rmw_request_id_t request_header;
  rcl_ret_t ret;
  (void) ret;

  rosidl_generator_c__String__Array parameter_names;
  rosidl_generator_c__String__Array__init(&parameter_names, num_params);

  //rcl_interfaces__msg__ParameterType__Array
  rosidl_generator_c__uint8__Array parameter_types;
  rosidl_generator_c__uint8__Array__init(&parameter_types, num_params);

  start_memory_checking();

  // TODO check return value
  fill_parameter_names_array(&parameter_names);
  ret = rcl_parameter_client_send_get_types_request(this->parameter_client, &parameter_names);
  ret = rcl_wait(this->wait_set, -1);

  rosidl_generator_c__String__Array * request = nullptr;
  ret = rcl_parameter_service_take_get_types_request(this->parameter_service, &request_header, request);
  // TODO Validate request

  size_t parameters_idx = 0;
  parameter_types.data[parameters_idx++] = rcl_interfaces__msg__ParameterType__PARAMETER_BOOL;
  parameter_types.data[parameters_idx++] = rcl_interfaces__msg__ParameterType__PARAMETER_INTEGER;
  parameter_types.data[parameters_idx++] = rcl_interfaces__msg__ParameterType__PARAMETER_DOUBLE;
  parameter_types.data[parameters_idx++] = rcl_interfaces__msg__ParameterType__PARAMETER_STRING;
  parameter_types.data[parameters_idx++] = rcl_interfaces__msg__ParameterType__PARAMETER_BYTES;
  ret = rcl_parameter_service_send_get_types_response(this->parameter_service, &request_header, &parameter_types);

  ret = rcl_wait_set_add_parameter_client(this->wait_set, this->parameter_client);
  ret = rcl_wait(this->wait_set, -1);
  rosidl_generator_c__uint8__Array * response;
  ret = rcl_parameter_client_take_get_types_response(this->parameter_client, &request_header, response);

  // TODO Validate everything
}

TEST_F(CLASSNAME(TestParametersFixture, RMW_IMPLEMENTATION), test_list_parameters)
{
  stop_memory_checking();
  rmw_request_id_t request_header;
  rcl_ret_t ret;
  (void) ret;

  rcl_interfaces__msg__ListParametersResult list_result;
  rcl_interfaces__msg__ListParametersResult__init(&list_result);
  rosidl_generator_c__String__Array__init(&list_result.names, num_params);
  rosidl_generator_c__String__Array__init(&list_result.prefixes, num_params);

  rosidl_generator_c__String__Array prefixes;
  rosidl_generator_c__String__Array__init(&prefixes, 0);

  start_memory_checking();

  uint64_t depth = 0;
  ret = rcl_parameter_client_send_list_request(this->parameter_client, &prefixes, depth);
  ret = rcl_wait(this->wait_set, -1);

  rosidl_generator_c__String__Array * prefixes_req;
  uint64_t depth_req;
  ret = rcl_parameter_service_take_list_request(this->parameter_service, &request_header, prefixes_req, &depth_req);

  // put some names in 
  fill_parameter_names_array(&list_result.names);
  ret = rcl_parameter_service_send_list_response(this->parameter_service, &request_header, &list_result);

  ret = rcl_wait_set_add_parameter_client(this->wait_set, this->parameter_client);
  ret = rcl_wait(this->wait_set, -1);

  // TODO Validate the parameter names
}
