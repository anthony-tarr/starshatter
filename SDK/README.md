# Mod SDK

This is the information pulled from starshatter.com (which no longer exists). Information is as up-to-date as I can find it.

## Modding Starshatter

**Starshatter** is a space-sim modder's dream platform. Because Starshatter was designed from the ground up to support so many different play styles - from space fighter combat to airborne strikes to starship battles to fleet tactical encounters - you can use it to create mods for just about any popular science fiction universe you can think of. Creating a new mod can be as simple as importing some new ship models, editing a few text files that define ship and mission parameters, and running the game. Starshatter includes a "Custom Mission" selection screen that allows the end user to play any mod that is installed in the game directory.

Starshatter is still in development. At the moment, mod support is limited to creating new ship designs, new weapons effects, and new custom missions. When the game is complete, you will be able to create Total Conversion packages that replace the GUI art, fonts, HUD, and campaign data with custom content.

To get started on a mod now, download Starshatter and the Magic modeling tool from the DOWNLOADS PAGE of the official website. Then create the following directories in the game install directory:

* Mods
* Mods\Missions
* Mods\Ships
* Mods\Weapons (optional)

## DEF File Syntax

Starshatter "def" files all use the same C-like syntax. Each def file begins with a file type identifier, usually appearing by itself on the first line, that indicates what type of data are contained in the file. For mod purposes, the most important file types are:

* SHIP
* MISSION
* WEAPON 

Following the file type ID is a list of name-value attribute pairs that encode the information contained in the file. Name-value pairs are written like this:

    name : value

The name is a symbolic identifier that consists of letters, numbers, and underscores. Names must not start with a number. Each type of def file uses a different set of names to identify the attributes of the object being modeled.

The value side of the declaration contains the value of the attribute identified by the name. Values may be of the following data types:

| Type    | Description                                                             | Samples           |
| ------- | ----------------------------------------------------------------------- | ----------------- |
| boolean | boolean logic values                                                    | `true` or `false` |
| numeric | integral or floating-point constants written in scientific notation     | 1 -2.5e3          |
| string  | double-quote delimited string of characters                             | "hello world"     |
| vector  | three numeric values, surrounded by parentheses and separated by commas | (1, 2, 3)         |
| struct  | curly brace delimited list of name:value pairs                          | _see below_       |

Note that string values may be written without double quotes IF the string contains only letters, numbers, and underscores. Strings, quoted or otherwise, may not contain newlines or carriage returns. Adjacent quoted strings are automatically concatenated. This allows you to create large strings without using long unreadable lines of text:

    "This is all just "
    "one big old string."

Complex data types are represented in def files by structures. A structure is a curly-brace delimited list of name:value pairs that is treated as a single value. Within a structure, the name:value pairs must be separated by commas. A single trailing comma is permitted after the last name:value pair in a structure, and is ignored. Since structures contain name:value pairs, they can be nested (although this is not used very often).

Starshatter def files may include C++ style comments. Comments either begin with two slashes "//" and extend to the end of line, or they begin with "/*" and extend to the next occurrance of "*/".

EXAMPLES:

```
SHIP /* File Type ID */

name:       Orion             // unquoted string

detail_0:   "model.mag"       // quoted string because of dot
feature_0:  1000              // integral numeric

mass:       517e3             // scientific notation
drag:       1.0e-7

chase:      (0, -1800, 170)   // vector
bridge:     (0,   600, 100)

power:      {                 // structure
   type:        Fusion,
   design:      "Fusion Reactor",
   max_output:  90e3,

   loc:         (0, -48, -180),
   size:        48,
   hull_factor: 0.3,
   explosion:   7
}
```

## Ship design

### Basics

All of the files that make up a custom ship design must be placed in a subdirectory of the Mods\Ships directory that bears the name of the ship design. For example, to create a new destroyer class of starships called the "Warrior", start by creating a directory named Mods\Ships\Warrior. All models, texture bitmaps, and other data files for the Warrior must be placed in this directory.

The properties for your ship design are defined in a text file called a "Ship Definition File". This file can be created with any text editor (e.g. notepad) and must have the same name as the ship design. For our Warrior example, the ship definition file would be Mods\Ships\Warrior\Warrior.def. The ship definition file contains all of the physical properties of the ship (mass, hull strength, turn rate, lift, drag, etc.) and also names the 3D mesh files that define what the ship looks like.

All physical objects in Starshatter are modeled using 3D mesh files created by the low-poly editor Magic, written especially for the game. Magic stores meshes in files with .mag extensions. You can use Magic to create ship designs from scratch, or you can import existing designs created in another 3D modeling tool (3DStudio, LightWave, Maya, Milkshape). Magic uses the popular Wavefront OBJ ASCII format for importing geometry from 3rd party modeling tools. You can download a free, unsupported copy of Magic 1.1 from the Downloads Page of the official website.

Texture maps for your ship design must be stored in PCX format. Textures must be square powers of two (e.g. 64x64 or 256x56) in size. You should store your textures in 24 bit RGB format. You can use Adobe Photoshop, JASC Paint Shop Pro, or any other bitmap graphics editor to create textures for your models.

### Ship Definition File

The ship definition file is a text file that has a .def extension. All definition files in Starshatter use the same C-like syntax. The ship definition file defines the logical, graphical, physical, and functional properties of a class of ships.

### Logical Properties

| **name**    | The name of the ship design. This is the same name as the directory in which this file is stored.                                                             |
| **display_name** | The name of the ship design as it should be displayed on the HUD in the game itself. This parameter is optional. If no display_name is specified, the design name will be used.                                                    |
| **abrv** | The name of the ship type abbreviation as it should be displayed on the HUD in the game itself (e.g. "F-15E"). This parameter is optional. If no abrv is specified, no abbreviation will be used.     |
| **class**  | The displacement class of the ship. See below for a list of valid class names.                             |
| **map**  | A structure containing a list of sprite file names to be used in rendering this ship design in the navigation map. |
| **beauty**  | The name of the beauty shot bitmap file for this ship. This image is used to represent the ship in pre-mission briefing screens. If this attribute is not specified, the default is "beauty.pcx".                          |
| **hud_icon**  | The name of the HUD icon bitmap file for this ship. This image is used to represent damage to the ship at the bottom of the HUD. If this attribute is not specified, the default is "hud_icon.pcx".                          |

