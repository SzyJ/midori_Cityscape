#pragma once

// General Configs
#define CONF_FPS_SMOOTHING 0.9f

// Folders
#define GAME_RESOURCES "res/"
#define GAME_SHADERS GAME_RESOURCES"shaders/"
#define GAME_TEXTURES GAME_RESOURCES"textures/"
#define GAME_MODELS GAME_RESOURCES"models/"

// Textures
#define TEXTURE_SKYBOX GAME_TEXTURES"skybox/NightSky"

// Models
#define MODEL_BUILDING GAME_MODELS"buildings/building_"
#define MODEL_HELICOPTER GAME_MODELS"Helicopter.obj"
#define MODEL_HELICOPTER_CHASSIS GAME_MODELS"helicopter/Chassis.obj"
#define MODEL_HELICOPTER_PROPELLER GAME_MODELS"helicopter/Propeller.obj"

// Shaders
#define SHADER_MODEL_LOADER GAME_SHADERS"MeshLoad"
#define SHADER_HELICOPTER GAME_SHADERS"Helicopter"
