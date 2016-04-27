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

#if __cplusplus
extern "C"
{
#endif

#include <rcl_interfaces/srv/get_parameters.h>
#include <rcl_interfaces/srv/list_parameters.h>
#include <rcl_interfaces/srv/set_parameters.h>
#include <rcl_interfaces/srv/set_parameters_atomically.h>

#include <rcl_interfaces/msg/parameter_type__functions.h>

#include "rcl/parameter_client.h"
#include "rcl/subscription.h"


typedef struct rcl_parameter_client_impl_t
{
  rcl_node_t * node;
  rcl_client_t * get_parameters_client;
  rcl_client_t * get_parameter_types_client;  // Also referred to as describe parameters in docs
  rcl_client_t * set_parameters_client;
  rcl_client_t * set_parameters_atomically_client;
  rcl_client_t * list_parameters_client;

  // Store the upcoming request/pending responses for each client
  rcl_interfaces__srv__GetParameters_Request get_parameters_request;
  rcl_interfaces__srv__GetParameters_Response get_parameters_response;

  rcl_interfaces__srv__SetParameters_Request set_parameters_request;
  rcl_interfaces__srv__SetParameters_Response set_parameters_response;

  rcl_interfaces__srv__SetParametersAtomically_Request set_parameters_atomically_request;
  rcl_interfaces__srv__SetParametersAtomically_Response set_parameters_atomically_response;

  rcl_interfaces__srv__ListParameters_Request list_parameters_request;
  rcl_interfaces__srv__ListParameters_Response list_parameters_response;

  // Storage for an optional parameter event subscriber (??)
  rcl_subscription_t * parameter_event_subscription;
} rcl_parameter_client_impl_t;

// TODO Macros and definitions
rcl_ret_t
rcl_parameter_client_set_bool(
  rcl_interfaces__msg__Parameter * parameter, const char * parameter_name, bool value, size_t i)
{
  rosidl_generator_c__String__assign(&parameter->name, parameter_name);
  parameter->value.type = rcl_interfaces__msg__ParameterType__PARAMETER_BOOL;
  parameter->value.bool_value = value;
  return RCL_RET_OK;
}


#if __cplusplus
}
#endif
