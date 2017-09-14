#!/bin/bash

# 433.88eMHz works here, may need to be adjusted according to your rtl-sdr frequency error

./rtlfloats 433.88e+6 87 | ./demod

