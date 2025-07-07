#!/bin/sh

#bazelisk sync --only=google-benchmark
bazelisk run -c opt parabolic_reflector_dish
