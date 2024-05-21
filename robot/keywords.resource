*** Settings ***
Documentation
Library          NobRobotLib

*** Keywords ***
Build file
    Build executable

Verify build
    ${file_time}=   Get Executable Time
    ${time}=        Get current time
    Should be true  ${time}-${file_time} <= 1
