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
#include <rcl_interfaces/srv/get_parameter_types.h>
#include <rcl_interfaces/srv/list_parameters.h>
#include <rcl_interfaces/srv/set_parameters.h>
#include <rcl_interfaces/srv/set_parameters_atomically.h>

#include <rcl_interfaces/msg/parameter_type__functions.h>

#include "rcl/error_handling.h"
#include "rcl/parameter_client.h"
#include "rcl/subscription.h"

#include "./common.h"

typedef struct rcl_parameter_client_impl_t
{
  rcl_node_t * node;
  rcl_client_t * get_client;
  rcl_client_t * get_types_client;  // Also referred to as describe parameters in docs
  rcl_client_t * set_client;
  rcl_client_t * set_atomically_client;
  rcl_client_t * list_client;

  // Store the upcoming request/pending responses for each client
  rcl_interfaces__srv__GetParameters_Request get_request;
  rcl_interfaces__srv__GetParameters_Response get_response;

  rcl_interfaces__srv__GetParameterTypes_Request get_types_request;
  rcl_interfaces__srv__GetParameterTypes_Response get_types_response;

  rcl_interfaces__srv__SetParameters_Request set_request;
  rcl_interfaces__srv__SetParameters_Response set_response;

  rcl_interfaces__srv__SetParametersAtomically_Request set_atomically_request;
  rcl_interfaces__srv__SetParametersAtomically_Response set_atomically_response;

  rcl_interfaces__srv__ListParameters_Request list_request;
  rcl_interfaces__srv__ListParameters_Response list_response;

  int64_t get_sequence_number;
  int64_t get_types_sequence_number;
  int64_t set_sequence_number;
  int64_t set_atomically_sequence_number;
  int64_t list_sequence_number;

  // Storage for an optional parameter event subscriber (??)
  rcl_subscription_t * parameter_event_subscription;
} rcl_parameter_client_impl_t;

rcl_ret_t
rcl_parameter_client_init(
  rcl_parameter_client_t * parameter_client,
  const rcl_node_t * node,
  const rcl_parameter_client_options_t * options
)
{
  // TODO
  return RCL_RET_OK;
}

rcl_ret_t
rcl_parameter_client_fini(rcl_parameter_client_t * parameter_client)
{
  // TODO
  return RCL_RET_OK;
}


// TODO We potentially need an array copy to make this work for some fields
#define DEFINE_RCL_PARAMETER_CLIENT_SEND_REQUEST(VERB, REQUEST_SUBTYPE, SUBFIELD_NAME) \
rcl_ret_t \
rcl_parameter_client_send_ ## VERB ## _request( \
  const rcl_parameter_client_t * parameter_client, \
  const REQUEST_SUBTYPE * SUBFIELD_NAME) \
{ \
  RCL_CHECK_ARGUMENT_FOR_NULL(parameter_client, RCL_RET_INVALID_ARGUMENT); \
  RCL_CHECK_ARGUMENT_FOR_NULL(SUBFIELD_NAME, RCL_RET_INVALID_ARGUMENT); \
 \
  parameter_client->impl->VERB ## _request.SUBFIELD_NAME = *SUBFIELD_NAME; \
 \
  rcl_ret_t ret = rcl_send_request( \
    parameter_client->impl->VERB ## _client, &parameter_client->impl->VERB ## _request, \
    &parameter_client->impl->VERB ## _sequence_number); \
 \
  return ret; \
}

DEFINE_RCL_PARAMETER_CLIENT_SEND_REQUEST(get, rosidl_generator_c__String__Array, names)
DEFINE_RCL_PARAMETER_CLIENT_SEND_REQUEST(get_types, rosidl_generator_c__String__Array, names)
DEFINE_RCL_PARAMETER_CLIENT_SEND_REQUEST(set, rcl_interfaces__msg__Parameter__Array, parameters)
DEFINE_RCL_PARAMETER_CLIENT_SEND_REQUEST(set_atomically, rcl_interfaces__msg__Parameter__Array, parameters)

// list has a to ba little different: the following won't work!
//DEFINE_RCL_PARAMETER_CLIENT_SEND_REQUEST(list, rosidl_generator_c__String__Array, names)


#define DEFINE_RCL_PARAMETER_CLIENT_TAKE_RESPONSE(VERB, REQUEST_SUBTYPE, SUBFIELD_NAME) \
rcl_ret_t \
rcl_parameter_client_take_ ## VERB ## _response( \
  const rcl_parameter_client_t * parameter_client, \
  rmw_request_id_t * request_header,\
  REQUEST_SUBTYPE * SUBFIELD_NAME) \
{ \
  RCL_CHECK_ARGUMENT_FOR_NULL(parameter_client, RCL_RET_INVALID_ARGUMENT); \
  RCL_CHECK_ARGUMENT_FOR_NULL(SUBFIELD_NAME, RCL_RET_INVALID_ARGUMENT); \
 \
  parameter_client->impl->VERB ## _response.SUBFIELD_NAME = *SUBFIELD_NAME; \
 \
  rcl_ret_t ret = rcl_take_response( \
    parameter_client->impl->VERB ## _client, request_header, &parameter_client->impl->VERB ## _response); \
 \
  return ret; \
}

DEFINE_RCL_PARAMETER_CLIENT_TAKE_RESPONSE(get, rcl_interfaces__msg__ParameterValue__Array, values)
DEFINE_RCL_PARAMETER_CLIENT_TAKE_RESPONSE(get_types, rosidl_generator_c__uint8__Array, types)
DEFINE_RCL_PARAMETER_CLIENT_TAKE_RESPONSE(set, rcl_interfaces__msg__SetParametersResult__Array, results)
DEFINE_RCL_PARAMETER_CLIENT_TAKE_RESPONSE(set_atomically, rcl_interfaces__msg__SetParametersResult, result)



#if __cplusplus
}
#endif