EXAMPLES:

```
name:         Orion
display_name: Orion
class:        carrier
beauty:       "beauty.pcx"
hud_icon:     "hud_icon.pcx"

map: {
   sprite: "map0.pcx",  //  9 oclock
   sprite: "map1.pcx",  // 10 oclock
   sprite: "map2.pcx",  // 12 oclock
   sprite: "map3.pcx",  //  2 oclock
   sprite: "map4.pcx",  //  3 oclock
   sprite: "map5.pcx",  //  4 oclock
   sprite: "map6.pcx",  //  6 oclock
   sprite: "map7.pcx"   //  8 oclock
}
```

VALID SHIP CLASSES

Fighters and Shuttles

* Fighter
* Attack
* LCA

Starships:

* Freighter
* Frigate
* Destroyer
* Cruiser
* Carrier

Space Stations:

* Station

Ground Units:

* Building
* Factory
* SAM
* EWR
* C3I
* Starbase

### Graphical Properties

| **model**    | The name of the 3D low-poly mesh file for a single level of detail ship.                                                             |
| **detail_n** | The name of the 3D low-poly mesh file for one level of detail in a multi-LOD ship. The n can be 0, 1, 2, or 3 from lowest detail to highest detail.                                                 |
| **feature_n** | The characteristic feature size in model units for one level of detail in a multi-LOD ship. The n can be 0, 1, 2, or 3 from lowest detail to highest detail.     |
| **spin**  | The 3D rate of spin in radians per second for one mesh component file. There must be exactly as many spin attributes as there are models per level of detail. For example, if a ship has three levels of detail, and each level has two meshes (one spinning one stationary) then there must be exactly two spin attributes in the definition.                            |

EXAMPLES:

```
// THREE LOD MESHES:

// big features
detail_0:   "low-detail.mag"
feature_0:  1000

// medium grain features
detail_1:   "medium-detail.mag"
feature_1:  100

// fine detail
detail_2:   "high-detail.mag"
feature_2:  10
```

```
// ONE STATIONARY AND TWO ROTATING SECTIONS, ONE LOD:

model:      "s2.mag"
model:      "s2mid.mag"
model:      "s2end.mag"

spin:       (0.0, 0.00, 0.0)
spin:       (0.0,-0.10, 0.0)
spin:       (0.0, 0.07, 0.0)
```

### Physical Properties

| **mass**    | The mass of the ship in metric tonnes. |
| **integrity** | Hull strength in "hit points". If the integrity of the ship reaches zero, the ship is destroyed. |
| **agility** | A general rating of the ship's ability to turn. Smaller numbers mean the ship turns more slowly, larger numbers mean that it turns more rapidly. |
| **vlimit**  | The exhaust velocity of the main drive in meters per second. Beyond this velocity, the ship will accelerate much more slowly. |
| **roll_rate**    | Similar to agility, but just sets the rate at which the ship rolls about its long axis. |
| **scale**    | A number used to multiply the vertex coordinates in the mesh when rendering the ship in the game engine. |
| **acs**    | Active Sensor Cross-section. A number that indicates how visible this ship is to active sensors. Similar to radar cross section in the real world. |
| **trans_x**    | Strength of the lateral (side-to-side) thrusters. Helps control how rapidly the ship can turn. |
| **trans_y**    | Strength of the longitudinal (foreward/aft) thrusters. Helps control how rapidly the ship can slow down. |
| **trans_z**    | Strength of the spinal/ventral (top/bottom) thrusters. Helps control how rapidly the ship can turn. |
| **roll_drag**    | How quickly the ship stops rolling after you let go of the stick. |
| **pitch_drag**    | How quickly the ship stops pitching after you let go of the stick. |
| **yaw_drag**    | How quickly the ship stops turning (yawing) after you let go of the stick. |
| **drag**    | Body drag coefficient. Used to calculate drag in flight regimes that have drag (e.g. atmosphere, arcade space mode). |
| **CL**    | Coefficient of Lift. Realistic flight model, atmospheric only. |
| **CD**    | Coefficient of Drag (Wings). Realistic flight model, atmospheric only. |
| **stall**    | Stall onset angle of attack (radians). Realistic flight model, atmospheric only. |
| **bridge**    | Vector location of bridge camera (F1 View) in model coordinates. |
| **chase**    | Vector location of chase camera (F2 View) in model coordinates. |

EXAMPLES:

```
mass:       517e3
agility:    2
vlimit:     220
roll_rate:  7
integrity:  75e3
scale:      5
acs:        1.5

trans_x:    1e6
trans_y:    5e5
trans_z:    1e6

drag:       1e-7
roll_drag:  1.2
pitch_drag: 1.2
yaw_drag:   1.2

chase:      (0, -1800, 170)
bridge:     (0,   600, 100)
```

## Ship systems

So far, we've only been looking at properties that affect the ship as a whole. Much of the customization possible for a ship design comes in the form of the dozen or more systems that give the ship its specialized abilities. Each ship system is defined by a structure in the ship definition file. All of the system structures have a few properties in common, but they each have custom attributes as well.

COMMON SYSTEM PROPERTIES

| **name**    | The name of the system, used in the engineering screen. |
| **abrv** | The abbreviation of the system, used in the master caution panel. |
| **design** | The name of the system design for this system. Starshatter includes a catalog of standard system designs that are made up of damageable/repairable components. |
| **loc**  | A vector providing the location of the system in model units. This is used for system-specific damage modeling to determine if the system was hit by incoming weapons fire. |
| **size**  | The radius of the system in model units. This is used for system-specific damage modeling to determine if the system was in range of the damage from incoming weapons fire. |
| **hull_factor**| A number from 0.0 to 1.0 that identifies well protected this system is by the ship's hull armor. 0 means the system is completely exposed, 1 means that all damage will be absorbed by the hull plating. |

Each system structure is introduced by a specific attribute name that identifies the type of system being declared. For example, reactors are declared with the name "power", like this: `power: { /* reactor attributes */ }`. The following system types are supported:

