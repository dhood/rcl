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

#include "rcl/parameter_service.h"
#include "./common.h"

typedef struct rcl_parameter_service_impl_t
{
  rcl_node_t * node;
  rcl_service_t * get_service;
  rcl_service_t * get_types_service;
  rcl_service_t * set_service;
  rcl_service_t * set_atomically_service;
  rcl_service_t * list_service;

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
} rcl_parameter_service_impl_t;

/*
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_parameter_service_options_t
rcl_parameter_service_get_default_options(void)
{
  // TODO
}

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_parameter_service_t
rcl_get_zero_initialized_parameter_service(void)
{
  // TODO
}
*/

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_init(
  rcl_parameter_service_t * parameter_service,
  const rcl_node_t * node,
  const rcl_parameter_service_options_t * options
)
{
  // TODO
  return RCL_RET_OK;
}

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_service_fini(rcl_parameter_service_t * parameter_service)
{
  // TODO
  return RCL_RET_OK;
}

// TODO We potentially need an array copy to make this work for some fields
#define DEFINE_RCL_PARAMETER_SERVICE_TAKE_REQUEST(VERB, REQUEST_SUBTYPE, SUBFIELD_NAME) \
rcl_ret_t \
rcl_parameter_service_send_ ## VERB ## _request( \
  const rcl_parameter_service_t * parameter_service, \
  rmw_request_id_t * request_header, \
  const REQUEST_SUBTYPE * SUBFIELD_NAME) \
{ \
  RCL_CHECK_ARGUMENT_FOR_NULL(parameter_service, RCL_RET_INVALID_ARGUMENT); \
  RCL_CHECK_ARGUMENT_FOR_NULL(SUBFIELD_NAME, RCL_RET_INVALID_ARGUMENT); \
 \
  parameter_service->impl->VERB ## _request.SUBFIELD_NAME = *SUBFIELD_NAME; \
 \
  rcl_ret_t ret = rcl_take_reqest( \
    parameter_service->impl->VERB ## _service, request_header, &parameter_service->impl->VERB ## _request); \
 \
  return ret; \
}

DEFINE_RCL_PARAMETER_SERVICE_TAKE_REQUEST(get, rosidl_generator_c__String__Array, names)
DEFINE_RCL_PARAMETER_SERVICE_TAKE_REQUEST(get_types, rosidl_generator_c__String__Array, names)
DEFINE_RCL_PARAMETER_SERVICE_TAKE_REQUEST(set, rcl_interfaces__msg__Parameter__Array, parameters)
DEFINE_RCL_PARAMETER_SERVICE_TAKE_REQUEST(set_atomically, rcl_interfaces__msg__Parameter__Array, parameters)


#define DEFINE_RCL_PARAMETER_SERVICE_SEND_RESPONSE(VERB, REQUEST_SUBTYPE, SUBFIELD_NAME) \
rcl_ret_t \
rcl_parameter_service_take_ ## VERB ## _response( \
  const rcl_parameter_service_t * parameter_service, \
  rmw_request_id_t * request_header,\
  REQUEST_SUBTYPE * SUBFIELD_NAME) \
{ \
  RCL_CHECK_ARGUMENT_FOR_NULL(parameter_service, RCL_RET_INVALID_ARGUMENT); \
  RCL_CHECK_ARGUMENT_FOR_NULL(SUBFIELD_NAME, RCL_RET_INVALID_ARGUMENT); \
 \
  parameter_service->impl->VERB ## _response.SUBFIELD_NAME = *SUBFIELD_NAME; \
 \
  rcl_ret_t ret = rcl_send_response( \
    parameter_service->impl->VERB ## _service, request_header, &parameter_service->impl->VERB ## _response); \
 \
  return ret; \
}

DEFINE_RCL_PARAMETER_SERVICE_SEND_RESPONSE(get, rcl_interfaces__msg__ParameterValue__Array, values)
DEFINE_RCL_PARAMETER_SERVICE_SEND_RESPONSE(get_types, rosidl_generator_c__uint8__Array, types)
DEFINE_RCL_PARAMETER_SERVICE_SEND_RESPONSE(set, rcl_interfaces__msg__SetParametersResult__Array, results)
DEFINE_RCL_PARAMETER_SERVICE_SEND_RESPONSE(set_atomically, rcl_interfaces__msg__SetParametersResult, result)


#if __cplusplus
}
#endif
