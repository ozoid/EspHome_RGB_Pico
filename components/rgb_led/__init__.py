import sys
sys.path.append('/home/super/pico/esphome')
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number

from esphome.const import (
    CONF_OUTPUT_ID, 
    CONF_BRIGHTNESS, 
    CONF_OUTPUT, 
    CONF_PATTERN, 
    CONF_SPEED, 
    CONF_COLOR, 
    CONF_REPEAT
)

MULTI_CONF = True
AUTO_LOAD = [ "number" ]

rgb_led_ns = cg.esphome_ns.namespace("rgb_led")
RGBLEDOutput = rgb_led_ns.class_("RGBLEDOutput", number.Number, cg.Component)

CONFIG_SCHEMA = number.NUMBER_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(RGBLEDOutput),

        cv.Optional(CONF_PATTERN): cv.declare_id(RGBLEDOutput),
        cv.Optional(CONF_OUTPUT): cv.declare_id(RGBLEDOutput),
        cv.Optional(CONF_COLOR): cv.declare_id(RGBLEDOutput),
        cv.Optional(CONF_SPEED): cv.declare_id(RGBLEDOutput),
        cv.Optional(CONF_REPEAT): cv.declare_id(RGBLEDOutput),
        cv.Optional(CONF_BRIGHTNESS): cv.declare_id(RGBLEDOutput),
    }
).extend(cv.COMPONENT_SCHEMA)

CONF_PATTERN = "pattern"
CONF_OUTPUT = "output"
CONF_COLOR = "color"
CONF_SPEED = "speed"
CONF_REPEAT = "repeat"
CONF_BRIGHTNESS = "brightness"

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var,config)
    
    if CONF_OUTPUT in config:
        out = await cg.get_variable(config[CONF_OUTPUT])
        cg.add(var.set_output(out))
    if CONF_PATTERN in config:
        pattern = await cg.get_variable(config[CONF_PATTERN])
        cg.add(var.set_pattern(pattern))
    if CONF_SPEED in config:
        speed = await cg.get_variable(config[CONF_SPEED])
        cg.add(var.set_speed(speed))
    if CONF_REPEAT in config:
        repeat = await cg.get_variable(config[CONF_REPEAT])
        cg.add(var.set_repeat(repeat))
    if CONF_COLOR in config:
        color = await cg.get_variable(config[CONF_COLOR])
        cg.add(var.set_color(color))
    if CONF_BRIGHTNESS in config:
        bright = await cg.get_variable(config[CONF_BRIGHTNESS])
        cg.add(var.set_bright(bright))