SYSTEM TYPES:

### Power

Power supplies, reactors and batteries

#### Power supply properties

|**type**| 	The type of power supply being created. Valid types are Battery, Auxilliary, and Fusion.|
|**max_output** |	The power output of the reactor in Watts.|
|**fuel_range**| 	The expected lifetime in hours of a tank of fuel under normal consumption. Optional.|
|**explosion**| 	A number identifying the type of explosion effect to use if the reactor is damaged. 7 (plasma leak) is a good choice for this attribute.|

EXAMPLE:

```
// STARSHIP FUSION REACTOR:

power: {
   type:             Fusion,
   design:           "Fusion Reactor",
   max_output:       90e3,

   loc:              (0, -48, -180),
   size:             48,
   hull_factor:      0.3,
   explosion:        7
}

// STARSHIP AUX REACTOR:

power: {
   type:             Auxilliary,
   design:           "Auxilliary Reactor",
   max_output:       10e3
}
```

### Drive

Main fusion or plasma conventional (slower-than-light) drive.

#### Drive properties

|type |	The type of drive being created. Valid types are Plasma and Fusion.|
|thrust |	The thrust output of the drive in kilo-Newtons.|
|augmenter |	The additional thrust output of an augmenter (afterburner) unit in kilo-Newtons. Optional.|
|scale |	A number indicating the relative scale of the drive flare sprite and cone.|
|port * |	Vector indicating the location in model units of the drive exhaust port nozzle. May be repeated up to 8 times per drive system.|
|explosion| 	A number identifying the type of explosion effect to use if the drive is damaged. 7 (plasma leak) is a good choice for this attribute.|

EXAMPLES:

```
// STARSHIP PLASMA DRIVE:

drive: {
   type:             Plasma,
   name:             "Plasma Drive",
   design:           "Plasma Drive",
   thrust:           40e3,
   scale:            10,

   // upper drive cluster
   port:             (-38,  32, -512),
   port:             (  0,  32, -524),
   port:             ( 38,  32, -512),

   // lower drive cluster
   port:             (-38, -64, -512),
   port:             (  0, -64, -524),
   port:             ( 38, -64, -512),

   loc:              (0, 48, -240),
   size:             48,
   hull_factor:      0.3,
   explosion:        7
}


// FIGHTER DRIVE W/ AUGMENTER:

drive: {
   type:            Plasma,
   thrust:          19,
   augmenter:       60,
   scale:           0.45,
   port:            (0, 0, -152),

   loc:             (0, 0, -100),
   size:            16,
   hull_factor:     0.85
}
```

### Thruster

Small turning and translation thrusters.

#### Thruster properties

|thrust |	The thrust output of the thruster in kilo-Newtons.|
|scale| 	A number indicating the relative scale of the drive flare sprite and cone.|
|port * |	Vector indicating the location in model units of the drive exhaust port nozzle. May be repeated up to 4 times per thruster system. LCA Shuttles Only!|
|explosion |	A number identifying the type of explosion effect to use if the system is damaged. 7 (plasma leak) is a good choice for this attribute.|

EXAMPLES:

```
// STARSHIP THRUSTERS:

thruster: {
   design:           Thruster,

   loc:              (0, 0, 128),
   size:             24,
   hull_factor:      0.3
}

// ZODIAC LCA VTOL THRUSTERS:

thruster: {
   design:           Thruster,
   scale:            0.35,
   port:             (-40, -28,  60),
   port:             ( 40, -28,  60),
   port:             (-40, -28, -60),
   port:             ( 40, -28, -60),

   loc:              (0, -20, 128),
   size:             24,
   hull_factor:      0.3
}
```

### Quantum

Faster-than-light quantum drive. Starships only.

#### Quantum properties

|capacity |	The energy capacity of the quantum drive in Joules. Optional.|
|consumption |	The rate of power consumption in Watts. Optional.|

EXAMPLES:

```
// STARSHIP QUANTUM DRIVE:

quantum: {
   design:           "Quantum Drive",
   loc:              (0, 0, 0),
   size:             24,
   hull_factor:      0.3
}
```

### Shield

Gravitational bias deflector shield. Starships only.

#### Shield properties

|type |	Number indicating type of shield system. Valid values are 1 (deflector) or 2 (gravitic).|
|model |	Name of shield model file.|
|shield_factor |	Strength or effectiveness of shield. Optional.|
|capacity |	The energy capacity of the quantum drive in Joules. Optional.|
|consumption 	|The rate of power consumption in Watts. Optional.|
|explosion |	A number identifying the type of explosion effect to use if the drive is damaged. 7 (plasma leak) is a good choice for this attribute.|

EXAMPLES:

```
// STARSHIP SHIELD SYSTEM:

shield: {
   type:             2,
   design:           "Grav Shield",
   loc:              (0, -150, 0),
   size:             24,
   hull_factor:      0.3
}
```

### Weapon

Primary (energy beam) or secondary (missile) weapons.

#### Weapon/Probe/Decoy Properties
|type |	Type of weapon system. Can be the name of a standard built-in weapon or a mod weapon. Standard weapon types are listed below.|
|group |	Name of weapon control group. All weapons in a single control group can be controlled with a single key or HUD button. Standard weapons have default control groups, so this parameter is optional.|
|azimuth| Azimuth offset for weapon mount in Radians. Optional, default is 0 (straight ahead). To direct a weapon to fire aft, use 3.1416.|
|elevation |	Elevation offset for weapon mount in Radians. Optional, default is 0 (level).|
|muzzle * |	Vector location of gun muzzle point in model units. If the weapon is a fixed mount weapon like an x-ray laser, the muzzle points are relative to the ship origin. If the weapon is turreted like a Phalanx |Cannon, the muzzle points are relative to the turret location, which is specified in the standard "loc" property for the weapon. Each weapon may have up to 8 muzzles.|
|explosion |	A number identifying the type of explosion effect to use if the system is damaged. 7 (plasma leak) is a good choice for this attribute.|

STANDARD WEAPONS:

Fighter Guns

* Alpha Cannon
* Delta Cannon

Fighter Missiles

