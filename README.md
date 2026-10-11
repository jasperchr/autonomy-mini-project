Mini project for presentation

Two simulated vehicles that emulate ROS / MAVLINK standards (with a translation layer for MAVLINK style)

Demonstrates automated test cases, prototyping for future projects

Architecture (WIP):

Scenario -> Autonomy -> Vehicle Interface -> Fault Injection -> Vehicle A / Adapter + Vehicle B -> Evaluate against true state -> Pass/fail 

TODO:

- add mock MAVLINK vehicle with adapter
- add ability to input test / sim params, likely through args
- lint
- organise into separate folders