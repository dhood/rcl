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

#include "rcl/node.h"
#include "rcl/service.h"

typedef struct rcl_parameter_service_impl_t
{
  rcl_node_t * node;
  rcl_service_t * get_parameters_service;
  rcl_service_t * get_parameter_types_service;
  rcl_service_t * set_parameters_service;
  // rcl_service_t * set_parameters_atomically_service;
  rcl_service_t * list_parameters_service;
} rcl_parameter_service_impl_t;



#if __cplusplus
}
#endif