* Archer
* Javelin
* Vector
* Harpoon
* Hammer 

Starship Guns

*  XRay Laser
*  Vulcan Cannon
*  Phalanx Cannon
*  Plasma Cannon 

Starship Missiles

* Mk7 Interceptor
* Fusion Torpedo 

EXAMPLES:

```
// STARSHIP WEAPON SYSTEMS:

weapon: {
   type:             "XRay Laser",
   design:           "Beam Weapon",
   name:             "XRay Laser 2",
   abrv:             "Laser-2",
   muzzle:           ( 128, -50, 360),
   muzzle:           (-128, -50, 300),

   size:             32,
   hull_factor:      0.2,
   explosion:        7
}

weapon: {
   type:             "Phalanx Cannon",
   design:           "Bolt Weapon",
   name:             "Port/Fwd Phalanx",
   abrv:             "Fwd PDB 1",
   group:            "FWD PDB",
   loc:              (-128, -30, 320),
   muzzle:           (-3, 0, 4),
   muzzle:           ( 3, 0, 4),

   size:             32,
   hull_factor:      0.2
}

// SENSOR PROBE LAUNCHER:

probe: {
   type:             "Sensor Drone",
   abrv:             "Probe",
   muzzle:           ( 0, -80, 0),

   loc:              (0, -80, 0),
   size:             16,
   hull_factor:      0.2
}

// FIGHTER DECOY:

decoy: {
   type:             "Fighter Decoy",
   abrv:             Decoy,
   muzzle:           ( 0, -32, 0),

   loc:              (0, 0, 0),
   size:             16,
   hull_factor:      0.5
}
```

### Flightdeck

Launch or recover deck for fighters Carriers only.

#### Flightdeck properties

|launch |	Boolean. True if this deck can launch fighters.|
|recovery |	Boolean. True if this deck can recover fighters.|
|explosion| 	A number identifying the type of explosion effect to use if the system is damaged. Not normally used, but 8 "smoke trail" might be OK.|

EXAMPLES:

```
// CARRIER 'LAUNCH' FLIGHT DECK:

flightdeck: {
   name:       "Deck 2",
   abrv:       "Deck 2",
   design:     "Flight Deck",
   loc:        (-36, -8,  440),
   cam:        (  0, -8,  440),
   start:      (-36, -12, 430),
   end:        (1e3,  0, 2500),

   spot:       { loc: (-12, -12, 405), filter: 0x7 },
   spot:       { loc: (-60, -12, 405), filter: 0x7 },
   spot:       { loc: (-36, -12, 415), filter: 0x8 },

   launch:     true,
   max_ships:  2,
   cycle_time: 15,

   size:       28,
   hull_factor: 0.5,
}

// CARRIER 'RECOVERY' FLIGHT DECK:

flightdeck: {
   name:       "Deck 3",
   abrv:       "Deck 3",
   design:     "Flight Deck",
   loc:        ( 192, -60,  -350),
   cam:        ( 160, -40,  -300),
   start:      ( 192, -57, -1350),
   end:        ( 192, -57,  -350),

   approach:   ( 200,  0, -2000),
   approach:   (1000,  0, -1000),
   approach:   (1000,  0,  1000),
   approach:   ( 250,  0,  2500),

   spot:       ( 192, -57, -350),

   recovery:   true,
   max_ships:  1,
   cycle_time: 15,

   size:       40,
   hull_factor: 0.5,
}

```

### Sensor

Combined passive/active sensor arrays.

#### Sensor properties

|range *|	Preset sensor range in meters. Can be repeated up to 8 times.|

EXAMPLES:

```
// STARSHIP SENSOR:

sensor: {
   design:           Sensor,

   range:            25e3,
   range:            50e3,
   range:            75e3,
   range:            100e3,

   loc:              (0, -16, 380),
   size:             16,
   hull_factor:      0.1
}

// FIGHTER SENSOR:

sensor: {
   design:           Sensor,

   range:            25e3,
   range:            50e3,
   range:            75e3,

   loc:              (0, 0, 100),
   size:             16,
   hull_factor:      0.25
}

```

### Probe

Sensor probe launcher.

#### Probe properties

See Weapon properties.

### Decoy

Missile countermeasure decoy.

#### Decoy properties

See Weapon properties.

### Gear

Fighter or shuttle landing gear.

#### Landing gear properties

|gear * 	|Substructure defining start and stop location and model of each gear strut. May be repeated up to 4 times per landing gear system.|

EXAMPLE:

```
// FIGHTER LANDING GEAR:

gear: {
   name:    "Undercarriage",
   abrv:    Gear,
   loc:     (0, 0, 0),
   size:    16,
   hull_factor: 0.5,

   gear: {
      model: "NoseGear.mag",
      start: (0, -18, 80),
      end:   (0, -43, 110)
   },

   gear: {
      model: "LeftGear.mag",
      start: (-20, -20, -54),
      end:   (-40, -56, -50)
   },

   gear: {
      model: "RightGear.mag",
      start: ( 20, -20, -54),
      end:   ( 40, -56, -50)
   },
}
```

### Nav

Auto navigation system.

#### Nav properties

No special properties.

EXAMPLE:

```
// STARSHIP AUTONAV SYSTEM:

nav: {
   loc:              (0, 16, 60),
   design:           Computer,
   size:             16,
   hull_factor:      0.1
}
```

### Navlight

Flashing navigation lights.

#### Navlight properties

|scale 	|Relative size of navlight sprite elements.|
|period 	|Length of repeating flash pattern in seconds.|
|light * |	Substructure defining location, color, and blink pattern of each light. May be repeated up to 8 times per navlight system.|

EXAMPLE:
```
// AIRBASE LANDING LIGHTS:

navlight:  {
   scale:  1.5,
   period: 1,

   // start of sequence
   light: { loc: (-640, 6, -448), pattern: 0x0003, type: 1 },
   light: { loc: (-512, 6, -320), pattern: 0x000c, type: 1 },
   light: { loc: (-384, 6, -192), pattern: 0x0030, type: 1 },
   light: { loc: (-256, 6,  -64), pattern: 0x00c0, type: 1 },
   light: { loc: (-128, 6,   64), pattern: 0x0300, type: 1 },
   light: { loc: (   0, 6,  192), pattern: 0x0c00, type: 1 },
   light: { loc: ( 128, 6,  320), pattern: 0x3000, type: 1 },
   light: { loc: ( 256, 6,  448), pattern: 0xc000, type: 1 }
   // end of sequence
}
```

