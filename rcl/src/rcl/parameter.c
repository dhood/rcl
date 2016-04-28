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

#include <rosidl_generator_c/string_functions.h>
#include <rcl_interfaces/msg/parameter__struct.h>
#include <rcl_interfaces/msg/parameter_type__functions.h>

#include "rcl/types.h"

// These macros could take us far in life
#define RCL_DEFINE_SET_PARAMETER(TYPE) \
rcl_ret_t \
rcl_parameter_set_ ## TYPE ## ( \
  rcl_interfaces__msg__Parameter * parameter, const char * parameter_name, CTYPE(TYPE) value) \
{ \
  rosidl_generator_c__String__assign(&parameter->name, parameter_name); \
  parameter->value.type = RCL_ENUM_TYPE(TYPE); \
  parameter->value. ## TYPE ## _value = value; \
  return RCL_RET_OK; \
}


rcl_ret_t
rcl_parameter_set_bool(
  rcl_interfaces__msg__Parameter * parameter, const char * parameter_name, bool value)
{
  rosidl_generator_c__String__assign(&parameter->name, parameter_name);
  parameter->value.type = rcl_interfaces__msg__ParameterType__PARAMETER_BOOL;
  parameter->value.bool_value = value;
  return RCL_RET_OK;
}



#if __cplusplus
}
#endif
