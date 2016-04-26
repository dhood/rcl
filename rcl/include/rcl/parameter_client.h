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

#ifndef RCL__PARAMETER_CLIENT_H_
#define RCL__PARAMETER_CLIENT_H_

#if __cplusplus
extern "C"
{
#endif

/// TODO:
/*
- Provide helper functions for topic names from node name
- documentation!!
- where should the parameter event subscription go, and where should the publisher go
- utilities for populating parameter_event?

- parameter_event msg can get populated with the response of a parameter_service
*/

/// Internal rcl implementation struct
struct rcl_parameter_client_impl_t;

/// There is no sync/async parameter client distinction in rcl.
typedef struct rcl_parameter_client_t
{
  // the parameter client provides storage and utility functions for parameter event subscription
  // Should instead have a create_parameter_event_subscription function for this parameter client?
  // rcl_subscription_t * parameter_event_subscription;
  rcl_parameter_client_impl_t * impl;
} rcl_parameter_client_t;

typedef struct rcl_parameter_client_options_t
{
  // quality of service settings for all parameter-related services
  rmw_qos_profile_t qos;
  rcl_allocator_t allocator;
} rcl_parameter_client_options_t;

// TODO Make sure default options are correct (rmw parameters profile?)
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_service_options_t
rcl_parameter_client_get_default_options(void);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_parameter_client_t
rcl_get_zero_initialized_parameter_client(void);


// Should pointers to the internal entities be allocated outside or...
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_client_init(
  rcl_parameter_client_t * parameter_client,
  const rcl_node_t * node,
  const rcl_client_options_t * options
);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_client_fini(rcl_parameter_client_t * parameter_client);

// I'm really not sure how useful any of the following stuff is

/*
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_send_get_parameters_request(
  const rcl_parameter_client_t * client,
  const rosidl_generator_c__String__Array * parameter_names);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_take_get_parameters_response(
  const rcl_parameter_client_t * client,
  rcl_interfaces__msg__ParameterValue__Array * parameter_values);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_send_get_parameter_types_request(
  const rcl_parameter_client_t * client,
  const rosidl_generator_c__String__Array * parameter_names);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_take_get_parameter_types_response(
  const rcl_parameter_client_t * client,
  rosidl_generator_c__uint8__Array * parameter_types);

// Function arguments pass messages defined in rcl_interfaces directly
// Wraps rcl_interfaces/Parameter array into Request
// wrap client send_request call
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_send_set_parameters_request(
  const rcl_parameter_client_t * client,
  const rcl_interfaces__msg__Parameter__Array * parameter_values);

// take response of set_parameters call
// converts srv to results structure
RCL_PUBLIC RCL_WARN_UNUSED rcl_ret_t
rcl_take_set_parameters_response(
  const rcl_parameter_client_t * client,
  rcl_interfaces__msg__SetParametersResult * set_parameters_result);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_send_set_parameters_atomically_request(
  const rcl_parameter_client_t * client,
  const rcl_interfaces__msg__Parameter__Array * parameter_values);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_take_set_parameters_atomically_response(
  const rcl_parameter_client_t * client,
  rcl_interfaces__msg__SetParametersResult * set_parameters_result);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_send_list_parameters_request(
  const rcl_parameter_client_t * client,
  rosidl_generator_c__String__Array * prefixes,
  uint64_t depth);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_take_list_parameters_response(
  const rcl_parameter_client_t * client,
  rcl_interfaces__msg__ListParametersResult * set_parameters_result);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_take_parameter_event(
  const rcl_parameter_client_t * client,
  rcl_interfaces__msg__ParameterEvent * parameter_event,
  rmw_message_info_t * message_info
);
*/

#if __cplusplus
}
#endif

#endif  // RCL__PARAMETER_CLIENT_H_