### Computer

Various avionics and computer systems.

#### Computer properties

| type | Number indicating computer system subtype. Valid values include 1 (avionics), 2 (flight), and 3 (tactical).

EXAMPLE:

```
// STARSHIP COMPUTER SYSTEMS:

computer: {
   name:             "Avionics Package",
   abrv:             "HUD",
   type:             1,
   design:           Computer,

   loc:              (20, 16, 80),
   size:             16,
   hull_factor:      0.5
}

computer: {
   name:             "Flight Computer",
   abrv:             "Flight",
   type:             2,
   design:           Computer,

   loc:              (-20, -16, 80),
   size:             16,
   hull_factor:      0.2
}
```

The order that systems are listed in the ship definition file is significant. Each system must draw power from a reactor in order to operate (except for power sources themselves!). Each system is automatically attached to the power source that was most recently declared in the definition file. For example: 

```
power: {
   type:    Fusion,
   design:  "Fusion Reactor",
   // ...
}

drive: {
   type:    Plasma,
   name:    "Plasma Drive",
   // ...
}

power: {
   type:    "Auxilliary Reactor",
   design:  "Auxilliary Reactor",
   // ...
}

shield: {
   type:    2,
   design:  "Grav Shield",
   // ...
}
```

In the above listing, the plasma drive will draw power from the main reactor, and the shield will be attached to the auxilliary reactor. Starship captains can adjust power distribution and connection during the game through the engineering screen. For example, a starship captain could easily move the shield system from the aux reactor to the main fusion reactor to help balance the load. However, fighter pilots do not have access to the engineering screen, so fighter power routing is fixed at design time. 

### Weapons Hardpoints and Loadouts

Starships are generally designed with fixed weapons systems. If the ship definition file declares that a particular type of destroyer has two x-ray laser mounts, and one torpedo launcher, all ships of that type will always have those weapons.

Fighters, on the other hand, usually have underwing weapons hardpoints that can carry a variety of different weapon types. You can configure each fighter to carry an appropriate set of weapons each time one takes off during the game. In order to do this, fighter ship designs use two additional structures: 

### Hardpoint

A system that can mount several different weapon types.

#### Hardpoint Properties

|type * |	Type of missile weapon system that can be mounted on this hardpoint. Can be repeated up to 8 times per hardpoint.|
|azimuth |	Azimuth offset for weapon mount in Radians. Optional, default is 0 (straight ahead). To direct a weapon to fire aft, use 3.1416.|
|elevation |	Elevation offset for weapon mount in Radians. Optional, default is 0 (level).|
|muzzle |	Vector location of missile launch point in model units. Unlike a weapon system, each hardpoint can have only one muzzle.|

STANDARD WEAPONS:

Fighter Missiles

*    Archer
*    Javelin
*    Vector
*    Harpoon
*    Hammer 

EXAMPLES:

```
// FIGHTER MISSILE HARDPOINTS:

hardpoint:     {
   type:    "Javelin",
   type:    "Vector",
   type:    "Harpoon",
   type:    "Hammer",
   abrv:    "STA-2",
   muzzle:  (-48, -16, -80),

   loc:     (-48, -16, -80),
   size:    16,
   hull_factor: 0.2
}
```

### Loadout

A pre-defined list of weapons to be mounted.

#### Loadout properties

A loadout is not a system at all, but a pre-planned grouping of weapons on fighter hardpoints to achieve a specific goal.

|name |	Name of loadout. This name is used in the pre-mission loadout dialog and the flight dialog to select a pre-planned loadout for a fighter element.|
|stations |	An array of numbers indicating which weapon type is to be loaded on each hardpoint. Each number in the array corresponds to a single hardpoint, in the order they are listed in the ship definition file. The value of the number indicates which weapon type should be loaded. A value of -1 means that the hardpoint should be left empty. |

STANDARD LOADOUT NAMES:

*    Clean
*    ACM Light
*    ACM Short Range
*    ACM Medium Range
*    Ground Strike
*    Ship Strike
*    Hvy Ship Strike 

EXAMPLES:

```
// FIGHTER MISSILE LOADOUTS:

loadout: {
   name:      "ACM Medium Range",
   stations:  ( 0,  1,  1,  0)
}

loadout: {
   name:      "Ground Strike",
   stations:  ( 0,  3,  3,  0)
}
```

Samples

Check the two files under the `ShipSamples` directory. The first one is a fighter design, similar to the Stormhawk fighter from the game. The second one is a fleet carrier design, based on the Orion class carrier:
      Fighter.def            Carrier.def

You can use these samples as the basis for your own designs. Be sure to rename them to match your mod design names. And remember to save the files using a .def extension.

## Custom Weapon Designs


All of the files that make up a custom weapon design must be placed in the Mods\Weapons directory, or one of its subdirectories. For example, to create a new Fighter Plasma cannon called the "PPG", start by creating a directory named Mods\Weapons. You may wish to create a subdirectory named `Mods\Weapons\PPG` to hold the meshes and texture bitmaps for the weapon.

The properties for your weapon design are defined in a text file called a "Weapon Definition File". This file can be created with any text editor and must have the same name as the weapon design. For our PPG example, the weapon definition file would be Mods\Weapons\PPG.def. The weapon definition file contains all of the physical properties of the ship (name, type, damage, fire rate, etc.) and also names the graphics and wave files that define how the weapon looks and sounds.

Weapon turrets and missile bodies are modeled using 3D mesh files created by the low-poly editor Magic, written especially for the game. Magic stores meshes in files with .mag extensions. You can download a free, unsupported copy of Magic 1.1 from the Downloads Page of the official website.

Texture maps for weapon graphics must be stored in PCX format. Textures must be square powers of two (e.g. 64x64 or 256x56) in size. You should store your textures in 24 bit RGB format. You can use Adobe Photoshop, JASC Paint Shop Pro, or any other bitmap graphics editor to create textures for your models.

