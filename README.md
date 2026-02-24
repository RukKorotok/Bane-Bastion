# Bane-Bastion
Roguelike sword&shield mechanic

BANE & BASTION: Core Gameplay Mechanics
Bane & Bastion is a tactical action game centered on the coordinated control of two interconnected entities governed by a strict interaction radius.

1. Distance Constraint Mechanics
The core gameplay revolves around a logical and physical distance limit between the two characters:

Maximum Radius: Characters cannot move beyond a fixed maximum distance from each other.

Movement Synchronization: Upon reaching the distance limit, the movement vector of one character directly impacts the position of the other, creating a "rigid coupling" effect.

Zone Management: Bastion’s position defines the center of a dynamic circle, within which Bane is permitted to maneuver.

2. Roles and Functions
BANE (Offense): The primary damage dealer. Mobility and engagement range are restricted by Bastion's current position. Effectiveness depends on maneuvering within the allowed radius.

BASTION (Defense & Anchor): Serves as the tactical pivot point. Bastion absorbs damage and controls the battlefield geometry by shifting the "anchor" for the offensive unit.

3. Control Scheme
The input system is split to emphasize coordination over high-APM (actions per minute) gameplay:

Movement (Keyboard): Controls Bastion. Positioning the anchor dictates the area where enemies can be engaged.

Combat (Mouse): Controls Bane. Movement is constrained to an orbit around Bastion, requiring precise timing and directional calculation for attacks.

Interaction Rhythm: Combat efficiency is tied to Bastion’s stability. If Bastion retreats, Bane is forced to reposition accordingly.

4. Tactical Design
Success is defined by Geometry and Timing:

The player must constantly manage the distance limit to maintain offensive uptime.

Strategic positioning is mandatory: the offensive unit remains tethered to the defensive unit, preventing reckless solo charges.