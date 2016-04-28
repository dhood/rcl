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

#ifndef RCL__PARAMETER_H_
#define RCL__PARAMETER_H_

#if __cplusplus
extern "C"
{
#endif

/// rcl_parameter_client_set_<TYPE> adds the parameter key, value pair to the
/// pending set_parameters_request
// CTYPE is a macro to sanitize human-readble type names to valid c types
#define RCL_DECLARE_SET_PARAMETER(TYPE) \
RCL_PUBLIC \
RCL_WARN_UNUSED \
rcl_ret_t \
rcl_parameter_set_ ## TYPE ## ( \
  rcl_interfaces__msg__Parameter * parameter, const char * name, CTYPE(TYPE) value); \

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_bool(
  rcl_interfaces__msg__Parameter * parameter, const char * parameter_name, bool value);

// Or, with the above macro, it would simply be RCL_DECLARE_SET_PARAMETER_REQUEST(bool)

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_int(
  rcl_interfaces__msg__Parameter * parameter, const char * parameter_name, int value);

// can't distinguish between floats and doubles. should use doubles underneath for more precision.
// But should we still call them floats?
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_float(
  rcl_interfaces__msg__Parameter * parameter, const char * parameter_name, double value);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_string(
  rcl_interfaces__msg__Parameter * parameter,
  const char * parameter_name,
  const char * value);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_bytes(
  rcl_interfaces__msg__Parameter * parameter,
  const char * parameter_name,
  const char * value);

// etc. for all types

#define RCL_DECLARE_GET_PARAMETER(TYPE) \
RCL_PUBLIC \
RCL_WARN_UNUSED \
rcl_ret_t \
rcl_parameter_get_ ## TYPE ## ( \
  const rcl_interfaces__msg__Parameter * parameter, CTYPE(TYPE) * output); \

/// Get the boolean value of the parameter, or return error if the parameter is not a boolean.
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_get_bool(const rcl_interfaces__msg__Parameter * parameter, bool * output);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_get_int(const rcl_interfaces__msg__Parameter * parameter, int * output);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_get_float(const rcl_interfaces__msg__Parameter * parameter, double * output);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_get_string(
  const rcl_interfaces__msg__Parameter * parameter, rosidl_generator_c__String * output);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_get_bytes(
  const rcl_interfaces__msg__Parameter * parameter, rosidl_generator_c__byte__Array * output);

// alternatively use RCL_DECLARE_GET_PARAMETER(TYPE) for the types to reduce LoC

// Similarly

#define RCL_DECLARE_SET_PARAMETER_VALUE(TYPE) \
RCL_PUBLIC \
RCL_WARN_UNUSED \
rcl_ret_t \
rcl_parameter_set_value_ ## TYPE ## ( \
  rcl_interfaces__msg__ParameterValue * parameter_value, CTYPE(TYPE) value); \

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_value_bool(
  rcl_interfaces__msg__ParameterValue * parameter_value, bool value);

// Or, with the above macro, it would simply be RCL_DECLARE_SET_PARAMETER_REQUEST(bool)

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_value_int(
  rcl_interfaces__msg__ParameterValue * parameter_value, int value);

// can't distinguish between floats and doubles. should use doubles underneath for more precision.
// But should we still call them floats?
RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_value_float(
  rcl_interfaces__msg__ParameterValue * parameter_value, double value);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_value_string(
  rcl_interfaces__msg__ParameterValue * parameter_value,
  const char * value);

RCL_PUBLIC
RCL_WARN_UNUSED
rcl_ret_t
rcl_parameter_set_value_bytes(
  rcl_interfaces__msg__ParameterValue * parameter_value,
  const char * value);

// etc. for all types

// Some other convenience function ideas:
// Get type enum from a ParameterValue
// rcl_parameter_get_type(rcl_interfaces__msg__ParameterValue * parameter_value, uint8 type)

// Get type enum from a Parameter
// rcl_parameter_get_type(rcl_interfaces__msg__Parameter * parameter, uint8 type)


#if __cplusplus
}
#endif

#endif  // RCL__PARAMETER_H_
