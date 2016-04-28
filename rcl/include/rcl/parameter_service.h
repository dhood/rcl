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

#ifndef RCL__PARAMETER_SERVICE_H_
#define RCL__PARAMETER_SERVICE_H_

#if __cplusplus
extern "C"
{
#endif

#include <rcl_interfaces/msg/list_parameters_result__struct.h>
#include <rcl_interfaces/msg/parameter__struct.h>
#include <rcl_interfaces/msg/set_parameters_result__struct.h>

#include "rcl/node.h"
#include "rcl/service.h"

struct rcl_parameter_service_impl_t;

typedef struct rcl_parameter_service_t
{
  struct rcl_parameter_service_impl_t * impl;
} rcl_parameter_service_t;

typedef struct rcl_parameter_service_options_t
{
  // quality of service settings for all services
  rmw_qos_profile_t qos;
  rcl_allocator_t allocator;
} rcl_parameter_service_options_t;

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_parameter_service_options_t
rcl_parameter_service_get_default_options(void);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_parameter_service_t
rcl_get_zero_initialized_parameter_service(void);


RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_init(
  rcl_parameter_service_t * parameter_service,
  const rcl_node_t * node,
  const rcl_parameter_service_options_t * options
);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_fini(rcl_parameter_service_t * parameter_service);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_take_set_request(
  const rcl_parameter_service_t * service,
  rcl_interfaces__msg__Parameter__Array * parameters);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_send_set_response(
  const rcl_parameter_service_t * service,
  const rcl_interfaces__msg__SetParametersResult__Array * set_parameter_results);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_take_get_request(
  const rcl_parameter_service_t * service,
  rosidl_generator_c__String__Array * parameter_names);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_send_get_response(
  const rcl_parameter_service_t * service,
  rcl_interfaces__msg__ParameterValue__Array * parameters);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_send_get_types_response(
  const rcl_parameter_service_t * service,
  rosidl_generator_c__uint8__Array * parameter_types);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_take_get_types_request(
  const rcl_parameter_service_t * service,
  const rosidl_generator_c__String__Array * parameter_names);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_take_set_atomically_request(
  const rcl_parameter_service_t * service,
  rcl_interfaces__msg__Parameter__Array * parameter_values);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_send_set_atomically_response(
  const rcl_parameter_service_t * service,
  const rcl_interfaces__msg__SetParametersResult * set_parameters_result);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_take_list_request(
  const rcl_parameter_service_t * service,
  rosidl_generator_c__String__Array * prefixes,
  uint64_t * depth);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_send_list_response(
  const rcl_parameter_service_t * service,
  const rcl_interfaces__msg__ListParametersResult * set_parameters_result);

/*
message/request/response conversion functions:

rcl_events_from_get_parameters_response

rcl_events_from_get_parameter_types_response

rcl_events_from_set_parameters_response

rcl_events_from_set_parameters_atomically_response

rcl_events_from_list_parameters_atomically_response
*/


#if __cplusplus
}
#endif

#endif  // RCL__PARAMETER_SERVICE_H_