Sounds for weapons fire are stored in standard Wave files. You may use any bit depth (8 or 16 bits per channel) and sample rate (22 kHz or 44 kHz) for your sound effects that you wish. Weapons fire is localized in 3D, so it should be record in a monaural, not stereo, format. 

### Weapon Definition File and Structure

The weapon definition file is a text file that has a .def extension. All definition files in Starshatter use the same C-like syntax. The weapon definition file defines the logical, graphical, physical, and functional properties of a type of weapon.

The weapon definition file starts with the keyword WEAPON, in all-caps to indicate that the file contains a weapon definition. The logical parameters for the weapon are contained in struct that is introduced by a type label name. There are several broad categories of weapons:

*    primary: bolt type energy weapons
*    beam: beam type energy weapons
*    missile: guided or unguided missiles
*    drone: sensor and combat drones 

Primary and beam weapons are tied to the primary weapon trigger. Missile and drone weapons are tied to the pickle switch or secondary weapon trigger.

Many weapon design parameters refer to the number of barrels the weapon has. The number of barrels is actually part of the weapon mounting, not the weapon design, and is determined by the number of muzzle attributes in the weapon structure in the ship definition file. A single Laser weapon may be deployed in single barrel or multibarrel configuration in different ships.

**Logical Properties**
|name |	The name of the weapon design. This is the same name as the definition file, without the .def extension.|
|group| 	The name of the default weapon group to use when controlling this type of weapon. This parameter is optional. If no group is specified, the design name will be used.|
|target_type |	A bit vector that identifies the ship classes that this weapon can target. Each bit corresponds to a single ship class from combat drones (bit 0) to starbases (bit 21). A weapon that can target fighters and drones would have a target type of 0x0000000f. Starship weapons have a target type of 0x0000fff0. Air-to-ground weapons have a target type of 0xffff0000.|
|value| 	A number from 1 to 5 that is used by the enemy AI to evaluate how dangerous this weapon is.|

EXAMPLES:

```
WEAPON

primary: {
   name:             Turret,
   group:            Turret,
   target_type:      0x000fffff,
   value:            3,
```

**Physical Properties**
|ammo |	The number of shots that may be fired from each barrel of this weapon in a given mission. This parameter is optional. If no ammo limit is specified, the weapon can continue to fire shots until the reactor power runs out.|
|capacity |	A number indicating the size of the weapon's capacitor in megajoules.|
|recharge_rate |	The rate at which the capacitor can be recharged in megawatts.|
|refire_delay |	A number defining the delay between shots fired from this weapon in seconds.|
|salvo_delay| 	Used by the AI to impose and additional delay between shots after the last barrel has been fired.|

EXAMPLES:

```
   capacity:         1000,
   recharge_rate:    20,
   refire_delay:     0.2,
   charge:           10,
   min_charge:       10,
   damage:           5,
   speed:            10e3,
   life:             2,
```

**Shot Properties**
|damage |	The amount of damage delivered by the shot in hitpoints per megajoule. This number is multiplied by the amount of energy in the shot to determine the number of hitpoints to apply to the target if the shot connects. For beam weapons, this is the amount of damage to deliver for each second the beam is in contact with the target.|
|lethal_radius |	The maximum distance in meters from the target at which this shot will do some damage if it misses. This property is only used in conjunction with the guided property.|
|speed| 	The speed of the shot in meters per second. For missile weapons, this is used as an approximate value for targeting purposes. The actual speed of a missile is controlled by the mass of the weapon, the drag value, and the thrust available in the missile's drive. See below for more information on those parameters.|
|life| 	The lifetime of the shot in seconds.|
|charge |	The normal or maximum amount of energy delivered from the capacitor for each shot in megajoules. Missile weapons should use a value of 1 for this parameter.|
|min_charge |	The minimum amount of energy that must be delivered from the capacitor in order to fire a single shot. This number must not be greater than the normal charge parameter. For most weapons, these two parameters should have the same value. Beam weapons can use the min_charge parameter to allow firing with less than a full load, full duration shot. Missile weapons should use a value of 1 for this parameter.|
|carry_mass |	The mass in tonnes of a single shot when it is still aboard ship.|
|carry_resist| 	The amount of agility lost by the ship for each shot that is still aboard ship.|
|mass| 	The mass in tonnes of a single shot while it is in flight.|
|thrust |	The thrust in kilonewtons of the missile drive.|
|drag |	Body drag coefficient of the shot.|
|roll_rate |	How quickly the shot can roll.|
|pitch_rate 	How quickly the shot pitch.|
|yaw_rate |	|How quickly the shot can yaw.|
|roll_drag| 	How quickly the shot stops rolling after the AI releases roll input.|
|pitch_drag |	How quickly the shot stops pitching after the AI releases pitch input.|
|yaw_drag |	How quickly the shot stops yawing after the AI releases yaw input.|

EXAMPLES:

```
   mass:             0.2,
   thrust:           2e3,
   drag:             2,
   roll_drag:        4,
   pitch_drag:       4,
   yaw_drag:         4,
   roll_rate:        1,
   pitch_rate:       8,
   yaw_rate:         8,
```

**Tracking and AI Properties**
|min_range |	The minimum effective range of the weapon in meters. The AI will not fire the weapon if the target is closer than this distance.|
|max_range |	The maximum effective range of the weapon in meters. The AI will not fire the weapon if the target is farther than this distance.|
|max_track |	The maximum range at which the fire control computer will track a target for this weapon.|
|guided |	A number indicating the level of guidance on the shot. Set to 1 if the weapon's shots have onboard guidance and will seek their target. If this parameter is 0, the shots will fly straight.|
|syncro |	True if all barrels of the weapon should fire at the same time. False if the barrels should fire in sequence.|
|self_aiming |	True if the weapon can adjust the initial firing angle of the shot to better aim at the target. This is true for turrets, and may also be used for non-turreted weapons to give the player a slight boost in aiming.|
|aim_az_max |	The azimuth range of the turret in radians. Optional, only used if self-aiming is true.|
|aim_el_max |	The elevation range of the turret in radians. Optional, only used if self-aiming is true.|

