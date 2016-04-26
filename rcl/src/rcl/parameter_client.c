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

#include <rcl_interfaces/srv/get_parameters.h>
#include <rcl_interfaces/srv/list_parameters.h>
#include <rcl_interfaces/srv/set_parameters.h>

#include "rcl/client.h"
#include "rcl/node.h"
#include "rcl/subscription.h"


typedef struct rcl_parameter_client_impl_t
{
  rcl_node_t * node;
  rcl_client_t * get_parameters_client;
  rcl_client_t * get_parameter_types_client;  // Also referred to as describe parameters in docs
  rcl_client_t * set_parameters_client;
  rcl_client_t * set_parameters_atomically_client;
  rcl_client_t * list_parameters_client;
  // Storage for an optional parameter event subscriber
  rcl_subscription_t * parameter_event_subscription;
} rcl_parameter_client_impl_t;


#if __cplusplus
}
#endif

#endif  // RCL__PARAMETER_CLIENT_H_
