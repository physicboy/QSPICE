Level1A SR-Buck External Compare PWM
Level1B SR-Buck External Compare PWM
Level1C SR-Buck External Compare PWM
Level1D SR-Buck External Compare PWM

Level2 SR-Buck Internal Cblock PWM
Level3 SR-Buck Internal Cblock PWM with Deadtime

Level1A uses Behavioral source for PWM generation (no-deadtime)
Level1B uses schmitt trigger comparator (no-deadtime)
Level1C uses 2 schmitt trigger comparator (1 w/ delay and 1 w/o delay) then combine with and gate
Level1D uses 2 schmitt trigger comparator ( w/o delay) but alter the duty command for upper and lower switches