EXAMPLES:

```
   guided:           1,
   self_aiming:      true,
   aim_az_max:       1,
   aim_el_max:       1,

   min_range:        10e3,
   max_range:        50e3,
   max_track:        90e3,
```

**Graphical Properties**
|visible_stores |	True if the weapon's shots should be visible even before they are fired. This is used by fighter missiles that are mounted on underwing hardpoints.|
|graphic_type |	A number identifying the type of graphic representation to use for the shot fired by this weapon. 1=solid, 2=sprite, 3=blob. A blob is an elongated polygon oriented to the direction of the shot's travel. Blobs are used for bolt and beam style primary weapons.|
|bitmap |	The name of the bitmap file to use when rendering the shot as a blob or sprite. Must be a PCX file that can be used as a texture.|
|model |	The name of the 3D model file to use when rendering the shot as a solid object like a missile. Must be a .mag file.|
|turret |	The name of the 3D model file to use when rendering the turret of the weapon itslef. Optional. Must be a .mag file.|
|trail |	The name of the bitmap file to use when rendering the vapor trail left behind by a missile type weapon. Optional. Must be a PCX file that can be used as a texture.|
|flash |	The name of the bitmap file to use when rendering the muzzle flash for the weapon. Optional. Must be a PCX file that can be used as a texture.|
|flare| 	The name of the bitmap file to use when rendering the drive flare of the shot. Optional. Must be a PCX file that can be used as a texture.|
|width |	The width of the blob in meters.|
|length |	The length of the blob in meters.|
|scale |	The scale of the sprite graphic.|
|trail_width| 	The width in meters of the missile vapor trail.|
|flash_scale |	The scale of the muzzle flash sprite.|
|flare_scale |	The scale of the drive flare sprite.|
|light |	The amount of light given off by the shot. Optional.|
|sound |	The name of the sound file to use when rendering firing the weapon. Optional. Must be a WAV file.|
|sound_min_dist |	The distance in meters from the weapon at which point the sound begins to diminish in volume. At any point closer than this distance, the sound will be heard at full volume.|
|sound_max_dist |	The distance in meters from the weapon at which point the sound of the weapon can no longer be heard at all.|

EXAMPLES:

```
   turret:           "rocket_launcher.mag",
   scale:            0.15,

   graphic_type:     1,	     // solid
   scale:            0.15,
   light:            200,
   model:            "missile00.mag",
   trail:            "smoke.pcx",
   trail_width:      1,
   flare:            "drive.pcx",
   flare_scale:      0.25,
   sound:            "missile2.wav",
   sound_min_dist:   1e3,
   sound_max_dist:   10e3
```

## Missions

You've finished your lovingly crafted Dark Flame class Imperial star destroyer, and you want to load her up in a mission and see how she performs. Well, you are in luck. Creating a custom mission for Starshatter is actually even easier than creating your new ship design was in the first place.

As with most other things, missions are defined in text files called "Mission Definition Files". These use the same basic syntax as the ship definition files that describe the properties of starships. Missions are composed of two kinds of data: the pre-mission briefing, and a list of mission elements. Briefing data includes the general type of mission, and summary information for the player such as a situation report and objectives list. Mission elements define the specific ships and space stations that are loaded into the simulation environment when you play the mission. 

**Briefing Data**
|name |	The name of the mission. This is the name that will appear in the custom mission list, and at the top of the mission briefing screen.|
|system |	The star system in which the mission will occur. Valid choices in the current demo are Solus and Janus.|
|region |	The starting region of the mission. Valid choices are shown below.|
|sitrep |	A text description that explains the overall situation that has led up to this mission. Use this attribute to "set the scene" for the player and explain why the mission is important. The situation report is displayed at the top of the briefing screen.|
|objective |	A text description of the player's key objectives. This wil be displayed on the mission briefing screen.|
|start| 	The start day and time of the mission. This is a string containing the day (relative to the start of the campaign) followed by a slash, followed by the time using 24 hour notation. To create a mission that starts at 5:15 PM on the third day of the campaign, use "03/17:15:00".|

VALID REGIONS

Janus System:

*    Ilon
*    Dante
*    Janek
*    Janek Station
*    Alystra
*    Navara 

Solus System:

*   Meridian
*   Solus
*   Telmera
*   Kalon
*   Jalah
*   Trellis 

Note: Janek Station and Telmera are planetary terrain regions.

EXAMPLES:

```
MISSION

name:    "Fighter Combat"
system:  Solus
region:  Solus

sitrep:  "Tensions are rising in the neutral system of Solus. "
         "Enemy Vipers and Cobras have been detected "
         "patrolling Omane Sector and may begin harassing "
         "legal shipping bound for Harmony Station. "
         "Your element has been assigned to sweep the "
         "Omane Sector and clear out the hostiles."

objective:
         "Fighter Sweep - destroy enemy units in Omane Sector, "
         "and protect Harmony Station."

start:   "01/08:00:00"
```

**Mission Elements**

Element structures are used to define all the starships, buildings, space-stations, and fighters that will be active during the mission. Element structures are also used to create fighter squadrons aboard carriers.

