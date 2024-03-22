Level 1A SR-Buck External Compare PWM
Level 1B SR-Buck External Compare PWM
Level 1C SR-Buck External Compare PWM
Level 1D SR-Buck External Compare PWM

Level 1A uses Behavioral source for PWM generation (no-deadtime)
Level 1B uses schmitt trigger comparator (no-deadtime)
Level 1C uses 2 schmitt trigger comparator (1 w/ delay and 1 w/o delay) then combine with and gate
Level 1D uses 2 schmitt trigger comparator ( w/o delay) but alter the duty command for upper and lower switches

Level 2 SR-Buck Internal Cblock PWM
Level 3 SR-Buck Internal Cblock PWM with Deadtime
Level 4 SR-Buck Internal Cblock PWM with Deadtime implemented with modular programming style (better for more complex project)