|name |	The callsign of a fighter element, or the name of the starship or squadron being defined.|
|design |	The ship design to be used in creating the element. This can be a standard built-in ship design, or a custom ship design in the Mods\Ships directory.|
|region |	The region in which to create the element. Optional, if omitted, the default region for the mission is used.|
|loc |	The vector location (in meters) of where to create this element. In the nav map, X is east-west, Y is north-south, and Z is altitude.|
|head |	A number representing the heading or direction this unit is facing, in radians. Optional. The default heading is 0 (north) for all units.|
|count |	The number of ships in the element. Optional, default is 1.|
|iff |	The team number for this element. Valid choices are 0 (neutral), 1 (alliance), and 2 (hegemony).|
|player |	A number indicating which ship in the element will be controlled by the player. Optional, default is 0. Valid choices are 0 (not a player element) or 1 (lead) through the number of ships in the count |attribute for the element. Exactly one element should have a non-zero player attribute.|
|mission |	The mission role that this element is tasked with. Valid mission roles are shown below.|
|commander |	The name of the element that is immediately above this one in the chain of command. NPC ships will only take orders from their commander, or their commander's commander (etc.).|
|instr * |	A line of text to be displayed in the Instructions window of the player's HUD during the mission. Can be repeated as often as necessary to complete the text instructions for the mission. Most commonly used in training exercises to provide walkthrough info for the player.|
**Fighter Squadron Attributes:**
|carrier| 	The name of the carrier or starbase that hosts this squadron. If you include this attribute, you are defining a squadron, not an active element. The number of ships in the squadron is defined by the count attribute.|
|dead_count |	The number of ships in this squadron that have been killed in action. These ships are not available during the mission.|
|maint_count |	The number of ships in this squadron that are currently undergoing maintenance. These ships are not immediately available during the mission, but will become available when the repairs are complete.|
**Fighter Element Attributes:**
|alert| 	Boolean indicating whether this element should start on alert (on the flight deck or runway) or in flight. Optional, default is false (meaning the element will start in flight).|
|squadron |	The name of the squadron that this element is derived from. This indirectly indicates the carrier or starbase that this element will launch from if the alert attribute is set.|
|loadout * |	A structure identifying the loadouts to use for each fighter in the element. Can be repeated up to count times, or a single loadout can be used for all fighters in the element.|
**Direction Attributes:**
|objective |	A structure providing specific mission objectives for this element.|
|navpt *| 	A structure defining one step in the scripted flight plan for this element.|

VALID MISSION ROLES
|Patrol |	Defensive combat patrol. Engage hostile fighters.|
|Sweep |	Offensive combat patrol. Engage hostile fighters.|
|Intercept |	Fighter mission. Destroy pre-planned fighter targets.|
|Strike |	Fighter mission. Destroy pre-planned ground targets.|
|Assault |	Fighter mission. Destroy pre-planned starship targets.|
|Escort |	General escort. Protect the named escorted element.|
|Escort_Freight| 	Protect the escorted freighter.|
|Escort_Shuttle |	Protect the escorted shuttle.|
|Escort_Strike |	Protect the escorted strike package.|
|Intel| 	Intelligence gathering.|
|Blockade |	Prevent ships from entering or leaving port.|
|Fleet |	Starship force-on-force engagement.|
|Bombardment |	Starship attack on space station.|
|Flight_Ops |	Carrier flight operations.|
|Transport| 	General movement of starships.|
|Cargo| 	Freight hauling.|
|Training| 	An exercise in a training scenario.|

EXAMPLES:

```
// FLEET CARRIER:

ship: {
   name:    Archon,
   design:  Orion,
   loc:     (0, 0, 0),
   count:   1,
   iff:     1
}

// FIGHTER SQUADRON ON CARRIER:

element: {
   name:    Stallions,
   design:  Stormhawk,
   count:   20,
   maint_count: 5,
   dead_count: 5,
   carrier: Archon, // <- this makes it a squadron!
   iff:     1,
   loadout: { ship: -1, stations: (0, 1, 1, 0) },
}

// PLAYER: TWO-SHIP FIGHTER SWEEP

element: {
   name:    Ranger,
   design:  Stormhawk,
   squadron: Stallions,

   loc:     (1e3, 2e3, 0),
   mission: sweep,

   objective: { cmd: intercept, tgt: Martak },
   objective: { cmd: intercept, tgt: Malik  },

   player:  1,
   alert:   true,
   count:   2,
   iff:     1,
   loadout: { ship: -1, name: "ACM Medium Range" },

   navpt:   { cmd: vector, loc: (80e3,-20e3, 0), speed: 750, rgn: Solus },
   navpt:   { cmd: vector, loc: (90e3,-70e3, 0), speed: 750, rgn: Omane },
   navpt:   { cmd: sweep,  loc: (40e3,-70e3, 0), speed: 500, rgn: Omane },
   navpt:   { cmd: sweep,  loc: (20e3,-30e3, 0), speed: 500, rgn: Omane },
   navpt:   { cmd: vector, loc: (90e3,-30e3, 0), speed: 500, rgn: Omane },
   navpt:   { cmd: vector, loc: ( 5e3, 30e3, 0), speed: 500, rgn: Solus },
}
```

**What to Wear: Loadouts**

The loadout structure is used to define specific pre-planned weapons packages for fighter elements.

|ship |	A number identifying which ship in the element to apply this loadout to. Can be -1 (meaning all ships) or a number from 0 (lead) to count -1. Optional, default is -1.|
|name |	The name of a standard loadout for this ship design.|
|stations |	An array of numbers indicating which weapon type is to be loaded on each hardpoint. Each number in the array corresponds to a single hardpoint, in the order they are listed in the ship definition file. The value of the number indicates which weapon type should be loaded. A value of -1 means that the hardpoint should be left empty.|

**Directions: Objectives and Navpoints**

Objectives and Navpoint structures tell the element (both player and AI) what to do during the mission.

|cmd |	The command for this navpoint or objective. Valid commands are shown below.|
|rgn |	The name of the region where the navpoint is located.|
|loc |	The vector location (in meters) of where to create this navpoint. In the nav map, X is east-west, Y is north-south, and Z is altitude.|
|speed| 	The speed (in meters per second) that the element should use enroute to this navpoint.|
|formation 	|The number of the formation that the element should use enroute to this navpoint. Valid formations are 0 (diamond), 1 (spread), 2 (box), and 3 (trail).|
|tgt |	The name of the target element for this navpoint or objective.|

VALID COMMANDS
|Vector |	Move to the destination loc.|
|Launch |	Take off from a flight deck or runway.|
|Dock |	Land on a flight deck or runway.|
|Defend |	Protect the target element from hostiles.|
|Escort |	Protect the target element from hostiles.|
|Patrol |	Engage hostiles near the location.|
|Sweep |	Engage hostiles along the route to the location.|
|Intercept |	Engage the named target fighter element.|
|Strike |	Engage the named target ground unit.|
|Assault| 	Engage the named target starship.|

Samples

Check out M0.def under `MissionSamples`.

You can use this sample as the basis for your own missions. Be sure to rename it to match your own mission names. And remember to save the files using a .def extension.
